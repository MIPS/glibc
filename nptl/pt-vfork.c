/* vfork ABI-compatibility entry points for libpthread.
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

#include <unistd.h>
#include <shlib-compat.h>

/* libpthread used to have its own vfork implementation that differed
   from libc's only in having a pointless micro-optimization.  There
   is no longer any use to having a separate copy in libpthread, but
   the historical ABI requires it.  For static linking, there is no
   need to provide anything here--the libc version will be linked in.
   For shared library ABI compatibility, there must be __vfork and
   vfork symbols in libpthread.so; so we define them using IFUNC to
   redirect to the libc function.  */

/* Note! If the architecture doesn't support IFUNC, then we need an
   alternate target-specific mechanism to implement this.  So we just
   assume IFUNC here and require that the target override this file
   if necessary.  */

#if !HAVE_IFUNC
# error
#endif

#if (SHLIB_COMPAT (libpthread, GLIBC_2_0, GLIBC_2_20) \
     || SHLIB_COMPAT (libpthread, GLIBC_2_1_2, GLIBC_2_20))

extern __typeof (vfork) __libc_vfork;   /* Defined in libc.  */

static __typeof (vfork) *
vfork_resolve (void)
{
  return &__libc_vfork;
}

void *vfork_ifunc(void *, const void *, size_t)
     __attribute__ ((ifunc ("vfork_resolve")));
#endif

#if SHLIB_COMPAT (libpthread, GLIBC_2_0, GLIBC_2_20)
compat_symbol (libpthread, vfork_ifunc, vfork, GLIBC_2_0);
#endif

#if SHLIB_COMPAT (libpthread, GLIBC_2_1_2, GLIBC_2_20)
strong_alias (vfork_ifunc, vfork_ifunc2)
compat_symbol (libpthread, vfork_ifunc2, __vfork, GLIBC_2_1_2);
#endif
