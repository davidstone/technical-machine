# Copyright (C) 2011 David Stone
#
# This file is part of Technical Machine.
#
# Technical Machine is free software: you can redistribute it and / or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([technical-machine], [1])
AM_INIT_AUTOMAKE([subdir-objects foreign])
AC_CONFIG_SRCDIR([src/ai.cpp])
#AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_CC_C_O

# Checks for libraries.
AC_CHECK_LIB([boost_filesystem], [main])
AC_CHECK_LIB([boost_system], [main])
AC_CHECK_LIB([fltk], [main])
AC_CHECK_LIB([pthread], [pthread_cancel])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h stdlib.h string.h sys/param.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset socket])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
