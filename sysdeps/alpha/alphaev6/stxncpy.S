/* Copyright (C) 2000-2025 Free Software Foundation, Inc.
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
   License along with the GNU C Library.  If not, see
   <https://www.gnu.org/licenses/>.  */

/* Copy no more than COUNT bytes of the null-terminated string from
   SRC to DST.

   This is an internal routine used by strncpy, stpncpy, and strncat.
   As such, it uses special linkage conventions to make implementation
   of these public functions more efficient.

   On input:
	t9 = return address
	a0 = DST
	a1 = SRC
	a2 = COUNT

   Furthermore, COUNT may not be zero.

   On output:
	t0  = last word written
	t8  = bitmask (with one bit set) indicating the last byte written
	t10 = bitmask (with one bit set) indicating the byte position of
	      the end of the range specified by COUNT
	a0  = unaligned address of the last *word* written
	a2  = the number of full words left in COUNT

   Furthermore, v0, a3-a5, t11, and t12 are untouched.
*/

#include <sysdep.h>

	.arch ev6
	.set noat
	.set noreorder

	.text
	.type	__stxncpy, @function
	.globl	__stxncpy
	.usepv	__stxncpy, no

	cfi_startproc
	cfi_return_column (t9)

	/* On entry to this basic block:
	   t0 == the first destination word for masking back in
	   t1 == the first source word.  */
	.align 4
stxncpy_aligned:
	/* Create the 1st output word and detect 0's in the 1st input word.  */
	lda	t2, -1		# E : build a mask against false zero
	mskqh	t2, a1, t2	# U :   detection in the src word (stall)
	mskqh	t1, a1, t3	# U :
	ornot	t1, t2, t2	# E : (stall)

	mskql	t0, a1, t0	# U : assemble the first output word
	cmpbge	zero, t2, t7	# E : bits set iff null found
	or	t0, t3, t0	# E : (stall)
	beq	a2, $a_eoc	# U :

	bne	t7, $a_eos	# U :
	nop
	nop
	nop

	/* On entry to this basic block:
	   t0 == a source word not containing a null.  */

	/*
	 * nops here to:
	 *	separate store quads from load quads
	 *	limit of 1 bcond/quad to permit training
	 */
$a_loop:
	stq_u	t0, 0(a0)	# L :
	addq	a0, 8, a0	# E :
	subq	a2, 1, a2	# E :
	nop

	ldq_u	t0, 0(a1)	# L :
	addq	a1, 8, a1	# E :
	cmpbge	zero, t0, t7	# E :
	beq	a2, $a_eoc      # U :

	beq	t7, $a_loop	# U :
	nop
	nop
	nop

	/* Take care of the final (partial) word store.  At this point
	   the end-of-count bit is set in t7 iff it applies.

	   On entry to this basic block we have:
	   t0 == the source word containing the null
	   t7 == the cmpbge mask that found it.  */
$a_eos:
	negq	t7, t8		# E : find low bit set
	and	t7, t8, t8	# E : (stall)
	/* For the sake of the cache, don't read a destination word
	   if we're not going to need it.  */
	and	t8, 0x80, t6	# E : (stall)
	bne	t6, 1f		# U : (stall)

	/* We're doing a partial word store and so need to combine
	   our source and original destination words.  */
	ldq_u	t1, 0(a0)	# L :
	subq	t8, 1, t6	# E :
	or	t8, t6, t7	# E : (stall)
	zapnot	t0, t7, t0	# U : clear src bytes > null (stall)

	zap	t1, t7, t1	# .. e1 : clear dst bytes <= null
	or	t0, t1, t0	# e1    : (stall)
	nop
	nop

1:	stq_u	t0, 0(a0)	# L :
	ret	(t9)		# L0 : Latency=3
	nop
	nop

	/* Add the end-of-count bit to the eos detection bitmask.  */
$a_eoc:
	or	t10, t7, t7	# E :
	br	$a_eos		# L0 : Latency=3
	nop
	nop

	.align 4
