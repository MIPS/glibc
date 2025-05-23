/* Return information about the filesystem on which FILE resides.
   Copyright (C) 1996-2025 Free Software Foundation, Inc.
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

#define __statfs __statfs_disable
#define statfs statfs_disable
#include <sys/statfs.h>
#include <sysdep.h>
#include <kernel_stat.h>
#undef __statfs
#undef statfs

/* Return information about the filesystem on which FILE resides.  */
int
__statfs64 (const char *file, struct statfs64 *buf)
{
#ifdef __NR_statfs64
  return INLINE_SYSCALL_CALL (statfs64, file, sizeof (*buf), buf);
#else
  return INLINE_SYSCALL_CALL (statfs, file, buf);
#endif
}
weak_alias (__statfs64, statfs64)

#if STATFS_IS_STATFS64
weak_alias (__statfs64, __statfs)
weak_alias (__statfs64, statfs)
libc_hidden_ver (__statfs64, __statfs)
#endif
