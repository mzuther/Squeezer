#! /usr/bin/env bash

#  ----------------------------------------------------------------------------
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
#  ----------------------------------------------------------------------------


###############################################################################
#
#  WARNING: this file was auto-generated, please do not edit!
#
###############################################################################

function print_line
{
    printf "%s\n" "$1"
}


# select platform
case $1 in
    1)
        PLATFORM="debug_x64"
        PLATFORM_PRINT="64-bit (Debug)"
        EXECUTABLE_EXTENSION="_x64_debug"
        ;;
    2)
        PLATFORM="release_x64"
        PLATFORM_PRINT="64-bit (Release)"
        EXECUTABLE_EXTENSION="_x64"
        ;;
    3)
        PLATFORM="debug_x32"
        PLATFORM_PRINT="32-bit (Debug)"
        EXECUTABLE_EXTENSION="_debug"
        ;;
    4)
        PLATFORM="release_x32"
        PLATFORM_PRINT="32-bit (Release)"
        EXECUTABLE_EXTENSION=""
        ;;
    *)
        print_line ""
        print_line "  Usage:  build PLATFORM TARGET [BUILD_OPTIONS]"
        print_line ""
        print_line ""
        print_line "  Platform:  ..."
        print_line ""
        print_line "  1: 64-bit (Debug)"
        print_line "  2: 64-bit (Release)"
        print_line ""
        print_line "  3: 32-bit (Debug)"
        print_line "  4: 32-bit (Release)"
        print_line ""

        exit
esac

print_line ""
print_line "  Platform:  $PLATFORM_PRINT"

# get next command line option
shift 1

# select target
case $1 in
    a)
        MAKEFILE="all"
        MAKEFILE_PRINT="all targets"
        ;;
    c)
        MAKEFILE="clean"
        MAKEFILE_PRINT="clean targets"
        ;;
    u)
        MAKEFILE="unittest"
        MAKEFILE_PRINT="Unit tests"
        EXECUTABLE="unittest/unittest"
        ;;
    1)
        MAKEFILE="squeezer_standalone_stereo"
        MAKEFILE_PRINT="Standalone (stereo)"
        EXECUTABLE="standalone/squeezer"
        ;;
    2)
        MAKEFILE="squeezer_standalone_mono"
        MAKEFILE_PRINT="Standalone (mono)"
        EXECUTABLE="standalone/squeezer_mono"
        ;;
    3)
        MAKEFILE="squeezer_vst2_stereo"
        MAKEFILE_PRINT="VST2 plug-in (stereo)"
        ;;
    4)
        MAKEFILE="squeezer_vst2_mono"
        MAKEFILE_PRINT="VST2 plug-in (mono)"
        ;;
    5)
        MAKEFILE="squeezer_vst2_sidechain_stereo"
        MAKEFILE_PRINT="VST2 plug-in (side-chain, stereo)"
        ;;
    6)
        MAKEFILE="squeezer_vst2_sidechain_mono"
        MAKEFILE_PRINT="VST2 plug-in (side-chain, mono)"
        ;;
    7)
        MAKEFILE="squeezer_vst3"
        MAKEFILE_PRINT="VST3 plug-in"
        ;;
    *)
        print_line "  Target:    ..."
        print_line ""
        print_line "  a: All targets"
        print_line "  c: Clean targets"
        print_line "  u: Unit tests"
        print_line ""
        print_line "  1: Standalone (stereo)"
        print_line "  2: Standalone (mono)"
        print_line ""
        print_line ""
        print_line "  3: VST2 plug-in (stereo)"
        print_line "  4: VST2 plug-in (mono)"
        print_line "  5: VST2 plug-in (side-chain, stereo)"
        print_line "  6: VST2 plug-in (side-chain, mono)"
        print_line ""
        print_line "  7: VST3 plug-in"
        print_line ""

        exit
esac

print_line "  Target:    $MAKEFILE_PRINT"

# get next command line option
shift 1

# display additional options
if [ -z "$1" ]; then
    print_line "  Options:   --no-print-directory"
    print_line ""
else
    print_line "  Options:  --no-print-directory $*"
    print_line ""
fi

# indent and format code
if [ "$MAKEFILE" != "clean" ]; then
    print_line "==== Indenting and formatting code ===="

    cd "../Source" || exit
    "./format_code.sh"
    cd "../Builds" || exit

    print_line ""
fi

# compile target
make --directory=linux/gmake/ --no-print-directory --output-sync config=$PLATFORM $MAKEFILE $* 2>&1

# compilation was successful
if [ $? -eq 0 ]; then
    # execute file if possible
    if [ -n "$EXECUTABLE" ]; then
        printf "\nExecuting \"%s\" ...\n\n" "$EXECUTABLE$EXECUTABLE_EXTENSION"

        ../bin/$EXECUTABLE$EXECUTABLE_EXTENSION
    fi
fi
