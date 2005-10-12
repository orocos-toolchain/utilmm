dnl -*- autoconf -*-
dnl $Id: general.m4 968 2005-03-23 15:09:03Z sjoyeux $

AC_DEFUN([FPY_DIRNAME_EXPR],
         [[expr ".$1" : '\(\.\)[^/]*$' \| "x$1" : 'x\(.*\)/[^/]*$']])

AC_DEFUN([FPY_PATH_PROG],
  [
   AC_PATH_TOOL([$1], [$2], no, [$5:$PATH])
   if test "x$1" = "xno" ; then 
	ifelse([$4], ,:, [$4])
   else
	ifelse([$3], ,:, [$3])
   fi
])

AC_DEFUN([CLBS_PROG_CC],
    [
    AC_PROG_CC
    if test "x$GCC" = "xyes"; then
        USING_GCC="yes"
        AC_SUBST(USING_GCC)
    fi
    ]
)

AC_DEFUN([CLBS_PROG_CXX],
    [
    AC_PROG_CXX
    if test "x$GCC" = "xyes"; then
        USING_GCC="yes"
        AC_SUBST(USING_GCC)
    fi
    ]
)


