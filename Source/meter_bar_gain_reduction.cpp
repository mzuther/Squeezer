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

#include "meter_bar_gain_reduction.h"


/// Default constructor.
///
MeterBarGainReduction::MeterBarGainReduction() :
   MeterBar(),
   originalOrientation_( frut::widgets::Orientation::orientations::bottomToTop )
{
   // initialize variables
   create();
};


void MeterBarGainReduction::create( frut::widgets::Orientation orientation,
                                    bool discreteMeter,
                                    int mainSegmentHeight,
                                    const Colour& segmentColour )
{
   frut::widgets::MeterBar::create();

   int numberOfBars = 15;

   int levelRange = 10;
   int trueLowerThreshold = ( numberOfBars - 1 ) * levelRange;

   for ( int n = 0; n < numberOfBars; ++n ) {
      bool hasHighestLevel = ( n == 0 );

      if ( discreteMeter ) {
         // meter segment outlines overlap
         int spacingBefore = -1;
         int segmentHeight = mainSegmentHeight + 1;

         addDiscreteSegment(
            trueLowerThreshold * 0.1f,
            levelRange * 0.1f,
            0.0f,
            1.0f,
            hasHighestLevel,
            segmentHeight,
            spacingBefore,
            segmentColour,
            segmentColour.withMultipliedBrightness( 0.7f ) );
      } else {
         // meter segment outlines must not overlap
         int spacingBefore = 0;
         int segmentHeight = mainSegmentHeight;

         addContinuousSegment(
            trueLowerThreshold * 0.1f,
            levelRange * 0.1f,
            ( levelRange * 0.1f ) / segmentHeight,
            hasHighestLevel,
            segmentHeight,
            spacingBefore,
            segmentColour,
            segmentColour.withMultipliedBrightness( 0.7f ) );
      }

      trueLowerThreshold -= levelRange;
   }

   // set orientation here to save some processing
   upwardExpansion_ = false;
   setOrientation( orientation );
}


void MeterBarGainReduction::setOrientation( const frut::widgets::Orientation& orientation )
{
   originalOrientation_ = orientation;

   if ( upwardExpansion_ ) {
      frut::widgets::MeterBar::setOrientation( originalOrientation_.mirror() );
   } else {
      frut::widgets::MeterBar::setOrientation( originalOrientation_ );
   }
}


void MeterBarGainReduction::setUpwardExpansion( bool upwardExpansion )
{
   if ( upwardExpansion != upwardExpansion_ ) {
      upwardExpansion_ = upwardExpansion;

      // apply change
      setOrientation ( originalOrientation_ );
   }
}
