#ifndef _BITS_ENDIANNESS_H
#define _BITS_ENDIANNESS_H 1

#ifndef _BITS_ENDIAN_H
# error "Never use <bits/endianness.h> directly; include <endian.h> instead."
#endif

#ifdef __riscv_big_endian
#define __BYTE_ORDER __BIG_ENDIAN
#else
/* RISC-V is little-endian.  */
#define __BYTE_ORDER __LITTLE_ENDIAN
#endif

#endif /* bits/endianness.h */
