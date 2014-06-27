/* Mips version of processor capability information handling macros.
   Copyright (C) 2007-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Robert Millan <rmh@gnu.org>.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _DL_PROCINFO_H
#define _DL_PROCINFO_H	1

#include <ldsodefs.h>


/* Mask to filter out platforms.  */
#define _DL_HWCAP_PLATFORM    (-1ULL)

#define _DL_PLATFORMS_COUNT   4

static inline const char *
__attribute__ ((unused))
_dl_platform_string (int idx)
{
  return GLRO(dl_mips_platforms)[idx];
};

static inline int
__attribute__ ((unused, always_inline))
_dl_string_platform (const char *str)
{
  int i;

  if (str != NULL)
    for (i = 0; i < _DL_PLATFORMS_COUNT; ++i)
      {
        if (strcmp (str, _dl_platform_string (i)) == 0)
          return i;
      }
  return -1;
};

#define _DL_HWCAP_COUNT	4

#define HWCAP_IMPORTANT         (HWCAP_MIPS_UFR | HWCAP_MIPS_MSA)

static inline int
__attribute__ ((unused))
_dl_procinfo (unsigned int type, unsigned long int word)
{
  int i;

  /* Fallback to unknown output mechanism.  */
  if (type == AT_HWCAP2)
    return -1;

  _dl_printf ("AT_HWCAP:   ");

  for (i = 0; i < _DL_HWCAP_COUNT; ++i)
    if (word & (1 << i))
      _dl_printf (" %s", GLRO(dl_mips_cap_flags)[i]);

  _dl_printf ("\n");

  return 0;
}

static inline const char *
__attribute__ ((unused))
_dl_hwcap_string (int idx)
{
  return GLRO(dl_mips_cap_flags)[idx];
};

static inline int
__attribute__ ((unused))
_dl_string_hwcap (const char *str)
{
  int i;

  for (i = 0; i < _DL_HWCAP_COUNT; i++)
    {
      if (strcmp (str, GLRO(dl_mips_cap_flags)[i]) == 0)
	return i;
    }
  return -1;
};

#endif /* dl-procinfo.h */
