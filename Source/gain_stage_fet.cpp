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

#include "gain_stage_fet.h"


GainStageFET::GainStageFET( int nSampleRate ) : GainStage( nSampleRate )
   /*  Constructor.

       nSampleRate (integer): internal sample rate

       return value: none
   */
{
   // reset (i.e. initialise) all relevant variables
   reset( 0.0 );
}


void GainStageFET::reset( double dCurrentGainReduction )
/*  Reset all relevant variables.

    dCurrentGainReduction (double): current gain reduction in decibels

    return value: none
*/
{
   dGainReduction = dCurrentGainReduction;
}


double GainStageFET::processGainReduction( double dGainReductionNew,
                                           double dGainReductionIdeal )
/*  Process current gain reduction.

    dGainReductionNew (double): calculated new gain reduction in
    decibels

    dGainReductionIdeal (double): calculated "ideal" gain reduction
    (without any envelopes) decibels

    return value (double): returns the processed gain reduction in
    decibel
 */
{
   ignoreUnused( dGainReductionIdeal );

   dGainReduction = dGainReductionNew;
   return dGainReduction;
}
