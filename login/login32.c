/* Write utmp and wtmp entries, 32-bit time compat version.
   Copyright (C) 2008-2020 Free Software Foundation, Inc.
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

#include <utmp.h>
#include <utmp-compat.h>
#include <shlib-compat.h>

#include "utmp32.h"
#include "utmp-convert.h"

#if SHLIB_COMPAT(libutil, GLIBC_2_0, UTMP_COMPAT_BASE)
/* Write the given entry into utmp and wtmp.  */
void
login32 (const struct utmp32 *entry)
{
  struct utmp in64;

  utmp_convert32to64 (entry, &in64);
  __login (&in64);
}

compat_symbol (libutil, login32, login, GLIBC_2_0);
#endif
