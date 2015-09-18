/* Architecture specific code for pthread cancellation handling.
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
#define _NPTL_CANCELLATION_SIGMASK_H

/* Add the SIGCANCEL signal on sigmask set at the ucontext_t CTX obtained from
   the sigaction handler.  */
static void
ucontext_block_sigcancel (void *ctx)
{
  __sigaddset (&((ucontext_t*) ctx)->uc_sigmask, SIGCANCEL);
}

#endif
