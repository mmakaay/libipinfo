# Makefile.am --- Makefile.am for the ipinfo command line utility
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

LDADD = ../libipinfo/libipinfo.la
AM_LDFLAGS = $(LTLIBINTL)
AM_CFLAGS = $(WERROR_CFLAGS) $(WARN_CFLAGS)
AM_CPPFLAGS = -I../libipinfo -I..
AM_LIBTOOLFLAGS=--silent

localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

bin_PROGRAMS = ipinfo
ipinfo_SOURCES = ipinfo.c
