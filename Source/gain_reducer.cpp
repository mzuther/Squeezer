/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose compressor with a touch of lemon

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

#include "gain_reducer.h"


float GainReducer::fMeterMinimumDecibel;


GainReducer::GainReducer(int nSampleRate)
/*  Constructor.

    return value: none
*/
{
    setSampleRate(nSampleRate);

    setDesign(SqueezerPluginParameters::selDesignModern);

    setThreshold(-12.0f);
    setRatio(2.0f);

    setAttackRate(10);
    setReleaseRate(100);
    setLogarithmic(SqueezerPluginParameters::selEnvelopeTypeLogarithmic);

    fCrestFactorAutoGain = 0.0f;
    fMeterMinimumDecibel = -70.01f;

    // reset (i.e. initialise) all relevant variables
    reset();
}


GainReducer::~GainReducer()
/*  Destructor.

    return value: none
*/
{
}


void GainReducer::reset()
/*  Reset all relevant variables.

    return value: none
*/
{
    fGainReduction = 0.0f;
    fGainCompensation = 0.0f;
}


void GainReducer::setSampleRate(int nSampleRate)
/*  Set new sample rate.

    nSampleRate (int): new sample rate in Hertz

    return value: none
 */
{
    // calculate time that passes for every sample (in fractional
    // seconds)
    fTimePassed = 1.0f / nSampleRate;
}


int GainReducer::getDesign()
/*  Get current compressor design.

    return value (integer): returns the current compressor design
 */
{
    return nDesign;
}


void GainReducer::setDesign(int nDesignNew)
/*  Set new compressor design.

    nDesignNew (integer): new compressor design

    return value: none
 */
{
    nDesign = nDesignNew;
}


bool GainReducer::getLogarithmic()
/*  Get current compressor envelope type.

    return value (boolean): returns whether envelope reacts in a
    logarithmic (as opposed to linear) way
 */
{
    return bLogarithmicEnvelope;
}


void GainReducer::setLogarithmic(bool bLogarithmicNew)
/*  Set new compressor envelope type.

    bLogarithmicNew (boolean): determines whether envelope reacts in a
    logarithmic (as opposed to linear) way

    return value: none
 */
{
    bLogarithmicEnvelope = bLogarithmicNew;

    setAttackRate(nAttackRate);
    setReleaseRate(nReleaseRate);
}


float GainReducer::getThreshold()
/*  Get current threshold.

    return value (float): returns the current threshold in decibels
 */
{
    return fThreshold;
}


void GainReducer::setThreshold(float fThresholdNew)
/*  Set new threshold.

    fThresholdNew (float): new threshold in decibels

    return value: none
 */
{
    fThreshold = fThresholdNew;
    fGainCompensation = calculateFinalGainReduction(fCrestFactorAutoGain);
}


float GainReducer::getRatio()
/*  Get current compression ratio.

    return value (float): returns the current compression ratio
 */
{
    return 1.0f / fRatio;
}


void GainReducer::setRatio(float fRatioNew)
/*  Set new compression ratio.

    nRatioNew (float): new compression ratio

    return value: none
 */
{
    fRatio = 1.0f / fRatioNew;
    fGainCompensation = calculateFinalGainReduction(fCrestFactorAutoGain);
}


int GainReducer::getAttackRate()
/*  Get current attack rate.

    return value (integer): returns the current attack rate in
    milliseconds
 */
{
    return nAttackRate;
}


void GainReducer::setAttackRate(int nAttackRateNew)
/*  Set new attack rate.

    nAttackRateNew (integer): new attack rate in milliseconds

    return value: none
 */
{
    nAttackRate = nAttackRateNew;

    if (nAttackRate == 0)
    {
        fAttackCoefficient = 0.0f;
    }
    else
    {
        float fAttackRateSeconds = nAttackRate / 1000.0f;

        if (bLogarithmicEnvelope)
        {
            // logarithmic envelope reaches 95% of the final reading
            // in the given attack time
            fAttackCoefficient = powf(0.05f, fTimePassed / fAttackRateSeconds);
        }
        else
        {
            // rise time: rises 10 dB per interval defined in release
            // rate (linear)
            fAttackCoefficient = 10.0f * fTimePassed / fAttackRateSeconds;
        }
    }
}


int GainReducer::getReleaseRate()
/*  Get current release rate.

    return value (integer): returns the current release rate in
    milliseconds
 */
{
    return nReleaseRate;
}


