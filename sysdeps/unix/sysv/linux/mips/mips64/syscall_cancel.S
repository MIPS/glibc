/* Cancellable syscall wrapper.  Linux/mips64 version.
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

#include <sysdep.h>
#include <sys/asm.h>

/* long int __syscall_cancel_arch (int *cancelhandling,
				   __syscall_arg_t nr,
				   __syscall_arg_t arg1,
				   __syscall_arg_t arg2,
				   __syscall_arg_t arg3,
				   __syscall_arg_t arg4,
				   __syscall_arg_t arg5,
				   __syscall_arg_t arg6,
				   __syscall_arg_t arg7)  */

#if _MIPS_SIM == _ABIN32
# define FRAME_SIZE 16
# define MASK       0x80010000
# define RA_OFF     8
# define S0_OFF     0
#else
# define FRAME_SIZE 32
# define MASK       0x90010000
# define RA_OFF     24
# define S0_OFF     8
#endif
#define ADDIU      LONG_ADDIU
#define ADDU       LONG_ADDU
#define SUBU       LONG_SUBU
#define LOAD_L     LONG_L

	.text
NESTED (__syscall_cancel_arch, FRAME_SIZE, ra)
	.mask	MASK, -SZREG
	.fmask	0x00000000, 0
	ADDIU	sp, sp, -FRAME_SIZE
	cfi_def_cfa_offset (FRAME_SIZE)
	sd	gp, 16(sp)
	cfi_offset (gp, -16)
#if _MIPS_SIM == _ABI64
	lui	gp, %hi(%neg(%gp_rel(__syscall_cancel_arch)))
	ADDU	gp, gp, t9
	daddiu	gp, gp, %lo(%neg(%gp_rel(__syscall_cancel_arch)))
#endif
	sd	ra, RA_OFF(sp)
	sd	s0, S0_OFF(sp)
	cfi_offset (ra, -RA_OFF)
	cfi_offset (s0, -S0_OFF)

	.global __syscall_cancel_arch_start
__syscall_cancel_arch_start:

	lw	v0, 0(a0)
	andi	v0, v0, TCB_CANCELED_BITMASK
	.set noreorder
	.set nomacro
	bne	v0, zero, 2f
	move	s0, a1
	.set macro
	.set reorder

	move	a0, a2
	move	a1, a3
	move	a2, a4
	move	a3, a5
	move	a4, a6
	move	a5, a7

	.set noreorder
	move	v0, s0
	syscall
	.set reorder

	.global __syscall_cancel_arch_end
__syscall_cancel_arch_end:

	.set noreorder
	.set nomacro
	bnel	a3, zero, 1f
	SUBU	v0, zero, v0
	.set macro
	.set reorder

1:
#if _MIPS_SIM == _ABI64
	ld	gp, 16(sp)
#endif
	ld	ra, RA_OFF(sp)
	ld	s0, S0_OFF(sp)

	.set	noreorder
	.set	nomacro
	jr	ra
	daddiu	sp, sp, FRAME_SIZE
	.set	macro
	.set	reorder

	cfi_remember_state
	cfi_def_cfa_offset (0)
#if _MIPS_SIM == _ABI64
	cfi_restore (gp)
#endif
	.align	3
2:
	cfi_restore_state
#if _MIPS_SIM == _ABI64
	ld	t9, %got_disp(__do_cancel)(gp)
	.reloc	3f, R_MIPS_JALR, __do_cancel
3:	jalr	t9
#else
	jal	__do_cancel
#endif
END (__syscall_cancel_arch)
libc_hidden_def (__syscall_cancel_arch)
