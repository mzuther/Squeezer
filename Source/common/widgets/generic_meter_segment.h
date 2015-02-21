/* ----------------------------------------------------------------------------

   K-Meter
   =======
   Implementation of a K-System meter according to Bob Katz' specifications

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
    GenericMeterSegment(const String &componentName, float fThreshold, float fRange, bool bDisplayPeakMeter);
    ~GenericMeterSegment();

    void setColour(float fHueNew, const Colour &colPeakNew);
    void setLevels(float fPeakLevel, float fAverageLevel, float fPeakLevelPeak, float fAverageLevelPeak);

    void paint(Graphics &g);
    void resized();
    void visibilityChanged();

private:
    JUCE_LEAK_DETECTOR(GenericMeterSegment);

    static const int PEAK_MARKER_NONE = 0;
    static const int PEAK_MARKER_PEAK = 1;
    static const int PEAK_MARKER_AVERAGE = 2;

    Colour colPeak;

    float fHue;
    float fBrightness;
    float fBrightnessOutline;

    float fLowerThreshold;
    float fUpperThreshold;
    float fThresholdRange;

    int nPeakMarker;
    bool displayPeakMeter;
};


#endif  // __GENERIC_METER_SEGMENT_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
