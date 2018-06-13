project_home=$(pwd)/../..


function lint_file
{
    filename="$1"
    dirname=$(dirname "$1")
    echo $filename

    cat $filename | /usr/bin/clang -x c++ - \
        -include $project_home/Source/frut/FrutHeader.h \
        -I $project_home/JuceLibraryCode \
        -I $project_home/libraries \
        -I $project_home/libraries/juce/modules \
        -I $project_home/Source \
        -I $project_home/Source/frut \
        -I $project_home/Source/frut/amalgamated \
        -I $dirname \
        -fsyntax-only \
        -fno-caret-diagnostics \
        -std=c++14 \
        -Wall
}


echo

find . \( -iname "*.cpp" -or -iname "*.h" \) -print | sort | while read filename
       do
        lint_file $filename \;
       done

echo
