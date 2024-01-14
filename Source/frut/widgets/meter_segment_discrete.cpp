/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2024 Martin Zuther (http://www.mzuther.de/)

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

namespace frut::widgets
{

/// Create a new discrete meter segment, complete with peak marker.
///
/// > #### Internals
/// >
/// > The meter segment's state depends on two levels, the normal
/// > (usually average) level and the discrete (usually peak) level:
/// >
/// > 1. upper and lower threshold
/// >
/// >    upper threshold = lower threshold + display range
/// >
/// > 2. normal level >= upper threshold
/// >
/// >    normal mode:     segment is fully lit
/// >    auto-fade mode:  segment is dark
/// >
/// > 3. lower threshold <= discrete level < upper threshold
/// >
/// >    segment is fully lit
/// >
/// > 4. normal level < lower threshold
/// >
/// >    segment is dark
/// >
/// > 5. otherwise
/// >
/// >    normal mode:     level affects segment's brightness
/// >    auto-fade mode:  segment is fully lit
/// >
/// > The segment's peak marker is lit when any level peak lies
/// > between the upper and lower threshold (or on the lower
/// > threshold).  If **isTopmost** is **true**, however, the
/// > segment's peak marker is lit when any level peak reaches or
/// > exceeds the lower threshold.
MeterSegmentDiscrete::MeterSegmentDiscrete() :
   lowerThreshold_( 0.0f ),
   upperThreshold_( 0.0f ),
   thresholdRange_( 0.0f ),
   attenuatedColour_( Colours::black.brighter( 0.15f ).withAlpha( 0.6f ) ),
   brightness_( 0.0f ),
   retainSignalFactor_( 0.0f ),
   newSignalFactor_( 0.0f ),
   displayPeakMarker_( true ),
   isTopmost_( false )
{
}


/// Initialise meter segment.
///
/// @param retainSignalFactor if set to value other than 0.0, the segment
///        automatically fades out and all segments with lower
///        thresholds remain dark.  This factor determines how much of
///        the original brightness remains between updates (range: 0.0
///        to 1.0).
///
/// @param newSignalFactor if retainSignalFactor is set to a value other
///        than 0.0, this factor determines how much of the new signal
///        is added to the brightness (range: 0.0 to 1.0).
///
void MeterSegmentDiscrete::init(
   float retainSignalFactor,
   float newSignalFactor )
{
   // initialise segment's brightness (ranging from 0 to 1)
   brightness_ = 0.0f;

   // initialise factor that determines how much of the original
   // brightness remains between updates (range: 0.0 to 1.0)
   retainSignalFactor_ = retainSignalFactor;

   // initialise factor that determines how much of the new
   // signal is added to the brightness (range: 0.0 to 1.0)
   newSignalFactor_ = newSignalFactor;

   // lowest level of a 24-bit-signal in decibels
   float initialLevel = -144.0f;

   // initialise thresholds and set this segment to not be topmost
   setThresholdAndRange( initialLevel, 1.0f, false );

   // make sure that segment does not light up on creation
   initialLevel -= 1.01f;

   // make sure that segment is drawn after initialisation
   setLevels( initialLevel, initialLevel, initialLevel, initialLevel );
}


/// Set lower threshold, display range and whether the segment is the
/// topmost in a meter bar.
///
/// @param lowerThreshold lower threshold level (in decibels)
///
/// @param thresholdRange difference between lower and upper level
///        threshold (in decibels)
///
/// @param isTopmost if set to **true**, the segment has no upper
///        level threshold
///
/// @return return new upper threshold of segment (may be useful for
///         creating continuous meters)
///
float MeterSegmentDiscrete::setThresholdAndRange(
   float lowerThreshold, float thresholdRange, bool isTopmost )
{
   // set lower level threshold (in decibels)
   lowerThreshold_ = lowerThreshold;

   // set level range above lower threshold (in decibels)
   thresholdRange_ = thresholdRange;

   // set upper level threshold (in decibels)
   upperThreshold_ = lowerThreshold_ + thresholdRange_;

   // is there a meter segment beyond this?
   isTopmost_ = isTopmost;

   // peak level marker is currently off
   displayPeakMarker_ = false;

   // return upper threshold (may be useful for creating continuous
   // meters)
   return upperThreshold_;
}


/// Set colours of segment and peak marker.
///
/// @param segmentColour colour of the segment
///
/// @param peakMarkerColour colour of the peak marker
///
void MeterSegmentDiscrete::setColours(
   const Colour& segmentColour, const Colour& peakMarkerColour )
{
   // initialise segment colour
   segmentColour_ = segmentColour;

   // initialise peak marker's colour
   peakMarkerColour_ = peakMarkerColour;

   // redraw meter segment
   repaint();
}


/// Paint segment.
///
/// @param g the graphics context that must be used to do the drawing
///        operations
///
void MeterSegmentDiscrete::paint(
   Graphics& g )
{
   // to look well, meter segments should be left with some
   // colour and not have maximum brightness
   float segmentBrightnessModifier = brightness_ * 0.80f + 0.20f;
   float outlineBrightnessModifier = brightness_ * 0.70f + 0.25f;

   // get meter segment's dimensions
   int width = getWidth();
   int height = getHeight();

   // initialise segment colour from brightness modifier
   Colour segmentColour = segmentColour_.withMultipliedBrightness(
                             segmentBrightnessModifier );

   // initialise outline colour from brightness modifier
   Colour outlineColour = segmentColour_.withMultipliedBrightness(
                             outlineBrightnessModifier );

   // set segment colour
   g.setColour( segmentColour );

   // fill meter segment, but leave a border of two pixels for
   // outline and peak marker
   g.fillRect( 2, 2, width - 4, height - 4 );

   // set outline colour
   g.setColour( outlineColour );

   // draw outline, but leave a border of one pixel for peak marker
   g.drawRect( 1, 1, width - 2, height - 2 );

   // if peak marker is lit, draw peak marker around meter segment
   // (width: 1 pixel)
   if ( displayPeakMarker_ ) {
      g.setColour( peakMarkerColour_ );
      g.drawRect( 0, 0, width, height );
   }

   // attenuate colours if segment is disabled
   if ( ! isEnabled() ) {
      g.setColour( attenuatedColour_ );
      g.fillAll();
   }
}


/// Called when this component's visibility changes.
///
/// If this function did not exist, the meter segment wouldn't be
/// drawn until the first level change!
///
void MeterSegmentDiscrete::visibilityChanged()
{
}


/// Set discrete (peak) and normal (average) levels.
///
/// @param normalLevel new normal level
///
/// @param normalLevelPeak new normal peak level
///
/// @param discreteLevel new discrete level
///
/// @param discreteLevelPeak new discrete peak level
///
void MeterSegmentDiscrete::setLevels(
   float normalLevel, float normalLevelPeak,
   float discreteLevel, float discreteLevelPeak )
{
   // store old brightness and peak marker value
   float brightnessOld = brightness_;
   bool displayPeakMarkerOld = displayPeakMarker_;

   // normal level lies on or above upper threshold
   if ( normalLevel >= upperThreshold_ ) {
      // auto-fade mode: set meter segment to dark (unless it is
      // topmost)
      if ( ( retainSignalFactor_ > 0.0f ) && ( ! isTopmost_ ) ) {
         brightness_ = 0.0f;
         // normal mode: fully light meter segment
      } else {
         brightness_ = 1.0f;
      }

      // discrete level lies within thresholds or on lower threshold, so
      // fully light meter segment
   } else if ( ( discreteLevel >= lowerThreshold_ ) &&
               ( discreteLevel < upperThreshold_ ) ) {
      brightness_ = 1.0f;
      // normal level lies below lower threshold, so set meter segment
      // to dark
   } else if ( normalLevel < lowerThreshold_ ) {
      brightness_ = 0.0f;
      // normal level lies within thresholds or on lower threshold
   } else {
      // auto-fade mode: fully light meter segment
      if ( retainSignalFactor_ > 0.0f ) {
         brightness_ = 1.0f;
         // normal mode: calculate brightness from current level
      } else {
         brightness_ = ( normalLevel - lowerThreshold_ ) /
                       thresholdRange_;
      }
   }

   // calculate auto-fade; save processing time and de-normalize
   if ( ( retainSignalFactor_ > 0.0f ) && ( brightnessOld > 0.001f ) ) {
      // fade out old brightness value
      float retainBrightness = brightnessOld * retainSignalFactor_;

      // attenuate new brightness value
      float newBrightness = newSignalFactor_ * brightness_;

      // combine old and new brightness value
      brightness_ = retainBrightness + newBrightness;

      // check bounds
      if ( brightness_ > 1.0f ) {
         brightness_ = 1.0f;
      }
   }

   // there is no meter segment beyond this; light peak marker if
   // peak level is on or above lower threshold
   if ( isTopmost_ ) {
      // light peak marker ("normal" peak level)
      if ( normalLevelPeak >= lowerThreshold_ ) {
         displayPeakMarker_ = true;
         // light peak marker ("discrete" peak level)
      } else if ( discreteLevelPeak >= lowerThreshold_ ) {
         displayPeakMarker_ = true;
         // peak marker is off
      } else {
         displayPeakMarker_ = false;
      }

      // otherwise, light peak marker if peak level lies within
      // thresholds or on lower threshold
   } else {
      // light peak marker ("normal" peak level)
      if ( ( normalLevelPeak >= lowerThreshold_ ) &&
           ( normalLevelPeak < upperThreshold_ ) ) {
         displayPeakMarker_ = true;
         // light peak marker ("discrete" peak level)
      } else if ( ( discreteLevelPeak >= lowerThreshold_ ) &&
                  ( discreteLevelPeak < upperThreshold_ ) ) {
         displayPeakMarker_ = true;
         // peak marker is off
      } else {
         displayPeakMarker_ = false;
      }
   }

   // re-paint meter segment only when brightness modifier or peak
   // marker have changed
   if ( ( brightness_ != brightnessOld ) ||
        ( displayPeakMarker_ != displayPeakMarkerOld ) ) {
      repaint();
   }
}

} // namespace
