/* Software floating-point emulation.
   Truncate IEEE half into bfloat16.
   Copyright (C) 2022-2025 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   In addition to the permissions in the GNU Lesser General Public
   License, the Free Software Foundation gives you unlimited
   permission to link the compiled version of this file into
   combinations with other programs, and to distribute those
   combinations without any restriction coming from the use of this
   file.  (The Lesser General Public License restrictions do apply in
   other respects; for example, they cover modification of the file,
   and distribution when not linked into a combine executable.)

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include "soft-fp.h"
#include "brain.h"
#include "half.h"
#include "single.h"

/* BFtype and HFtype are unordered, neither is a superset or subset
   of each other.  Convert HFtype to SFtype (lossless) and then
   truncate to BFtype.  */

BFtype
__trunchfbf2 (HFtype a)
{
  FP_DECL_EX;
  FP_DECL_H (A);
  FP_DECL_S (B);
  FP_DECL_B (R);
  SFtype b;
  BFtype r;

  FP_INIT_ROUNDMODE;
  FP_UNPACK_RAW_H (A, a);
  FP_EXTEND (S, H, 1, 1, B, A);
  FP_PACK_RAW_S (b, B);
  FP_UNPACK_SEMIRAW_S (B, b);
  FP_TRUNC (B, S, 1, 1, R, B);
  FP_PACK_SEMIRAW_B (r, R);
  FP_HANDLE_EXCEPTIONS;

  return r;
}
