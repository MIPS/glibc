/* Cancellable syscall wrapper.  Linux/riscv version.
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

#include <sysdep.h>

/* long int __syscall_cancel_arch (int *cancelhandling,
				   __syscall_arg_t nr,
				   __syscall_arg_t arg1,
				   __syscall_arg_t arg2,
				   __syscall_arg_t arg3,
				   __syscall_arg_t arg4,
				   __syscall_arg_t arg5,
				   __syscall_arg_t arg6)  */

#ifdef SHARED
	.option pic
#else
	.option nopic
#endif

ENTRY (__syscall_cancel_arch)
	mv	t1, a7

	.globl __syscall_cancel_arch_start
__syscall_cancel_arch_start:
	lw	a7, 0(a0)
	/* if (*ch & CANCELED_BITMASK)  */
	andi	a7, a7, TCB_CANCELED_BITMASK
	bnez	a7, 1f

	mv	a7,a1
	mv	a0,a2
	mv	a1,a3
	mv	a2,a4
	mv	a3,a5
	mv	a4,a6
	mv	a5,t1
	scall

	.globl __syscall_cancel_arch_end
__syscall_cancel_arch_end:
	ret

1:
	addi	sp, sp, -16
	cfi_def_cfa_offset (16)
	sd	ra, 8(sp)
	cfi_offset (ra, -8)
	call	__syscall_do_cancel

END (__syscall_cancel_arch)
libc_hidden_def (__syscall_cancel_arch)
