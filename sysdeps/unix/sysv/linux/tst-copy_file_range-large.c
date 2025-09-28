/* Test for copy_file_range with large sizes (bug 33245).
   Copyright (C) 2025 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

/* This test exercises copy_file_range with various large file sizes
   on FUSE filesystems to verify proper handling of system call return
   values.  No data is actually copied.  */

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <support/check.h>
#include <support/fuse.h>
#include <support/support.h>
#include <support/test-driver.h>
#include <support/xthread.h>
#include <support/xunistd.h>
#include <sys/stat.h>
#include <unistd.h>

static void
fuse_thread (struct support_fuse *f, void *closure)
{
  /* Node IDs for our test files.  */
  enum { NODE_SOURCE = 2, NODE_DEST = 3 };
  /* A large size, so that the kernel does not fail the
     copy_file_range attempt before performing the FUSE callback.
     Only the source file size matters to the kernel, but both files
     use the same size for simplicity.  */
  const uint64_t file_size = 1LLU << 61;

  struct fuse_in_header *inh;
  while ((inh = support_fuse_next (f)) != NULL)
    {
      if (support_fuse_handle_mountpoint (f)
          || (inh->nodeid == 1 && support_fuse_handle_directory (f)))
        continue;

      switch (inh->opcode)
        {
        case FUSE_LOOKUP:
          {
            char *name = support_fuse_cast (LOOKUP, inh);
            int node = 0;
            if (inh->nodeid == 1 && strcmp (name, "source") == 0)
              node = NODE_SOURCE;
            else if (inh->nodeid == 1 && strcmp (name, "dest") == 0)
              node = NODE_DEST;

            if (node != 0)
              {
                struct fuse_entry_out *out
                  = support_fuse_prepare_entry (f, node);
                out->attr.mode = S_IFREG | 0600;
                out->attr.size = file_size;
                support_fuse_reply_prepared (f);
              }
            else
              support_fuse_reply_error (f, ENOENT);
          }
          break;

        case FUSE_OPEN:
          /* File open */
          {
            if (inh->nodeid == NODE_SOURCE || inh->nodeid == NODE_DEST)
              {
                struct fuse_open_out out = { .fh = inh->nodeid };
                support_fuse_reply (f, &out, sizeof (out));
              }
            else
              support_fuse_reply_error (f, ENOENT);
          }
          break;

        case FUSE_GETATTR:
          /* Get file attributes */
          if (inh->nodeid == NODE_SOURCE || inh->nodeid == NODE_DEST)
            {
              struct fuse_attr_out *out = support_fuse_prepare_attr (f);
              out->attr.mode = S_IFREG | 0600;
              out->attr.size = file_size;
              support_fuse_reply_prepared (f);
            }
          else
            support_fuse_reply_error (f, ENOENT);
          break;

        case FUSE_COPY_FILE_RANGE:
          {
            struct fuse_copy_file_range_in *p
              = support_fuse_cast (COPY_FILE_RANGE, inh);

            /* Verify this is a copy from source to dest, starting at
               offset 0.  */
            TEST_COMPARE (p->fh_in, NODE_SOURCE);
            TEST_COMPARE (p->nodeid_out, NODE_DEST);
            TEST_COMPARE (p->off_in, 0);
            TEST_COMPARE (p->off_out, 0);
            TEST_VERIFY (p->len > 0);
            TEST_VERIFY (p->len <= file_size);

            /* Pretend the copy succeeded.  */
            struct fuse_write_out out = { .size = p->len };
            support_fuse_reply (f, &out, sizeof (out));
          }
          break;

        case FUSE_FLUSH:
          support_fuse_reply_empty (f);
          break;

        default:
          support_fuse_reply_error (f, EIO);
        }
    }
}

static void
test_size (struct support_fuse *f, off64_t size)
{
  /* On 32-bit targets, not all possible return values from
     copy_file_range are representable.  The current (Linux 6.5.18)
     kernel FUSE implementation can produce negative non-error results
     from copy_file_range in the range [1UL << 31, UINT_MAX - 4095],
     but this seems to be a FUSE bug.  */
  if (size != (ssize_t) size)
    {
      printf ("info:%s size 0x%llx is too large for ssize_t\n",
              test_verbose ? "    " : "", (unsigned long long int) size);
      return;
    }

  verbose_printf ("info:     testing copy size 0x%llx\n",
                  (unsigned long long int) size);

  const char *mountpoint = support_fuse_mountpoint (f);
  char *source_path = xasprintf ("%s/source", mountpoint);
  char *dest_path = xasprintf ("%s/dest", mountpoint);

  int source_fd = xopen (source_path, O_RDONLY, 0);
  int dest_fd = xopen (dest_path, O_WRONLY, 0);

  ssize_t copied = copy_file_range (source_fd, NULL, dest_fd, NULL, size, 0);
  /* Avoid FAIL_UNSUPPORTED if it is likely bogus due to previous
     copy_file_range successes.  */
  if (copied == -1 && errno == ENOSYS)
    {
      /* Unmounting avoids a test hang on exit.  */
      xclose (dest_fd);
      xclose (source_fd);
      support_fuse_unmount (f);
      FAIL_UNSUPPORTED ("copy_file_range not supported");
    }

  /* To avoid the negative return value in Linux versions 6.18 the size is
     silently clamped to UINT_MAX & PAGE_MASK.  Accept that return value
     too.  See:
     <https://github.com/torvalds/linux/commit/1e08938c3694f707bb165535df352ac97a8c75c9>.
  */
  if (copied != size)
    TEST_COMPARE (copied, UINT_MAX & ~(getpagesize () - 1));

  xclose (dest_fd);
  xclose (source_fd);
  free (dest_path);
  free (source_path);
}

static void
test_all_sizes (struct support_fuse *f)
{
  test_size (f, 0); /* Not actually handled by the callback.  */
  test_size (f, 20);
  test_size (f, 1 << 30);
  test_size (f, INT_MAX);
  for (int i = 0; i <= 5; ++i)
    test_size (f, (1U << 31) + i);
  for (int i = -4100; i <= -4090; ++i)
    test_size (f, UINT_MAX + i);
  for (int i = -100; i <= 0; ++i)
    test_size (f, UINT_MAX + i);

  /* We would like to test larger values than UINT_MAX here, but they
     do not work because the FUSE protocol uses uint32_t for the
     copy_file_range result in struct fuse_write_out.  */
}

static void *
test_cancel_state_variants (void *f_ptr)
{
  struct support_fuse *f = (struct support_fuse *) f_ptr;

  verbose_printf ("info:   testing default cancellation settings\n");
  test_all_sizes (f);

  verbose_printf ("info:   testing with cancellation disabled\n");
  TEST_COMPARE (pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, NULL), 0);
  test_all_sizes (f);

  verbose_printf ("info:   testing with cancellation enabled\n");
  TEST_COMPARE (pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, NULL), 0);
  test_all_sizes (f);

  return NULL;
}

static int
do_test (void)
{
  support_fuse_init ();
  struct support_fuse *f = support_fuse_mount (fuse_thread, NULL);

  verbose_printf ("info: testing on main thread\n");
  test_cancel_state_variants (f);

  verbose_printf ("info: testing on secondary thread\n");
  TEST_VERIFY (xpthread_join (xpthread_create
                              (NULL, test_cancel_state_variants, f))
               == NULL);

  verbose_printf ("info: testing on separate thread\n");

  support_fuse_unmount (f);
  return 0;
}

#include <support/test-driver.c>
