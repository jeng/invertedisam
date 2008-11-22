#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(invertedIsamFiles, 0.1, jhe@jeremyenglish.org)
AM_INIT_AUTOMAKE(foreign)
AC_CONFIG_SRCDIR([buildInvertedIsam.c])
#AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([stdint.h stdlib.h string.h strings.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STAT
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([floor strcasecmp strerror])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
