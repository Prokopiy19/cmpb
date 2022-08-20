bin_PROGRAMS = dptest
# lib_LIBRARIES = libcmpb.a
lib_LTLIBRARIES = libcmpb.la

dptest_SOURCES = main.cpp cmpb.hpp
dptest_LDFLAGS = -rpath $(libdir)
dptest_LDADD = libcmpb.la
dptest_CFLAGS = $(NLOHMANN_JSON_CFLAGS)

libcmpb_la_SOURCES = cmpb.cpp
libcmpb_la_LIBADD = $(LIBCURL_LIBS)
libcmpb_la_CFLAGS = $(LIBCURL_CFLAGS) $(NLOHMANN_JSON_CFLAGS)

# libcmpb_a_SOURCES = cmpb.cpp
# libcmpb_a_LIBADD = $(LIBCURL_LIBS)
# libcmpb_a_CFLAGS = $(LIBCURL_CFLAGS) $(NLOHMANN_JSON_CFLAGS) $(AM_CPPFLAGS)

include_HEADERS = cmpb.hpp