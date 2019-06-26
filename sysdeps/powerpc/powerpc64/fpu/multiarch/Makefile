ifeq ($(subdir),math)
# These functions are built both for libc and libm because they're required
# by printf.  While the libc objects have the prefix s_, the libm ones are
# prefixed with  m_.
sysdep_calls := s_copysign-power6 s_copysign-ppc64 \
		s_finite-power8 s_finite-power7 s_finite-ppc64 \
		s_finitef-ppc64 \
		s_isinf-power8 s_isinf-ppc64 \
		s_isinff-ppc64 s_isinf-power7 \
		s_isnan-power8 s_isnan-power7 s_isnan-power6x s_isnan-power6 \
		s_isnan-power5 s_isnan-ppc64 \
		s_modf-power5+ s_modf-ppc64 \
		s_modff-power5+ s_modff-ppc64

sysdep_routines += $(sysdep_calls)
libm-sysdep_routines += s_llround-power6x \
			s_llround-power5+ s_llround-ppc64 s_ceil-power5+ \
			s_ceil-ppc64 s_ceilf-power5+ s_ceilf-ppc64 \
			s_floor-power5+ s_floor-ppc64 s_floorf-power5+ \
			s_floorf-ppc64 s_round-power5+ s_round-ppc64 \
			s_roundf-power5+ s_roundf-ppc64 s_trunc-power5+ \
			s_trunc-ppc64 s_truncf-power5+ s_truncf-ppc64 \
			s_llrint-power6x s_llrint-ppc64 \
			s_logb-power7 s_logbf-power7 \
			s_logbl-power7 s_logb-ppc64 s_logbf-ppc64 \
			s_logbl-ppc64 e_hypot-ppc64 \
			e_hypot-power7 e_hypotf-ppc64 e_hypotf-power7 \
			s_llrint-power8 s_llround-power8 s_llroundf-ppc64 \
			e_expf-power8 e_expf-ppc64 \
			$(sysdep_calls:s_%=m_%)

CFLAGS-s_logbf-power7.c = -mcpu=power7
CFLAGS-s_logbl-power7.c = -mcpu=power7
CFLAGS-s_logb-power7.c = -mcpu=power7
CFLAGS-s_modf-power5+.c = -mcpu=power5+
CFLAGS-s_modff-power5+.c = -mcpu=power5+
CFLAGS-e_hypot-power7.c = -mcpu=power7
CFLAGS-e_hypotf-power7.c = -mcpu=power7

# These files quiet sNaNs in a way that is optimized away without
# -fsignaling-nans.
CFLAGS-s_modf-ppc64.c += -fsignaling-nans
CFLAGS-s_modff-ppc64.c += -fsignaling-nans
endif

ifeq ($(subdir),mathvec)
libmvec-sysdep_routines += vec_d_cos2_vsx vec_s_cosf4_vsx \
			   vec_d_sin2_vsx vec_s_sinf4_vsx \
			   vec_d_log2_vsx vec_d_log_data \
			   vec_s_logf4_vsx vec_s_logf_data \
			   vec_s_expf4_vsx vec_s_exp2f_data \
			   vec_s_powf4_vsx e_powf_log2_data \
			   vec_math_errf vec_math_err \
			   vec_d_exp2_vsx vec_d_exp_data \
			   vec_d_pow2_vsx s_pow_log2_data \
			   vec_d_sincos2_vsx vec_s_sincosf4_vsx
CFLAGS-vec_d_cos2_vsx.c += -mabi=altivec -maltivec -mvsx -mpower8-vector
CFLAGS-vec_d_log2_vsx.c += -mabi=altivec -maltivec -mvsx -mpower8-vector
CFLAGS-vec_d_log_data.c += -mabi=altivec -maltivec -mvsx -mpower8-vector
CFLAGS-vec_math_errf.c += -mabi=altivec -maltivec -mvsx
CFLAGS-vec_s_cosf4_vsx.c += -mabi=altivec -maltivec -mvsx
CFLAGS-vec_s_logf4_vsx.c += -mabi=altivec -maltivec -mvsx
CFLAGS-vec_s_logf_data.c += -mabi=altivec -maltivec -mvsx
CFLAGS-vec_d_sin2_vsx.c += -mabi=altivec -maltivec -mvsx -mpower8-vector
CFLAGS-vec_s_sinf4_vsx.c += -mabi=altivec -maltivec -mvsx
CFLAGS-vec_d_sincos2_vsx.c += -mabi=altivec -maltivec -mvsx -mpower8-vector
CFLAGS-vec_s_sincosf4_vsx.c += -mabi=altivec -maltivec -mvsx
CFLAGS-vec_s_expf4_vsx.c += -mabi=altivec -maltivec -mvsx
CFLAGS-vec_s_exp2f_data.c += -mabi=altivec -maltivec -mvsx
CFLAGS-vec_d_exp2_vsx.c += -mabi=altivec -maltivec -mvsx -mpower8-vector
CFLAGS-vec_d_exp_data.c += -mabi=altivec -maltivec -mvsx
CFLAGS-vec_s_powf4_vsx.c += -mabi=altivec -maltivec -mvsx -mpower8-vector
CFLAGS-vec_d_pow2_vsx.c += -mabi=altivec -maltivec -mvsx -mpower8-vector
CFLAGS-s_pow_log2_data.c += -mabi=altivec -maltivec -mvsx
CFLAGS-vec_math_err.c += -mabi=altivec -maltivec -mvsx
endif

# Variables for libmvec tests.
ifeq ($(subdir),math)
ifeq ($(build-mathvec),yes)
libmvec-tests += double-vlen2 float-vlen4

double-vlen2-funcs = cos sin sincos log exp pow
float-vlen4-funcs = cos sin sincos log exp pow

double-vlen2-arch-ext-cflags = -mabi=altivec -maltivec -mvsx -DREQUIRE_VSX
float-vlen4-arch-ext-cflags = -mabi=altivec -maltivec -mvsx -DREQUIRE_VSX

CFLAGS-test-double-vlen2-wrappers.c += -mabi=altivec -maltivec -mvsx -DREQUIRE_VSX
CFLAGS-test-float-vlen4-wrappers.c += -mabi=altivec -maltivec -mvsx -DREQUIRE_VSX

endif
endif
