GLIBC_PROVIDES dnl See aclocal.m4 in the top level source directory.
# Local configure fragment for sysdeps/s390.

AC_CACHE_CHECK(for __builtin_tbegin, libc_cv_gcc_builtin_tbegin, [dnl
cat > conftest.c <<\EOF
#include <htmintrin.h>
void testtransaction ()
{
  if (__builtin_tbegin (0) == _HTM_TBEGIN_STARTED)
    {
      __builtin_tend ();
    }
}
EOF
dnl
dnl test, if the tbegin instruction is used by __builtin_tbegin
if AC_TRY_COMMAND([${CC-cc} -mhtm -O2 -S conftest.c -o - | grep -w tbegin > /dev/null]) ;
then
  libc_cv_gcc_builtin_tbegin=yes
else
  libc_cv_gcc_builtin_tbegin=no
fi
rm -f conftest* ])

if test "$libc_cv_gcc_builtin_tbegin" = no ; then
   critic_missing="$critic_missing The used GCC has no support for __builtin_tbegin, which is needed for lock-elision on target S390."
fi


AC_CACHE_CHECK([for S390 vector instruction support], libc_cv_asm_s390_vx, [
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
void testvecinsn ()
{
    __asm__ (".machine \"z13\" \n\t"
	     ".machinemode \"zarch_nohighgprs\" \n\t"
	     "vistrbs %%v16,%%v17 \n\t"
	     "locghie %%r1,0" : :);
}
]])],
		  [libc_cv_asm_s390_vx=yes],
		  [libc_cv_asm_s390_vx=no])])
if test "$libc_cv_asm_s390_vx" = yes ;
then
  AC_DEFINE(HAVE_S390_VX_ASM_SUPPORT)
else
  AC_MSG_WARN([Use binutils with vector-support in order to use optimized implementations.])
fi


dnl test, if gcc supports S390 vector registers as clobber in inline assembly
AC_CACHE_CHECK([for S390 vector support in gcc], libc_cv_gcc_s390_vx, [
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
void testvecclobber ()
{
  __asm__ ("" : : : "v16");
}
]])],
		  [libc_cv_gcc_s390_vx=yes],
		  [libc_cv_gcc_s390_vx=no])])
if test "$libc_cv_gcc_s390_vx" = yes ;
then
  AC_DEFINE(HAVE_S390_VX_GCC_SUPPORT)
fi


AC_CACHE_CHECK([for S390 arch13 zarch instruction support],
	       libc_cv_asm_s390_arch13, [
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
void testinsn (char *buf)
{
    __asm__ (".machine \"arch13\" \n\t"
	     ".machinemode \"zarch_nohighgprs\" \n\t"
	     "lghi %%r0,16 \n\t"
	     "mvcrl 0(%0),32(%0) \n\t"
	     "vstrs %%v20,%%v20,%%v20,%%v20,0,2"
	     : : "a" (buf) : "memory", "r0");
}
]])],
		  [libc_cv_asm_s390_arch13=yes],
		  [libc_cv_asm_s390_arch13=no])])
if test "$libc_cv_asm_s390_arch13" = yes ;
then
  AC_DEFINE(HAVE_S390_ARCH13_ASM_SUPPORT)
fi


AC_CACHE_CHECK([for S390 z10 zarch instruction support as default],
	       libc_cv_asm_s390_min_z10_zarch, [
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
void testinsn (void *a, void *b, int n)
{
    __asm__ ("exrl %2,1f \n\t"
	     "j 2f \n\t"
	     "1: mvc 0(1,%0),0(%1) \n\t"
	     "2:"
	     : : "a" (a), "a" (b), "d" (n)
	     : "memory", "cc");
}
]])],
		  [libc_cv_asm_s390_min_z10_zarch=yes],
		  [libc_cv_asm_s390_min_z10_zarch=no])])
if test "$libc_cv_asm_s390_min_z10_zarch" = yes ;
then
  AC_DEFINE(HAVE_S390_MIN_Z10_ZARCH_ASM_SUPPORT)
fi


AC_CACHE_CHECK([for S390 z196 zarch instruction support as default],
	       libc_cv_asm_s390_min_z196_zarch, [
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
float testinsn (double e)
{
    float d;
    __asm__ ("ledbra %0,5,%1,4" : "=f" (d) : "f" (e) );
    return d;
}
]])],
		  [libc_cv_asm_s390_min_z196_zarch=yes],
		  [libc_cv_asm_s390_min_z196_zarch=no])])
if test "$libc_cv_asm_s390_min_z196_zarch" = yes ;
then
  AC_DEFINE(HAVE_S390_MIN_Z196_ZARCH_ASM_SUPPORT)
fi


AC_CACHE_CHECK([for S390 z13 zarch instruction support as default],
	       libc_cv_asm_s390_min_z13_zarch, [
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
int testinsn (void)
{
    int i;
    __asm__ ("vl %%v16,0(%%r15)\n\t"
	     "vlgvf %0,%%v16,0"
	     : "=d" (i) : : "memory", "v16");
    return i;
}
]])],
		  [libc_cv_asm_s390_min_z13_zarch=yes],
		  [libc_cv_asm_s390_min_z13_zarch=no])])
if test "$libc_cv_asm_s390_min_z13_zarch" = yes ;
then
  AC_DEFINE(HAVE_S390_MIN_Z13_ZARCH_ASM_SUPPORT)
fi


AC_CACHE_CHECK([for S390 arch13 zarch instruction support as default],
	       libc_cv_asm_s390_min_arch13_zarch, [
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
void testinsn (char *buf)
{
    __asm__ ("lghi %%r0,16 \n\t"
	     "mvcrl 0(%0),32(%0) \n\t"
	     "vstrs %%v20,%%v20,%%v20,%%v20,0,2"
	     : : "a" (buf) : "memory", "r0");
}
]])],
		  [libc_cv_asm_s390_min_arch13_zarch=yes],
		  [libc_cv_asm_s390_min_arch13_zarch=no])])
if test "$libc_cv_asm_s390_min_arch13_zarch" = yes ;
then
  AC_DEFINE(HAVE_S390_MIN_ARCH13_ZARCH_ASM_SUPPORT)
fi

test -n "$critic_missing" && AC_MSG_ERROR([
*** $critic_missing])
