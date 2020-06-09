/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of citrus

   Copyright (c) 2013-2020 Martin Zuther (http://www.mzuther.de/)

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
                            int mainSegmentHeight,
                            const Array<Colour>& segmentColours )
{
   frut::widgets::MeterBar::create();

   int numberOfBars = 15;

   crestFactor *= 10;

   int levelRange = 20;
   int trueLowerThreshold = 0;
   int lowerThreshold = trueLowerThreshold + crestFactor;

   for ( int n = 0; n < numberOfBars; ++n ) {
      int colourId;
      int segmentHeight;
      int spacingBefore = 0;

      if ( trueLowerThreshold >= -40 ) {
         colourId = colourSelector::overload;
         segmentHeight = mainSegmentHeight;
      } else if ( trueLowerThreshold >= -120 ) {
         colourId = colourSelector::warning;
         segmentHeight = mainSegmentHeight;
      } else {
         colourId = colourSelector::fine;
         segmentHeight = mainSegmentHeight;
      }

      bool hasHighestLevel = ( n == 0 );

      if ( ( ! discreteMeter ) && ( ! hasHighestLevel ) ) {
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
      } else {
         // meter segment outlines overlap
         spacingBefore -= 1;
         segmentHeight += 1;

         addDiscreteSegment(
            lowerThreshold * 0.1f,
            levelRange * 0.1f,
            0.0f,
            1.0f,
            hasHighestLevel,
            segmentHeight,
            spacingBefore,
            segmentColours[colourId],
            segmentColours[colourId].withMultipliedBrightness( 0.7f ) );
      }

      trueLowerThreshold -= levelRange;
      lowerThreshold = trueLowerThreshold + crestFactor;
   }

   // set orientation here to save some processing
   setOrientation( orientation );
}
