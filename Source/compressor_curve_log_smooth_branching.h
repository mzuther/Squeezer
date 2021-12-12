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

#ifndef SQUEEZER_COMPRESSOR_CURVE_LOG_SMOOTH_BRANCHING_H
#define SQUEEZER_COMPRESSOR_CURVE_LOG_SMOOTH_BRANCHING_H

template <typename FloatType> class CompressorCurveLogSmoothBranching;

#include "FrutHeader.h"
#include "compressor_curve.h"


template <typename FloatType>
class CompressorCurveLogSmoothBranching :
   virtual public CompressorCurve<FloatType>
{
private:
   FloatType gainReduction_;

   JUCE_LEAK_DETECTOR( CompressorCurveLogSmoothBranching );

public:
   explicit CompressorCurveLogSmoothBranching( FloatType sampleRate ) :
      CompressorCurve<FloatType>( sampleRate ),
      gainReduction_( 0.0 )
      /*  Constructor.

          sampleRate: internal sample rate

          return value: none
      */
   {
      this->selectedCurve_ = CompressorCurve<FloatType>::LogSmoothBranching;
   }


   virtual void reset() override
   /*  Reset processor.

       return value: none
    */
   {
      gainReduction_ = 0.0;
   }


   virtual void setAttackRate( FloatType attackRate ) override
   /*  Set new attack rate.

       attackRate: new attack rate in milliseconds

       return value: none
    */
   {
      this->attackRateMilliseconds_ = attackRate;
      this->attackCoefficient_ = this->calculateLogAttackCoefficient( this->attackRateMilliseconds_ );
   }


   virtual void setReleaseRate( FloatType releaseRate ) override
   /*  Set new release rate.

       nReleaseRateNew: new release rate in milliseconds

       return value: none
    */
   {
      this->releaseRateMilliseconds_ = releaseRate;
      this->releaseCoefficient_ = this->calculateLogReleaseCoefficient( this->releaseRateMilliseconds_ );
   }


   virtual FloatType processSample( FloatType gainReduction ) override
   /*  Calculate smooth branching detector ("Logarithmic").

       gainReduction: calculated gain reduction in decibels

       return value: none
   */
   {
      // apply attack rate if proposed gain reduction is above old gain
      // reduction
      if ( gainReduction >= gainReduction_ ) {
         if ( this->attackCoefficient_ == 0.0 ) {
            gainReduction_ = gainReduction;
         } else {
            // algorithm adapted from Giannoulis et al., "Digital
            // Dynamic Range Compressor Design - A Tutorial and
            // Analysis", JAES, 60(6):399-408, 2012
            gainReduction_ = ( this->attackCoefficient_ * gainReduction_ ) +
                             ( 1.0 - this->attackCoefficient_ ) * gainReduction;
         }

         // otherwise, apply release rate if proposed gain reduction is
         // below old gain reduction
      } else {
         if ( this->releaseCoefficient_ == 0.0 ) {
            gainReduction_ = gainReduction;
         } else {
            // algorithm adapted from Giannoulis et al., "Digital
            // Dynamic Range Compressor Design - A Tutorial and
            // Analysis", JAES, 60(6):399-408, 2012
            gainReduction_ = ( this->releaseCoefficient_ * gainReduction_ ) +
                             ( 1.0 - this->releaseCoefficient_ ) * gainReduction;
         }
      }

      return gainReduction_;
   }
};

#endif  // SQUEEZER_COMPRESSOR_CURVE_LOG_SMOOTH_BRANCHING_H
