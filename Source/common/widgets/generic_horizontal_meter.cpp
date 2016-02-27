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

#include "generic_horizontal_meter.h"


GenericHorizontalMeter::GenericHorizontalMeter()
{
    // this component does not have any transparent areas (increases
    // performance on redrawing)
    setOpaque(true);

    needlePosition_ = -1;
    needleTravelPath_ = 1;
    isVerticalMeter_ = false;

    width_ = 0;
    height_ = 0;
    spacingLeft_ = 0;
    spacingTop_ = 0;
}


void GenericHorizontalMeter::paint(
    Graphics &g)

{
    g.setColour(Colours::black);
    g.drawImageAt(imageBackground_, 0, 0, false);

    if (isVerticalMeter_)
    {
        g.drawImageAt(imageNeedle_, spacingLeft_, needlePosition_, false);
    }
    else
    {
        g.drawImageAt(imageNeedle_, needlePosition_, spacingTop_, false);
    }
}


void GenericHorizontalMeter::resized()
{
    width_ = getWidth();
    height_ = getHeight();

    isVerticalMeter_ = (height_ > width_);

    if (isVerticalMeter_)
    {
        needleTravelPath_ = height_ - 2 * spacingTop_;
        needleTravelPath_ -= imageNeedle_.getHeight();
    }
    else
    {
        needleTravelPath_ = width_ - 2 * spacingLeft_;
        needleTravelPath_ -= imageNeedle_.getWidth();
    }
}


void GenericHorizontalMeter::setValue(
    float value)

{
    int needlePositionOld = needlePosition_;

    needlePosition_ = static_cast<int>(value * needleTravelPath_ + 0.5f);
    needlePosition_ += isVerticalMeter_ ? spacingTop_ : spacingLeft_;

    if (needlePosition_ != needlePositionOld)
    {
        repaint(getLocalBounds());
    }
}


void GenericHorizontalMeter::setImages(
    const Image &imageBackground,
    const Image &imageNeedle,
    int spacingLeft,
    int spacingTop)

{
    spacingLeft_ = spacingLeft;
    spacingTop_ = spacingTop;

    imageBackground_ = Image(imageBackground);
    imageNeedle_ = Image(imageNeedle);

    resized();
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
