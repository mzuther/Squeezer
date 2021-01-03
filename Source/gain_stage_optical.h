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

#ifndef SQUEEZER_GAIN_STAGE_OPTICAL_H
#define SQUEEZER_GAIN_STAGE_OPTICAL_H

#include "FrutHeader.h"
#include "gain_stage.h"


template <typename FloatType>
class GainStageOptical :
   virtual public GainStage<FloatType>
{
private:
   JUCE_LEAK_DETECTOR( GainStageOptical );

   FloatType gainReduction_;

   const int decibelRange_;
   const int coefficientsPerDecibel_;
   const int numberOfCoefficients_;

   Array<FloatType> attackCoefficients_;
   Array<FloatType> releaseCoefficients_;


public:
   explicit GainStageOptical( int sampleRate ) :
      GainStage<FloatType>( sampleRate ),
      gainReduction_( 0.0 ),
      decibelRange_( 37 ),
      coefficientsPerDecibel_( 2 ),
      numberOfCoefficients_( decibelRange_ * coefficientsPerDecibel_ )
      /*  Constructor.

          sampleRate: internal sample rate

          return value: none
      */
   {
      for ( int coefficient = 0; coefficient < numberOfCoefficients_; ++coefficient ) {
         //  0 dB:  Attack: 16 ms, Release: 160 ms
         //  6 dB:  Attack:  5 ms, Release:  53 ms
         // 12 dB:  Attack:  3 ms, Release:  32 ms
         // 18 dB:  Attack:  2 ms, Release:  23 ms
         // 24 dB:  Attack:  2 ms, Release:  18 ms

         FloatType decibels = ( FloatType ) coefficient / ( FloatType ) coefficientsPerDecibel_;
         FloatType resistance = 480.0 / ( 3.0 + decibels );
         FloatType attackRate = resistance / 10.0;
         FloatType releaseRate = resistance;

         // if (coefficient % (6 * coefficientsPerDecibel_) == 0)
         // {
         //     DBG(String(decibels) + " dB:  Attack: " + String(attackRate, 1) + " ms, Release: " + String(releaseRate, 1) + " ms");
         // }

         FloatType attackRateSeconds = attackRate / 1000.0;
         FloatType releaseRateSeconds = releaseRate / 1000.0;

         // logarithmic envelopes that reach 73% of the final reading
         // in the given attack time
         attackCoefficients_.add( exp( log( 0.27 ) / ( attackRateSeconds * ( FloatType ) sampleRate ) ) );
         releaseCoefficients_.add( exp( log( 0.27 ) / ( releaseRateSeconds * ( FloatType ) sampleRate ) ) );
      }
   }


   void initialise( FloatType currentGainReduction ) override
   /*  Initialise all relevant variables.

       currentGainReduction: current gain reduction in decibels

       return value: none
   */
   {
      gainReduction_ = currentGainReduction;
   }


   FloatType processGainReduction( FloatType currentGainReduction,
                                   FloatType idealGainReduction ) override
   /*  Process current gain reduction.

       currentGainReduction: calculated new gain reduction in decibels

       idealGainReduction: calculated "ideal" gain reduction (without
       any envelopes) decibels

       return value: returns the processed gain reduction in decibel
    */
   {
      FloatType gainReductionOld = gainReduction_;

      int coefficient = int( currentGainReduction * ( FloatType ) coefficientsPerDecibel_ );
      coefficient = juce::jlimit( 0, numberOfCoefficients_ - 1, coefficient );

      if ( currentGainReduction > gainReduction_ ) {
         // algorithm adapted from Giannoulis et al., "Digital Dynamic
         // Range Compressor Design - A Tutorial and Analysis", JAES,
         // 60(6):399-408, 2012
         gainReduction_ = ( attackCoefficients_[coefficient] * gainReductionOld ) +
                          ( 1.0 - attackCoefficients_[coefficient] ) * currentGainReduction;

         // otherwise, apply release rate if proposed gain reduction is
         // below old gain reduction
      } else {
         // algorithm adapted from Giannoulis et al., "Digital Dynamic
         // Range Compressor Design - A Tutorial and Analysis", JAES,
         // 60(6):399-408, 2012
         gainReduction_ = ( releaseCoefficients_[coefficient] * gainReductionOld ) +
                          ( 1.0 - releaseCoefficients_[coefficient] ) * currentGainReduction;
      }

      // saturation of optical element
      if ( gainReduction_ < idealGainReduction ) {
         FloatType diff = idealGainReduction - gainReduction_;
         FloatType limit = 24.0;

         diff = limit - limit / ( 1.0 + diff / limit );
         return idealGainReduction - diff;
      } else {
         return gainReduction_;
      }
   }
};

#endif  // SQUEEZER_GAIN_STAGE_OPTICAL_H
