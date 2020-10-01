/* Architecture-specific glibc-hwcaps subdirectories.  s390x version.
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
   <https://www.gnu.org/licenses/>.  */

#include <dl-hwcaps.h>
#include <ldsodefs.h>

const char _dl_hwcaps_subdirs[] = "z15:z14:z13";

int32_t
_dl_hwcaps_subdirs_active (void)
{
  int32_t result = 0;
  int32_t bit = 1 << 2;

  /* Test in reverse preference order.  */

  /* z13.  */
  if (!(GLRO (dl_hwcap) & HWCAP_S390_VX))
    return result;
  result |= bit;
  bit >>= 1;

  /* z14.  */
  if (!((GLRO (dl_hwcap) & HWCAP_S390_VXD)
        && (GLRO (dl_hwcap) & HWCAP_S390_VXE)
        && (GLRO (dl_hwcap) & HWCAP_S390_GS)))
    return result;
  result |= bit;
  bit >>= 1;

  /* z15.  */
  if (!((GLRO (dl_hwcap) & HWCAP_S390_VXRS_EXT2)
        && (GLRO (dl_hwcap) & HWCAP_S390_VXRS_PDE)))
    return result;
  result |= bit;
  bit >>= 1;

  return result;
}
