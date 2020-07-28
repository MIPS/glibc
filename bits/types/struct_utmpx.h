/* The 'struct utmpx' type.
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

#ifndef _UTMPX_H
# error "Never include <bits/struct_utmpx.h> directly; use <utmpx.h> instead."
#endif

/* The structure describing an entry in the user accounting database.  */
struct utmpx
{
  short int ut_type;		/* Type of login.  */
  __pid_t ut_pid;		/* Process ID of login process.  */
  char ut_line[__UT_LINESIZE]
    __attribute_nonstring__;	/* Devicename.  */
  char ut_id[4]
    __attribute_nonstring__;	/* Inittab ID.  */
  char ut_user[__UT_NAMESIZE]
    __attribute_nonstring__;	/* Username.  */
  char ut_host[__UT_HOSTSIZE]
    __attribute_nonstring__;	/* Hostname for remote login.  */
  struct __exit_status ut_exit;	/* Exit status of a process marked
				   as DEAD_PROCESS.  */

/* The fields ut_session and ut_tv must be the same size when compiled
   32- and 64-bit.  This allows files and shared memory to be shared
   between 32- and 64-bit applications.  */
#if __WORDSIZE_TIME64_COMPAT32
  __int32_t ut_session;		/* Session ID, used for windowing.  */
  struct
  {
    __int32_t tv_sec;		/* Seconds.  */
    __int32_t tv_usec;		/* Microseconds.  */
  } ut_tv;			/* Time entry was made.  */
#else
  long int ut_session;		/* Session ID, used for windowing.  */
  struct timeval ut_tv;		/* Time entry was made.  */
#endif
  __int32_t ut_addr_v6[4];	/* Internet address of remote host.  */
  char __glibc_reserved[20];		/* Reserved for future use.  */
};
