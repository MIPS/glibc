/* Defines for bits in AT_HWCAP.
   Copyright (C) 2014 Free Software Foundation, Inc.
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

#ifndef _SYS_AUXV_H
# error "Never include <bits/hwcap.h> directly; use <sys/auxv.h> instead."
#endif

#define HWCAP_MIPS_FR1	0x00000001
#define HWCAP_MIPS_MSA	0x00000002
#define HWCAP_MIPS_FRE	0x00000004
#define HWCAP_MIPS_R6	0x00000008
