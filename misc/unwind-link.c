/* Dynamic loading of the libgcc unwinder.
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

#ifdef SHARED

#include <allocate_once.h>
#include <assert.h>
#include <dlfcn.h>
#include <gnu/lib-names.h>
#include <unwind-link.h>

/* Used by allocate_one.  */
static void *place;

/* Open libgcc_s and look up the symbols required by glibc.  */
static void *
allocate_link (void *closure)
{
  struct unwind_link *result = malloc (sizeof (*result));
  if (result == NULL)
    return NULL;

  result->libgcc_handle = __libc_dlopen (LIBGCC_S_SO);
  if (result->libgcc_handle == NULL)
    {
      /* Assume that this is a transient failure.  */
      free (result);
      return NULL;
    }

  result->ptr__Unwind_Backtrace
    = __libc_dlsym (result->libgcc_handle, "_Unwind_Backtrace");
  result->ptr__Unwind_ForcedUnwind
    = __libc_dlsym (result->libgcc_handle, "_Unwind_ForcedUnwind");
  result->ptr__Unwind_GetCFA
    = __libc_dlsym (result->libgcc_handle, "_Unwind_GetCFA");
#ifndef UNWIND_LINK_NO_GETIP
  result->ptr__Unwind_GetIP
    = __libc_dlsym (result->libgcc_handle, "_Unwind_GetIP");
#endif
  result->ptr__Unwind_Resume
    = __libc_dlsym (result->libgcc_handle, "_Unwind_Resume");
  result->ptr_personality
    = __libc_dlsym (result->libgcc_handle, "__gcc_personality_v0");
#ifdef UNWIND_LINK_EXTRA_INIT
  UNWIND_LINK_EXTRA_INIT
#endif

  /* If a symbol is missing, libgcc_s has somehow been corrupted.  */
  assert (result->ptr__Unwind_Backtrace != NULL);
  assert (result->ptr__Unwind_ForcedUnwind != NULL);
  assert (result->ptr__Unwind_GetCFA != NULL);
#ifndef UNWIND_LINK_NO_GETIP
  assert (result->ptr__Unwind_GetIP != NULL);
#endif
  assert (result->ptr__Unwind_Resume != NULL);
  assert (result->ptr_personality != NULL);

#ifdef PTR_MANGLE
  PTR_MANGLE (result->ptr__Unwind_Backtrace);
  PTR_MANGLE (result->ptr__Unwind_ForcedUnwind);
  PTR_MANGLE (result->ptr__Unwind_GetCFA);
# ifndef UNWIND_LINK_NO_GETIP
  PTR_MANGLE (result->ptr__Unwind_GetIP);
# endif
  PTR_MANGLE (result->ptr__Unwind_Resume);
  PTR_MANGLE (result->ptr_personality);
#endif

  return result;
}

/* Deallocate the object after the race was lost.  */
static void
deallocate_link (void *closure, void *result)
{
  struct unwind_link *link = result;
  __libc_dlclose (link->libgcc_handle);
  free (link);
}

struct unwind_link *
__libc_unwind_link_get (void)
{
  return allocate_once (&place, allocate_link, deallocate_link, NULL);
}
libc_hidden_def (__libc_unwind_link_get)

void __libc_freeres_fn_section
__libc_unwind_link_freeres (void)
{
  if (place != NULL)
    {
      deallocate_link (NULL, place);
      place = NULL;
    }
}

#endif /* SHARED */
