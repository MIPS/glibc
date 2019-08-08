#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

static int
do_test (void)
{
  Dl_info i;
  if (dladdr (&printf, &i) == 0)
    {
      puts ("not found");
      return 1;
    }
  printf ("found symbol %s in %s\n", i.dli_sname, i.dli_fname);
  return i.dli_sname == NULL
	 || (strcmp (i.dli_sname, "printf") != 0
	     /* On architectures which create PIC code by default
		&printf may resolve to an address in libc.so
		rather than in the binary.  printf and _IO_printf
		are aliased and which one comes first in the
		hash table is up to the linker.  */
	     && strcmp (i.dli_sname, "_IO_printf") != 0
	     /* On architectures where long double with IEEE binary128
		format is available as a third option (initially, true
		for powerpc64le), printf may be redirected to
		__printfieee128.  */
	     && strcmp (i.dli_sname, "__printfieee128") != 0);
}

#include <support/test-driver.c>
