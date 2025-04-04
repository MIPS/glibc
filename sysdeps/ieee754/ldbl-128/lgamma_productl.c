/* Compute a product of 1 + (T/X), 1 + (T/(X+1)), ....
   Copyright (C) 2015-2025 Free Software Foundation, Inc.
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
#include <mul_splitl.h>

/* Compute the product of 1 + (T / (X + X_EPS)), 1 + (T / (X + X_EPS +
   1)), ..., 1 + (T / (X + X_EPS + N - 1)), minus 1.  X is such that
   all the values X + 1, ..., X + N - 1 are exactly representable, and
   X_EPS / X is small enough that factors quadratic in it can be
   neglected.  */

_Float128
__lgamma_productl (_Float128 t, _Float128 x, _Float128 x_eps, int n)
{
  _Float128 ret = 0, ret_eps = 0;
  for (int i = 0; i < n; i++)
    {
      _Float128 xi = x + i;
      _Float128 quot = t / xi;
      _Float128 mhi, mlo;
      mul_splitl (&mhi, &mlo, quot, xi);
      _Float128 quot_lo = (t - mhi - mlo) / xi - t * x_eps / (xi * xi);
      /* We want (1 + RET + RET_EPS) * (1 + QUOT + QUOT_LO) - 1.  */
      _Float128 rhi, rlo;
      mul_splitl (&rhi, &rlo, ret, quot);
      _Float128 rpq = ret + quot;
      _Float128 rpq_eps = (ret - rpq) + quot;
      _Float128 nret = rpq + rhi;
      _Float128 nret_eps = (rpq - nret) + rhi;
      ret_eps += (rpq_eps + nret_eps + rlo + ret_eps * quot
		  + quot_lo + quot_lo * (ret + ret_eps));
      ret = nret;
    }
  return ret + ret_eps;
}
