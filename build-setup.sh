#!/bin/sh
#
# build-setup.sh --- Add required files for building to the svn export
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

mkdir m4
autoreconf
autopoint
ln -s /usr/share/gettext/gettext.h
automake --add-missing

