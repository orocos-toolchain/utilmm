dnl  -*- Autoconf -*-
dnl  $Id: configure.ac 1046 2005-10-12 14:00:07Z fpy $

AC_INIT(utilmm, 0.1.0, fpy@laas.fr)
AC_CONFIG_HEADER([utilmm/config/config.h])

AC_PREREQ(2.54)

dnl package info

AC_LANG(C++)

dnl check for programs
AC_CANONICAL_HOST

CLBS_PROG_CXX

HAVE_DOX="yes" 

FPY_INIT_DOXYGEN( , , [
  AC_MSG_WARN("Cannot find doxygen")
  HAVE_DOX="no"
])

dnl Check for libtool
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)
AC_PROG_INSTALL

dnl Enable PCH support
AC_PCH_SUPPORT
CLBS_DEBUG_OPTIONS

dnl check for libraries

FPY_LIB_BOOST(, [AC_MSG_ERROR([Unable to find Boost libraries])])

dnl Check for pthread support, needed for boost::threads
CLBS_BOOST_THREAD([], [AC_MSG_ERROR([Unable to find the Boost/Thread library])])
CLBS_BOOST_REGEX([], [AC_MSG_ERROR([Unable to find the Boost/Regex library])])
CLBS_BOOST_FILESYSTEM([], [AC_MSG_ERROR([Unable to find the Boost/Filesystem library])])
CLBS_TEST_SUPPORT

dnl check or types

AC_TYPE_SIZE_T
AC_TYPE_PID_T
AC_TYPE_SIGNAL

dnl output

AC_CONFIG_FILES(
    mk/init.mk
    utilmm.pc
    Init.make
    Makefile
    utilmm/Makefile
    src/Makefile
    src/singleton/Makefile
    src/system/Makefile
    src/configfile/Makefile
    src/memory/Makefile
    src/demangle/Makefile

    test/Makefile
)
if test "x$HAVE_DOX" = "xyes"; then
   AC_CONFIG_FILES([Doxyfile])
fi

AC_SUBST([HAVE_DOX])
AC_OUTPUT

