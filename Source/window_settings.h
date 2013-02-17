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

#ifndef __WINDOW_SETTINGS_H__
#define __WINDOW_SETTINGS_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "resources/resources.h"
#include "prohibiting_bounds_constrainer.h"


class WindowSettings : public ResizableWindow, ButtonListener
{
public:
    WindowSettings(int nWidth, int nHeight, String& strPluginSettings);
    ~WindowSettings();

    void paint(Graphics& g);
    void buttonClicked(Button* button);

private:
    JUCE_LEAK_DETECTOR(WindowSettings);

    Component* contentComponent;
    ProhibitingBoundsConstrainer* pConstrainer;

    TextEditor* TextEditorSettings;
    TextButton* ButtonSettings;
};


#endif  // __WINDOW_SETTINGS_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
