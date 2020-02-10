/* Dynamic loading of the libgcc unwinder.  Arm overrides.
   Copyright (C) 2020 Free Software Foundation, Inc.
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

#ifndef _UNWIND_LINK_H

/* On arm, _Unwind_GetIP is a macro.  */
#define UNWIND_LINK_NO_GETIP
#define UNWIND_LINK_EXTRA_FIELDS \
  __typeof (_Unwind_VRS_Get) *ptr__Unwind_VRS_Get;
#define UNWIND_LINK_EXTRA_INIT                                  \
  result->ptr__Unwind_VRS_Get                                   \
    = __libc_dlsym (result->libgcc_handle, "_Unwind_VRS_Get");  \
  assert (result->ptr__Unwind_VRS_Get != NULL);                 \
  PTR_MANGLE (result->ptr__Unwind_VRS_Get);

#include <sysdeps/generic/unwind-link.h>

#endif /* _UNWIND_LINK_H */
