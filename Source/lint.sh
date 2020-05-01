#! /usr/bin/env bash

project_home=$(pwd)/..


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
        -I "$project_home/libraries/googletest/googletest/include" \
        -I "$project_home/Source" \
        -I "$project_home/Source/frut" \
        -I "$project_home/Source/frut/amalgamated" \
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

find . -maxdepth 1 \( -iname "*.cpp" -or -iname "*.h" \) -print | sort | while read -r filename
do
    lint_file "$filename" \;
done

printf "\n"

find ./frut/amalgamated -maxdepth 1 \( -iname "*.cpp" -or -iname "*.h" \) -print | sort | while read -r filename
do
    lint_file "$filename" \;
done

printf "\n"
