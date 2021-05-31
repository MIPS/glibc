/* Architecture specific code for pthread cancellation handling.
   Copyright (C) 2020 Free Software Foundation, Inc.
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
   <http://www.gnu.org/licenses/>.  */

#ifndef _NPTL_CANCELLATION_PC_CHECK
#define _NPTL_CANCELLATION_PC_CHECK

#include <sigcontextinfo.h>

/* When a syscall with side-effects is interrupted by a signal, the kernel
   returns with whatever partial result (e.g. partial I/O operations).  In
   this case, the saved program counter points just after the syscall
   instruction and the cancellation handler should not act.

   The __syscall_cancel_arch function, used for all cancellable syscalls,
   contains two markers: __syscall_cancel_arch_start and
   __syscall_cancel_arch_end.  The prior points to just before the initial
   conditional branch that checks if the thread has received a cancellation
   request, while former points to the instruction after the one responsible
   to issue the syscall.

   The function checks if the program counter (PC) from ucontext_t CTX is
   within the start and then end boundary from the __syscall_cancel_arch
   bridge.  Return TRUE if the PC is within the boundary, meaning the
   syscall does not have any side effects; or FALSE otherwise.  */

static bool
cancellation_pc_check (void *ctx)
{
  /* Both are defined in syscall_cancel.S.  */
  extern const char __syscall_cancel_arch_start[1];
  extern const char __syscall_cancel_arch_end[1];

  uintptr_t pc = sigcontext_get_pc (ctx);
  return pc >= (uintptr_t) __syscall_cancel_arch_start
	 && pc < (uintptr_t) __syscall_cancel_arch_end;
}

#endif