__stxncpy:
	/* Are source and destination co-aligned?  */
	lda	t2, -1		# E :
	xor	a0, a1, t1	# E :
	and	a0, 7, t0	# E : find dest misalignment
	nop			# E :

	srl	t2, 1, t2	# U :
	and	t1, 7, t1	# E :
	cmovlt	a2, t2, a2	# E : bound count to LONG_MAX (stall)
	nop			# E :

	addq	a2, t0, a2	# E : bias count by dest misalignment
	subq	a2, 1, a2	# E : (stall)
	and	a2, 7, t2	# E : (stall)
	lda	t10, 1		# E :

	srl	a2, 3, a2	# U : a2 = loop counter = (count - 1)/8
	sll	t10, t2, t10	# U : t10 = bitmask of last count byte
	nop			# E :
	bne	t1, $unaligned	# U : (stall)

	/* We are co-aligned; take care of a partial first word.  */
	ldq_u	t1, 0(a1)	# L : load first src word
	addq	a1, 8, a1	# E :
	beq	t0, stxncpy_aligned # U : avoid loading dest word if not needed
	ldq_u	t0, 0(a0)	# L :

	br	stxncpy_aligned	# U :
	nop
	nop
	nop



/* The source and destination are not co-aligned.  Align the destination
   and cope.  We have to be very careful about not reading too much and
   causing a SEGV.  */

	.align 4
$u_head:
	/* We know just enough now to be able to assemble the first
	   full source word.  We can still find a zero at the end of it
	   that prevents us from outputting the whole thing.

	   On entry to this basic block:
	   t0 == the first dest word, unmasked
	   t1 == the shifted low bits of the first source word
	   t6 == bytemask that is -1 in dest word bytes */

	ldq_u	t2, 8(a1)	# L : Latency=3 load second src word
	addq	a1, 8, a1	# E :
	mskql	t0, a0, t0	# U : mask trailing garbage in dst
	extqh	t2, a1, t4	# U : (3 cycle stall on t2)

	or	t1, t4, t1	# E : first aligned src word complete (stall)
	mskqh	t1, a0, t1	# U : mask leading garbage in src (stall)
	or	t0, t1, t0	# E : first output word complete (stall)
	or	t0, t6, t6	# E : mask original data for zero test (stall)

	cmpbge	zero, t6, t7	# E :
	beq	a2, $u_eocfin	# U :
	lda	t6, -1		# E :
	nop

	bne	t7, $u_final	# U :
	mskql	t6, a1, t6	# U : mask out bits already seen
	stq_u	t0, 0(a0)	# L : store first output word
	or      t6, t2, t2	# E :

	cmpbge	zero, t2, t7	# E : find nulls in second partial
	addq	a0, 8, a0	# E :
	subq	a2, 1, a2	# E :
	bne	t7, $u_late_head_exit	# U :

	/* Finally, we've got all the stupid leading edge cases taken care
	   of and we can set up to enter the main loop.  */
	extql	t2, a1, t1	# U : position hi-bits of lo word
	beq	a2, $u_eoc	# U :
	ldq_u	t2, 8(a1)	# L : read next high-order source word
	addq	a1, 8, a1	# E :

	extqh	t2, a1, t0	# U : position lo-bits of hi word (stall)
	cmpbge	zero, t2, t7	# E :
	nop
	bne	t7, $u_eos	# U :

	/* Unaligned copy main loop.  In order to avoid reading too much,
	   the loop is structured to detect zeros in aligned source words.
	   This has, unfortunately, effectively pulled half of a loop
	   iteration out into the head and half into the tail, but it does
	   prevent nastiness from accumulating in the very thing we want
	   to run as fast as possible.

	   On entry to this basic block:
	   t0 == the shifted low-order bits from the current source word
	   t1 == the shifted high-order bits from the previous source word
	   t2 == the unshifted current source word

	   We further know that t2 does not contain a null terminator.  */

	.align 4
$u_loop:
	or	t0, t1, t0	# E : current dst word now complete
	subq	a2, 1, a2	# E : decrement word count
	extql	t2, a1, t1	# U : extract high bits for next time
	addq	a0, 8, a0	# E :

	stq_u	t0, -8(a0)	# L : save the current word
	beq	a2, $u_eoc	# U :
	ldq_u	t2, 8(a1)	# L : Latency=3 load high word for next time
	addq	a1, 8, a1	# E :

	extqh	t2, a1, t0	# U : extract low bits (2 cycle stall)
	cmpbge	zero, t2, t7	# E : test new word for eos
	nop
	beq	t7, $u_loop	# U :

	/* We've found a zero somewhere in the source word we just read.
	   If it resides in the lower half, we have one (probably partial)
	   word to write out, and if it resides in the upper half, we
	   have one full and one partial word left to write out.

	   On entry to this basic block:
	   t0 == the shifted low-order bits from the current source word
	   t1 == the shifted high-order bits from the previous source word
	   t2 == the unshifted current source word.  */
