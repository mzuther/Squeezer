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

#ifndef __GENERIC_HORIZONTAL_METER_H__
#define __GENERIC_HORIZONTAL_METER_H__

#include "JuceHeader.h"


//==============================================================================
/**
*/
class GenericHorizontalMeter : public Component
{
public:
    GenericHorizontalMeter(const String &componentName);

    void setImages(Image &imageBackgroundNew, Image &imageNeedleNew, int nSpacingLeftNew, int nSpacingTopNew);
    void setValue(float newValue);
    void paint(Graphics &g);
    void initialise();
    void resized();

private:
    JUCE_LEAK_DETECTOR(GenericHorizontalMeter);

    int nNeedlePosition;
    int nNeedleTravelPath;
    bool bVerticalMeter;

    int nWidth;
    int nHeight;
    int nSpacingLeft;
    int nSpacingTop;

    Image imageBackground;
    Image imageNeedle;
};


#endif  // __GENERIC_HORIZONTAL_METER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
