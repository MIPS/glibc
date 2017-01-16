/* Architecture specific bits for cancellation handling.
   Copyright (C) 2019 Free Software Foundation, Inc.
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

#ifndef _NPTL_CANCELLATION_SIGMASK_H
#define _NPTL_CANCELLATION_SIGMASK_H 1

/* Add the SIGCANCEL signal on sigmask set at the ucontext CTX obtained from
   the sigaction handler.  */
static void
ucontext_block_sigcancel (void *ctx)
{
  sigset_t *set;
#ifdef __arch64__
  struct pt_regs *regs = (struct pt_regs*) ((siginfo_t *)(ctx) + 1);
  __siginfo_fpu_t *f = (__siginfo_fpu_t *)(regs + 1);
  set = (sigset_t *) ((stack_t *)(f + 1) + 1);
#else
  struct pt_regs32 *ptregs = (struct pt_regs32 *) (ctx);
  set = (sigset_t *) (ptregs + 1);
#endif
  __sigaddset (set, SIGCANCEL);
}

#endif
