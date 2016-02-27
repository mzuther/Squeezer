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


class GenericHorizontalMeter :
    public Component
{
public:
    GenericHorizontalMeter();

    void setImages(const Image &imageBackground,
                   const Image &imageNeedle,
                   int spacingLeft,
                   int spacingTop);

    void setValue(float value);

    virtual void paint(Graphics &g);
    virtual void resized();

protected:
    int needlePosition_;
    int needleTravelPath_;
    bool isVerticalMeter_;

    int width_;
    int height_;
    int spacingLeft_;
    int spacingTop_;

    Image imageBackground_;
    Image imageNeedle_;

private:
    JUCE_LEAK_DETECTOR(GenericHorizontalMeter);
};


#endif  // __GENERIC_HORIZONTAL_METER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
