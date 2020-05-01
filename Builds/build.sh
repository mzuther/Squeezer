#!/bin/bash

function print_line
{
    printf "%s\n" "$1"
}


# select platform
case $1 in
    1)
        PLATFORM="debug_x64"
        PLATFORM_PRINT="64-bit (Debug)"
        EXECUTABLE_EXTENSION="_debug_x64"
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
        print_line "  Usage:  mz_build PLATFORM TARGET [BUILD_OPTIONS]"
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
        MAKEFILE_PRINT="Standalone (Stereo)"
        EXECUTABLE="standalone/squeezer_stereo"
        ;;
    2)
        MAKEFILE="squeezer_standalone_mono"
        MAKEFILE_PRINT="Standalone (Mono)"
        EXECUTABLE="standalone/squeezer_mono"
        ;;
    3)
        MAKEFILE="squeezer_vst2_stereo"
        MAKEFILE_PRINT="VST2 plug-in (Stereo)"
        ;;
    4)
        MAKEFILE="squeezer_vst2_mono"
        MAKEFILE_PRINT="VST2 plug-in (Mono)"
        ;;
    5)
        MAKEFILE="squeezer_vst2_stereo_no_sidechain"
        MAKEFILE_PRINT="VST2 plug-in (Stereo, no side-chain)"
        ;;
    6)
        MAKEFILE="squeezer_vst2_mono_no_sidechain"
        MAKEFILE_PRINT="VST2 plug-in (Mono, no side-chain)"
        ;;
    *)
        print_line "  Target:    ..."
        print_line ""
        print_line "  a: All targets"
        print_line "  c: Clean targets"
        print_line "  u: Unit tests"
        print_line ""
        print_line "  1: Standalone (Stereo)"
        print_line "  2: Standalone (Mono)"
        print_line ""
        print_line "  3: VST2 plug-in (Stereo)"
        print_line "  4: VST2 plug-in (Mono)"
        print_line ""
        print_line "  5: VST2 plug-in (Stereo, no side-chain)"
        print_line "  6: VST2 plug-in (Mono, no side-chain)"
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
make --directory=linux/gmake/ --no-print-directory config=$PLATFORM $MAKEFILE $* 2>&1

# compilation was successful
if [ $? -eq 0 ]; then
    # execute file if possible
    if [ -n "$EXECUTABLE" ]; then
        printf "\nExecuting \"%s\" ...\n\n" "$EXECUTABLE$EXECUTABLE_EXTENSION"

        ../bin/$EXECUTABLE$EXECUTABLE_EXTENSION
    fi
fi
