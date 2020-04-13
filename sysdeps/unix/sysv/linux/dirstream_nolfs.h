/* Copyright (C) 1993-2020 Free Software Foundation, Inc.
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

#ifndef _DIRSTREAM_NOLFS_H
#define _DIRSTREAM_NOLFS_H

#include <dirstream.h>

/* This is used for non-LFS support where the returned 'struct dirent'
   points on a reserved part of the 'data' allocated buffer.   */

/* The largest possible practical length of the d_name member are 255
   Unicode characters in UTF-8 encoding, so d_name is 766 bytes long, plus
   10 bytes from header, for a total of 776 bytes total.  */
enum { return_buffer_size = 1024 };

/* Ensure that the buffer that is passed on getdents64 call is sufficient
   aligned.  */
_Static_assert (return_buffer_size % _Alignof (max_align_t) == 0,
		"return_buffer_size % alignof (max_align_t) != 0");

/* Return the avaliable buffer size to use with getdents64 calls.  */
static inline size_t
dirstream_alloc_size (struct __dirstream *ds)
{
  return ds->allocation - return_buffer_size;
}

/* Return the start of the allocated buffer minus the reserved part
   to use on non-LFS readdir call.  */
static inline void *
dirstream_data (struct __dirstream *ds)
{
  return (char *) ds->data + return_buffer_size;
}

/* Return the allocated buffer used on non-LFS readdir call.  */
static inline struct dirent *
dirstream_ret (struct __dirstream *ds)
{
  return (struct dirent *) ds->data;
}

static inline struct dirent64 *
dirstream_entry (struct __dirstream *ds)
{
  size_t offset = return_buffer_size + ds->offset;
  return (struct dirent64 *) ((char *) ds->data + offset);
}

/* Copy one obtained entry from 'getdents64' call to the reserved space
   on DS allocated buffer and updated its internal state.  */
static inline struct dirent *
dirstream_ret_entry (struct __dirstream *ds)
{
  struct dirent64 *dp64 = dirstream_entry (ds);
  struct dirent *dp = dirstream_ret (ds);

  dp->d_ino = dp64->d_ino;
  dp->d_off = dp64->d_off;
  dp->d_reclen = dp64->d_reclen;
  dp->d_type = dp64->d_type;
  memcpy (dp->d_name, dp64->d_name,
	  dp64->d_reclen - offsetof (struct dirent64, d_name));

  ds->offset += dp->d_reclen;
  ds->filepos = dp->d_off;
  if (ds->filepos != dp->d_off)
    /* Overflow.  */
    return NULL;

  return dp;
}

#endif
