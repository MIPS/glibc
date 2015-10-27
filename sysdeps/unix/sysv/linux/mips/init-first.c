/* Initialization code run first thing by the ELF startup code.  Linux/ARM.
   Copyright (C) 2015 Free Software Foundation, Inc.

   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef SHARED
#include <ldsodefs.h>

static inline void
_mips_stack_exec_override (void)
{
  /* Program requests a non-executable stack, but architecture does
     not support it.  */
  if ((_dl_stack_flags & PF_X) == 0 &&
      ! (getauxval (AT_FLAGS) & AT_FLAG_NOEXEC_BIT))
    {
      uintptr_t page = ((uintptr_t) __libc_stack_end
		    & -(intptr_t) GLRO(dl_pagesize));
      __mprotect ((void *) page, GLRO(dl_pagesize),
		  PROT_READ | PROT_WRITE | PROT_EXEC);
    }
}

# define STACK_EXEC_OVERRIDE _mips_stack_exec_override

#endif

#include <csu/init-first.c>
