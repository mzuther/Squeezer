#!/bin/bash

# ----------------------------------------------------------------------------
#
#  Squeezer
#  ========
#  Flexible general-purpose compressor with a touch of lemon
#
#  Copyright (c) 2013 Martin Zuther (http://www.mzuther.de/)
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#  Thank you for using free software!
#
# ----------------------------------------------------------------------------

cd $(dirname $0)

echo
premake4 --file=visual_studio.lua --os=windows vs2010

echo
premake4 --cc=gcc --os=linux gmake

echo
