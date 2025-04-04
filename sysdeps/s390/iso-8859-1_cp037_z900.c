/* Conversion between ISO 8859-1 and IBM037.

   This module uses the translate instruction.
   Copyright (C) 1997-2025 Free Software Foundation, Inc.

   This is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#include <dlfcn.h>
#include <stdint.h>

// conversion table from ISO-8859-1 to IBM037
static const unsigned char table_iso8859_1_to_cp037[256]
__attribute__ ((aligned (8))) =
{
  [0x00] = 0x00, [0x01] = 0x01, [0x02] = 0x02, [0x03] = 0x03,
  [0x04] = 0x37, [0x05] = 0x2D, [0x06] = 0x2E, [0x07] = 0x2F,
  [0x08] = 0x16, [0x09] = 0x05, [0x0A] = 0x25, [0x0B] = 0x0B,
  [0x0C] = 0x0C, [0x0D] = 0x0D, [0x0E] = 0x0E, [0x0F] = 0x0F,
  [0x10] = 0x10, [0x11] = 0x11, [0x12] = 0x12, [0x13] = 0x13,
  [0x14] = 0x3C, [0x15] = 0x3D, [0x16] = 0x32, [0x17] = 0x26,
  [0x18] = 0x18, [0x19] = 0x19, [0x1A] = 0x3F, [0x1B] = 0x27,
  [0x1C] = 0x1C, [0x1D] = 0x1D, [0x1E] = 0x1E, [0x1F] = 0x1F,
  [0x20] = 0x40, [0x21] = 0x5A, [0x22] = 0x7F, [0x23] = 0x7B,
  [0x24] = 0x5B, [0x25] = 0x6C, [0x26] = 0x50, [0x27] = 0x7D,
  [0x28] = 0x4D, [0x29] = 0x5D, [0x2A] = 0x5C, [0x2B] = 0x4E,
  [0x2C] = 0x6B, [0x2D] = 0x60, [0x2E] = 0x4B, [0x2F] = 0x61,
  [0x30] = 0xF0, [0x31] = 0xF1, [0x32] = 0xF2, [0x33] = 0xF3,
  [0x34] = 0xF4, [0x35] = 0xF5, [0x36] = 0xF6, [0x37] = 0xF7,
  [0x38] = 0xF8, [0x39] = 0xF9, [0x3A] = 0x7A, [0x3B] = 0x5E,
  [0x3C] = 0x4C, [0x3D] = 0x7E, [0x3E] = 0x6E, [0x3F] = 0x6F,
  [0x40] = 0x7C, [0x41] = 0xC1, [0x42] = 0xC2, [0x43] = 0xC3,
  [0x44] = 0xC4, [0x45] = 0xC5, [0x46] = 0xC6, [0x47] = 0xC7,
  [0x48] = 0xC8, [0x49] = 0xC9, [0x4A] = 0xD1, [0x4B] = 0xD2,
  [0x4C] = 0xD3, [0x4D] = 0xD4, [0x4E] = 0xD5, [0x4F] = 0xD6,
  [0x50] = 0xD7, [0x51] = 0xD8, [0x52] = 0xD9, [0x53] = 0xE2,
  [0x54] = 0xE3, [0x55] = 0xE4, [0x56] = 0xE5, [0x57] = 0xE6,
  [0x58] = 0xE7, [0x59] = 0xE8, [0x5A] = 0xE9, [0x5B] = 0xBA,
  [0x5C] = 0xE0, [0x5D] = 0xBB, [0x5E] = 0xB0, [0x5F] = 0x6D,
  [0x60] = 0x79, [0x61] = 0x81, [0x62] = 0x82, [0x63] = 0x83,
  [0x64] = 0x84, [0x65] = 0x85, [0x66] = 0x86, [0x67] = 0x87,
  [0x68] = 0x88, [0x69] = 0x89, [0x6A] = 0x91, [0x6B] = 0x92,
  [0x6C] = 0x93, [0x6D] = 0x94, [0x6E] = 0x95, [0x6F] = 0x96,
  [0x70] = 0x97, [0x71] = 0x98, [0x72] = 0x99, [0x73] = 0xA2,
  [0x74] = 0xA3, [0x75] = 0xA4, [0x76] = 0xA5, [0x77] = 0xA6,
  [0x78] = 0xA7, [0x79] = 0xA8, [0x7A] = 0xA9, [0x7B] = 0xC0,
  [0x7C] = 0x4F, [0x7D] = 0xD0, [0x7E] = 0xA1, [0x7F] = 0x07,
  [0x80] = 0x20, [0x81] = 0x21, [0x82] = 0x22, [0x83] = 0x23,
  [0x84] = 0x24, [0x85] = 0x15, [0x86] = 0x06, [0x87] = 0x17,
  [0x88] = 0x28, [0x89] = 0x29, [0x8A] = 0x2A, [0x8B] = 0x2B,
  [0x8C] = 0x2C, [0x8D] = 0x09, [0x8E] = 0x0A, [0x8F] = 0x1B,
  [0x90] = 0x30, [0x91] = 0x31, [0x92] = 0x1A, [0x93] = 0x33,
  [0x94] = 0x34, [0x95] = 0x35, [0x96] = 0x36, [0x97] = 0x08,
  [0x98] = 0x38, [0x99] = 0x39, [0x9A] = 0x3A, [0x9B] = 0x3B,
  [0x9C] = 0x04, [0x9D] = 0x14, [0x9E] = 0x3E, [0x9F] = 0xFF,
  [0xA0] = 0x41, [0xA1] = 0xAA, [0xA2] = 0x4A, [0xA3] = 0xB1,
  [0xA4] = 0x9F, [0xA5] = 0xB2, [0xA6] = 0x6A, [0xA7] = 0xB5,
  [0xA8] = 0xBD, [0xA9] = 0xB4, [0xAA] = 0x9A, [0xAB] = 0x8A,
  [0xAC] = 0x5F, [0xAD] = 0xCA, [0xAE] = 0xAF, [0xAF] = 0xBC,
  [0xB0] = 0x90, [0xB1] = 0x8F, [0xB2] = 0xEA, [0xB3] = 0xFA,
  [0xB4] = 0xBE, [0xB5] = 0xA0, [0xB6] = 0xB6, [0xB7] = 0xB3,
  [0xB8] = 0x9D, [0xB9] = 0xDA, [0xBA] = 0x9B, [0xBB] = 0x8B,
  [0xBC] = 0xB7, [0xBD] = 0xB8, [0xBE] = 0xB9, [0xBF] = 0xAB,
  [0xC0] = 0x64, [0xC1] = 0x65, [0xC2] = 0x62, [0xC3] = 0x66,
  [0xC4] = 0x63, [0xC5] = 0x67, [0xC6] = 0x9E, [0xC7] = 0x68,
  [0xC8] = 0x74, [0xC9] = 0x71, [0xCA] = 0x72, [0xCB] = 0x73,
  [0xCC] = 0x78, [0xCD] = 0x75, [0xCE] = 0x76, [0xCF] = 0x77,
  [0xD0] = 0xAC, [0xD1] = 0x69, [0xD2] = 0xED, [0xD3] = 0xEE,
  [0xD4] = 0xEB, [0xD5] = 0xEF, [0xD6] = 0xEC, [0xD7] = 0xBF,
  [0xD8] = 0x80, [0xD9] = 0xFD, [0xDA] = 0xFE, [0xDB] = 0xFB,
  [0xDC] = 0xFC, [0xDD] = 0xAD, [0xDE] = 0xAE, [0xDF] = 0x59,
  [0xE0] = 0x44, [0xE1] = 0x45, [0xE2] = 0x42, [0xE3] = 0x46,
  [0xE4] = 0x43, [0xE5] = 0x47, [0xE6] = 0x9C, [0xE7] = 0x48,
  [0xE8] = 0x54, [0xE9] = 0x51, [0xEA] = 0x52, [0xEB] = 0x53,
  [0xEC] = 0x58, [0xED] = 0x55, [0xEE] = 0x56, [0xEF] = 0x57,
  [0xF0] = 0x8C, [0xF1] = 0x49, [0xF2] = 0xCD, [0xF3] = 0xCE,
  [0xF4] = 0xCB, [0xF5] = 0xCF, [0xF6] = 0xCC, [0xF7] = 0xE1,
  [0xF8] = 0x70, [0xF9] = 0xDD, [0xFA] = 0xDE, [0xFB] = 0xDB,
  [0xFC] = 0xDC, [0xFD] = 0x8D, [0xFE] = 0x8E, [0xFF] = 0xDF
};

// conversion table from IBM037 to ISO-8859-1
static const unsigned char table_cp037_iso8859_1[256]
__attribute__ ((aligned (8))) =
{
  [0x00] = 0x00, [0x01] = 0x01, [0x02] = 0x02, [0x03] = 0x03,
  [0x04] = 0x9C, [0x05] = 0x09, [0x06] = 0x86, [0x07] = 0x7F,
  [0x08] = 0x97, [0x09] = 0x8D, [0x0A] = 0x8E, [0x0B] = 0x0B,
  [0x0C] = 0x0C, [0x0D] = 0x0D, [0x0E] = 0x0E, [0x0F] = 0x0F,
  [0x10] = 0x10, [0x11] = 0x11, [0x12] = 0x12, [0x13] = 0x13,
  [0x14] = 0x9D, [0x15] = 0x85, [0x16] = 0x08, [0x17] = 0x87,
  [0x18] = 0x18, [0x19] = 0x19, [0x1A] = 0x92, [0x1B] = 0x8F,
  [0x1C] = 0x1C, [0x1D] = 0x1D, [0x1E] = 0x1E, [0x1F] = 0x1F,
  [0x20] = 0x80, [0x21] = 0x81, [0x22] = 0x82, [0x23] = 0x83,
  [0x24] = 0x84, [0x25] = 0x0A, [0x26] = 0x17, [0x27] = 0x1B,
  [0x28] = 0x88, [0x29] = 0x89, [0x2A] = 0x8A, [0x2B] = 0x8B,
  [0x2C] = 0x8C, [0x2D] = 0x05, [0x2E] = 0x06, [0x2F] = 0x07,
  [0x30] = 0x90, [0x31] = 0x91, [0x32] = 0x16, [0x33] = 0x93,
  [0x34] = 0x94, [0x35] = 0x95, [0x36] = 0x96, [0x37] = 0x04,
  [0x38] = 0x98, [0x39] = 0x99, [0x3A] = 0x9A, [0x3B] = 0x9B,
  [0x3C] = 0x14, [0x3D] = 0x15, [0x3E] = 0x9E, [0x3F] = 0x1A,
  [0x40] = 0x20, [0x41] = 0xA0, [0x42] = 0xE2, [0x43] = 0xE4,
  [0x44] = 0xE0, [0x45] = 0xE1, [0x46] = 0xE3, [0x47] = 0xE5,
  [0x48] = 0xE7, [0x49] = 0xF1, [0x4A] = 0xA2, [0x4B] = 0x2E,
  [0x4C] = 0x3C, [0x4D] = 0x28, [0x4E] = 0x2B, [0x4F] = 0x7C,
  [0x50] = 0x26, [0x51] = 0xE9, [0x52] = 0xEA, [0x53] = 0xEB,
  [0x54] = 0xE8, [0x55] = 0xED, [0x56] = 0xEE, [0x57] = 0xEF,
  [0x58] = 0xEC, [0x59] = 0xDF, [0x5A] = 0x21, [0x5B] = 0x24,
  [0x5C] = 0x2A, [0x5D] = 0x29, [0x5E] = 0x3B, [0x5F] = 0xAC,
  [0x60] = 0x2D, [0x61] = 0x2F, [0x62] = 0xC2, [0x63] = 0xC4,
  [0x64] = 0xC0, [0x65] = 0xC1, [0x66] = 0xC3, [0x67] = 0xC5,
  [0x68] = 0xC7, [0x69] = 0xD1, [0x6A] = 0xA6, [0x6B] = 0x2C,
  [0x6C] = 0x25, [0x6D] = 0x5F, [0x6E] = 0x3E, [0x6F] = 0x3F,
  [0x70] = 0xF8, [0x71] = 0xC9, [0x72] = 0xCA, [0x73] = 0xCB,
  [0x74] = 0xC8, [0x75] = 0xCD, [0x76] = 0xCE, [0x77] = 0xCF,
  [0x78] = 0xCC, [0x79] = 0x60, [0x7A] = 0x3A, [0x7B] = 0x23,
  [0x7C] = 0x40, [0x7D] = 0x27, [0x7E] = 0x3D, [0x7F] = 0x22,
  [0x80] = 0xD8, [0x81] = 0x61, [0x82] = 0x62, [0x83] = 0x63,
  [0x84] = 0x64, [0x85] = 0x65, [0x86] = 0x66, [0x87] = 0x67,
  [0x88] = 0x68, [0x89] = 0x69, [0x8A] = 0xAB, [0x8B] = 0xBB,
  [0x8C] = 0xF0, [0x8D] = 0xFD, [0x8E] = 0xFE, [0x8F] = 0xB1,
  [0x90] = 0xB0, [0x91] = 0x6A, [0x92] = 0x6B, [0x93] = 0x6C,
  [0x94] = 0x6D, [0x95] = 0x6E, [0x96] = 0x6F, [0x97] = 0x70,
  [0x98] = 0x71, [0x99] = 0x72, [0x9A] = 0xAA, [0x9B] = 0xBA,
  [0x9C] = 0xE6, [0x9D] = 0xB8, [0x9E] = 0xC6, [0x9F] = 0xA4,
  [0xA0] = 0xB5, [0xA1] = 0x7E, [0xA2] = 0x73, [0xA3] = 0x74,
  [0xA4] = 0x75, [0xA5] = 0x76, [0xA6] = 0x77, [0xA7] = 0x78,
  [0xA8] = 0x79, [0xA9] = 0x7A, [0xAA] = 0xA1, [0xAB] = 0xBF,
  [0xAC] = 0xD0, [0xAD] = 0xDD, [0xAE] = 0xDE, [0xAF] = 0xAE,
  [0xB0] = 0x5E, [0xB1] = 0xA3, [0xB2] = 0xA5, [0xB3] = 0xB7,
  [0xB4] = 0xA9, [0xB5] = 0xA7, [0xB6] = 0xB6, [0xB7] = 0xBC,
  [0xB8] = 0xBD, [0xB9] = 0xBE, [0xBA] = 0x5B, [0xBB] = 0x5D,
  [0xBC] = 0xAF, [0xBD] = 0xA8, [0xBE] = 0xB4, [0xBF] = 0xD7,
  [0xC0] = 0x7B, [0xC1] = 0x41, [0xC2] = 0x42, [0xC3] = 0x43,
  [0xC4] = 0x44, [0xC5] = 0x45, [0xC6] = 0x46, [0xC7] = 0x47,
  [0xC8] = 0x48, [0xC9] = 0x49, [0xCA] = 0xAD, [0xCB] = 0xF4,
  [0xCC] = 0xF6, [0xCD] = 0xF2, [0xCE] = 0xF3, [0xCF] = 0xF5,
  [0xD0] = 0x7D, [0xD1] = 0x4A, [0xD2] = 0x4B, [0xD3] = 0x4C,
  [0xD4] = 0x4D, [0xD5] = 0x4E, [0xD6] = 0x4F, [0xD7] = 0x50,
  [0xD8] = 0x51, [0xD9] = 0x52, [0xDA] = 0xB9, [0xDB] = 0xFB,
  [0xDC] = 0xFC, [0xDD] = 0xF9, [0xDE] = 0xFA, [0xDF] = 0xFF,
  [0xE0] = 0x5C, [0xE1] = 0xF7, [0xE2] = 0x53, [0xE3] = 0x54,
  [0xE4] = 0x55, [0xE5] = 0x56, [0xE6] = 0x57, [0xE7] = 0x58,
  [0xE8] = 0x59, [0xE9] = 0x5A, [0xEA] = 0xB2, [0xEB] = 0xD4,
  [0xEC] = 0xD6, [0xED] = 0xD2, [0xEE] = 0xD3, [0xEF] = 0xD5,
  [0xF0] = 0x30, [0xF1] = 0x31, [0xF2] = 0x32, [0xF3] = 0x33,
  [0xF4] = 0x34, [0xF5] = 0x35, [0xF6] = 0x36, [0xF7] = 0x37,
  [0xF8] = 0x38, [0xF9] = 0x39, [0xFA] = 0xB3, [0xFB] = 0xDB,
  [0xFC] = 0xDC, [0xFD] = 0xD9, [0xFE] = 0xDA, [0xFF] = 0x9F
};

/* Definitions used in the body of the `gconv' function.  */
#define CHARSET_NAME		"ISO-8859-1//"
#define FROM_LOOP		iso8859_1_to_cp037_z900
#define TO_LOOP			cp037_to_iso8859_1_z900
#define DEFINE_INIT		1
#define DEFINE_FINI		1
#define MIN_NEEDED_FROM		1
#define MIN_NEEDED_TO		1

