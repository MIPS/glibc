/* Coefficients for single-precision SVE pow(x) function.

   Copyright (C) 2024-2025 Free Software Foundation, Inc.
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


#include "vecmath_config.h"

const struct v_powf_data __v_powf_data = {
  .invc = { 0x1.6489890582816p+0,
	    0x1.5cf19b35e3472p+0,
	    0x1.55aac0e956d65p+0,
	    0x1.4eb0022977e01p+0,
	    0x1.47fcccda1dd1fp+0,
	    0x1.418ceabab68c1p+0,
	    0x1.3b5c788f1edb3p+0,
	    0x1.3567de48e9c9ap+0,
	    0x1.2fabc80fd19bap+0,
	    0x1.2a25200ce536bp+0,
	    0x1.24d108e0152e3p+0,
	    0x1.1facd8ab2fbe1p+0,
	    0x1.1ab614a03efdfp+0,
	    0x1.15ea6d03af9ffp+0,
	    0x1.1147b994bb776p+0,
	    0x1.0ccbf650593aap+0,
	    0x1.0875408477302p+0,
	    0x1.0441d42a93328p+0,
	    0x1p+0,
	    0x1.f1d006c855e86p-1,
	    0x1.e28c3341aa301p-1,
	    0x1.d4bdf9aa64747p-1,
	    0x1.c7b45a24e5803p-1,
	    0x1.bb5f5eb2ed60ap-1,
	    0x1.afb0bff8fe6b4p-1,
	    0x1.a49badf7ab1f5p-1,
	    0x1.9a14a111fc4c9p-1,
	    0x1.901131f5b2fdcp-1,
	    0x1.8687f73f6d865p-1,
	    0x1.7d7067eb77986p-1,
	    0x1.74c2c1cf97b65p-1,
	    0x1.6c77f37cff2a1p-1
  },
  .logc = { -0x1.e960f97b22702p+3,
	    -0x1.c993406cd4db6p+3,
	    -0x1.aa711d9a7d0f3p+3,
	    -0x1.8bf37bacdce9bp+3,
	    -0x1.6e13b3519946ep+3,
	    -0x1.50cb8281e4089p+3,
	    -0x1.341504a237e2bp+3,
	    -0x1.17eaab624ffbbp+3,
	    -0x1.f88e708f8c853p+2,
	    -0x1.c24b6da113914p+2,
	    -0x1.8d02ee397cb1dp+2,
	    -0x1.58ac1223408b3p+2,
	    -0x1.253e6fd190e89p+2,
	    -0x1.e5641882c12ffp+1,
	    -0x1.81fea712926f7p+1,
	    -0x1.203e240de64a3p+1,
	    -0x1.8029b86a78281p0,
	    -0x1.85d713190fb9p-1,
	    0x0p+0,
	    0x1.4c1cc07312997p0,
	    0x1.5e1848ccec948p+1,
	    0x1.04cfcb7f1196fp+2,
	    0x1.582813d463c21p+2,
	    0x1.a936fa68760ccp+2,
	    0x1.f81bc31d6cc4ep+2,
	    0x1.2279a09fae6b1p+3,
	    0x1.47ec0b6df5526p+3,
	    0x1.6c71762280f1p+3,
	    0x1.90155070798dap+3,
	    0x1.b2e23b1d3068cp+3,
	    0x1.d4e21b0daa86ap+3,
	    0x1.f61e2a2f67f3fp+3
  },
  .scale = { 0x3ff0000000000000, 0x3fefd9b0d3158574, 0x3fefb5586cf9890f,
	     0x3fef9301d0125b51, 0x3fef72b83c7d517b, 0x3fef54873168b9aa,
	     0x3fef387a6e756238, 0x3fef1e9df51fdee1, 0x3fef06fe0a31b715,
	     0x3feef1a7373aa9cb, 0x3feedea64c123422, 0x3feece086061892d,
	     0x3feebfdad5362a27, 0x3feeb42b569d4f82, 0x3feeab07dd485429,
	     0x3feea47eb03a5585, 0x3feea09e667f3bcd, 0x3fee9f75e8ec5f74,
	     0x3feea11473eb0187, 0x3feea589994cce13, 0x3feeace5422aa0db,
	     0x3feeb737b0cdc5e5, 0x3feec49182a3f090, 0x3feed503b23e255d,
	     0x3feee89f995ad3ad, 0x3feeff76f2fb5e47, 0x3fef199bdd85529c,
	     0x3fef3720dcef9069, 0x3fef5818dcfba487, 0x3fef7c97337b9b5f,
	     0x3fefa4afa2a490da, 0x3fefd0765b6e4540,
  },
};
