AC_INIT(Makefile.in)
AM_INIT_AUTOMAKE(nics, 0.1)
AM_SANITY_CHECK
AM_CONFIG_HEADER(config.h)

AC_LANG_CPLUSPLUS
AC_PROG_CXX
AC_PROG_RANLIB
AC_PROG_YACC
AM_PROG_LEX
AM_PROG_LIBTOOL

AC_TYPE_SIZE_T

AC_HEADER_STDC
AC_CHECK_HEADERS(assert.h)
AC_CHECK_HEADER(cc++/thread.h)
dnl AC_CHECK_HEADER(gc/gc_cpp.h)

AC_CHECK_LIB(pq++, main,true)
dnl AC_CHECK_LIB(ccxx, main, true,,-lpthread)
dnl AC_CHECK_LIB(ccstd, main, true,,-lpthread -lz -lccxx)
dnl AC_CHECK_LIB(ccio, main, true,,-lpthread -ldl -lz -lccxx -lccstd)
dnl AC_CHECK_LIB(gc, GC_malloc, true,,-lpthread)

CXXFLAGS="$CXXFLAGS -D_REENTRANT -D_THREAD_SAFE -D_GNU_SOURCE"

AC_ARG_ENABLE(debug,
     [  --enable-debug    Turn on debugging],
     [if test "$enableval" = "no"; then
        CXXFLAGS="$CXXFLAGS -DNDEBUG"
      elif test "$enableval" != "yes"; then
        for debugflag in $enableval; do
	  AC_DEFINE_UNQUOTED(DEBUG_$debugflag)
        done
     fi])

AC_ARG_ENABLE(profiling,
     [  --enable-profiling Turn on profiling],
     [case "${enableval}" in
       yes) profiling=true ;;
       no) profiling=false ;;
       *) AC_MSG_ERROR(bad value ${enableval} for --enable-profiling) ;;
     esac],[profiling=false])

if test x$profiling = xtrue; then
  CXXFLAGS="$CXXFLAGS -pg"
fi

AC_OUTPUT(Makefile libmchess/Makefile nics/Makefile
	tests/Makefile tests/libmchess/Makefile tests/libmchess/pgn/Makefile
	tests/nics/Makefile)
