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

#ifndef __METER_SEGMENT_H__
#define __METER_SEGMENT_H__

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class MeterSegment : public Component
{
public:
    MeterSegment(const String& componentName, float fThreshold, float fRange, bool bDisplayPeakMeter, int nColor);
    ~MeterSegment();

    void setLevels(float fPeakLevel, float fAverageLevel, float fPeakLevelPeak, float fAverageLevelPeak);
    void paint(Graphics& g);
    void resized();
    void visibilityChanged();

private:
    JUCE_LEAK_DETECTOR(MeterSegment);
    static const int PEAK_MARKER_NONE = 0;
    static const int PEAK_MARKER_PEAK = 1;
    static const int PEAK_MARKER_AVERAGE = 2;

    float fHue;
    float fBrightness;

    float fLowerThreshold;
    float fUpperThreshold;
    float fThresholdRange;

    int nPeakMarker;
    bool displayPeakMeter;
};


#endif  // __METER_SEGMENT_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
