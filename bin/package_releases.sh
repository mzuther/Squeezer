#!/bin/bash

# ----------------------------------------------------------------------------
#
#  Squeezer
#  ========
#  Flexible general-purpose audio compressor with a touch of lemon.
#
#  Copyright (c) 2013-2018 Martin Zuther (http://www.mzuther.de/)
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

SQUEEZER_VERSION="1.0"

SQUEEZER_EXECUTABLE_DIR="final"
SQUEEZER_RELEASE_DIR="releases"
SQUEEZER_DOCUMENTATION_DIR="../doc"

function copy_new_executable
{
	if [ -f "$1" ]; then
		echo "  Finalising file $1..."
		cp "$1" "$SQUEEZER_EXECUTABLE_DIR"/
	fi
}

function move_new_executable
{
	if [ -f "$1" ]; then
		echo "  Finalising file $1..."
		mv "$1" "$SQUEEZER_EXECUTABLE_DIR"/
	fi
}

function fill_archive
{
	if [ ! -d "$2" ]; then
		mkdir -p "$2"
	fi

	if [ -f "$1" ]; then
		echo "    $1"
		cp "$1" "$2"
	fi
}

function delete_old_archive
{
	if [ -f "$1" ]; then
		echo "  Deleting old archive \"$1\"..."
		rm "$1"
	fi
}

function create_new_archive
{
	echo "  Creating folder \"$1\"..."
	echo "  Copying files to \"$1\"..."
	mkdir -p "$1"
	echo
}