# if defined __s390x__
#  define BRANCH_ON_COUNT(REG,LBL) "brctg %" #REG "," #LBL "\n\t"
# else
#  define BRANCH_ON_COUNT(REG,LBL) "brct %" #REG "," #LBL "\n\t"
# endif

#define TR_LOOP(TABLE)							\
  {									\
    size_t length = (inend - inptr < outend - outptr			\
		     ? inend - inptr : outend - outptr);		\
									\
    /* Process in 256 byte blocks.  */					\
    if (__builtin_expect (length >= 256, 0))				\
      {									\
	size_t blocks = length / 256;					\
	__asm__ __volatile__("0: mvc 0(256,%[R_OUT]),0(%[R_IN])\n\t"	\
			     "   tr 0(256,%[R_OUT]),0(%[R_TBL])\n\t"	\
			     "   la %[R_IN],256(%[R_IN])\n\t"		\
			     "   la %[R_OUT],256(%[R_OUT])\n\t"		\
			     BRANCH_ON_COUNT ([R_LI], 0b)		\
			     : /* outputs */ [R_IN] "+a" (inptr)	\
			       , [R_OUT] "+a" (outptr), [R_LI] "+d" (blocks) \
			     : /* inputs */ [R_TBL] "a" (TABLE)		\
			     : /* clobber list */ "memory"		\
			     );						\
	length = length % 256;						\
      }									\
									\
    /* Process remaining 0...248 bytes in 8byte blocks.  */		\
    if (length >= 8)							\
      {									\
	size_t blocks = length / 8;					\
	for (int i = 0; i < blocks; i++)				\
	  {								\
	    outptr[0] = TABLE[inptr[0]];				\
	    outptr[1] = TABLE[inptr[1]];				\
	    outptr[2] = TABLE[inptr[2]];				\
	    outptr[3] = TABLE[inptr[3]];				\
	    outptr[4] = TABLE[inptr[4]];				\
	    outptr[5] = TABLE[inptr[5]];				\
	    outptr[6] = TABLE[inptr[6]];				\
	    outptr[7] = TABLE[inptr[7]];				\
	    inptr += 8;							\
	    outptr += 8;						\
	  }								\
	length = length % 8;						\
      }									\
									\
    /* Process remaining 0...7 bytes.  */				\
    switch (length)							\
      {									\
      case 7: outptr[6] = TABLE[inptr[6]];				\
	/* Fall through.  */						\
      case 6: outptr[5] = TABLE[inptr[5]];				\
	/* Fall through.  */						\
      case 5: outptr[4] = TABLE[inptr[4]];				\
	/* Fall through.  */						\
      case 4: outptr[3] = TABLE[inptr[3]];				\
	/* Fall through.  */						\
      case 3: outptr[2] = TABLE[inptr[2]];				\
	/* Fall through.  */						\
      case 2: outptr[1] = TABLE[inptr[1]];				\
	/* Fall through.  */						\
      case 1: outptr[0] = TABLE[inptr[0]];				\
	/* Fall through.  */						\
      case 0: break;							\
      }									\
    inptr += length;							\
    outptr += length;							\
  }


/* First define the conversion function from ISO 8859-1 to CP037.  */
#define MIN_NEEDED_INPUT	MIN_NEEDED_FROM
#define MIN_NEEDED_OUTPUT	MIN_NEEDED_TO
#define LOOPFCT			FROM_LOOP
#define BODY			TR_LOOP (table_iso8859_1_to_cp037)

#include <iconv/loop.c>


/* Next, define the conversion function from CP037 to ISO 8859-1.  */
#define MIN_NEEDED_INPUT	MIN_NEEDED_TO
#define MIN_NEEDED_OUTPUT	MIN_NEEDED_FROM
#define LOOPFCT			TO_LOOP
#define BODY			TR_LOOP (table_cp037_iso8859_1);

#include <iconv/loop.c>


/* Now define the toplevel functions.  */
#include <iconv/skeleton.c>
