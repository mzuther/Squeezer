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

#ifndef SQUEEZER_GAIN_STAGE_FET_H
#define SQUEEZER_GAIN_STAGE_FET_H

#include "FrutHeader.h"
#include "gain_stage.h"


template <typename FloatType>
class GainStageFET :
   virtual public GainStage<FloatType>
{
public:
   explicit GainStageFET( FloatType sampleRate ) :
      GainStage<FloatType>( sampleRate )
      /*  Constructor.

          nSampleRate: internal sample rate

          return value: none
      */
   {
   }


   void resetGainReduction( FloatType currentGainReduction ) override
   /*  Reset gain reduction.

       currentGainReduction: current gain reduction in decibels

       return value: none
   */
   {
      ignoreUnused( currentGainReduction );
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

      return currentGainReduction;
   }
};

#endif  // SQUEEZER_GAIN_STAGE_FET_H