void GainReducer::setReleaseRate(int nReleaseRateNew)
/*  Set new release rate.

    nReleaseRateNew (integer): new release rate in milliseconds

    return value: none
 */
{
    nReleaseRate = nReleaseRateNew;

    if (nReleaseRate == 0)
    {
        fReleaseCoefficient = 0.0f;
    }
    else
    {
        float fReleaseRateSeconds = nReleaseRate / 1000.0f;

        if (bLogarithmicEnvelope)
        {
            // logarithmic envelope reaches 95% of the final reading
            // in the given release time
            fReleaseCoefficient = powf(0.05f, fTimePassed / fReleaseRateSeconds);
        }
        else
        {
            // fall time: falls 10 dB per interval defined in release
            // rate (linear)
            fReleaseCoefficient = 10.0f * fTimePassed / fReleaseRateSeconds;
        }
    }
}


float GainReducer::getGainReduction(bool useGainCompensation)
/*  Get current gain reduction.

    useGainCompensation (boolean): determines whether the gain
    reduction should be level compensated or not

    return value (float): returns the current gain reduction in
    decibel
 */
{
    if (useGainCompensation)
    {
        return fGainReduction - fGainCompensation;
    }
    else
    {
        return fGainReduction;
    }
}


float GainReducer::calculateFinalGainReduction(float fInputLevel)
/*  Calculate final gain reduction from input level.

    fInputLevel (float): current input level in decibels

    return value: calculated final gain reduction in decibels
 */
{
    if (fInputLevel > fThreshold)
    {
        return (fInputLevel - fThreshold) * (1.0f - fRatio);
    }
    else
    {
        return 0.0f;
    }
}


void GainReducer::processSample(float fInputLevel)
/*  Process a single audio sample value.

    fInputLevel (float): current audio sample value in decibels

    return value: current gain reduction in decibels
*/
{
    float fGainReductionFinal = calculateFinalGainReduction(fInputLevel);

    if (bLogarithmicEnvelope)
    {
        applyLogarithmicEnvelope(fGainReductionFinal);
    }
    else
    {
        applyLinearEnvelope(fGainReductionFinal);
    }
}


void GainReducer::applyLinearEnvelope(float fGainReductionFinal)
/*  Calculate linear envelope follower.

    fGainReductionFinal (float): calculated final gain reduction in
    decibels

    return value: none
*/
{
    if (fGainReductionFinal == fGainReduction)
    {
        return;
    }
    // apply rise time if proposed gain reduction is above old gain
    // reduction
    else if (fGainReductionFinal > fGainReduction)
    {
        if (fAttackCoefficient == 0.0f)
        {
            fGainReduction = fGainReductionFinal;
        }
        else
        {
            fGainReduction += fAttackCoefficient;

            if (fGainReduction > fGainReductionFinal)
            {
                fGainReduction = fGainReductionFinal;
            }
        }
    }
    // otherwise, apply fall time if proposed gain reduction is below
    // old gain reduction
    else
    {
        if (fReleaseCoefficient == 0.0f)
        {
            fGainReduction = fGainReductionFinal;
        }
        else
        {
            fGainReduction -= fReleaseCoefficient;

            if (fGainReduction < fGainReductionFinal)
            {
                fGainReduction = fGainReductionFinal;
            }
        }
    }
}


void GainReducer::applyLogarithmicEnvelope(float fGainReductionFinal)
/*  Calculate logarithmic envelope follower.

    fGainReductionFinal (float): calculated final gain reduction in
    decibels

    return value: none
*/
{
    float fTimeCoefficient;

    if (fGainReductionFinal == fGainReduction)
    {
        return;
    }
    // apply rise time if proposed gain reduction is above old gain
    // reduction
    else if (fGainReductionFinal > fGainReduction)
    {
        fTimeCoefficient = fAttackCoefficient;
    }
    // otherwise, apply fall time if proposed gain reduction is below
    // old gain reduction
    else
    {
        fTimeCoefficient = fReleaseCoefficient;
    }

    if (fTimeCoefficient == 0.0f)
    {
        fGainReduction = fGainReductionFinal;
    }
    else
    {
        // Thanks to Bram from Smartelectronix for the code snippet!
        // (http://www.musicdsp.org/showone.php?id=136)
        //
        // rise and fall: 95% of final reading in "fMeterInertia"
        // seconds
        fGainReduction = fTimeCoefficient * (fGainReduction - fGainReductionFinal) + fGainReductionFinal;
    }
}


float GainReducer::level2decibel(float fLevel)
/*  Convert level from linear scale to decibels (dB).

    fLevel (float): audio level

    return value (float): returns given level in decibels (dB) when
    above "fMeterMinimumDecibel", otherwise "fMeterMinimumDecibel"
*/
{
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


float GainReducer::decibel2level(float fDecibels)
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
