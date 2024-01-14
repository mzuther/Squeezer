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

#ifndef SQUEEZER_COMPRESSOR_CURVE_LOG_SMOOTH_DECOUPLED_H
#define SQUEEZER_COMPRESSOR_CURVE_LOG_SMOOTH_DECOUPLED_H

template <typename FloatType> class CompressorCurveLogSmoothDecoupled;

#include "FrutHeader.h"
#include "compressor_curve.h"


template <typename FloatType>
class CompressorCurveLogSmoothDecoupled :
   virtual public CompressorCurve<FloatType>
{
private:
   FloatType gainReduction_;
   FloatType intermediateGainReduction_;

   JUCE_LEAK_DETECTOR( CompressorCurveLogSmoothDecoupled );

public:
   explicit CompressorCurveLogSmoothDecoupled( FloatType sampleRate ) :
      CompressorCurve<FloatType>( sampleRate ),
      gainReduction_( 0.0 ),
      intermediateGainReduction_( 0.0 )
      /*  Constructor.

          sampleRate: internal sample rate

          return value: none
      */
   {
      this->selectedCurve_ = CompressorCurve<FloatType>::LogSmoothDecoupled;
   }


   virtual void reset() override
   /*  Reset processor.

       return value: none
    */
   {
      gainReduction_ = 0.0;
      intermediateGainReduction_ = 0.0;
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
   /*  Calculate smooth decoupled detector ("Smooth").

       gainReduction: calculated gain reduction in decibels

       return value: none
   */
   {
      // algorithm adapted from Giannoulis et al., "Digital Dynamic
      // Range Compressor Design - A Tutorial and Analysis", JAES,
      // 60(6):399-408, 2012
      //
      // apply release envelope
      if ( this->releaseCoefficient_ == 0.0 ) {
         intermediateGainReduction_ = gainReduction;
      } else {
         intermediateGainReduction_ = ( this->releaseCoefficient_ * intermediateGainReduction_ ) +
                                      ( 1.0 - this->releaseCoefficient_ ) * gainReduction;

         // maximally fast peak detection
         intermediateGainReduction_ = juce::jmax( intermediateGainReduction_, gainReduction );
      }

      // apply attack envelope
      if ( this->attackCoefficient_ == 0.0 ) {
         gainReduction_ = intermediateGainReduction_;
      } else {
         gainReduction_ = ( this->attackCoefficient_ * gainReduction_ ) +
                          ( 1.0 - this->attackCoefficient_ ) * intermediateGainReduction_;
      }

      return gainReduction_;
   }
};

#endif  // SQUEEZER_COMPRESSOR_CURVE_LOG_SMOOTH_DECOUPLED_H
