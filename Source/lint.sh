#! /usr/bin/env bash

#  ----------------------------------------------------------------------------
#  
#  Squeezer
#  ========
#  Flexible general-purpose audio compressor with a touch of citrus
#  
#  Copyright (c) 2013-2020 Martin Zuther (http://www.mzuther.de/)
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
#  ----------------------------------------------------------------------------


###############################################################################
#
#  WARNING: this file is auto-generated, please do not edit!
#
###############################################################################

project_home=$(pwd)/..
parallel_threads=12


function lint_file
{
    filename="$1"
    dirname=$(dirname "$1")
    printf "%s\n" "$filename"

    /usr/bin/clang \
        -x c++ - \
        -include "$project_home/Source/frut/FrutHeader.h" \
        -I "$project_home/JuceLibraryCode" \
        -I "$project_home/libraries" \
        -I "$project_home/libraries/juce/modules" \
        -I "$project_home/Source/frut" \
        -I "$project_home/libraries/googletest/googletest/include" \
        -I "$dirname" \
        -fsyntax-only \
        -fno-caret-diagnostics \
        -std=c++14 \
        -Wall \
        < "$filename"

    /usr/bin/cppcheck \
        --template=gcc \
        --enable=style \
        --include="$project_home/Source/frut/FrutHeader.h" \
        --inline-suppr \
        --language=c++ \
        --quiet \
        "$filename"

    # find error-like codetags
    GREP_COLORS="mt=01;31" /bin/grep \
               --extended-regexp --colour \
               '\<(FIXME|BUG)\>' \
               "$filename"

    # find warning-like codetags
    GREP_COLORS="mt=01;33" /bin/grep \
               --extended-regexp --colour \
               '\<(TODO|@todo)\>' \
               "$filename"
}


printf "\n"

for filename in $(find . -maxdepth 1 \( -iname "*.cpp" -or -iname "*.h" \) -print | sort); do
    # use parallel threads to improve performance; adapted from
    # https://unix.stackexchange.com/a/216475
    ((i=i%parallel_threads)); ((i++==0)) && wait
    lint_file "$filename" &
done

for filename in $(find ./frut/amalgamated -maxdepth 1 \( -iname "*.cpp" -or -iname "*.h" \) -print | sort); do
    # use parallel threads to improve performance; adapted from
    # https://unix.stackexchange.com/a/216475
    ((i=i%parallel_threads)); ((i++==0)) && wait
    lint_file "$filename" &
done

printf "\n"
