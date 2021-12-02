/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2021 Martin Zuther (http://www.mzuther.de/)

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

#ifndef FRUT_WIDGETS_METER_SEGMENT_CONTINUOUS_H
#define FRUT_WIDGETS_METER_SEGMENT_CONTINUOUS_H

namespace frut::widgets
{

/// Continuous meter segment component.  This widget consists of a
/// coloured filled rectangle (meter segment) surrounded by a small
/// coloured rectangle (peak marker).  Both rectangles react to level
/// changes with a change in colour or visibility.
///
/// @see MeterBar
///
class MeterSegmentContinuous :
   public MeterSegment
{
public:
   MeterSegmentContinuous();
   void init();

   virtual float setThresholdAndRange( float lowerThreshold,
                                       float thresholdRange,
                                       float nextPixelRange,
                                       bool isTopmost );

   virtual void setColours( const Colour& segmentColour,
                            const Colour& peakMarkerColour );

   virtual void setLevels( float normalLevel,
                           float normalLevelPeak,
                           float discreteLevel,
                           float discreteLevelPeak ) override;

   virtual void paint( Graphics& g )  override;
   virtual void resized() override;
   virtual void visibilityChanged() override;

protected:
   virtual void drawBar( Graphics& g,
                         float levelPosition );

   virtual void drawMarker( Graphics& g,
                            const Colour& markerColour,
                            float levelPosition );

   float calculateLevelPosition( float level,
                                 bool isBar );

   float lowerThreshold_;
   float upperThreshold_;
   float thresholdRange_;
   float nextPixelThreshold_;

   Colour segmentColour_;
   Colour backgroundColour_;
   Colour peakMarkerColour_;
   Colour attenuatedColour_;

   int maximumX_;
   int maximumY_;

   float normalLevelPosition_;
   float normalPeakPosition_;

   float discreteLevelPosition_;
   float discretePeakPosition_;

   bool isTopmost_;

private:
   JUCE_LEAK_DETECTOR( MeterSegmentContinuous );
};

} // namespace

#endif  // FRUT_WIDGETS_METER_SEGMENT_CONTINUOUS_H
