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

namespace frut::widgets
{

/// Create a meter with a needle.
///
NeedleMeter::NeedleMeter()
{
   // this component does not have any transparent areas (increases
   // performance on redrawing)
   setOpaque( true );

   // initialise needle position
   needlePosition_ = -1;

   // initialise the length the needle can travel
   needleTravelPath_ = 1;

   // initialise spacing between needle and image border
   needleSpacingLeft_ = 0;
   needleSpacingTop_ = 0;

   // initialise meter orientation
   isVerticalMeter_ = false;
}


/// Paint meter bar.
///
/// @param g graphics context
///
void NeedleMeter::paint(
   Graphics& g )

{
   // draw background image
   g.drawImageAt( imageBackground_, 0, 0, false );

   // draw needle according to orientation
   if ( isVerticalMeter_ ) {
      g.drawImageAt( imageNeedle_, needleSpacingLeft_, needlePosition_, false );
   } else {
      g.drawImageAt( imageNeedle_, needlePosition_, needleSpacingTop_, false );
   }
}


/// Called when this component's size has been changed.
///
void NeedleMeter::resized()
{
   // get component size
   int width = getWidth();
   int height = getHeight();

   // determine meter orientation (will prefer horizontal if height
   // and width are the same)
   isVerticalMeter_ = ( height > width );

   // calculate the length the needle can travel
   if ( isVerticalMeter_ ) {
      needleTravelPath_ = height - 2 * needleSpacingTop_;
      needleTravelPath_ -= imageNeedle_.getHeight();
   } else {
      needleTravelPath_ = width - 2 * needleSpacingLeft_;
      needleTravelPath_ -= imageNeedle_.getWidth();
   }
}


/// Set new value (and possibly reposition needle).
///
/// @param value new value
///
void NeedleMeter::setValue(
   float value )

{
   // store old needle position
   int needlePositionOld = needlePosition_;

   // calculate new needle position
   needlePosition_ = math::SimpleMath::round( value * needleTravelPath_ );

   if ( isVerticalMeter_ ) {
      needlePosition_ += needleSpacingTop_;
   } else {
      needlePosition_ += needleSpacingLeft_;
   }

   // update of needle position is necessary
   if ( needlePosition_ != needlePositionOld ) {
      repaint( getLocalBounds() );
   }
}


/// Set new images and needle spacing.
///
/// @param imageBackground image of meter background
///
/// @param imageNeedle image of needle
///
/// @param needleSpacingLeft spacing between needle and left (or
///        right) image border
///
/// @param needleSpacingTop spacing between needle and top (or bottom)
///        image border
///
void NeedleMeter::setImages(
   const Image& imageBackground,
   const Image& imageNeedle,
   int needleSpacingLeft,
   int needleSpacingTop )

{
   // update images (creates a copy of each image)
   imageBackground_ = imageBackground.createCopy();
   imageNeedle_ = imageNeedle.createCopy();

   // update needle spacing
   needleSpacingLeft_ = needleSpacingLeft;
   needleSpacingTop_ = needleSpacingTop;

   // update dimensions of meter
   resized();
}

} // namespace
