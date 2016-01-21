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

#ifndef __GENERIC_METER_BAR_H__
#define __GENERIC_METER_BAR_H__

#include "JuceHeader.h"
#include "generic_meter_segment_discrete.h"


/// Meter bar component.  This widget is an empty container that can
/// be filled with meter segment widgets.  These can then be
/// comfortably updated with a single function call.
///
/// @see GenericMeterSegment
///
class GenericMeterBar : public Component
{
public:
    GenericMeterBar();

    /// Orientation for the meter.
    enum Orientation  // public namespace!
    {
        /// horizontal meter (bottom to top)
        orientationHorizontal = 0,
        /// inverted horizontal meter (top to bottom)
        orientationHorizontalInverted,
        /// vertical meter (left to right)
        orientationVertical,
        /// inverted vertical meter (right to left)
        orientationVerticalInverted
    };

    void create();
    void addSegment(float lowerThreshold, float thresholdRange, bool isTopmost,
                    int segmentHeight, int spacingBefore, float segmentHue,
                    const Colour &colPeakMarker);

    GenericMeterBar::Orientation getOrientation();
    void setOrientation(GenericMeterBar::Orientation barOrientationNew);

    void invertMeter(bool isOrientationInvertedNew);
    bool isMeterInverted();

    int getSegmentWidth();
    void setSegmentWidth(int segmentWidthNew);

    void setNormalLevels(float normalLevelNew,
                         float normalLevelPeakNew);

    void setDiscreteLevels(float discreteLevelNew,
                           float discreteLevelPeakNew);

    void setLevels(float normalLevelNew,
                   float normalLevelPeakNew,
                   float discreteLevelNew,
                   float discreteLevelPeakNew);

    void paint(Graphics &g);
    void resized();

private:
    JUCE_LEAK_DETECTOR(GenericMeterBar);

    float normalLevel;
    float normalLevelPeak;

    float discreteLevel;
    float discreteLevelPeak;

    int barWidth;
    int barHeight;
    int segmentWidth;

    bool isOrientationVertical;
    bool isOrientationInverted;

    GenericMeterBar::Orientation barOrientation;
    Array<int> arrSegmentSpacing;
    OwnedArray<GenericMeterSegment> p_arrMeterSegments;
};


#endif  // __GENERIC_METER_BAR_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
