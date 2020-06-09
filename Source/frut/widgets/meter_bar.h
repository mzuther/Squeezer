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

#ifndef FRUT_WIDGETS_METER_BAR_H
#define FRUT_WIDGETS_METER_BAR_H

namespace frut
{
namespace widgets
{

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
   MeterBar();

   virtual void create();

   virtual void addSegment( widgets::MeterSegment* segment,
                            int segmentHeight,
                            int spacingBefore );

   virtual void addDiscreteSegment( float lowerThreshold,
                                    float thresholdRange,
                                    float retainSignalFactor,
                                    float newSignalFactor,
                                    bool isTopmost,
                                    int segmentHeight,
                                    int spacingBefore,
                                    const Colour& segmentColour,
                                    const Colour& colPeakMarker );

   virtual void addContinuousSegment( float lowerThreshold,
                                      float thresholdRange,
                                      float nextPixelRange,
                                      bool isTopmost,
                                      int segmentHeight,
                                      int spacingBefore,
                                      const Colour& segmentColour,
                                      const Colour& colPeakMarker );

   virtual widgets::Orientation getOrientation();
   virtual void setOrientation( const widgets::Orientation& orientation );

   virtual int getSegmentWidth();
   virtual void setSegmentWidth( int segmentWidth );

   virtual void setNormalLevels( float normalLevel,
                                 float normalLevelPeak );

   virtual void setDiscreteLevels( float discreteLevel,
                                   float discreteLevelPeak );

   virtual void setLevels( float normalLevel,
                           float normalLevelPeak,
                           float discreteLevel,
                           float discreteLevelPeak );

   virtual void paint( Graphics& g );
   virtual void resized();

private:
   JUCE_LEAK_DETECTOR( MeterBar );

   float normalLevel_;
   float normalLevelPeak_;

   float discreteLevel_;
   float discreteLevelPeak_;

   int barWidth_;
   int barHeight_;
   int segmentWidth_;

   bool isVertical_;
   bool isInverted_;

   widgets::Orientation orientation_;
   Array<int> segmentSpacing_;
   OwnedArray<widgets::MeterSegment> meterSegments_;
};

}
}

#endif  // FRUT_WIDGETS_METER_BAR_H
