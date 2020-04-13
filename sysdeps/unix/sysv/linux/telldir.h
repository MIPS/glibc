/* TODO
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

#ifndef _TELLDIR_H
#define _TELLDIR_H 1

#ifndef __LP64__
# define INDEX_BITS  31
# define OFFSET_BITS 12
# define SEEK_BITS   19

struct dirstream_loc
{
  off64_t  filepos;
};

union dirstream_packed
{
  long int l;
  struct
  {
    unsigned long is_packed:1;
    unsigned long index:INDEX_BITS;
  } i;
  struct
  {
    unsigned long is_packed:1;
    unsigned long offset:OFFSET_BITS;
    unsigned long seek:SEEK_BITS;
  } s;
};

_Static_assert (sizeof (long int) == sizeof (union dirstream_packed),
		"sizeof (long int) != sizeof (union dirstream_packed)");

# define DYNARRAY_STRUCT  dirstream_loc_t
# define DYNARRAY_ELEMENT struct dirstream_loc
# define DYNARRAY_PREFIX  dirstream_loc_
# include <malloc/dynarray-skeleton.c>
#endif /* __LP64__  */

#endif /* _TELLDIR_H  */
