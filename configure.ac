AC_INIT([dptest], [0.1], [1997danilov@mail.ru])
m4_pattern_forbid([PKG_CHECK_MODULES])dnl
AM_PROG_AR
LT_INIT
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CXX
AC_CONFIG_HEADERS([config.h])
PKG_CHECK_MODULES([NLOHMANN_JSON], [nlohmann_json], [], AC_MSG_ERROR([nlohmann_json devel package not found]))
PKG_CHECK_MODULES([LIBCURL], [libcurl], [], AC_MSG_ERROR([libcurl devel package not found]))
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 libcmpb.pc
])
AC_OUTPUT