$u_eos:
	or	t0, t1, t0	# E : first (partial) source word complete
	nop
	cmpbge	zero, t0, t7	# E : is the null in this first bit? (stall)
	bne	t7, $u_final	# U : (stall)

	stq_u	t0, 0(a0)	# L : the null was in the high-order bits
	addq	a0, 8, a0	# E :
	subq	a2, 1, a2	# E :
	nop

$u_late_head_exit:
	extql	t2, a1, t0	# U :
	cmpbge	zero, t0, t7	# E :
	or	t7, t10, t6	# E : (stall)
	cmoveq	a2, t6, t7	# E : Latency=2, extra map slot (stall)

	/* Take care of a final (probably partial) result word.
	   On entry to this basic block:
	   t0 == assembled source word
	   t7 == cmpbge mask that found the null.  */
$u_final:
	negq	t7, t6		# E : isolate low bit set
	and	t6, t7, t8	# E : (stall)
	and	t8, 0x80, t6	# E : avoid dest word load if we can (stall)
	bne	t6, 1f		# U : (stall)

	ldq_u	t1, 0(a0)	# L :
	subq	t8, 1, t6	# E :
	or	t6, t8, t7	# E : (stall)
	zapnot	t0, t7, t0	# U : kill source bytes > null

	zap	t1, t7, t1	# U : kill dest bytes <= null
	or	t0, t1, t0	# E : (stall)
	nop
	nop

1:	stq_u	t0, 0(a0)	# L :
	ret	(t9)		# L0 : Latency=3

        /* Got to end-of-count before end of string.
           On entry to this basic block:
           t1 == the shifted high-order bits from the previous source word  */
$u_eoc:
	and	a1, 7, t6	# E :
	sll	t10, t6, t6	# U : (stall)
	and	t6, 0xff, t6	# E : (stall)
	bne	t6, 1f		# U : (stall)

	ldq_u	t2, 8(a1)	# L : load final src word
	nop
	extqh	t2, a1, t0	# U : extract low bits for last word (stall)
	or	t1, t0, t1	# E : (stall)

1:	cmpbge	zero, t1, t7	# E :
	mov	t1, t0

$u_eocfin:			# end-of-count, final word
	or	t10, t7, t7	# E :
	br	$u_final	# L0 : Latency=3

	/* Unaligned copy entry point.  */
	.align 4
$unaligned:

	ldq_u	t1, 0(a1)	# L : load first source word
	and	a0, 7, t4	# E : find dest misalignment
	and	a1, 7, t5	# E : find src misalignment
	/* Conditionally load the first destination word and a bytemask
	   with 0xff indicating that the destination byte is sacrosanct.  */
	mov	zero, t0	# E :

	mov	zero, t6	# E :
	beq	t4, 1f		# U :
	ldq_u	t0, 0(a0)	# L :
	lda	t6, -1		# E :

	mskql	t6, a0, t6	# U :
	nop
	nop
1:	subq	a1, t4, a1	# E : sub dest misalignment from src addr

	/* If source misalignment is larger than dest misalignment, we need
	   extra startup checks to avoid SEGV.  */

	cmplt	t4, t5, t8	# E :
	extql	t1, a1, t1	# U : shift src into place
	lda	t2, -1		# E : for creating masks later
	beq	t8, $u_head	# U : (stall)

	mskqh	t2, t5, t2	# U : begin src byte validity mask
	cmpbge	zero, t1, t7	# E : is there a zero?
	extql	t2, a1, t2	# U :
	or	t7, t10, t5	# E : test for end-of-count too

	cmpbge	zero, t2, t3	# E :
	cmoveq	a2, t5, t7	# E : Latency=2, extra map slot
	nop			# E : keep with cmoveq
	andnot	t7, t3, t7	# E : (stall)

	beq	t7, $u_head	# U :
	/* At this point we've found a zero in the first partial word of
	   the source.  We need to isolate the valid source data and mask
	   it into the original destination data.  (Incidentally, we know
	   that we'll need at least one byte of that original dest word.) */
	ldq_u	t0, 0(a0)	# L :
	negq	t7, t6		# E : build bitmask of bytes <= zero
	mskqh	t1, t4, t1	# U :

	and	t6, t7, t8	# E :
	subq	t8, 1, t6	# E : (stall)
	or	t6, t8, t7	# E : (stall)
	zapnot	t2, t7, t2	# U : prepare source word; mirror changes (stall)

	zapnot	t1, t7, t1	# U : to source validity mask
	andnot	t0, t2, t0	# E : zero place for source to reside
	or	t0, t1, t0	# E : and put it there (stall both t0, t1)
	stq_u	t0, 0(a0)	# L : (stall)

	ret	(t9)		# L0 : Latency=3

	cfi_endproc
