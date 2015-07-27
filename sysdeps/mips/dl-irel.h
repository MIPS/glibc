/* Machine-dependent ELF indirect relocation inline functions.
   MIPS version.
   Copyright (C) 2009-2013 Free Software Foundation, Inc.
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
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _DL_IREL_H
#define _DL_IREL_H

#include <stdio.h>
#include <unistd.h>
#include <sgidefs.h>
#include <link.h>
#include <elf.h>
#include <ldsodefs.h>
#include <dl-ifunc-ctrl.h>

#define ELF_MACHINE_IREL	1

static inline ElfW(Addr)
__attribute ((always_inline))
elf_ifunc_invoke (ElfW(Addr) addr)
{
#if 1
  /* Print some debugging info if wanted.  */
  if (__glibc_unlikely (GLRO(dl_debug_mask) & DL_DEBUG_SYMBOLS))
    {
      ElfW(Addr) t_addr =
	((ElfW(Addr) (*) (unsigned long int)) (addr)) (GLRO(dl_hwcap));
      GLRO(dl_debug_printf) ("In elf_ifunc_invoke(0x%lx), return(0x%lx)\n",
			      (unsigned long int)addr,
			      (unsigned long int)t_addr);
    }
#endif

  return ((ElfW(Addr) (*) (unsigned long int, int (int, int))) addr)
	    (GLRO(dl_hwcap), dl_ifunc_control);
}

/* Allow either R_MIPS_RELATIVE or the nop R_MIPS_NONE */
static inline void
__attribute ((always_inline))
elf_irel (const ElfW(Rel) *reloc)
{
  ElfW(Addr) *const reloc_addr = (void *) reloc->r_offset;
  const unsigned long int r_type = ELFW(R_TYPE) (reloc->r_info);

  if (__builtin_expect (r_type == R_MIPS_IRELATIVE, 1))
    *reloc_addr = elf_ifunc_invoke (*reloc_addr);
  else if (r_type)
     __libc_fatal ("unexpected reloc type in static binary\n");
}

#endif /* dl-irel.h */
