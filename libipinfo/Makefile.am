# Makefile.am --- Makefile.am for the libipinfo library
# Copyright (c) 2010
# Maurice Makaay
#
# This file is part of the IPinfo package.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the MIT license. You should have received a copy
# of the license along with this program. If not, see the website
# <http://www.opensource.org/licenses/mit-license.php>
#
# Process this file with automake to produce Makefile.in.

LIBRARY_VERSION=0:0:0

AM_CFLAGS = $(WERROR_CFLAGS) $(WARN_CFLAGS)
AM_CPPFLAGS = -I../libipinfo -I..
AM_LIBTOOLFLAGS=--silent

lib_LTLIBRARIES = libipinfo.la
libipinfo_la_SOURCES = \
	parse.c \
	tokenize.c \
	format.c \
	ipv4_netmask.c \
	ipv4_format.c \
	ipv6_classify.c \
	ipv6_format.c
libipinfo_la_LDFLAGS = -version-info $(LIBRARY_VERSION)

library_includedir=$(includedir)
library_include_HEADERS = ipinfo.h 

EXTRA_DIST = \
	tokenize.h \
	ipv4_netmask.h \
	ipv4_format.h \
	ipv6_format.h \
	ipv6_classify.h

