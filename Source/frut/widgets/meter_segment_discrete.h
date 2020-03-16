/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2020 Martin Zuther (http://www.mzuther.de/)

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

#ifndef FRUT_WIDGETS_METER_SEGMENT_DISCRETE_H
#define FRUT_WIDGETS_METER_SEGMENT_DISCRETE_H

namespace frut
{
namespace widgets
{

/// Discrete meter segment component.  This widget consists of a
/// coloured filled rectangle (meter segment) surrounded by a small
/// coloured rectangle (peak marker).  Both rectangles react to level
/// changes with a change in colour or visibility.
///
/// @see MeterBar
///
class MeterSegmentDiscrete :
    public MeterSegment
{
public:
    explicit MeterSegmentDiscrete(float retainSignalFactor,
                                  float newSignalFactor);


    virtual float setThresholdAndRange(float lowerThreshold,
                                       float thresholdRange,
                                       bool isTopmost);

    virtual void setColours(const Colour &segmentColour,
                            const Colour &peakMarkerColour);

    virtual void setLevels(float normalLevel,
                           float normalLevelPeak,
                           float discreteLevel,
                           float discreteLevelPeak);

    virtual void paint(Graphics &g);
    virtual void visibilityChanged();

protected:
    float lowerThreshold_;
    float upperThreshold_;
    float thresholdRange_;

    Colour segmentColour_;
    Colour peakMarkerColour_;
    Colour attenuatedColour_;

    float brightness_;
    float retainSignalFactor_;
    float newSignalFactor_;

    bool displayPeakMarker_;
    bool isTopmost_;

private:
    JUCE_LEAK_DETECTOR(MeterSegmentDiscrete);
};

}
}

#endif  // FRUT_WIDGETS_METER_SEGMENT_DISCRETE_H
