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

#include "optical_cell.h"


OpticalCell::OpticalCell(int nSampleRate)
/*  Constructor.

    return value: none
*/
{
    fSampleRate = (float) nSampleRate;

    int nAttackRate = 8;
    int nReleaseRate = 78;

    float fAttackRateSeconds = nAttackRate / 1000.0f;
    float fReleaseRateSeconds = nReleaseRate / 1000.0f;

    // logarithmic envelopes that reach 63% of the final reading in
    // the given attack time
    fAttackCoefficient = expf(logf(0.37f) / (fAttackRateSeconds * fSampleRate));
    fReleaseCoefficient = expf(logf(0.37f) / (fReleaseRateSeconds * fSampleRate));

    // reset (i.e. initialise) all relevant variables
    reset();
}


OpticalCell::~OpticalCell()
/*  Destructor.

    return value: none
*/
{
}


void OpticalCell::reset()
/*  Reset all relevant variables.

    return value: none
*/
{
    fGainReduction = 0.0f;
}


float OpticalCell::processGainReduction(float fGainReductionNew)
/*  Get current gain reduction.

    bAutoMakeupGain (boolean): determines whether the gain reduction
    should be level compensated or not

    return value (float): returns the current gain reduction in
    decibel
 */
{
    if (fGainReductionNew > fGainReduction)
    {
        // algorithm adapted from Giannoulis et al., "Digital Dynamic
        // Range Compressor Design - A Tutorial and Analysis", JAES,
        // 60(6):399-408, 2012

        float fGainReductionOld = fGainReduction;
        fGainReduction = (fAttackCoefficient * fGainReductionOld) + (1.0f - fAttackCoefficient) * fGainReductionNew;
    }
    // otherwise, apply release rate if proposed gain reduction is
    // below old gain reduction
    else
    {
        // algorithm adapted from Giannoulis et al., "Digital Dynamic
        // Range Compressor Design - A Tutorial and Analysis", JAES,
        // 60(6):399-408, 2012

        float fGainReductionOld = fGainReduction;
        fGainReduction = (fReleaseCoefficient * fGainReductionOld) + (1.0f - fReleaseCoefficient) * fGainReductionNew;
    }

    return fGainReduction;
}


float OpticalCell::level2decibel(float fLevel)
/*  Convert level from linear scale to decibels (dB).

    fLevel (float): audio level

    return value (float): returns given level in decibels (dB) when
    above "fMeterMinimumDecibel", otherwise "fMeterMinimumDecibel"
*/
{
    float fMeterMinimumDecibel = -70.01f;

    // log(0) is not defined, so return "fMeterMinimumDecibel"
    if (fLevel == 0.0f)
    {
        return fMeterMinimumDecibel;
    }
    else
    {
        // calculate decibels from audio level (a factor of 20.0 is
        // needed to calculate *level* ratios, whereas 10.0 is needed
        // for *power* ratios!)
        float fDecibels = 20.0f * log10f(fLevel);

        // to make meter ballistics look nice for low levels, do not
        // return levels below "fMeterMinimumDecibel"
        if (fDecibels < fMeterMinimumDecibel)
        {
            return fMeterMinimumDecibel;
        }
        else
        {
            return fDecibels;
        }
    }
}


float OpticalCell::decibel2level(float fDecibels)
/*  Convert level from decibels (dB) to linear scale.

    fLevel (float): audio level in decibels (dB)

    return value (float): given level in linear scale
*/
{
    // calculate audio level from decibels (a divisor of 20.0 is
    // needed to calculate *level* ratios, whereas 10.0 is needed for
    // *power* ratios!)
    float fLevel = powf(10.0f, fDecibels / 20.0f);
    return fLevel;
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
