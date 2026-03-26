/* Copyright (C) 2026 Free Software Foundation, Inc.
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

#include <fenv.h>
#include <math.h>
#include <math-tests.h>
#include <signal.h>
#include <stdlib.h>
#include <support/check.h>
#include <support/xunistd.h>

static int
do_test (void)
{
#ifdef FE_INVALID
  pid_t pid;

  if (!EXCEPTION_ENABLE_SUPPORTED (FE_INVALID))
    FAIL_UNSUPPORTED ("feenableexcept (FE_INVALID) not supported");

  pid = xfork ();
  if (pid == 0)
    {
      int r = feenableexcept (FE_INVALID);

      TEST_COMPARE (r, 0);
      fdiv (0.0, 0.0);
      _exit (0);
    }
  else
    {
      int status;
      xwaitpid (pid, &status, 0);

      TEST_VERIFY (WIFSIGNALED (status));
      TEST_COMPARE (WTERMSIG (status), SIGFPE);
    }

  return 0;
#else
  FAIL_UNSUPPORTED ("FE_INVALID is not defined");
#endif
}

#include <support/test-driver.c>
