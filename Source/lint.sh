echo
find ./frut/amalgamated/ -maxdepth 1 -iname "*.cpp" -or -iname "*.h" -print -exec sh -c "cat {} | /usr/bin/clang -fsyntax-only -fno-caret-diagnostics -Wall -x c++ -I ./frut/amalgamated -I ../JuceLibraryCode -I ../libraries -I ../libraries/juce/modules -I . -I ./frut - -std=c++14 -include ./frut/FrutHeader.h -Wno-pragma-once-outside-header " \;
echo
find . -maxdepth 1 -iname "*.cpp" -or -iname "*.h" -print -exec sh -c "cat {} | /usr/bin/clang -fsyntax-only -fno-caret-diagnostics -Wall -x c++ -I ./frut/amalgamated -I ../JuceLibraryCode -I ../libraries -I ../libraries/juce/modules -I . -I ./frut - -std=c++14 -include ./frut/FrutHeader.h -Wno-pragma-once-outside-header " \;
echo
