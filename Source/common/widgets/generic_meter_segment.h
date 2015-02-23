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

#ifndef __GENERIC_METER_SEGMENT_H__
#define __GENERIC_METER_SEGMENT_H__

#include "JuceHeader.h"


//==============================================================================
/**
*/
class GenericMeterSegment : public Component
{
public:
    GenericMeterSegment(float fLowerThreshold = -144.0f, float fDisplayRange = 1.0f, bool bHasHighestLevel = false);
    ~GenericMeterSegment();

    float setThresholds(float fLowerThreshold, float fDisplayRange, bool bHasHighestLevel);
    void setColour(float fHueNew, const Colour &colPeakNew);

    void setNormalLevels(float normalLevel, float normalLevelPeak);
    void setDiscreteLevels(float discreteLevel, float discreteLevelPeak);
    void setLevels(float normalLevel, float discreteLevel, float normalLevelPeak, float discreteLevelPeak);

    void paint(Graphics &g);
    void resized();
    void visibilityChanged();

private:
    JUCE_LEAK_DETECTOR(GenericMeterSegment);

    Colour colPeak;

    float fHue;
    float fBrightness;
    float fBrightnessOutline;

    float lowerThreshold;
    float upperThreshold;
    float thresholdRange;

    bool lightPeakMarker;
    bool hasHighestLevel;
};


#endif  // __GENERIC_METER_SEGMENT_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
