/* Support for reading /etc/ld.so.cache files written by Linux ldconfig.
   Copyright (C) 1999-2020 Free Software Foundation, Inc.
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

#ifndef _DL_CACHE_H
#define _DL_CACHE_H

#include <endian.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef _DL_CACHE_DEFAULT_ID
# define _DL_CACHE_DEFAULT_ID	3
#endif

#ifndef _dl_cache_check_flags
# define _dl_cache_check_flags(flags)			\
  ((flags) == 1 || (flags) == _DL_CACHE_DEFAULT_ID)
#endif

#ifndef LD_SO_CACHE
# define LD_SO_CACHE SYSCONFDIR "/ld.so.cache"
#endif

#ifndef add_system_dir
# define add_system_dir(dir) add_dir (dir)
#endif

#define CACHEMAGIC "ld.so-1.7.0"

/* libc5 and glibc 2.0/2.1 use the same format.  For glibc 2.2 another
   format has been added in a compatible way:
   The beginning of the string table is used for the new table:
	old_magic
	nlibs
	libs[0]
	...
	libs[nlibs-1]
	pad, new magic needs to be aligned
	     - this is string[0] for the old format
	new magic - this is string[0] for the new format
	newnlibs
	...
	newlibs[0]
	...
	newlibs[newnlibs-1]
	string 1
	string 2
	...
*/
struct file_entry
{
  int flags;		/* This is 1 for an ELF library.  */
  unsigned int key, value; /* String table indices.  */
};

struct cache_file
{
  char magic[sizeof CACHEMAGIC - 1];
  unsigned int nlibs;
  struct file_entry libs[0];
};

#define CACHEMAGIC_NEW "glibc-ld.so.cache"
#define CACHE_VERSION "1.1"
#define CACHEMAGIC_VERSION_NEW CACHEMAGIC_NEW CACHE_VERSION


struct file_entry_new
{
  int32_t flags;		/* This is 1 for an ELF library.  */
  uint32_t key, value;		/* String table indices.  */
  uint32_t osversion;		/* Required OS version.	 */
  uint64_t hwcap;		/* Hwcap entry.	 */
};

/* See flags member of struct cache_file_new below.  */
enum
  {
   cache_file_new_flags_endian = (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
				  ? 2 : 3)
  };

struct cache_file_new
{
  char magic[sizeof CACHEMAGIC_NEW - 1];
  char version[sizeof CACHE_VERSION - 1];
  uint32_t nlibs;		/* Number of entries.  */
  uint32_t len_strings;		/* Size of string table. */

  /* flags & 3 is used to indicate the endianness of the cache.
     0: no endianness information available
        (An old ldconfig version without endianness support wrote the file.)
     1: cache is invalid
     2: little endian
     3: big endian

     The remaining bits are unused and should be generated as zero and
     ignored by readers.  */
  uint8_t flags;

  uint8_t padding_unsed[3];	/* Not used, for future extensions.  */

  uint32_t unused[4];		/* Leave space for future extensions
				   and align to 8 byte boundary.  */
  struct file_entry_new libs[0]; /* Entries describing libraries.  */
  /* After this the string table of size len_strings is found.	*/
};

/* Returns false if *CACHE has the wrong endianness for this
   architecture, and true if the endianness matches (or is
   unknown).  */
static inline bool
cache_file_new_matches_endian (const struct cache_file_new *cache)
{
  /* A zero value for cache->flags means that no endianness
     information is available.  */
  return cache->flags == 0
    || (cache->flags & 3) == cache_file_new_flags_endian;
}


/* Used to align cache_file_new.  */
#define ALIGN_CACHE(addr)				\
(((addr) + __alignof__ (struct cache_file_new) -1)	\
 & (~(__alignof__ (struct cache_file_new) - 1)))

extern int _dl_cache_libcmp (const char *p1, const char *p2) attribute_hidden;

#endif /* _DL_CACHE_H */
