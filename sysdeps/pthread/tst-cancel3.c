/* Copyright (C) 2002-2021 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@redhat.com>, 2002.

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

#include <pthread.h>
#include <signal.h>

#include <support/xunistd.h>
#include <support/xthread.h>
#include <support/check.h>
#include <support/process_state.h>


/* The pipe will be used pass the thread TID to master thread.  */
static int tidfd[2];
/* The pipe will be used to check the cancellation.  */
static int fd[2];


static void *
tf (void *arg)
{
  pid_t tid = gettid ();
  TEST_COMPARE (write (tidfd[1], &tid, sizeof (tid)), sizeof (tid));

  char buf[100];
  read (fd[0], buf, sizeof (buf));

  return NULL;
}


static int
do_test (void)
{
  pthread_t th;
  void *r;

  xpipe (tidfd);
  xpipe (fd);

  th = xpthread_create (NULL, tf, NULL);

  pid_t tid;
  TEST_COMPARE (read (tidfd[0], &tid, sizeof (tid)), sizeof (tid));

  support_process_state_wait (tid, support_process_state_sleeping);

  xpthread_cancel (th);

  r = xpthread_join (th);

  TEST_VERIFY (r == PTHREAD_CANCELED);

  return 0;
}

#include <support/test-driver.c>
