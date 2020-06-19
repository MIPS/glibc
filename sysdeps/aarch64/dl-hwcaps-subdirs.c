/* Architecture-specific glibc-hwcaps subdirectories.  aarch64 version.
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

const char _dl_hwcaps_subdirs[] = "atomics";

int32_t
_dl_hwcaps_subdirs_active (void)
{
  if (GLRO (dl_hwcap) & HWCAP_ATOMICS)
    return 1;

  return 0;
}
