/* Single-precision vector (Advanced SIMD) asinh function

   Copyright (C) 2024 Free Software Foundation, Inc.
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

#include "v_math.h"
#include "v_log1pf_inline.h"

#define SignMask v_u32 (0x80000000)

const static struct data
{
  struct v_log1pf_data log1pf_consts;
  uint32x4_t big_bound;
#if WANT_SIMD_EXCEPT
  uint32x4_t tiny_bound;
#endif
} data = {
  .log1pf_consts = V_LOG1PF_CONSTANTS_TABLE,
  .big_bound = V4 (0x5f800000), /* asuint(0x1p64).  */
#if WANT_SIMD_EXCEPT
  .tiny_bound = V4 (0x30800000) /* asuint(0x1p-30).  */
#endif
};

static float32x4_t NOINLINE VPCS_ATTR
special_case (float32x4_t x, float32x4_t y, uint32x4_t special)
{
  return v_call_f32 (asinhf, x, y, special);
}

/* Single-precision implementation of vector asinh(x), using vector log1p.
   Worst-case error is 2.66 ULP, at roughly +/-0.25:
   __v_asinhf(0x1.01b04p-2) got 0x1.fe163ep-3 want 0x1.fe1638p-3.  */
VPCS_ATTR float32x4_t NOINLINE V_NAME_F1 (asinh) (float32x4_t x)
{
  const struct data *dat = ptr_barrier (&data);
  uint32x4_t iax = vbicq_u32 (vreinterpretq_u32_f32 (x), SignMask);
  float32x4_t ax = vreinterpretq_f32_u32 (iax);
  uint32x4_t special = vcgeq_u32 (iax, dat->big_bound);
  float32x4_t special_arg = x;

#if WANT_SIMD_EXCEPT
  /* Sidestep tiny and large values to avoid inadvertently triggering
     under/overflow.  */
  special = vorrq_u32 (special, vcltq_u32 (iax, dat->tiny_bound));
  if (__glibc_unlikely (v_any_u32 (special)))
    {
      ax = v_zerofy_f32 (ax, special);
      x = v_zerofy_f32 (x, special);
    }
#endif

  /* asinh(x) = log(x + sqrt(x * x + 1)).
     For positive x, asinh(x) = log1p(x + x * x / (1 + sqrt(x * x + 1))).  */
  float32x4_t d
      = vaddq_f32 (v_f32 (1), vsqrtq_f32 (vfmaq_f32 (v_f32 (1), x, x)));
  float32x4_t y = log1pf_inline (
      vaddq_f32 (ax, vdivq_f32 (vmulq_f32 (ax, ax), d)), dat->log1pf_consts);

  if (__glibc_unlikely (v_any_u32 (special)))
    return special_case (special_arg, vbslq_f32 (SignMask, x, y), special);
  return vbslq_f32 (SignMask, x, y);
}
libmvec_hidden_def (V_NAME_F1 (asinh))
HALF_WIDTH_ALIAS_F1 (asinh)
