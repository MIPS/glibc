/* Set floating-point environment exception handling.
   Copyright (C) 1997-2025 Free Software Foundation, Inc.
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

#include <fenv.h>
#include <shlib-compat.h>

int
__fesetexceptflag (const fexcept_t *flagp, int excepts)
{
  /* This always succeeds, as all exceptions are always clear
     (including in the saved state) so nothing needs to be done.  */
  return 0;
}
#if SHLIB_COMPAT (libm, GLIBC_2_1, GLIBC_2_2)
strong_alias (__fesetexceptflag, __old_fesetexceptflag)
compat_symbol (libm, __old_fesetexceptflag, fesetexceptflag, GLIBC_2_1);
#endif
versioned_symbol (libm, __fesetexceptflag, fesetexceptflag, GLIBC_2_2);

stub_warning (fesetexceptflag)
