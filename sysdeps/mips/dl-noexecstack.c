/* Non-executable stack check for GNU dynamic linker.  MIPS specific
   version.
   Copyright (C) 2003-2015 Free Software Foundation, Inc.
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

#include <elf.h>
#include <ldsodefs.h>
#include <errno.h>
#include <misc/sys/auxv.h>

/* FIXME: Temporary pattern for testing */
#define AT_FLAG_NOEXEC_BIT 0x1

bool
internal_function
_dl_allow_noexec_stack_p (void)
{
  if ((getauxval (AT_FLAGS) & AT_FLAG_NOEXEC_BIT) == 0)
    return false;
  else
    return true;
}
rtld_hidden_def (_dl_allow_noexec_stack_p)
