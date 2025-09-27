! Platform-specific declarations of SIMD math functions for Fortran. -*- f90 -*-
!   Copyright (C) 2019-2025 Free Software Foundation, Inc.
!   This file is part of the GNU C Library.
!
!   The GNU C Library is free software; you can redistribute it and/or
!   modify it under the terms of the GNU Lesser General Public
!   License as published by the Free Software Foundation; either
!   version 2.1 of the License, or (at your option) any later version.
!
!   The GNU C Library is distributed in the hope that it will be useful,
!   but WITHOUT ANY WARRANTY; without even the implied warranty of
!   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
!   Lesser General Public License for more details.
!
!   You should have received a copy of the GNU Lesser General Public
!   License along with the GNU C Library; if not, see
!   <https://www.gnu.org/licenses/>.

!GCC$ builtin (acos) attributes simd (notinbranch)
!GCC$ builtin (acosf) attributes simd (notinbranch)
!GCC$ builtin (acosh) attributes simd (notinbranch)
!GCC$ builtin (acoshf) attributes simd (notinbranch)
!GCC$ builtin (acospi) attributes simd (notinbranch)
!GCC$ builtin (acospif) attributes simd (notinbranch)
!GCC$ builtin (asin) attributes simd (notinbranch)
!GCC$ builtin (asinf) attributes simd (notinbranch)
!GCC$ builtin (asinh) attributes simd (notinbranch)
!GCC$ builtin (asinhf) attributes simd (notinbranch)
!GCC$ builtin (asinpi) attributes simd (notinbranch)
!GCC$ builtin (asinpif) attributes simd (notinbranch)
!GCC$ builtin (atan) attributes simd (notinbranch)
!GCC$ builtin (atan2) attributes simd (notinbranch)
!GCC$ builtin (atan2f) attributes simd (notinbranch)
!GCC$ builtin (atan2pi) attributes simd (notinbranch)
!GCC$ builtin (atan2pif) attributes simd (notinbranch)
!GCC$ builtin (atanf) attributes simd (notinbranch)
!GCC$ builtin (atanh) attributes simd (notinbranch)
!GCC$ builtin (atanhf) attributes simd (notinbranch)
!GCC$ builtin (atanpi) attributes simd (notinbranch)
!GCC$ builtin (atanpif) attributes simd (notinbranch)
!GCC$ builtin (cbrt) attributes simd (notinbranch)
!GCC$ builtin (cbrtf) attributes simd (notinbranch)
!GCC$ builtin (cos) attributes simd (notinbranch)
!GCC$ builtin (cosf) attributes simd (notinbranch)
!GCC$ builtin (cosh) attributes simd (notinbranch)
!GCC$ builtin (coshf) attributes simd (notinbranch)
!GCC$ builtin (cospi) attributes simd (notinbranch)
!GCC$ builtin (cospif) attributes simd (notinbranch)
!GCC$ builtin (erf) attributes simd (notinbranch)
!GCC$ builtin (erfc) attributes simd (notinbranch)
!GCC$ builtin (erfcf) attributes simd (notinbranch)
!GCC$ builtin (erff) attributes simd (notinbranch)
!GCC$ builtin (exp) attributes simd (notinbranch)
!GCC$ builtin (exp10) attributes simd (notinbranch)
!GCC$ builtin (exp10f) attributes simd (notinbranch)
!GCC$ builtin (exp10m1) attributes simd (notinbranch)
!GCC$ builtin (exp10m1f) attributes simd (notinbranch)
!GCC$ builtin (exp2) attributes simd (notinbranch)
!GCC$ builtin (exp2f) attributes simd (notinbranch)
!GCC$ builtin (exp2m1) attributes simd (notinbranch)
!GCC$ builtin (exp2m1f) attributes simd (notinbranch)
!GCC$ builtin (expf) attributes simd (notinbranch)
!GCC$ builtin (expm1) attributes simd (notinbranch)
!GCC$ builtin (expm1f) attributes simd (notinbranch)
!GCC$ builtin (hypot) attributes simd (notinbranch)
!GCC$ builtin (hypotf) attributes simd (notinbranch)
!GCC$ builtin (log) attributes simd (notinbranch)
!GCC$ builtin (log10) attributes simd (notinbranch)
!GCC$ builtin (log10f) attributes simd (notinbranch)
!GCC$ builtin (log1p) attributes simd (notinbranch)
!GCC$ builtin (log1pf) attributes simd (notinbranch)
!GCC$ builtin (log2) attributes simd (notinbranch)
!GCC$ builtin (log2f) attributes simd (notinbranch)
!GCC$ builtin (log2p1) attributes simd (notinbranch)
!GCC$ builtin (log2p1f) attributes simd (notinbranch)
!GCC$ builtin (logf) attributes simd (notinbranch)
!GCC$ builtin (logp1) attributes simd (notinbranch)
!GCC$ builtin (logp1f) attributes simd (notinbranch)
!GCC$ builtin (pow) attributes simd (notinbranch)
!GCC$ builtin (powf) attributes simd (notinbranch)
!GCC$ builtin (sin) attributes simd (notinbranch)
!GCC$ builtin (sinf) attributes simd (notinbranch)
!GCC$ builtin (sinh) attributes simd (notinbranch)
!GCC$ builtin (sinhf) attributes simd (notinbranch)
!GCC$ builtin (sinpi) attributes simd (notinbranch)
!GCC$ builtin (sinpif) attributes simd (notinbranch)
!GCC$ builtin (tan) attributes simd (notinbranch)
!GCC$ builtin (tanf) attributes simd (notinbranch)
!GCC$ builtin (tanh) attributes simd (notinbranch)
!GCC$ builtin (tanhf) attributes simd (notinbranch)
!GCC$ builtin (tanpi) attributes simd (notinbranch)
!GCC$ builtin (tanpif) attributes simd (notinbranch)
