/* ----------------------------------------------------------------------------

   MZ common JUCE
   ==============
   Common classes for use with the JUCE library

   Copyright (c) 2010-2015 Martin Zuther (http://www.mzuther.de/)

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

#ifndef __GENERIC_METER_BAR_H__
#define __GENERIC_METER_BAR_H__

#include "JuceHeader.h"
#include "generic_meter_segment.h"


//==============================================================================
/**
*/
class GenericMeterBar : public Component
{
public:
    GenericMeterBar();
    ~GenericMeterBar();

    enum Orientation  // public namespace!
    {
        orientationHorizontal = 0,
        orientationVertical,
    };

    void create();
    void setOrientation(GenericMeterBar::Orientation orientationNew);
    GenericMeterBar::Orientation getOrientation();

    void addSegment(float fLowerThreshold, float fDisplayRange, bool bHasHighestLevel, int nSegmentHeight, int nSpacingBefore, float fHue, const Colour &colPeak);

    void setNormalLevels(float normalLevel, float normalLevelPeak);
    void setDiscreteLevels(float discreteLevel, float discreteLevelPeak);
    void setLevels(float normalLevel, float discreteLevel, float normalLevelPeak, float discreteLevelPeak);

    void paint(Graphics &g);
    void resized();

private:
    JUCE_LEAK_DETECTOR(GenericMeterBar);

    float fNormalLevel;
    float fDiscreteLevel;

    float fNormalLevelPeak;
    float fDiscreteLevelPeak;

    int nNumberOfBars;

    int nWidth;
    int nHeight;

    GenericMeterBar::Orientation orientation;

    OwnedArray<GenericMeterSegment> p_arrMeterSegments;
};


#endif  // __GENERIC_METER_BAR_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
