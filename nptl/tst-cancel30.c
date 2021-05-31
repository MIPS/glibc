/* Check if cancellation type is not change when executing a cancelation
   entrypoint (BZ #14147).
   Copyright (C) 2021 Free Software Foundation, Inc.
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

#include <setjmp.h>
#include <support/check.h>
#include <support/xsignal.h>
#include <support/xthread.h>
#include <support/xunistd.h>

static int pipefds[2];
static pthread_barrier_t b;

static __thread jmp_buf jbuf;

static _Noreturn void
sigusr1_handler (int sig)
{
  longjmp (jbuf, 1);
}

static void *
tf (void *arg)
{
  xpthread_barrier_wait (&b);

  int r = setjmp (jbuf);
  if (r == 0)
    {
      char c;
      read (pipefds[0], &c, 1);
    }

  int type;
  TEST_VERIFY (pthread_setcanceltype (PTHREAD_CANCEL_DEFERRED, &type) == 0);
  TEST_COMPARE (type, PTHREAD_CANCEL_DEFERRED);

  return NULL;
}

static int
do_test (void)
{
  {
    struct sigaction sa;
    sa.sa_handler = sigusr1_handler;
    sa.sa_flags = 0;
    sigemptyset (&sa.sa_mask);
    xsigaction (SIGUSR1, &sa, NULL);
  }

  xpipe (pipefds);

  xpthread_barrier_init (&b, NULL, 2);

  pthread_t th = xpthread_create (NULL, tf, NULL);
  xpthread_barrier_wait (&b);
  xpthread_kill (th, SIGUSR1);
  void *r = xpthread_join (th);
  TEST_VERIFY (r == NULL);

  return 0;
}

#include <support/test-driver.c>
