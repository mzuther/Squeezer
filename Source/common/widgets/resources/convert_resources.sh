#!/bin/bash

# ----------------------------------------------------------------------------
#
#  MZ common JUCE
#  ==============
#  Common classes for use with the JUCE library
#
#  Copyright (c) 2010-2016 Martin Zuther (http://www.mzuther.de/)
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

BINARY_BUILDER_DIR=../../libraries/juce/extras/binarybuilder/Builds/Linux/build
OUTPUT_DIR=.

if [ ! -x "$BINARY_BUILDER_DIR/BinaryBuilder" ]; then
	echo
	echo "BinaryBuilder not found in $BINARY_BUILDER_DIR"
	echo
	exit 1
fi

cd $(dirname $0)
"$BINARY_BUILDER_DIR/BinaryBuilder" output "$OUTPUT_DIR" resources '*.png'

echo
echo --------------------------------------------------------------------------------
echo

cat "$OUTPUT_DIR/resources.h"

echo
echo --------------------------------------------------------------------------------
echo
