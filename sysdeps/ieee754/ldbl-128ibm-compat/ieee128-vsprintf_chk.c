/* Wrapper for __vsprintf_chk.  IEEE128 version.
   Copyright (C) 2019-2025 Free Software Foundation, Inc.
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

#include <libio/libioP.h>

extern int
___ieee128___vsprintf_chk (char *string, int flag, size_t slen,
			const char *format, va_list ap)
{
  unsigned int mode = PRINTF_LDBL_USES_FLOAT128;
  if (flag > 0)
    mode |= PRINTF_FORTIFY;

  /* Regardless of the value of flag, let __vsprintf_internal know that
     this is a call from *printf_chk.  */
  mode |= PRINTF_CHK;

  if (slen == 0)
    __chk_fail ();

  return __vsprintf_internal (string, slen, format, ap, mode);
}
hidden_def (___ieee128___vsprintf_chk)
strong_alias (___ieee128___vsprintf_chk, __vsprintf_chkieee128)
