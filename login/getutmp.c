/* Copyright (C) 1999-2020 Free Software Foundation, Inc.
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

#include <string.h>
#include <utmp.h>
#include <stddef.h>
#define getutmpx __redirect_getutmpx
#include <utmpx.h>
#undef getutmpx
#include <utmp-compat.h>
#include <shlib-compat.h>

#define CHECK_SIZE_AND_OFFSET(field) \
  _Static_assert (sizeof ((struct utmp){0}.field)		\
		  == sizeof ((struct utmpx){0}.field),		\
		  "sizeof ((struct utmp){0}." #field " != "	\
		  "sizeof ((struct utmpx){0}" #field);	\
  _Static_assert (offsetof (struct utmp, field)			\
		  == offsetof (struct utmpx, field),		\
		  "offsetof (struct utmp, " #field ") != "	\
		  "offsetof (struct utmpx, " #field ")");

/* This ensure the getutmp to getutmpx alias is valid.  */
_Static_assert (sizeof (struct utmp) == sizeof (struct utmpx),
		"sizeof (struct utmp) != sizeof (struct utmpx)");
CHECK_SIZE_AND_OFFSET (ut_type)
CHECK_SIZE_AND_OFFSET (ut_pid)
CHECK_SIZE_AND_OFFSET (ut_line)
CHECK_SIZE_AND_OFFSET (ut_user)
CHECK_SIZE_AND_OFFSET (ut_id)
CHECK_SIZE_AND_OFFSET (ut_host)
CHECK_SIZE_AND_OFFSET (ut_tv)


/* Copy the information in UTMPX to UTMP. */
void
__getutmp (const struct utmpx *utmpx, struct utmp *utmp)
{
  memset (utmp, 0, sizeof (struct utmpx));
  utmp->ut_type = utmpx->ut_type;
  utmp->ut_pid = utmpx->ut_pid;
  memcpy (utmp->ut_line, utmpx->ut_line, sizeof (utmp->ut_line));
  memcpy (utmp->ut_user, utmpx->ut_user, sizeof (utmp->ut_user));
  memcpy (utmp->ut_id, utmpx->ut_id, sizeof (utmp->ut_id));
  memcpy (utmp->ut_host, utmpx->ut_host, sizeof (utmp->ut_host));
  utmp->ut_tv.tv_sec = utmpx->ut_tv.tv_sec;
  utmp->ut_tv.tv_usec = utmpx->ut_tv.tv_usec;
}

#if SHLIB_COMPAT(libc, GLIBC_2_0, UTMP_COMPAT_BASE)
versioned_symbol (libc, __getutmp, getutmp, UTMP_COMPAT_BASE);
strong_alias (__getutmp, __getutmpx)
versioned_symbol (libc, __getutmpx, getutmpx, UTMP_COMPAT_BASE);
#else
strong_alias (__getutmp, getutmp)
strong_alias (__getutmp, getutmpx)
#endif
