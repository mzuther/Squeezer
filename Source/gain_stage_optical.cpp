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

#include "gain_stage_optical.h"


#define NUMBER_OF_DECIBELS 37
#define COEFFICIENTS_PER_DECIBEL 2.0
#define NUMBER_OF_COEFFICIENTS NUMBER_OF_DECIBELS * int(COEFFICIENTS_PER_DECIBEL)


GainStageOptical::GainStageOptical(int nSampleRate) : GainStage(nSampleRate)
/*  Constructor.

    nSampleRate (integer): internal sample rate

    return value: none
*/
{
    dSampleRate = (double) nSampleRate;

    pAttackCoefficients = new double[NUMBER_OF_COEFFICIENTS];
    pReleaseCoefficients = new double[NUMBER_OF_COEFFICIENTS];

    for (int nCoefficient = 0; nCoefficient < NUMBER_OF_COEFFICIENTS; nCoefficient++)
    {
        //  0 dB:  Attack: 16 ms, Release: 160 ms
        //  6 dB:  Attack:  5 ms, Release:  53 ms
        // 12 dB:  Attack:  3 ms, Release:  32 ms
        // 18 dB:  Attack:  2 ms, Release:  23 ms
        // 24 dB:  Attack:  2 ms, Release:  18 ms

        double dDecibels = double(nCoefficient) / COEFFICIENTS_PER_DECIBEL;
        double dResistance = 480.0 / (3.0 + dDecibels);
        double dAttackRate = dResistance / 10.0;
        double dReleaseRate = dResistance;

        // if (nCoefficient % (6 * int(COEFFICIENTS_PER_DECIBEL)) == 0)
        // {
        //     DBG(String(dDecibels) + " dB:  Attack: " + String(dAttackRate, 1) + " ms, Release: " + String(dReleaseRate, 1) + " ms");
        // }

        double dAttackRateSeconds = dAttackRate / 1000.0;
        double dReleaseRateSeconds = dReleaseRate / 1000.0;

        // logarithmic envelopes that reach 73% of the final reading
        // in the given attack time
        pAttackCoefficients[nCoefficient] = exp(log(0.27) / (dAttackRateSeconds * dSampleRate));
        pReleaseCoefficients[nCoefficient] = exp(log(0.27) / (dReleaseRateSeconds * dSampleRate));
    }

    // reset (i.e. initialise) all relevant variables
    reset(0.0);
}


GainStageOptical::~GainStageOptical()
/*  Destructor.

    return value: none
*/
{
    delete[] pAttackCoefficients;
    pAttackCoefficients = nullptr;

    delete[] pReleaseCoefficients;
    pReleaseCoefficients = nullptr;
}


void GainStageOptical::reset(double dCurrentGainReduction)
/*  Reset all relevant variables.

    dCurrentGainReduction (double): current gain reduction in decibels

    return value: none
*/
{
    dGainReduction = dCurrentGainReduction;
}


double GainStageOptical::processGainReduction(double dGainReductionNew, double dGainReductionIdeal)
/*  Process current gain reduction.

    dGainReductionNew (double): calculated new gain reduction in
    decibels

    dGainReductionIdeal (double): calculated "ideal" gain reduction
    (without any envelopes) decibels

    return value (double): returns the processed gain reduction in
    decibel
 */
{
    double dGainReductionOld = dGainReduction;
    double dCoefficient = dGainReductionNew;
    int nCoefficient = int(dCoefficient * COEFFICIENTS_PER_DECIBEL);

    if (nCoefficient < 0)
    {
        nCoefficient = 0;
    }
    else if (nCoefficient > (NUMBER_OF_COEFFICIENTS - 1))
    {
        nCoefficient = (NUMBER_OF_COEFFICIENTS - 1);
    }

    if (dGainReductionNew > dGainReduction)
    {
        // algorithm adapted from Giannoulis et al., "Digital Dynamic
        // Range Compressor Design - A Tutorial and Analysis", JAES,
        // 60(6):399-408, 2012

        dGainReduction = (pAttackCoefficients[nCoefficient] * dGainReductionOld) + (1.0 - pAttackCoefficients[nCoefficient]) * dGainReductionNew;
    }
    // otherwise, apply release rate if proposed gain reduction is
    // below old gain reduction
    else
    {
        // algorithm adapted from Giannoulis et al., "Digital Dynamic
        // Range Compressor Design - A Tutorial and Analysis", JAES,
        // 60(6):399-408, 2012

        dGainReduction = (pReleaseCoefficients[nCoefficient] * dGainReductionOld) + (1.0 - pReleaseCoefficients[nCoefficient]) * dGainReductionNew;
    }

    // saturation of optical element
    if (dGainReduction < dGainReductionIdeal)
    {
        double dDiff = dGainReductionIdeal - dGainReduction;
        double dLimit = 24.0;

        dDiff = dLimit - dLimit / (1.0 + dDiff / dLimit);
        return dGainReductionIdeal - dDiff;
    }
    else
    {
        return dGainReduction;
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
