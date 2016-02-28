/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
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

#ifndef __FRUT_WIDGET_METER_BAR_H__
#define __FRUT_WIDGET_METER_BAR_H__


/// Meter bar component.  This widget is an empty container that can
/// be filled with meter segment widgets.  These can then be
/// comfortably updated with a single function call.
///
/// @see MeterSegment
///
class MeterBar :
    public Component
{
public:
    virtual void create();

    virtual void addSegment(widget::MeterSegment *segment,
                            int segmentHeight,
                            int spacingBefore);

    virtual void addDiscreteSegment(float lowerThreshold,
                                    float thresholdRange,
                                    bool isTopmost,
                                    int segmentHeight,
                                    int spacingBefore,
                                    const Colour &segmentColour,
                                    const Colour &colPeakMarker);

    virtual void addContinuousSegment(float lowerThreshold,
                                      float thresholdRange,
                                      bool isTopmost,
                                      int segmentHeight,
                                      int spacingBefore,
                                      const Colour &segmentColour,
                                      const Colour &colPeakMarker);

    virtual widget::Orientation getOrientation();
    virtual void setOrientation(widget::Orientation orientation);

    virtual void invertMeter(bool invert);
    virtual bool isMeterInverted();

    virtual int getSegmentWidth();
    virtual void setSegmentWidth(int segmentWidth);

    virtual void setNormalLevels(float normalLevel,
                                 float normalLevelPeak);

    virtual void setDiscreteLevels(float discreteLevel,
                                   float discreteLevelPeak);

    virtual void setLevels(float normalLevel,
                           float normalLevelPeak,
                           float discreteLevel,
                           float discreteLevelPeak);

    virtual void paint(Graphics &g);
    virtual void resized();

private:
    JUCE_LEAK_DETECTOR(MeterBar);

    float normalLevel_;
    float normalLevelPeak_;

    float discreteLevel_;
    float discreteLevelPeak_;

    int barWidth_;
    int barHeight_;
    int segmentWidth_;

    bool isVertical_;
    bool isInverted_;

    widget::Orientation orientation_;
    Array<int> segmentSpacing_;
    OwnedArray<widget::MeterSegment> meterSegments_;
};


#endif  // __FRUT_WIDGET_METER_BAR_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
