/* Copyright (C) 2018-2019 Free Software Foundation, Inc.

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

/* Get the current time of day and timezone information,
   putting it into *tv and *tz.  If tz is null, *tz is not filled.
   Returns 0 on success, -1 on errors.  */

#include <sys/time.h>
#define HAVE_VSYSCALL
#include <dl-vdso.h>
#include <sysdep-vdso.h>

#ifdef SHARED
/* Used as a fallback in the ifunc resolver if VDSO is not available
   and for libc.so internal __gettimeofday calls.  */
int
__gettimeofday_vsyscall (struct timeval *restrict tv, void *restrict tz)
{
  return INLINE_SYSCALL_CALL (gettimeofday, tv, tz);
}

/* PREPARE_VERSION_KNOWN will need an __LP64__ ifdef when ILP32 support
   goes in.  See _libc_vdso_platform_setup in
   sysdeps/unix/sysv/linux/aarch64/init-first.c.  */

# undef INIT_ARCH
# define INIT_ARCH() \
	   PREPARE_VERSION_KNOWN (linux_version, LINUX_2_6_39); \
	   void *vdso_gettimeofday = \
	     _dl_vdso_vsym ("__kernel_gettimeofday", &linux_version);

libc_ifunc (__gettimeofday,
	    vdso_gettimeofday ?: (void *) __gettimeofday_vsyscall)
#else
int
__gettimeofday (struct timeval *restrict tv, void *restrict tz)
{
  return INLINE_VSYSCALL (gettimeofday, 2, tv, tz);
}
#endif
weak_alias (__gettimeofday, gettimeofday)