function compress_new_archive
{
	echo
	echo "  Creating archive \"$1\"..."
	echo

	if [ "$3" = "bzip2" ]; then
		tar --create --bzip2 --verbose --file "$1" "$2"/* | gawk ' { print "    adding: " $1 } '
	elif [ "$3" = "zip" ]; then
		zip --recurse-paths "$1" "$2"/* | gawk ' { print "  " $0 } '
	fi

	echo
	echo "  Removing folder \"$2\"..."

	rm -r "$2"/

	echo "  Done."
	echo
}

echo


# ----- GNU/Linux Standalone (32 bit) -----

echo "  === GNU/Linux Standalone v$SQUEEZER_VERSION (32 bit) ==="
echo

move_new_executable "squeezer_stereo"

delete_old_archive "$SQUEEZER_RELEASE_DIR/linux32/squeezer-standalone.tar.bz2"

SQUEEZER_ARCHIVE_DIR="squeezer-standalone_$SQUEEZER_VERSION"

create_new_archive "$SQUEEZER_ARCHIVE_DIR"

fill_archive "$SQUEEZER_EXECUTABLE_DIR/squeezer_stereo" "$SQUEEZER_ARCHIVE_DIR"
fill_archive "$SQUEEZER_DOCUMENTATION_DIR/LICENSE" "$SQUEEZER_ARCHIVE_DIR"
fill_archive "$SQUEEZER_DOCUMENTATION_DIR/squeezer.pdf" "$SQUEEZER_ARCHIVE_DIR"

compress_new_archive "$SQUEEZER_RELEASE_DIR/linux32/$SQUEEZER_ARCHIVE_DIR.tar.bz2" "$SQUEEZER_ARCHIVE_DIR" "bzip2"


# ----- GNU/Linux VST (32 bit) -----

echo "  === GNU/Linux VST v$SQUEEZER_VERSION (32 bit) ==="
echo

move_new_executable "squeezer_stereo_vst.so"

delete_old_archive "$SQUEEZER_RELEASE_DIR/linux32/squeezer-vst.tar.bz2"

SQUEEZER_ARCHIVE_DIR="squeezer-vst_$SQUEEZER_VERSION"

create_new_archive "$SQUEEZER_ARCHIVE_DIR"

fill_archive "$SQUEEZER_EXECUTABLE_DIR/squeezer_stereo_vst.so" "$SQUEEZER_ARCHIVE_DIR"
fill_archive "$SQUEEZER_DOCUMENTATION_DIR/LICENSE" "$SQUEEZER_ARCHIVE_DIR"
fill_archive "$SQUEEZER_DOCUMENTATION_DIR/squeezer.pdf" "$SQUEEZER_ARCHIVE_DIR"

compress_new_archive "$SQUEEZER_RELEASE_DIR/linux32/$SQUEEZER_ARCHIVE_DIR.tar.bz2" "$SQUEEZER_ARCHIVE_DIR" "bzip2"


# ----- GNU/Linux Standalone (64 bit) -----

echo "  === GNU/Linux Standalone v$SQUEEZER_VERSION (64 bit) ==="
echo

move_new_executable "squeezer_stereo_x64"

delete_old_archive "$SQUEEZER_RELEASE_DIR/linux64/squeezer-standalone.tar.bz2"

SQUEEZER_ARCHIVE_DIR="squeezer-standalone_$SQUEEZER_VERSION"

create_new_archive "$SQUEEZER_ARCHIVE_DIR"

fill_archive "$SQUEEZER_EXECUTABLE_DIR/squeezer_stereo_x64" "$SQUEEZER_ARCHIVE_DIR"
fill_archive "$SQUEEZER_DOCUMENTATION_DIR/LICENSE" "$SQUEEZER_ARCHIVE_DIR"
fill_archive "$SQUEEZER_DOCUMENTATION_DIR/squeezer.pdf" "$SQUEEZER_ARCHIVE_DIR"

compress_new_archive "$SQUEEZER_RELEASE_DIR/linux64/$SQUEEZER_ARCHIVE_DIR.tar.bz2" "$SQUEEZER_ARCHIVE_DIR" "bzip2"


# ----- GNU/Linux VST (64 bit) -----

echo "  === GNU/Linux VST v$SQUEEZER_VERSION (64 bit) ==="
echo

move_new_executable "squeezer_stereo_vst_x64.so"

delete_old_archive "$SQUEEZER_RELEASE_DIR/linux64/squeezer-vst.tar.bz2"

SQUEEZER_ARCHIVE_DIR="squeezer-vst_$SQUEEZER_VERSION"

create_new_archive "$SQUEEZER_ARCHIVE_DIR"

fill_archive "$SQUEEZER_EXECUTABLE_DIR/squeezer_stereo_vst_x64.so" "$SQUEEZER_ARCHIVE_DIR"
fill_archive "$SQUEEZER_DOCUMENTATION_DIR/LICENSE" "$SQUEEZER_ARCHIVE_DIR"
fill_archive "$SQUEEZER_DOCUMENTATION_DIR/squeezer.pdf" "$SQUEEZER_ARCHIVE_DIR"

compress_new_archive "$SQUEEZER_RELEASE_DIR/linux64/$SQUEEZER_ARCHIVE_DIR.tar.bz2" "$SQUEEZER_ARCHIVE_DIR" "bzip2"


# ----- Windows Standalone (32 bit) -----

echo "  === Windows Standalone v$SQUEEZER_VERSION (32 bit) ==="
echo

move_new_executable "Squeezer (Stereo).exe"

delete_old_archive "$SQUEEZER_RELEASE_DIR/w32/squeezer-standalone.zip"

SQUEEZER_ARCHIVE_DIR="squeezer-standalone_$SQUEEZER_VERSION"

create_new_archive "$SQUEEZER_ARCHIVE_DIR"

fill_archive "$SQUEEZER_EXECUTABLE_DIR/Squeezer (Stereo).exe" "$SQUEEZER_ARCHIVE_DIR"
fill_archive "$SQUEEZER_DOCUMENTATION_DIR/LICENSE" "$SQUEEZER_ARCHIVE_DIR"
fill_archive "$SQUEEZER_DOCUMENTATION_DIR/squeezer.pdf" "$SQUEEZER_ARCHIVE_DIR"

compress_new_archive "$SQUEEZER_RELEASE_DIR/w32/$SQUEEZER_ARCHIVE_DIR.zip" "$SQUEEZER_ARCHIVE_DIR" "zip"


# ----- Windows VST (32 bit) -----

echo "  === Windows VST v$SQUEEZER_VERSION (32 bit) ==="
echo

move_new_executable "Squeezer (Stereo).dll"

delete_old_archive "$SQUEEZER_RELEASE_DIR/w32/squeezer-vst.zip"

SQUEEZER_ARCHIVE_DIR="squeezer-vst_$SQUEEZER_VERSION"

create_new_archive "$SQUEEZER_ARCHIVE_DIR"

fill_archive "$SQUEEZER_EXECUTABLE_DIR/Squeezer (Stereo).dll" "$SQUEEZER_ARCHIVE_DIR"
fill_archive "$SQUEEZER_DOCUMENTATION_DIR/LICENSE" "$SQUEEZER_ARCHIVE_DIR"
fill_archive "$SQUEEZER_DOCUMENTATION_DIR/squeezer.pdf" "$SQUEEZER_ARCHIVE_DIR"

compress_new_archive "$SQUEEZER_RELEASE_DIR/w32/$SQUEEZER_ARCHIVE_DIR.zip" "$SQUEEZER_ARCHIVE_DIR" "zip"
