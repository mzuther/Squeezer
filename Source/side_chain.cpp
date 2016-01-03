/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013-2016 Martin Zuther (http://www.mzuther.de/)

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

#include "side_chain.h"


SideChain::SideChain(int nSampleRate) :
    gainStageFET(nSampleRate),
    gainStageOptical(nSampleRate)
/*  Constructor.

    nSampleRate (integer): internal sample rate

    return value: none
*/
{
    dSampleRate = (double) nSampleRate;

    setThreshold(-12.0);
    setRatio(2.0);
    setKneeWidth(0.0);

    setDetectorRmsFilter(10.0);
    nDetectorType = Compressor::DetectorSmoothBranching;
    nGainStageType = Compressor::GainStageFET;

    setAttackRate(10);
    setReleaseRate(100);
    setDetector(nDetectorType);
    setGainStage(nGainStageType);

    // reset (i.e. initialise) all relevant variables
    reset();

#if DEBUG_RELEASE_RATE
    fTimePassed = 1.0 / double(nSampleRate);

    dDebugFinalValue90 = -1.0;
    dDebugTimeInReleasePhase = 0.0;
#endif
}


SideChain::~SideChain()
/*  Destructor.

    return value: none
*/
{
}


void SideChain::reset()
/*  Reset all relevant variables.

    return value: none
*/
{
    dGainReduction = 0.0;
    dGainCompensation = 0.0;
    dDetectorOutputLevelSquared = 0.0;

    dCrestFactorAutoGain = 20.0;
}


double SideChain::getDetectorRmsFilter()
/*  Get current detector RMS filter rate.

    return value (double): returns current current detector RMS filter
    rate
*/
{
    return dDetectorRateMilliSeconds;
}


void SideChain::setDetectorRmsFilter(double dDetectorRateMilliSecondsNew)
/*  Set new detector RMS filter rate.

    dDetectorRateMilliSecondsNew (double): new detector RMS filter rate

    return value: none
*/
{
    dDetectorRateMilliSeconds = dDetectorRateMilliSecondsNew;
    double dDetectorRateSeconds = dDetectorRateMilliSeconds / 1000.0;

    // logarithmic envelope reaches 90% of the final reading
    // in the given attack time
    dDetectorCoefficient = exp(log(0.10) / (dDetectorRateSeconds * dSampleRate));
}


int SideChain::getDetector()
/*  Get current compressor detector type.

    return value (integer): returns compressor detector type
 */
{
    return nDetectorType;
}


void SideChain::setDetector(int nDetectorTypeNew)
/*  Set new compressor detector type.

    nDetectorTypeNew (integer): new compressor detector type

    return value: none
 */
{
    nDetectorType = nDetectorTypeNew;
    dGainReductionIntermediate = 0.0;

    setAttackRate(nAttackRate);
    setReleaseRate(nReleaseRate);
}


int SideChain::getGainStage()
/*  Get current compressor detector type.

    return value (integer): returns compressor detector type
 */
{
    return nGainStageType;
}


void SideChain::setGainStage(int nGainStageTypeNew)
/*  Set new compressor gain stage type.

    nGainStageTypeNew (integer): new compressor gain stage type

    return value: none
 */
{
    nGainStageType = nGainStageTypeNew;

    // update gain compensation
    setThreshold(dThreshold);

    if (nGainStageType == Compressor::GainStageFET)
    {
        gainStageFET.reset(dGainReduction);
    }
    else
    {
        gainStageOptical.reset(dGainReduction);
    }
}


double SideChain::getThreshold()
/*  Get current threshold.

    return value (double): returns the current threshold in decibels
 */
{
    return dThreshold;
}


void SideChain::setThreshold(double dThresholdNew)
/*  Set new threshold.

    dThresholdNew (double): new threshold in decibels

    return value: none
 */
{
    dThreshold = dThresholdNew;
    dGainCompensation = queryGainComputer(dCrestFactorAutoGain) / 2.0;
}


double SideChain::getRatio()
/*  Get current compression ratio.

    return value (double): returns the current compression ratio
 */
{
    return 1.0 / (1.0 - dRatioInternal);
}


void SideChain::setRatio(double dRatioNew)
/*  Set new compression ratio.

    dRatioNew (double): new compression ratio

    return value: none
 */
{
    dRatioInternal = 1.0 - (1.0 / dRatioNew);
    dGainCompensation = queryGainComputer(dCrestFactorAutoGain) / 2.0;
}


double SideChain::getKneeWidth()
/*  Get current knee width.

    return value (double): returns the current knee width in decibels
 */
{
    return dKneeWidth;
}


void SideChain::setKneeWidth(double dKneeWidthNew)
/*  Set new knee width.

    dKneeWidthNew (double): new knee width in decibels

    return value: none
 */
{
    dKneeWidth = dKneeWidthNew;
    dKneeWidthHalf = dKneeWidth / 2.0;
    dKneeWidthDouble = dKneeWidth * 2.0;

    dGainCompensation = queryGainComputer(dCrestFactorAutoGain) / 2.0;
}


int SideChain::getAttackRate()
/*  Get current attack rate.

    return value (integer): returns the current attack rate in
    milliseconds
 */
{
    return nAttackRate;
}


void SideChain::setAttackRate(int nAttackRateNew)
/*  Set new attack rate.

    nAttackRateNew (integer): new attack rate in milliseconds

    return value: none
 */
{
    nAttackRate = nAttackRateNew;

    if (nAttackRate == 0)
    {
        dAttackCoefficient = 0.0;
    }
    else
    {
        double dAttackRateSeconds = nAttackRate / 1000.0;

        // logarithmic envelope reaches 90% of the final reading in
        // the given attack time
        dAttackCoefficient = exp(log(0.10) / (dAttackRateSeconds * dSampleRate));
    }
}


int SideChain::getReleaseRate()
/*  Get current release rate.

    return value (integer): returns the current release rate in
    milliseconds
 */
{
    return nReleaseRate;
}


void SideChain::setReleaseRate(int nReleaseRateNew)
/*  Set new release rate.

    nReleaseRateNew (integer): new release rate in milliseconds

    return value: none
 */
{
    nReleaseRate = nReleaseRateNew;

    if (nReleaseRate == 0)
    {
        dReleaseCoefficient = 0.0;
    }
    else
    {
        double dReleaseRateSeconds = nReleaseRate / 1000.0;

        if (nDetectorType == Compressor::DetectorLinear)
        {
            // fall time: falls 10 dB per interval defined in release
            // rate (linear)
            dReleaseCoefficient = 10.0 / (dReleaseRateSeconds * dSampleRate);
        }
        else
        {
            // logarithmic envelope reaches 90% of the final reading
            // in the given release time
            dReleaseCoefficient = exp(log(0.10) / (dReleaseRateSeconds * dSampleRate));
        }
    }
}


double SideChain::getGainReduction(bool bAutoMakeupGain)
/*  Get current gain reduction.

    bAutoMakeupGain (boolean): determines whether the gain reduction
    should be level compensated or not

    return value (double): returns the current gain reduction in
    decibel
 */
{
    double dGainReductionTemp;

    if (nGainStageType == Compressor::GainStageFET)
    {
        dGainReductionTemp = gainStageFET.processGainReduction(dGainReduction, dGainReductionIdeal);
    }
    else
    {
        dGainReductionTemp = gainStageOptical.processGainReduction(dGainReduction, dGainReductionIdeal);
    }

    if (bAutoMakeupGain)
    {

        return dGainReductionTemp - dGainCompensation;
    }
    else
    {
        return dGainReductionTemp;
    }
}


double SideChain::queryGainComputer(double dInputLevel)
/*  Calculate gain reduction and envelopes from input level.

    dInputLevel (double): current input level in decibels

    return value: calculated gain reduction in decibels
 */
{
    double dAboveThreshold = dInputLevel - dThreshold;

    if (dKneeWidth == 0.0)
    {
        if (dInputLevel <= dThreshold)
        {
            return 0.0;
        }
        else
        {
            return dAboveThreshold * dRatioInternal;
        }
    }
    else
    {
        // algorithm adapted from Giannoulis et al., "Digital Dynamic
        // Range Compressor Design - A Tutorial and Analysis", JAES,
        // 60(6):399-408, 2012
        if (dAboveThreshold < -dKneeWidthHalf)
        {
            return 0.0;
        }
        else if (dAboveThreshold > dKneeWidthHalf)
        {
            return dAboveThreshold * dRatioInternal;
        }
        else
        {
            double dFactor = dAboveThreshold + dKneeWidthHalf;
            double dFactorSquared = dFactor * dFactor;

            return dFactorSquared / dKneeWidthDouble * dRatioInternal;
        }
    }
}


void SideChain::processSample(double dInputLevel)
/*  Process a single audio sample value.

    dInputLevel (double): current audio sample value in decibels

    return value: current gain reduction in decibels
*/
{
    // feed input level to gain computer
    dGainReductionIdeal = queryGainComputer(dInputLevel);

    // filter calculated gain reduction through level detection filter
    double dGainReductionNew = applyLevelDetectionFilter(dGainReductionIdeal);

    // feed output from gain computer to level detector
    switch (nDetectorType)
    {
    case Compressor::DetectorLinear:
        applyDetectorLinear(dGainReductionNew);
        break;

    case Compressor::DetectorSmoothDecoupled:
        applyDetectorSmoothDecoupled(dGainReductionNew);
        break;

    case Compressor::DetectorSmoothBranching:
        applyDetectorSmoothBranching(dGainReductionNew);
        break;

    default:
        DBG("[Squeezer] sidechain::processSample --> invalid detector");
        break;
    }

#if DEBUG_RELEASE_RATE

    // reset things during attack phase
    if (dGainReductionNew > dGainReduction)
    {
        dDebugFinalValue90 = -1.0;
        dDebugTimeInReleasePhase = 0.0;
    }
    // we're in release phase
    else
    {
        // release phase has just started
        if (dDebugFinalValue90 < 0.0)
        {
            // only measure "real" data
            if (dGainReduction >= 1.0)
            {
                // determine time when the envelope reaches 90% of the
                // final reading
                dDebugFinalValue90 = dGainReduction * 0.1;

                // reset time measurement
                dDebugTimeInReleasePhase = 0.0;
            }
        }
        // test for 90% of the final reading
        else if (dGainReduction <= dDebugFinalValue90)
        {
            // display measured time
            DBG(String(dDebugTimeInReleasePhase * 1000.0, 1) + " ms");

            // reset things
            dDebugFinalValue90 = -1.0;
            dDebugTimeInReleasePhase = 0.0;
        }
        // update time since start of release phase
        else
        {
            dDebugTimeInReleasePhase += dTimePassed;
        }
    }

#endif
}


double SideChain::applyLevelDetectionFilter(double dDetectorInputLevel)
{
    double dDetectorInputLevelSquared = dDetectorInputLevel * dDetectorInputLevel;
    double dDetectorOutputLevelSquaredOld = dDetectorOutputLevelSquared;

    dDetectorOutputLevelSquared = (dDetectorCoefficient * dDetectorOutputLevelSquaredOld) + (1.0 - dDetectorCoefficient) * dDetectorInputLevelSquared;

    double dDetectorOutputLevel = sqrt(dDetectorOutputLevelSquared);
    return dDetectorOutputLevel;
}


void SideChain::applyDetectorLinear(double dGainReductionNew)
/*  Calculate detector with logarithmic attack and linear release
    ("Linear").

    dGainReductionNew (double): calculated new gain reduction in
    decibels

    return value: none
*/
{
    // apply attack rate if proposed gain reduction is above old gain
    // reduction
    if (dGainReductionNew >= dGainReduction)
    {
        if (dAttackCoefficient == 0.0)
        {
            dGainReduction = dGainReductionNew;
        }
        else
        {
            // algorithm adapted from Giannoulis et al., "Digital
            // Dynamic Range Compressor Design - A Tutorial and
            // Analysis", JAES, 60(6):399-408, 2012

            double dGainReductionOld = dGainReduction;
            dGainReduction = (dAttackCoefficient * dGainReductionOld) + (1.0 - dAttackCoefficient) * dGainReductionNew;
        }
    }
    // otherwise, apply release rate if proposed gain reduction is
    // below old gain reduction
    else
    {
        if (dReleaseCoefficient == 0.0)
        {
            dGainReduction = dGainReductionNew;
        }
        else
        {
            dGainReduction -= dReleaseCoefficient;

            if (dGainReduction < dGainReductionNew)
            {
                dGainReduction = dGainReductionNew;
            }
        }
    }
}


void SideChain::applyDetectorSmoothDecoupled(double dGainReductionNew)
/*  Calculate smooth decoupled detector ("S-Curve").

    dGainReductionNew (double): calculated gain reduction in decibels

    return value: none
*/
{
    // algorithm adapted from Giannoulis et al., "Digital Dynamic
    // Range Compressor Design - A Tutorial and Analysis", JAES,
    // 60(6):399-408, 2012
    //
    // apply release envelope
    if (dReleaseCoefficient == 0.0)
    {
        dGainReductionIntermediate = dGainReductionNew;
    }
    else
    {
        double dGainReductionIntermediateOld = dGainReductionIntermediate;
        dGainReductionIntermediate = (dReleaseCoefficient * dGainReductionIntermediateOld) + (1.0 - dReleaseCoefficient) * dGainReductionNew;

        // maximally fast peak detection
        if (dGainReductionNew > dGainReductionIntermediate)
        {
            dGainReductionIntermediate = dGainReductionNew;
        }
    }

    // apply attack envelope
    if (dAttackCoefficient == 0.0)
    {
        dGainReduction = dGainReductionIntermediate;
    }
    else
    {
        double dGainReductionOld = dGainReduction;
        dGainReduction = (dAttackCoefficient * dGainReductionOld) + (1.0 - dAttackCoefficient) * dGainReductionIntermediate;
    }
}


void SideChain::applyDetectorSmoothBranching(double dGainReductionNew)
/*  Calculate smooth branching detector ("Logarithmic").

    dGainReductionNew (double): calculated gain reduction in decibels

    return value: none
*/
{
    // apply attack rate if proposed gain reduction is above old gain
    // reduction
    if (dGainReductionNew > dGainReduction)
    {
        if (dAttackCoefficient == 0.0)
        {
            dGainReduction = dGainReductionNew;
        }
        else
        {
            // algorithm adapted from Giannoulis et al., "Digital
            // Dynamic Range Compressor Design - A Tutorial and
            // Analysis", JAES, 60(6):399-408, 2012

            double dGainReductionOld = dGainReduction;
            dGainReduction = (dAttackCoefficient * dGainReductionOld) + (1.0 - dAttackCoefficient) * dGainReductionNew;
        }
    }
    // otherwise, apply release rate if proposed gain reduction is
    // below old gain reduction
    else
    {
        if (dReleaseCoefficient == 0.0)
        {
            dGainReduction = dGainReductionNew;
        }
        else
        {
            // algorithm adapted from Giannoulis et al., "Digital
            // Dynamic Range Compressor Design - A Tutorial and
            // Analysis", JAES, 60(6):399-408, 2012

            double dGainReductionOld = dGainReduction;
            dGainReduction = (dReleaseCoefficient * dGainReductionOld) + (1.0 - dReleaseCoefficient) * dGainReductionNew;
        }
    }
}


double SideChain::level2decibel(double dLevel)
/*  Convert level from linear scale to decibels (dB).

    dLevel (double): audio level

    return value (double): returns given level in decibels (dB) when
    above "dMeterMinimumDecibel", otherwise "dMeterMinimumDecibel"
*/
{
    // just an inch below the meter's lowest segment
    double dMeterMinimumDecibel = -70.01;

    // log(0) is not defined, so return "fMeterMinimumDecibel"
    if (dLevel == 0.0)
    {
        return dMeterMinimumDecibel;
    }
    else
    {
        // calculate decibels from audio level (a factor of 20.0 is
        // needed to calculate *level* ratios, whereas 10.0 is needed
        // for *power* ratios!)
        double dDecibels = 20.0 * log10(dLevel);

        // to make meter ballistics look nice for low levels, do not
        // return levels below "fMeterMinimumDecibel"
        if (dDecibels < dMeterMinimumDecibel)
        {
            return dMeterMinimumDecibel;
        }
        else
        {
            return dDecibels;
        }
    }
}


double SideChain::decibel2level(double dDecibels)
/*  Convert level from decibels (dB) to linear scale.

    dLevel (double): audio level in decibels (dB)

    return value (double): given level in linear scale
*/
{
    // calculate audio level from decibels (a divisor of 20.0 is
    // needed to calculate *level* ratios, whereas 10.0 is needed for
    // *power* ratios!)
    double dLevel = pow(10.0, dDecibels / 20.0);
    return dLevel;
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
