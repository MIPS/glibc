/* Euclidean distance function.  Double/Binary64 i386 version.
   Copyright (C) 2021 Free Software Foundation, Inc.
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

#include <math.h>
#include <math_private.h>
#include <math-underflow.h>
#include <math-narrow-eval.h>
#include <libm-alias-finite.h>
#include <math_config.h>

/* The i386 allows to use the default excess of precision to optimize the
   hypot implementation, since internal multiplication and sqrt is carried
   with 80-bit FP type.  */
double
__ieee754_hypot (double x, double y)
{
  if (!isfinite (x) || !isfinite (y))
    {
      if ((isinf (x) || isinf (y))
	  && !issignaling (x) && !issignaling (y))
	return INFINITY;
      return x + y;
    }

  long double lx = x;
  long double ly = y;
  double r = sqrtl (math_narrow_eval (lx * lx + ly * ly));
  math_check_force_underflow_nonneg (r);
  return r;
}
libm_alias_finite (__ieee754_hypot, __hypot)
