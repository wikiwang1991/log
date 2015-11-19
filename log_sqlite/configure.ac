#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT(log_sqlite, 0.0.0.0, wikiwang@live.com)
AC_CONFIG_SRCDIR([log_sqlite.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE(log_sqlite, 0.0.0.0)

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_PROG_LIBTOOL

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h inttypes.h limits.h malloc.h stddef.h stdint.h stdlib.h string.h sys/file.h sys/ioctl.h sys/mount.h sys/param.h sys/statvfs.h sys/time.h unistd.h utime.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_UID_T
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_CHECK_MEMBERS([struct stat.st_blksize])
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRERROR_R
AC_FUNC_STRTOD
AC_CHECK_FUNCS([clock_gettime fdatasync getpagesize gettimeofday localtime_r memmove memset strerror strstr utime])

AC_CONFIG_FILES(makefile)
AC_OUTPUT

