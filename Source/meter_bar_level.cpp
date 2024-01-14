/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of citrus

   Copyright (c) 2013-2024 Martin Zuther (http://www.mzuther.de/)

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

#include "meter_bar_level.h"


void MeterBarLevel::create( int crestFactor,
                            frut::widgets::Orientation orientation,
                            bool discreteMeter,
                            frut::skin::Skin& skin )
{
   frut::widgets::MeterBar::create();

   int numberOfBars = 15;
   crestFactor *= 10;

   int levelRange = 30;
   int trueLowerThreshold = -levelRange;
   int lowerThreshold = trueLowerThreshold + crestFactor;

   if ( discreteMeter ) {
      Array<Image> imagesOn;

      auto drawableOn = skin.loadImageAsDrawable(
                           "meter_segment_level_normal", "image_on" );
      imagesOn.add( skin.imageFromDrawable( drawableOn ) );

      drawableOn = skin.loadImageAsDrawable(
                      "meter_segment_level_warning", "image_on" );
      imagesOn.add( skin.imageFromDrawable( drawableOn ) );

      drawableOn = skin.loadImageAsDrawable(
                      "meter_segment_level_overload", "image_on" );
      imagesOn.add( skin.imageFromDrawable( drawableOn ) );

      Array<Image> imagesOff;

      auto drawableOff = skin.loadImageAsDrawable(
                            "meter_segment_level_normal", "image_off" );
      imagesOff.add( skin.imageFromDrawable( drawableOff ) );

      drawableOff = skin.loadImageAsDrawable(
                       "meter_segment_level_warning", "image_off" );
      imagesOff.add( skin.imageFromDrawable( drawableOff ) );

      drawableOff = skin.loadImageAsDrawable(
                       "meter_segment_level_overload", "image_off" );
      imagesOff.add( skin.imageFromDrawable( drawableOff ) );

      Array<Image> imagesPeak;

      auto drawablePeak = skin.loadImageAsDrawable(
                             "meter_segment_level_normal", "image_peak" );
      imagesPeak.add( skin.imageFromDrawable( drawablePeak ) );

      drawablePeak = skin.loadImageAsDrawable(
                        "meter_segment_level_warning", "image_peak" );
      imagesPeak.add( skin.imageFromDrawable( drawablePeak ) );

      drawablePeak = skin.loadImageAsDrawable(
                        "meter_segment_level_overload", "image_peak" );
      imagesPeak.add( skin.imageFromDrawable( drawablePeak ) );

      Point<int> segmentTopLeft( 1, 1 );
      Point<int> peakTopLeft( 0, 0 );

      for ( int n = 0; n < numberOfBars; ++n ) {
         int colourId;

         if ( trueLowerThreshold >= -60 ) {
            colourId = colourSelector::overload;
         } else if ( trueLowerThreshold >= -120 ) {
            colourId = colourSelector::warning;
         } else {
            colourId = colourSelector::normal;
         }

         bool hasHighestLevel = ( n == 0 );

         // meter segment outlines overlap by 1 pixel
         int spacingBefore = -1;

         addDiscreteImageSegment(
            lowerThreshold * 0.1f,
            levelRange * 0.1f,
            0.0f,
            1.0f,
            hasHighestLevel,
            spacingBefore,
            imagesOn[colourId],
            segmentTopLeft,
            imagesOff[colourId],
            segmentTopLeft,
            imagesPeak[colourId],
            peakTopLeft );

         trueLowerThreshold -= levelRange;
         lowerThreshold = trueLowerThreshold + crestFactor;
      }
   } else {
      Colour segmentStroke;
      Colour segmentFill;
      Rectangle<int> segmentBounds;

      skin.getAttributesFromSvgFile(
         "meter_segment_level_normal", "image_on",
         segmentStroke, segmentFill, segmentBounds );

      Colour ColourNormal = segmentStroke;
      int segmentHeight = segmentBounds.getHeight() + 1;

      skin.getAttributesFromSvgFile(
         "meter_segment_level_warning", "image_on",
         segmentStroke, segmentFill, segmentBounds );

      Colour ColourWarning = segmentStroke;

      skin.getAttributesFromSvgFile(
         "meter_segment_level_overload", "image_on",
         segmentStroke, segmentFill, segmentBounds );

      Colour ColourOverload = segmentStroke;

      Array<Colour> segmentColours;
      segmentColours.add( ColourNormal );
      segmentColours.add( ColourWarning );
      segmentColours.add( ColourOverload );

      for ( int n = 0; n < numberOfBars; ++n ) {
         int colourId;
         int spacingBefore = 0;

         if ( trueLowerThreshold >= -60 ) {
            colourId = colourSelector::overload;
         } else if ( trueLowerThreshold >= -120 ) {
            colourId = colourSelector::warning;
         } else {
            colourId = colourSelector::normal;
         }

         bool hasHighestLevel = ( n == 0 );

         // meter segment outlines must not overlap

         addContinuousSegment(
            lowerThreshold * 0.1f,
            levelRange * 0.1f,
            ( levelRange * 0.1f ) / segmentHeight,
            hasHighestLevel,
            segmentHeight,
            spacingBefore,
            segmentColours[colourId],
            segmentColours[colourId].withMultipliedBrightness( 0.7f ) );

         trueLowerThreshold -= levelRange;
         lowerThreshold = trueLowerThreshold + crestFactor;
      }
   }

   // set orientation here to save some processing
   setOrientation( orientation );
}
