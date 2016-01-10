/* ----------------------------------------------------------------------------

   MZ common JUCE
   ==============
   Common classes for use with the JUCE library

   Copyright (c) 2010-2016 Martin Zuther (http://www.mzuther.de/)

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

#ifndef __GENERIC_STATE_LABEL_H__
#define __GENERIC_STATE_LABEL_H__

#include "JuceHeader.h"


/// State label component.  This widget loads three images of the same
/// size which will be displayed according to the label's state.  It
/// can also display a text string.
///
/// | %State | %Example                            |
/// | :----: | ----------------------------------- |
/// | off    | no overload detected yet            |
/// | on     | overload(s) detected in the past    |
/// | active | currently dectecting an overload    |
///
class GenericStateLabel : public Component
{
public:
    enum Parameters  // public namespace!
    {
        /// off, no overload detected yet
        stateOff = 0,
        /// on, overload(s) detected in the past
        stateOn,
        /// active, currently dectecting an overload
        stateActive,

        numberOfStates,
    };

    GenericStateLabel(const String &componentName);

    void resized();
    void setState(int newState, bool forceUpdate = false);
    void setLabelColour(const Colour &newColour);
    void setLabelText(const String &newText);

    void setImages(Image &imageOffNew, Image &imageOnNew, Image &imageActiveNew, String &colourStringOn, String &colourStringActive, int horizontalTextSpacingNew, int verticalTextSpacingNew, float fontSize);

protected:
    void updateState();

private:
    JUCE_LEAK_DETECTOR(GenericStateLabel);

    int currentState;
    int horizontalTextSpacing;
    int verticalTextSpacing;

    Label textLabel;
    ImageComponent imageComponent;

    Image imageOff;
    Image imageOn;
    Image imageActive;

    Colour colOn;
    Colour colActive;
};


#endif  // __GENERIC_STATE_LABEL_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
