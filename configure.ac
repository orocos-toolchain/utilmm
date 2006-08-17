dnl  -*- Autoconf -*-
dnl  $Id: configure.ac 1318 2006-01-31 17:21:48Z sjoyeux $

AC_INIT(utilmm, 0.2, sjoyeux@laas.fr)
AC_CONFIG_HEADER([utilmm/config/config.h])

AC_PREREQ(2.54)

dnl package info

AC_LANG(C++)

dnl check for programs
AC_CANONICAL_HOST

dnl Check for libtool
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)
AC_PROG_INSTALL

dnl Enable PCH support
CLBS_PCH_SUPPORT
CLBS_DEBUG_OPTIONS

dnl Check boost
CLBS_CHECK_DOXYGEN
CLBS_CHECK_BOOST(, [AC_MSG_ERROR([Unable to find Boost libraries])])
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
    Doxyfile

    utilmm/Makefile
    src/Makefile
    src/singleton/Makefile
    src/system/Makefile
    src/configfile/Makefile
    src/memory/Makefile
    src/demangle/Makefile

    test/Makefile
)
AC_OUTPUT

