/* Copyright (C) 1997-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Mark Kettenis <kettenis@phys.uva.nl>, 1997.

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
#include <string.h>
#include <unistd.h>
#include <utmp-compat.h>
#include <shlib-compat.h>
#include "utmp-private.h"

#ifndef TRANSFORM_UTMP_FILE_NAME
# define TRANSFORM_UTMP_FILE_NAME(file_name)	\
  ((strcmp (file_name, _PATH_UTMP "x") == 0	\
    && __access (_PATH_UTMP "x", F_OK) != 0)	\
   ? _PATH_UTMP					\
   : ((strcmp (file_name, _PATH_WTMP "x") == 0	\
       && __access (_PATH_WTMP "x", F_OK) != 0)	\
      ? _PATH_WTMP				\
      : file_name))
#endif

void
__updwtmp (const char *wtmp_file, const struct utmp *utmp)
{
  const char *file_name = TRANSFORM_UTMP_FILE_NAME (wtmp_file);

  __libc_updwtmp (file_name, utmp);
}
libc_hidden_def (__updwtmp)
#if SHLIB_COMPAT(libc, GLIBC_2_0, UTMP_COMPAT_BASE)
versioned_symbol (libc, __updwtmp, updwtmp, UTMP_COMPAT_BASE);
#else
weak_alias (__updwtmp, updwtmp)
#endif
