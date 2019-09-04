/* Copyright (C) 2005-2019 Free Software Foundation, Inc.
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

#include <time.h>
#define HAVE_VSYSCALL
#include <dl-vdso.h>
#include <libc-vdso.h>
#include <dl-machine.h>

#ifdef SHARED
int
__gettimeofday_syscall (struct timeval *restrict tv,
			struct timezone *restrict tz)
{
  return INLINE_SYSCALL_CALL (gettimeofday, tv, tz);
}

# define INIT_ARCH()						\
  PREPARE_VERSION_KNOWN (linux2615, LINUX_2_6_15);		\
  void *vdso_gettimeofday = _dl_vdso_vsym ("__kernel_gettimeofday", &linux2615);

/* If the vDSO is not available we fall back syscall.  */
libc_ifunc (__gettimeofday,
	    vdso_gettimeofday
	    ? VDSO_IFUNC_RET (vdso_gettimeofday)
	    : (void *) __gettimeofday_syscall);
#else
int
__gettimeofday (struct timeval *restrict tv, struct timezone *restrict tz)
{
  return INLINE_VSYSCALL (gettimeofday, 2, tv, tz);
}
#endif
weak_alias (__gettimeofday, gettimeofday)
