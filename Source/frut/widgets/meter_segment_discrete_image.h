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

#ifndef FRUT_WIDGETS_METER_SEGMENT_DISCRETE_IMAGE_H
#define FRUT_WIDGETS_METER_SEGMENT_DISCRETE_IMAGE_H

namespace frut::widgets
{

/// Discrete meter segment component.  This widget consists of a
/// coloured filled rectangle (meter segment) surrounded by a small
/// coloured rectangle (peak marker).  Both rectangles react to level
/// changes with a change in colour or visibility.
///
/// @see MeterBar
///
class MeterSegmentDiscreteImage :
   public MeterSegment
{
public:
   MeterSegmentDiscreteImage();
   void init( float retainSignalFactor,
              float newSignalFactor,
              Image imageOn,
              Point<int> topLeftImageOn,
              Image imageOff,
              Point<int> topLeftImageOff,
              Image imagePeak,
              Point<int> topLeftImagePeak );

   virtual float setThresholdAndRange( float lowerThreshold,
                                       float thresholdRange,
                                       bool isTopmost );

   virtual void setLevels( float normalLevel,
                           float normalLevelPeak,
                           float discreteLevel,
                           float discreteLevelPeak ) override;

   virtual void resized() override;
   virtual void visibilityChanged() override;

protected:
   ImageComponent imageComponentOn_;
   ImageComponent imageComponentOff_;
   ImageComponent imageComponentPeak_;

   Point<int> topLeftImageOn_;
   Point<int> topLeftImageOff_;
   Point<int> topLeftImagePeak_;

   float lowerThreshold_;
   float upperThreshold_;
   float thresholdRange_;

   float brightness_;
   float retainSignalFactor_;
   float newSignalFactor_;

   bool displayPeakMarker_;
   bool isTopmost_;

private:
   JUCE_LEAK_DETECTOR( MeterSegmentDiscreteImage );
};

} // namespace

#endif  // FRUT_WIDGETS_METER_SEGMENT_DISCRETE_IMAGE_H
