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

#define AV_FLAGS_MIPS_GNU_STACK	(1 << 24) /* Non-executable stack supported. */

void
internal_function
_dl_stack_exec_override (void* flags)
{
  if ((*(ElfW(Word) *)flags & PF_X) == 0
      && (getauxval (AT_FLAGS) & AV_FLAGS_MIPS_GNU_STACK) == 0)
    {
#ifndef SHARED
      /* For static executable, we need to set stack permission here. */
      uintptr_t page = ((uintptr_t) __libc_stack_end
		    & -(intptr_t) GLRO(dl_pagesize));
      __mprotect ((void *) page, GLRO(dl_pagesize),
		  PROT_READ | PROT_WRITE | PROT_EXEC);
#endif /* !SHARED */
      *(ElfW(Word) *)flags |= PF_X;
    }

  return;
}
rtld_hidden_def (_dl_stack_exec_override)
