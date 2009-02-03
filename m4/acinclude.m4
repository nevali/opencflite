# CF_BSD_SOURCE
# --------------
AC_DEFUN([CF_BSD_SOURCE],
[AH_VERBATIM([_BSD_SOURCE],
[/* Enable BSD extensions on systems that have them.  */
#ifndef _BSD_SOURCE
# undef _BSD_SOURCE
#endif])dnl
AC_BEFORE([$0], [AC_COMPILE_IFELSE])dnl
AC_BEFORE([$0], [AC_RUN_IFELSE])dnl
AC_DEFINE([_BSD_SOURCE])
])
