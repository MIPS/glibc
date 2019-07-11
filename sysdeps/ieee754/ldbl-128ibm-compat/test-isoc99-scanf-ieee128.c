#undef _GNU_SOURCE
/* The following macro definitions are a hack.  They word around disabling
   the GNU extension while still using a few internal headers.  */
#define u_char unsigned char
#define u_short unsigned short
#define u_int unsigned int
#define u_long unsigned long
#include <libio/stdio.h>
#include <stdlib/stdlib.h>
#include <string/string.h>
#include <wcsmbs/wchar.h>

#include <test-isoc99-scanf-ldbl-compat.c>
