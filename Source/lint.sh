project_home=$(pwd)/..


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

    /usr/bin/cppcheck \
        --template=gcc \
        --enable=style \
        --include=$project_home/Source/frut/FrutHeader.h \
        --inline-suppr \
        --language=c++ \
        --quiet \
        $filename

    # find error-like codetags
    GREP_COLORS="mt=01;31" \
    /bin/egrep \
        --colour \
        '\<(FIXME|BUG)\>' \
        $filename

    # find warning-like codetags
    GREP_COLORS="mt=01;33" \
    /bin/egrep \
        --colour \
        '\<(TODO|@todo)\>' \
        $filename
}


echo

find . -maxdepth 1 \( -iname "*.cpp" -or -iname "*.h" \) -print | sort | while read filename
       do
        lint_file $filename \;
       done

echo

find ./frut/amalgamated -maxdepth 1 \( -iname "*.cpp" -or -iname "*.h" \) -print | sort | while read filename
       do
        lint_file $filename \;
       done

echo
