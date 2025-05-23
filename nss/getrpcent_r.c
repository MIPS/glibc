/* Copyright (C) 1996-2025 Free Software Foundation, Inc.
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

#include <netdb.h>


#define LOOKUP_TYPE		struct rpcent
#define SETFUNC_NAME		setrpcent
#define	GETFUNC_NAME		getrpcent
#define	ENDFUNC_NAME		endrpcent
#define DATABASE_NAME		rpc
#define STAYOPEN		int stayopen
#define STAYOPEN_VAR		stayopen

/* There is no nscd support for the rpc file.  */
#undef	USE_NSCD

#include "../nss/getXXent_r.c"
