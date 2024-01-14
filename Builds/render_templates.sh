#!/bin/bash

# ----------------------------------------------------------------------------
#
#  Squeezer
#  ========
#  Flexible general-purpose audio compressor with a touch of citrus
#
#  Copyright (c) 2013-2024 Martin Zuther (http://www.mzuther.de/)
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

cd "$(dirname "$0")" || exit

printf "\n=== Rendering templates ===\n"
cd "../Source/frut/templates/stempelwerk/" || exit
python3 -m src.StempelWerk --quiet "../settings.json"

printf "\n"
cd "../../../../Builds/" || exit
premake5 --os=windows vs2019

printf "\n"
premake5 --cc=clang --os=linux gmake
# premake5 --cc=gcc --os=linux gmake

printf "\n"
