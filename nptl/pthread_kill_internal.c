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

#include <unistd.h>
#include <pthreadP.h>

/* Used internally by pthread_cancel, so we can't filter SIGCANCEL.  */
int
__pthread_kill_internal (pthread_t threadid, int signo)
{
  struct pthread *pd = (struct pthread *) threadid;

  /* Force load of pd->tid into local variable or register.  Otherwise
     if a thread exits between ESRCH test and tgkill, we might return
     EINVAL, because pd->tid would be cleared by the kernel.  */
  pid_t tid = atomic_forced_read (pd->tid);
  if (__glibc_unlikely (tid <= 0))
    /* Not a valid thread handle.  */
    return ESRCH;

  /* We have a special syscall to do the work.  */
  INTERNAL_SYSCALL_DECL (err);

  pid_t pid = __getpid ();

  int val = INTERNAL_SYSCALL_CALL (tgkill, err, pid, tid, signo);
  return (INTERNAL_SYSCALL_ERROR_P (val, err)
	  ? INTERNAL_SYSCALL_ERRNO (val, err) : 0);
}
