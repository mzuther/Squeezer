echo
find . -iname "*.cpp" -or -iname '*.h' -print -exec sh -c "cat {} | /usr/bin/clang -fsyntax-only -fno-caret-diagnostics -Wall -x c++ -I ./amalgamated -I ../../JuceLibraryCode -I ../../libraries -I ../../libraries/juce/modules -I . - -std=c++14 -include ./FrutHeader.h -Wno-pragma-once-outside-header " \;
echo