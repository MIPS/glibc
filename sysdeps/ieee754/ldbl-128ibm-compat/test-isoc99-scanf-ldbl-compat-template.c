/* Test for the long double variants of __isoc99_*scanf functions.
   Copyright (C) 2018 Free Software Foundation, Inc.
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

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include <support/check.h>

#define CLEAR_VARGS							\
  va_start (args, format);						\
  ldptr = va_arg (args, long double *);					\
  fptr = va_arg (args, float *);					\
  *ldptr = 0;								\
  *fptr = 0;								\
  va_end (args);

#define CHECK_VARGS							\
  va_start (args, format);						\
  ldptr = va_arg (args, long double *);					\
  fptr = va_arg (args, float *);					\
  va_end (args);							\
  if (*ldptr == -1 && *fptr == -2)					\
    printf ("OK");							\
  else									\
    printf ("ERROR (%Lf %f)", *ldptr, *fptr);				\
  printf ("\n");

#define CLEAR_VALUE							\
  ld = 0;								\
  f = 0;

#define CHECK_VALUE							\
  if (ld == -1 && f == -2)						\
    printf ("OK");							\
  else									\
    printf ("ERROR (%Lf %f)", ld, f);					\
  printf ("\n");

static void
do_test_call (FILE *stream, CHAR *string, const CHAR *format, ...)
{
  float *fptr;
  float f;
  long double *ldptr;
  long double ld;
  va_list args;

  CLEAR_VALUE
  printf ("fscanf: ");
  FSCANF (stream, format, &ld, &f);
  CHECK_VALUE

  CLEAR_VALUE
  printf ("scanf: ");
  SCANF (format, &ld, &f);
  CHECK_VALUE

  CLEAR_VALUE
  printf ("sscanf: ");
  SSCANF (string, format, &ld, &f);
  CHECK_VALUE

  CLEAR_VARGS
  printf ("vfscanf: ");
  va_start (args, format);
  VFSCANF (stream, format, args);
  va_end (args);
  CHECK_VARGS

  CLEAR_VARGS
  printf ("vscanf: ");
  va_start (args, format);
  VSCANF (format, args);
  va_end (args);
  CHECK_VARGS

  CLEAR_VARGS
  printf ("vsscanf: ");
  va_start (args, format);
  VSSCANF (string, format, args);
  va_end (args);
  CHECK_VARGS
}

static int
do_test (void)
{
  CHAR string[256];
  float f;
  long double ld;

  /* Scan floating-point value with '%as'.  */
  STRCPY (string,
	  L ("-0x1.0p+0 -0x2.0p+0\n")
	  L ("-0x1.0p+0 -0x2.0p+0\n") );
  do_test_call (stdin, string, L("%La %as"), &ld, &f);

  return 0;
}

#include <support/test-driver.c>
