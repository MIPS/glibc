/* Copyright (C) 2002-2019 Free Software Foundation, Inc.
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

#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include "pthreadP.h"
#include <atomic.h>
#include <sysdep.h>
#include <unistd.h>

int
__pthread_cancel (pthread_t th)
{
  volatile struct pthread *pd = (volatile struct pthread *) th;

  /* Make sure the descriptor is valid.  */
  if (INVALID_TD_P (pd))
    /* Not a valid thread handle.  */
    return ESRCH;

#ifdef SHARED
  pthread_cancel_init ();
#endif

  THREAD_ATOMIC_BIT_SET (pd, cancelhandling, CANCELED_BIT);

  /* A single-threaded process should be able to kill itself, since there is
     nothing in the POSIX specification that says that it cannot.  So we set
     multiple_threads to true so that cancellation points get executed.  */
  THREAD_SETMEM (THREAD_SELF, header.multiple_threads, 1);
#ifndef TLS_MULTIPLE_THREADS_IN_TCB
  __pthread_multiple_threads = *__libc_multiple_threads_ptr = 1;
#endif

  /* Avoid signaling when thread attempts cancel itself (pthread_kill
     is expensive).  */
  if (pd == THREAD_SELF && !(pd->cancelhandling & CANCELTYPE_BITMASK))
    return 0;

  return __pthread_kill_internal (th, SIGCANCEL);
}
weak_alias (__pthread_cancel, pthread_cancel)

PTHREAD_STATIC_FN_REQUIRE (__pthread_create)
