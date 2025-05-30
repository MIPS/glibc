/* llrint(), sparc32 v9 vis3 version.
   Copyright (C) 2012-2025 Free Software Foundation, Inc.
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

#include <sysdep.h>

	/* VIS instructions are used to facilitate the formation of
	   easier constants, and the propagation of the sign bit.  */

#define TWO_FIFTYTWO	0x43300000		/* 2**52 */

#define ZERO		%f10			/* 0.0 */
#define SIGN_BIT	%f12			/* -0.0 */

ENTRY (__llrint_vis3)
	sethi	%hi(TWO_FIFTYTWO), %o2
	sllx	%o0, 32, %o0

	or	%o0, %o1, %o0
	fzero	ZERO

	movxtod	%o0, %f0
	sllx	%o2, 32, %o2
	fnegd	ZERO, SIGN_BIT

	movxtod	%o2, %f16
	fabsd	%f0, %f14

	fcmpd	%fcc3, %f14, %f16

	fmovduge %fcc3, ZERO, %f16
	fand	%f0, SIGN_BIT, SIGN_BIT

	for	%f16, SIGN_BIT, %f16
	faddd	%f0, %f16, %f6
	fsubd	%f6, %f16, %f0
	fabsd	%f0, %f0
	for	%f0, SIGN_BIT, %f0
	fdtox	%f0, %f4
	movstouw %f4, %o0
	retl
	 movstouw %f5, %o1
END (__llrint_vis3)
