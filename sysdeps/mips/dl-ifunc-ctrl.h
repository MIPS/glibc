/* Machine-dependent callback control function for ifunc resolver functions.
   Copyright (C) 2014 Free Software Foundation, Inc.
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

#ifndef _DL_IFUNC_CTRL_H
#define _DL_IFUNC_CTRL_H 1

#include <stdio.h>
#include <unistd.h>
#include <sgidefs.h>
#include <link.h>
#include <elf.h>
#include <ldsodefs.h>
#include <sys/dlifnctl.h>
#include <dl-machine-reject-phdr.h>

static int __attribute_used__
dl_ifunc_control (int operation, int value)
{
  int in_abi = -1;

  switch (operation)
    {
    case DL_IFUNC_MIPS_FR_MODE_SWITCH:
      if (value == 0)
	in_abi = Val_GNU_MIPS_ABI_FP_DOUBLE;
      else if (value == 1)
	in_abi = Val_GNU_MIPS_ABI_FP_64;
      else
	{
	  if (__glibc_unlikely (GLRO(dl_debug_mask) & DL_DEBUG_SYMBOLS))
	    GLRO(dl_debug_printf) ("  requested invalid FR mode: 0x%x\n",
				   value);
#if _MIPS_SIM == _ABIO32
# if HAVE_PRCTL_FP_MODE
	  return __prctl (PR_GET_FP_MODE);
# else
	  /* If the PR_GET_FP_MODE is not supported then only FR0
	     is available.  */
	  return 0;
# endif
#else
	  return 1;
#endif
	}
      /* Fall through.  */
    case DL_IFUNC_MIPS_FP_ABI:
      if (in_abi == -1)
	in_abi = value;

      /* Try the new requirement as if it was an object. */
      if (!dl_reject_fpabi_req (in_abi))
	{
	   /* Remember the choice for the last compabitle FP ABI.  */
	   ifunc_fpabi = in_abi;
	}
#if _MIPS_SIM == _ABIO32
# if HAVE_PRCTL_FP_MODE
      return __prctl (PR_GET_FP_MODE);
# else
      return 0;
# endif
#else
      return 1;
#endif
    default:
#ifdef SHARED
      GLRO(dl_debug_printf) ("unrecognized ifunc operation\n");
      _exit (127);
#else
      _dl_fatal_printf ("unrecognized ifunc operation in static binary\n");
#endif
      /* NOT REACHED */
    }
}
#endif
