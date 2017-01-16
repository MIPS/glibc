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

#include <stdlib.h>
#include "pthreadP.h"

/* Cancellation function called by all cancellable syscalls.  */
long int
__syscall_cancel (__syscall_arg_t nr, __syscall_arg_t a1,
		  __syscall_arg_t a2, __syscall_arg_t a3,
		  __syscall_arg_t a4, __syscall_arg_t a5,
		  __syscall_arg_t a6 __SYSCALL_CANCEL7_ARG_DEF)
{
  struct pthread *self = THREAD_SELF;

  /* If cancellation is not enabled, call the syscall directly.  */
  if (self->cancelstate == PTHREAD_CANCEL_DISABLE)
    {
      long int r = INTERNAL_SYSCALL_NCS_CALL (nr, a1, a2, a3, a4, a5, a6
					      __SYSCALL_CANCEL7_ARG7);
      return INTERNAL_SYSCALL_ERROR_P (r) ? -INTERNAL_SYSCALL_ERRNO (r) : r;
    }

  /* Call the arch-specific entry points that contains the globals markers
     to be checked by SIGCANCEL handler.  */
  long int r = __syscall_cancel_arch (&self->cancelhandling, nr, a1, a2, a3,
				      a4, a5, a6 __SYSCALL_CANCEL7_ARG7);

  if (r == -EINTR
      && atomic_load_relaxed (&self->cancelhandling) & CANCELED_BITMASK
      && self->cancelstate == PTHREAD_CANCEL_ENABLE)
    {
      __do_cancel ();
    }

  return r;
}
libc_hidden_def (__syscall_cancel)

void
__do_cancel (void)
{
  __exit_thread (PTHREAD_CANCELED);
}
