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


//==============================================================================
/**
*/
class GenericStateLabel : public Component
{
public:
    enum Parameters  // public namespace!
    {
        stateOff = 0,
        stateOn,
        stateActive,

        nNumStates,
    };

    GenericStateLabel(const String &componentName);
    ~GenericStateLabel();

    void resized();
    void setState(int nStateNew, bool bForceUpdate = false);
    void updateState();

    void setImages(Image &imageOffNew, Image &imageOnNew, Image &imageActiveNew, String &strColourOn, String &strColourActive, int nSpacingLeftNew, int nSpacingTopNew, int nFontSize);

protected:
    ScopedPointer<Label> label;
    ScopedPointer<ImageComponent> backgroundImage;

private:
    JUCE_LEAK_DETECTOR(GenericStateLabel);

    int nSpacingLeft;
    int nSpacingTop;
    int nState;

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
