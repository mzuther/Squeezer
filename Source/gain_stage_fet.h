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

#ifndef SQUEEZER_GAIN_STAGE_FET_H
#define SQUEEZER_GAIN_STAGE_FET_H

#include "FrutHeader.h"
#include "gain_stage.h"


template <typename FloatType>
class GainStageFET :
   virtual public GainStage<FloatType>
{
private:
   JUCE_LEAK_DETECTOR( GainStageFET );

   FloatType gainReduction_;


public:
   explicit GainStageFET( int sampleRate ) :
      GainStage<FloatType>( sampleRate ),
      gainReduction_( 0.0 )
      /*  Constructor.

          nSampleRate: internal sample rate

          return value: none
      */
   {
   };


   void initialise( FloatType currentGainReduction ) override
   /*  Inititalise all relevant variables.

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

       idealReduction: calculated "ideal" gain reduction (without any
       envelopes) decibels

       return value: returns the processed gain reduction in decibel
   */
   {
      ignoreUnused( idealGainReduction );

      gainReduction_ = currentGainReduction;
      return gainReduction_;
   }
};

#endif  // SQUEEZER_GAIN_STAGE_FET_H
