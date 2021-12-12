/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of citrus

   Copyright (c) 2013-2021 Martin Zuther (http://www.mzuther.de/)

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

#ifndef SQUEEZER_GAIN_COMPUTER_H
#define SQUEEZER_GAIN_COMPUTER_H

#include "FrutHeader.h"


template <typename FloatType>
class GainComputer
{
private:
   FloatType threshold_;
   FloatType internalCompressionRatio_;
   FloatType kneeWidth_;

   JUCE_LEAK_DETECTOR( GainComputer );


public:
   explicit GainComputer()
   /*  Constructor.

       return value: none
   */
   {
      setThreshold( -32.0 );
      setRatio( 2.0 );
      setKneeWidth( 0.0 );
   }


   FloatType getThreshold()
   /*  Get current threshold.

       return value: returns the current threshold in decibels
    */
   {
      return threshold_;
   }


   void setThreshold( FloatType threshold )
   /*  Set new threshold.

       threshold: new threshold in decibels

       return value: none
    */
   {
      threshold_ = threshold;
   }


   FloatType getRatio()
   /*  Get current compression ratio.

       return value: returns the current compression ratio
    */
   {
      return 1.0 / ( 1.0 - internalCompressionRatio_ );
   }


   void setRatio( FloatType compressionRatio )
   /*  Set new compression ratio.

       compressionRatio: new compression ratio

       return value: none
    */
   {
      internalCompressionRatio_ = 1.0 - ( 1.0 / compressionRatio );
   }


   FloatType getKneeWidth()
   /*  Get current knee width.

       return value: returns the current knee width in decibels
    */
   {
      return kneeWidth_;
   }


   void setKneeWidth( FloatType kneeWidth )
   /*  Set new knee width.

       kneeWidth: new knee width in decibels

       return value: none
    */
   {
      kneeWidth_ = kneeWidth;
   }


   FloatType processGain( FloatType inputLevel )
   /*  Calculate target gain reduction for given input level.

       inputLevel: current input level in decibels

       return value: calculated gain reduction in decibels
    */
   {
      auto aboveThreshold = inputLevel - threshold_;

      if ( kneeWidth_ == 0.0 ) {
         if ( inputLevel <= threshold_ ) {
            return 0.0;
         } else {
            return aboveThreshold * internalCompressionRatio_;
         }
      } else {
         // algorithm adapted from Giannoulis et al., "Digital Dynamic
         // Range Compressor Design - A Tutorial and Analysis", JAES,
         // 60(6):399-408, 2012
         if ( aboveThreshold < -( kneeWidth_ / 2.0 ) ) {
            return 0.0;
         } else if ( aboveThreshold > ( kneeWidth_ / 2.0 ) ) {
            return aboveThreshold * internalCompressionRatio_;
         } else {
            auto factor = aboveThreshold + ( kneeWidth_ / 2.0 );
            return factor * factor / ( kneeWidth_ * 2.0 ) * internalCompressionRatio_;
         }
      }
   }
};

#endif  // SQUEEZER_GAIN_COMPUTER_H
