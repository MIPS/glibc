/* Vector optimized 32/64 bit S/390 version of memchr.
   Copyright (C) 2015-2025 Free Software Foundation, Inc.
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

#include <ifunc-memchr.h>
#if HAVE_MEMCHR_Z13

# include "sysdep.h"
# include "asm-syntax.h"

	.text

/* void *memchr (const void *s, int c, size_t n)
   Scans memory for character c
   and returns pointer to first c.

   Register usage:
   -r0=tmp
   -r1=tmp
   -r2=s
   -r3=c
   -r4=n
   -r5=current_len
   -v16=part of s
   -v17=index of found c
   -v18=c replicated
*/
ENTRY(MEMCHR_Z13)

	.machine "z13"
	.machinemode "zarch_nohighgprs"

# if !defined __s390x__
	llgfr	%r4,%r4
# endif /* !defined __s390x__ */

	clgije	%r4,0,.Lnf_end	/* If len == 0 then exit.  */

	vlbb	%v16,0(%r2),6	/* Load s until next 4k-byte boundary.  */
	lcbb	%r0,0(%r2),6	/* Get bytes to 4k-byte boundary or 16.  */
	llgfr	%r0,%r0		/* Convert 32bit to 64bit.  */

	vlvgb	%v18,%r3,0	/* Generate vector which elements are all c.
				   if c > 255, c will be truncated.  */
	vrepb	%v18,%v18,0
	lghi	%r5,16		/* current_len = 16.  */

	clgrjhe	%r0,%r4,.Llastcmp /* If (bytes to boundary) >= n,
				     jump to lastcmp.  */

	vfeebs	%v17,%v16,%v18	/* Find c.  */
	vlgvb	%r1,%v17,7	/* Load byte index of c.  */
	clgrjl	%r1,%r0,.Lfound2 /* Found c is within loaded bytes.  */

	/* Align s to 16 byte.  */
	risbgn	%r1,%r2,60,128+63,0 /* %r3 = bits 60-63 of %r2 'and' 15.  */
	slr	%r5,%r1		/* Compute bytes to 16bytes boundary.  */

	lgr	%r0,%r5		/* If %r5 + 64 < n? -> loop64.  */
	aghi	%r0,64
	clgrjl	%r0,%r4,.Lloop64
.Llt64:
	vl	%v16,0(%r5,%r2)
	aghi	%r5,16
	clgrjhe	%r5,%r4,.Llastcmp /* Do last compare if curr-len >= n.  */
	vfeebs	%v17,%v16,%v18	/* Find c.  */
	jl	.Lfound	/* Jump away if c was found.  */

	vl	%v16,0(%r5,%r2)
	aghi	%r5,16
	clgrjhe	%r5,%r4,.Llastcmp
	vfeebs	%v17,%v16,%v18
	jl	.Lfound

	vl	%v16,0(%r5,%r2)
	aghi	%r5,16
	clgrjhe	%r5,%r4,.Llastcmp
	vfeebs	%v17,%v16,%v18
	jl	.Lfound

	vl	%v16,0(%r5,%r2)
	aghi	%r5,16

.Llastcmp:
	/* Use comparison result only if located within first n characters.
	   %r5: current_len;
	   %r4: n;
	   (current_len - n): [0...16[
	   first ignored match index: vr-width - (current_len - n) ]0...16]
	*/
	vfeebs	%v17,%v16,%v18	/* Find c.  */
	slgrk	%r4,%r5,%r4	/* %r5 = current_len - n.  */
	lghi	%r0,16		/* Register width = 16.  */
	vlgvb	%r1,%v17,7	/* Extract found index or 16 if all equal.  */
	slr	%r0,%r4		/* %r0 = first ignored match index.  */
	clrjl	%r1,%r0,.Lfound2 /* Go away if miscompare is below n bytes.  */
	/* c not found within n-bytes.  */
.Lnf_end:
	lghi	%r2,0		/* Return null.  */
	br	%r14

.Lfound48:
	aghi	%r5,16
.Lfound32:
	aghi	%r5,16
.Lfound16:
	aghi	%r5,16
.Lfound0:
	aghi	%r5,16
.Lfound:
	vlgvb	%r1,%v17,7	/* Load byte index of c.  */
.Lfound2:
	slgfi	%r5,16		/* current_len -=16 */
	algr	%r5,%r1		/* Zero byte index is added to current len.  */
	la	%r2,0(%r5,%r2)	/* Return pointer to c.  */
	br	%r14


.Lloop64:
	vl	%v16,0(%r5,%r2)
	vfeebs	%v17,%v16,%v18	/* Find c.  */
	jl	.Lfound0	/* Jump away if c was found.  */
	vl	%v16,16(%r5,%r2)
	vfeebs	%v17,%v16,%v18
	jl	.Lfound16
	vl	%v16,32(%r5,%r2)
	vfeebs	%v17,%v16,%v18
	jl	.Lfound32
	vl	%v16,48(%r5,%r2)
	vfeebs	%v17,%v16,%v18
	jl	.Lfound48

	aghi	%r5,64
	lgr	%r0,%r5		/* If %r5 + 64 < n? -> loop64.  */
	aghi	%r0,64
	clgrjl	%r0,%r4,.Lloop64

	j	.Llt64
END(MEMCHR_Z13)

# if ! HAVE_MEMCHR_IFUNC
strong_alias (MEMCHR_Z13, __memchr)
weak_alias (__memchr, memchr)
# endif

# if ! HAVE_MEMCHR_Z900_G5 && defined SHARED && IS_IN (libc)
strong_alias (MEMCHR_Z13, __GI_memchr)
# endif
#endif
