/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013 Martin Zuther (http://www.mzuther.de/)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   Thank you for using free software!

---------------------------------------------------------------------------- */

#ifndef __STANDALONE_APPLICATION_H__
#define __STANDALONE_APPLICATION_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "plugin_processor_squeezer.h"
#include "juce/modules/juce_audio_plugin_client/Standalone/juce_StandaloneFilterWindow.h"

class StandaloneApplication : public JUCEApplication
{
public:
    StandaloneApplication();
    ~StandaloneApplication();

    void initialise(const String& commandLineParameters);
    void shutdown();

    const String getApplicationName();
    const String getApplicationVersion();

private:
    JUCE_LEAK_DETECTOR(StandaloneApplication);

    StandaloneFilterWindow* filterWindow;
};

#endif   // __STANDALONE_APPLICATION_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
