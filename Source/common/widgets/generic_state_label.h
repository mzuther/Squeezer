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
class GenericStateLabel :
    public Component
{
public:

    enum State  // public namespace!
    {
        /// off, no overload detected yet
        off = 0,
        /// on, overload(s) detected in the past
        on,
        /// active, currently dectecting an overload
        active,

        numberOfStates,
    };


    GenericStateLabel(const String &componentName);

    virtual void resized();
    void setState(int state,
                  bool forceUpdate = false);

    void setLabelText(const String &text);

    void setImages(const Image &imageOff,
                   const Image &imageOn,
                   const Image &imageActive,
                   const String &textColourOn,
                   const String &textColourActive,
                   int horizontalTextSpacing,
                   int verticalTextSpacing,
                   float fontSize);

protected:
    void updateState();
    void setLabelColour(const Colour &textColour);

    int state_;
    int horizontalTextSpacing_;
    int verticalTextSpacing_;

    Label textLabel_;
    ImageComponent imageComponent_;

    Image imageOff_;
    Image imageOn_;
    Image imageActive_;

    Colour textColourOn_;
    Colour textColourActive_;

private:
    JUCE_LEAK_DETECTOR(GenericStateLabel);
};


#endif  // __GENERIC_STATE_LABEL_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
