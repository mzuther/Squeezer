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
    MeterSegment(const String& componentName, float fThreshold, float fRange, bool bDiscreteLevels, bool bDisplayPeaks, int nColor);
    ~MeterSegment();

    void setLevels(float fLevel, float fLevelPeak);
    void paint(Graphics& g);
    void resized();
    void visibilityChanged();

private:
    JUCE_LEAK_DETECTOR(MeterSegment);

    float fHue;
    float fBrightness;

    float fLowerThreshold;
    float fUpperThreshold;
    float fThresholdRange;

    bool bPeakMarker;
    bool discreteLevels;
    bool displayPeaks;
};


#endif  // __METER_SEGMENT_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
