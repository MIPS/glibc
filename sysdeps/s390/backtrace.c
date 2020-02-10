/* Return backtrace of current program state.
   Copyright (C) 2000-2020 Free Software Foundation, Inc.
   Contributed by Martin Schwidefsky <schwidefsky@de.ibm.com>.
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

#include <libc-lock.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <stddef.h>
#include <stdlib.h>
#include <unwind.h>

struct trace_arg
{
  void **array;
  int cnt, size;
};

#ifdef SHARED
static _Unwind_Reason_Code (*unwind_backtrace) (_Unwind_Trace_Fn, void *);
static _Unwind_Ptr (*unwind_getip) (struct _Unwind_Context *);

static void
init (void)
{
  void *handle = __libc_dlopen ("libgcc_s.so.1");

  if (handle == NULL)
    return;

  unwind_backtrace = __libc_dlsym (handle, "_Unwind_Backtrace");
  unwind_getip = __libc_dlsym (handle, "_Unwind_GetIP");
  if (unwind_getip == NULL)
    unwind_backtrace = NULL;
}
#else
# define unwind_backtrace _Unwind_Backtrace
# define unwind_getip _Unwind_GetIP
#endif

static _Unwind_Reason_Code
backtrace_helper (struct _Unwind_Context *ctx, void *a)
{
  struct trace_arg *arg = a;

  /* We are first called with address in the __backtrace function.
     Skip it.  */
  if (arg->cnt != -1)
    arg->array[arg->cnt] = (void *) unwind_getip (ctx);
  if (++arg->cnt == arg->size)
    return _URC_END_OF_STACK;
  return _URC_NO_REASON;
}

int
__backtrace (void **array, int size)
{
  struct trace_arg arg = { .array = array, .size = size, .cnt = -1 };

  if (size <= 0)
    return 0;

#ifdef SHARED
  __libc_once_define (static, once);

  __libc_once (once, init);

  if (unwind_backtrace == NULL)
    return 0;
#endif

  unwind_backtrace (backtrace_helper, &arg);

  return arg.cnt != -1 ? arg.cnt : 0;
}

weak_alias (__backtrace, backtrace)
libc_hidden_def (__backtrace)
