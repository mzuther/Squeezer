/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013 Martin Zuther (http://www.mzuther.de/)

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


#define NUMBER_OF_DECIBELS 37
#define COEFFICIENTS_PER_DECIBEL 2.0f
#define NUMBER_OF_COEFFICIENTS NUMBER_OF_DECIBELS * int(COEFFICIENTS_PER_DECIBEL)


GainStageFET::GainStageFET(int nSampleRate) : GainStage::GainStage(nSampleRate)
/*  Constructor.

    nSampleRate (integer): internal sample rate

    return value: none
*/
{
    // reset (i.e. initialise) all relevant variables
    reset(0.0f);
}


GainStageFET::~GainStageFET()
/*  Destructor.

    return value: none
*/
{
}


void GainStageFET::reset(float fCurrentGainReduction)
/*  Reset all relevant variables.

    fCurrentGainReduction (float): current gain reduction in decibels

    return value: none
*/
{
    fGainReduction = fCurrentGainReduction;
}


float GainStageFET::processGainReduction(float fGainReductionNew, float fGainReductionIdeal)
/*  Process current gain reduction.

    fGainReductionNew (float): calculated new gain reduction in
    decibels

    fGainReductionIdeal (float): calculated "ideal" gain reduction
    (without any envelopes) decibels

    return value (float): returns the processed gain reduction in
    decibel
 */
{
    fGainReduction = fGainReductionNew;
    return fGainReduction;
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
