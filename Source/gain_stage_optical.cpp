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
#define COEFFICIENTS_PER_DECIBEL 2.0f
#define NUMBER_OF_COEFFICIENTS NUMBER_OF_DECIBELS * int(COEFFICIENTS_PER_DECIBEL)


GainStageOptical::GainStageOptical(int nSampleRate) : GainStage::GainStage(nSampleRate)
/*  Constructor.

    return value: none
*/
{
    fSampleRate = (float) nSampleRate;

    float fLightHistoryRateSeconds = 10.0f;

    // logarithmic envelope reaches 90% of the final reading
    // in the given attack/release time
    fLightHistoryCoefficient = expf(logf(0.10f) / (fLightHistoryRateSeconds * fSampleRate));

    pAttackCoefficients = new float[NUMBER_OF_COEFFICIENTS];
    pReleaseCoefficients = new float[NUMBER_OF_COEFFICIENTS];

    for (int nCoefficient = 0; nCoefficient < NUMBER_OF_COEFFICIENTS; nCoefficient++)
    {
        //  0 dB:  Attack: 16 ms, Release: 160 ms
        //  6 dB:  Attack:  5 ms, Release:  53 ms
        // 12 dB:  Attack:  3 ms, Release:  32 ms
        // 18 dB:  Attack:  2 ms, Release:  23 ms
        // 24 dB:  Attack:  2 ms, Release:  18 ms

        float fDecibels = float(nCoefficient) / COEFFICIENTS_PER_DECIBEL;
        float fResistance = 480.0f / (3.0f + fDecibels);
        float fAttackRate = fResistance / 10.0f;
        float fReleaseRate = fResistance;

        // if (nCoefficient % (6 * int(COEFFICIENTS_PER_DECIBEL)) == 0)
        // {
        //     DBG(String(fDecibels) + " dB:  Attack: " + String(fAttackRate, 1) + " ms, Release: " + String(fReleaseRate, 1) + " ms");
        // }

        float fAttackRateSeconds = fAttackRate / 1000.0f;
        float fReleaseRateSeconds = fReleaseRate / 1000.0f;

        // logarithmic envelopes that reach 73% of the final reading
        // in the given attack time
        pAttackCoefficients[nCoefficient] = expf(logf(0.27f) / (fAttackRateSeconds * fSampleRate));
        pReleaseCoefficients[nCoefficient] = expf(logf(0.27f) / (fReleaseRateSeconds * fSampleRate));
    }

    // reset (i.e. initialise) all relevant variables
    reset(0.0f);
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


void GainStageOptical::reset(float fCurrentGainReduction)
/*  Reset all relevant variables.

    return value: none
*/
{
    fGainReduction = fCurrentGainReduction;
    fLightHistory = 0.0f;
}


float GainStageOptical::processGainReduction(float fGainReductionNew)
/*  Get current gain reduction.

    bAutoMakeupGain (boolean): determines whether the gain reduction
    should be level compensated or not

    return value (float): returns the current gain reduction in
    decibel
 */
{
    float fLightHistoryOld = fLightHistory;
    fLightHistory = (fLightHistoryCoefficient * fLightHistoryOld) + (1.0f - fLightHistoryCoefficient) * fGainReductionNew;

    float fGainReductionOld = fGainReduction;
    float fCoefficient = fGainReductionNew - (fLightHistory * 0.15f);
    int nCoefficient = int(fCoefficient * COEFFICIENTS_PER_DECIBEL);

    if (nCoefficient < 0)
    {
        nCoefficient = 0;
    }
    else if (nCoefficient > (NUMBER_OF_COEFFICIENTS - 1))
    {
        nCoefficient = (NUMBER_OF_COEFFICIENTS - 1);
    }

    if (fGainReductionNew > fGainReduction)
    {
        // algorithm adapted from Giannoulis et al., "Digital Dynamic
        // Range Compressor Design - A Tutorial and Analysis", JAES,
        // 60(6):399-408, 2012

        fGainReduction = (pAttackCoefficients[nCoefficient] * fGainReductionOld) + (1.0f - pAttackCoefficients[nCoefficient]) * fGainReductionNew;
    }
    // otherwise, apply release rate if proposed gain reduction is
    // below old gain reduction
    else
    {
        // algorithm adapted from Giannoulis et al., "Digital Dynamic
        // Range Compressor Design - A Tutorial and Analysis", JAES,
        // 60(6):399-408, 2012

        fGainReduction = (pReleaseCoefficients[nCoefficient] * fGainReductionOld) + (1.0f - pReleaseCoefficients[nCoefficient]) * fGainReductionNew;
    }

    return fGainReduction;
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
