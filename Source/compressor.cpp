/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013-2015 Martin Zuther (http://www.mzuther.de/)

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

#include "compressor.h"


//==============================================================================

Compressor::Compressor(int channels, int sample_rate) :
    nChannels(channels),
    nSampleRate(sample_rate),
    nMeterBufferSize((int)(nSampleRate *dBufferLength)),
    MeterInputBuffer(channels, nMeterBufferSize),
    MeterOutputBuffer(channels, nMeterBufferSize),
    dither(24)
{
    jassert((channels == 1) || (channels == 2));

    dCrestFactor = 20.0;
    bUpwardExpansion = false;

    // the meter's sample buffer holds 50 ms worth of samples

    // fall time: 26 dB in 3 seconds (linear)
    dReleaseCoefLinear = 26.0 * dBufferLength / 3.0;

    nMeterBufferPosition = 0;

    // reset meter and set up array members
    resetMeters();

    nWetMix = 100;
    setBypass(false);
    setDesign(Compressor::DesignFeedForward);

    setStereoLink(100);

    setAutoMakeupGain(false);
    setMakeupGain(0.0);
    setWetMix(100);

    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        p_arrSideChain.add(new SideChain(nSampleRate));

        arrInputSamples.add(0.0);
        arrSidechainSamples.add(0.0);
        arrOutputSamples.add(0.0);

        // initialise side-chain filter (HPF, 0.5% ripple, 6 poles)
        p_arrSidechainFilter.add(new FilterChebyshev(0.001, true, 0.5, 6));
    }

    // disable side-chain filter
    setSidechainFilterState(false);
    setSidechainFilterCutoff(100);
    setSidechainFilterGain(0.0);
    setSidechainListen(false);
}


Compressor::~Compressor()
{
}


void Compressor::resetMeters()
{
    double dMeterMinimumDecibel = -(70.01 + dCrestFactor);

    // loop through all audio channels
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        // set peak meter levels to meter minimum
        arrPeakMeterInputLevels.set(nChannel, dMeterMinimumDecibel);
        arrPeakMeterOutputLevels.set(nChannel, dMeterMinimumDecibel);

        // set peak meter peak levels to meter minimum
        arrPeakMeterPeakInputLevels.set(nChannel, dMeterMinimumDecibel);
        arrPeakMeterPeakOutputLevels.set(nChannel, dMeterMinimumDecibel);

        // set maximum meter levels to meter minimum
        arrMaximumInputLevels.set(nChannel, dMeterMinimumDecibel);
        arrMaximumOutputLevels.set(nChannel, dMeterMinimumDecibel);

        // set average meter levels to meter minimum
        arrAverageMeterInputLevels.set(nChannel, dMeterMinimumDecibel);
        arrAverageMeterOutputLevels.set(nChannel, dMeterMinimumDecibel);

        // set gain reduction meter to meter minimum
        arrGainReduction.set(nChannel, dMeterMinimumDecibel);
        arrGainReductionPeak.set(nChannel, dMeterMinimumDecibel);

        // reset variables for meter hold feature
        arrPeakMeterPeakInputHoldTime.set(nChannel, 0.0);
        arrPeakMeterPeakOutputHoldTime.set(nChannel, 0.0);
        arrGainReductionHoldTime.set(nChannel, 0.0);
    }
}


bool Compressor::getBypass()
/*  Get current bypass state.

    return value (boolean): returns current bypass state
 */
{
    return bBypassCompressor;
}


void Compressor::setBypass(bool bBypassCompressorNew)
/*  Set new bypass state.

    bBypassCompressorNew (boolean): new bypass state

    return value: none
 */
{
    bBypassCompressor = bBypassCompressorNew;
    bBypassCompressorCombined = (bBypassCompressor || (nWetMix == 0));
}


double Compressor::getDetectorRmsFilter()
/*  Get current detector RMS filter rate.

    return value (double): returns current current detector RMS filter
    rate
*/
{
    return p_arrSideChain[0]->getDetectorRmsFilter();
}


void Compressor::setDetectorRmsFilter(double dDetectorRateMilliSecondsNew)
/*  Set new detector RMS filter rate.

    dDetectorRateMilliSecondsNew (double): new detector RMS filter rate

    return value: none
*/
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        p_arrSideChain[nChannel]->setDetectorRmsFilter(dDetectorRateMilliSecondsNew);
    }
}


int Compressor::getDesign()
/*  Get current compressor design.

    return value (integer): returns the current compressor design
 */
{
    return nDesign;
}


void Compressor::setDesign(int nDesignNew)
/*  Set new compressor design.

    nDesignNew (integer): new compressor design

    return value: none
 */
{
    nDesign = nDesignNew;
    bDesignFeedForward = (nDesign == Compressor::DesignFeedForward);
}


double Compressor::getThreshold()
/*  Get current threshold.

    return value (double): returns the current threshold in decibels
 */
{
    return p_arrSideChain[0]->getThreshold();
}


void Compressor::setThreshold(double dThresholdNew)
/*  Set new threshold.

    dThresholdNew (double): new threshold in decibels

    return value: none
 */
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        p_arrSideChain[nChannel]->setThreshold(dThresholdNew);
    }
}


double Compressor::getRatio()
/*  Get current compression ratio.

    return value (double): returns the current compression ratio
 */
{
    double dRatioNew = p_arrSideChain[0]->getRatio();

    if (bUpwardExpansion)
    {
        return 1.0 / dRatioNew;
    }
    else
    {
        return dRatioNew;
    }
}


void Compressor::setRatio(double dRatioNew)
/*  Set new compression ratio.

    dRatioNew (double): new compression ratio

    return value: none
 */
{
    if (dRatioNew < 1.0)
    {
        bUpwardExpansion = true;
        dRatioNew = 1.0 / dRatioNew;
    }
    else
    {
        bUpwardExpansion = false;
    }

    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        p_arrSideChain[nChannel]->setRatio(dRatioNew);
    }
}


double Compressor::getKneeWidth()
/*  Get current knee width.

    return value (double): returns the current knee width in decibels
 */
{
    return p_arrSideChain[0]->getKneeWidth();
}


void Compressor::setKneeWidth(double dKneeWidthNew)
/*  Set new knee width.

    dKneeWidthNew (double): new knee width in decibels

    return value: none
 */
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        p_arrSideChain[nChannel]->setKneeWidth(dKneeWidthNew);
    }

}


int Compressor::getAttackRate()
/*  Get current attack rate.

    return value (integer): returns the current attack rate in
    milliseconds
 */
{
    return p_arrSideChain[0]->getAttackRate();
}


void Compressor::setAttackRate(int nAttackRateNew)
/*  Set new attack rate.

    nAttackRateNew (integer): new attack rate in milliseconds

    return value: none
 */
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        p_arrSideChain[nChannel]->setAttackRate(nAttackRateNew);
    }
}


int Compressor::getReleaseRate()
/*  Get current release rate.

    return value (integer): returns the current release rate in
    milliseconds
 */
{
    return p_arrSideChain[0]->getReleaseRate();
}


void Compressor::setReleaseRate(int nReleaseRateNew)
/*  Set new release rate.

    nReleaseRateNew (integer): new release rate in milliseconds

    return value: none
 */
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        p_arrSideChain[nChannel]->setReleaseRate(nReleaseRateNew);
    }
}


int Compressor::getDetector()
/*  Get current compressor detector type.

    return value (integer): returns compressor detector type
 */
{
    return p_arrSideChain[0]->getDetector();
}


void Compressor::setDetector(int nDetectorTypeNew)
/*  Set new compressor detector type.

    nDetectorTypeNew (integer): new compressor detector type

    return value: none
 */
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        p_arrSideChain[nChannel]->setDetector(nDetectorTypeNew);
    }
}


int Compressor::getGainStage()
/*  Get current compressor detector type.

    return value (integer): returns compressor detector type
 */
{
    return p_arrSideChain[0]->getGainStage();
}


void Compressor::setGainStage(int nGainStageTypeNew)
/*  Set new compressor gain stage type.

    nGainStageTypeNew (integer): new compressor gain stage type

    return value: none
 */
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        p_arrSideChain[nChannel]->setGainStage(nGainStageTypeNew);
    }
}


int Compressor::getStereoLink()
/*  Get current stereo link percentage.

    return value (integer): returns the current stereo link percentage
    (0 to 100)
 */
{
    return nStereoLink;
}


void Compressor::setStereoLink(int nStereoLinkNew)
/*  Set new stereo link percentage.

    nStereoLinkNew (integer): new stereo link percentage (0 to 100)

    return value: none
 */
{
    nStereoLink = nStereoLinkNew;

    // amplification factor for other channel ranging from 0.0 (no
    // stereo linking) to 0.5 (full stereo linking)
    dStereoLinkOther = nStereoLink / 200.0;

    // amplification factor for original channel ranging from 1.0 (no
    // stereo linking) to 0.5 (full stereo linking)
    dStereoLinkOriginal = 1.0 - dStereoLinkOther;
}


bool Compressor::getAutoMakeupGain()
/*  Get current auto make-up gain state.

    return value (boolean): returns current auto make-up gain state
 */
{
    return bAutoMakeupGain;
}


void Compressor::setAutoMakeupGain(bool bAutoMakeupGainNew)
/*  Set new auto make-up gain state.

    bAutoMakeupGainNew (boolean): new auto make-up gain state

    return value: none
 */
{
    bAutoMakeupGain = bAutoMakeupGainNew;
}


double Compressor::getMakeupGain()
/*  Get current make-up gain.

    return value (double): returns the current make-up gain in decibels
 */
{
    return dMakeupGainDecibel;
}


void Compressor::setMakeupGain(double dMakeupGainNew)
/*  Set new make-up gain.

    nMakeupGainNew (double): new make-up gain in decibels

    return value: none
 */
{
    dMakeupGainDecibel = dMakeupGainNew;
    dMakeupGain = SideChain::decibel2level(dMakeupGainDecibel);
}


int Compressor::getWetMix()
/*  Get current wet mix percentage.

    return value (integer): returns the current wet mix percentage
    (0 to 100)
 */
{
    return nWetMix;
}


void Compressor::setWetMix(int nWetMixNew)
/*  Set new wet mix percentage.

    nWetMixNew (integer): new wet mix percentage (0 to 100)

    return value: none
 */
{
    nWetMix = nWetMixNew;

    dWetMix = nWetMix / 100.0;
    dDryMix = 1.0 - dWetMix;

    bBypassCompressorCombined = (bBypassCompressor || (nWetMix == 0));
}


bool Compressor::getSidechainFilterState()
/*  Get current side-chain filter state.

    return value (boolean): returns current side-chain filter state
 */
{
    return bSidechainFilterState;
}


void Compressor::setSidechainFilterState(bool bSidechainFilterStateNew)
/*  Set new side-chain filter state.

    bSidechainFilterStateNew (boolean): new side-chain filter state

    return value: none
 */
{
    bSidechainFilterState = bSidechainFilterStateNew;
}


int Compressor::getSidechainFilterCutoff()
/*  Get current side-chain filter cutoff frequency.

    return value (integer): side-chain filter cutoff frequency (in
    Hertz)
 */
{
    return nSidechainFilterCutoff;
}


void Compressor::setSidechainFilterCutoff(int nSidechainFilterCutoffNew)
/*  Set new side-chain filter cutoff frequency.

    nSidechainFilterCutoff (integer): new side-chain filter cutoff
    frequency (in Hertz)

    return value: none
 */
{
    nSidechainFilterCutoff = nSidechainFilterCutoffNew;

    double dRelativeCutoffFrequency = double(nSidechainFilterCutoff) / double(nSampleRate);
    bool bIsHighpass = (nSidechainFilterCutoff < 2900) ? true : false;

    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        p_arrSidechainFilter[nChannel]->changeParameters(dRelativeCutoffFrequency, bIsHighpass);
    }
}


double Compressor::getSidechainFilterGain()
/*  Get current side-chain filter gain.

    return value (double): side-chain filter gain (in decibels)
 */
{
    return SideChain::level2decibel(dSidechainFilterGain);
}


void Compressor::setSidechainFilterGain(double dSidechainFilterGainNew)
/*  Set new side-chain filter gain.

    dSidechainFilterGain (double): new side-chain filter gain (in
    decibels)

    return value: none
 */
{
    dSidechainFilterGain = SideChain::decibel2level(dSidechainFilterGainNew);

    // normalise filtered output (the values below were found by
    // sending several hundred samples of 1.0 through a high-pass
    // filter with a relative cutoff frequency of 0.1 and then reading
    // the filter's output value)
    //
    //  2 poles:  1.09487492
    //  4 poles:  2.22993454
    //  6 poles:  6.53854690
    //  8 poles:  22.0697314
    // 10 poles:  79.4259655
    // 12 poles:  295.703923
    dGainNormalise = dSidechainFilterGain / 6.53854690;
}


bool Compressor::getSidechainListen()
/*  Get current side-chain listen state.

    return value (boolean): returns current side-chain listen state
 */
{
    return bSidechainListen;
}


void Compressor::setSidechainListen(bool bSidechainListenNew)
/*  Set new side-chain listen state.

    bSidechainListenNew (boolean): new side-chain listen state

    return value: none
 */
{
    bSidechainListen = bSidechainListenNew;
}


double Compressor::getGainReduction(int nChannel)
/*  Get current gain reduction.

    nChannel (integer): queried audio channel

    return value (double): returns the current gain reduction in
    decibel
 */
{
    jassert(nChannel >= 0);
    jassert(nChannel < nChannels);

    if (bBypassCompressorCombined)
    {
        return 0.0;
    }
    else
    {
        return arrGainReduction[nChannel];
    }
}


double Compressor::getGainReductionPeak(int nChannel)
/*  Get current peak gain reduction.

    nChannel (integer): queried audio channel

    return value (double): returns the current peak gain reduction in
    decibel
 */
{
    jassert(nChannel >= 0);
    jassert(nChannel < nChannels);

    return arrGainReductionPeak[nChannel];
}


double Compressor::getPeakMeterInputLevel(int nChannel)
/*  Get current input peak level.

    nChannel (integer): selected audio channel

    return value (double): returns current input peak level in decibel
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nChannels);

    return arrPeakMeterInputLevels[nChannel] + dCrestFactor;
}


double Compressor::getPeakMeterOutputLevel(int nChannel)
/*  Get current output peak level.

    nChannel (integer): selected audio channel

    return value (double): returns current output peak level in decibel
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nChannels);

    return arrPeakMeterOutputLevels[nChannel] + dCrestFactor;
}


double Compressor::getPeakMeterPeakInputLevel(int nChannel)
/*  Get current input peak meter peak level.

    nChannel (integer): selected audio channel

    return value (double): returns current input peak meter peak level
    in decibel
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nChannels);

    return arrPeakMeterPeakInputLevels[nChannel] + dCrestFactor;
}


double Compressor::getPeakMeterPeakOutputLevel(int nChannel)
/*  Get current output peak meter peak level.

    nChannel (integer): selected audio channel

    return value (double): returns current output peak meter peak level
    in decibel
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nChannels);

    return arrPeakMeterPeakOutputLevels[nChannel] + dCrestFactor;
}


double Compressor::getMaximumInputLevel(int nChannel)
/*  Get current input maximum level.

    nChannel (integer): selected audio channel

    return value (double): returns current input maximum level in
    decibel
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nChannels);

    return arrMaximumInputLevels[nChannel] + dCrestFactor;
}


double Compressor::getMaximumOutputLevel(int nChannel)
/*  Get current output maximum level.

    nChannel (integer): selected audio channel

    return value (double): returns current output maximum level in
    decibel
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nChannels);

    return arrMaximumOutputLevels[nChannel] + dCrestFactor;
}


double Compressor::getAverageMeterInputLevel(int nChannel)
/*  Get current input average level.

    nChannel (integer): selected audio channel

    return value (double): returns current input average level in
    decibel
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nChannels);

    return arrAverageMeterInputLevels[nChannel] + dCrestFactor;
}


double Compressor::getAverageMeterOutputLevel(int nChannel)
/*  Get current output average level.

    nChannel (integer): selected audio channel

    return value (double): returns current output average level in
    decibel
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nChannels);

    return arrAverageMeterOutputLevels[nChannel] + dCrestFactor;
}


void Compressor::processBlock(AudioSampleBuffer &buffer)
{
    int nNumSamples = buffer.getNumSamples();

    for (int nSample = 0; nSample < nNumSamples; nSample++)
    {
        // get and prepare input samples (all channels have to be
        // prepared before any processing can take place!)
        for (int nChannel = 0; nChannel < nChannels; nChannel++)
        {
            // get current input sample
            float fInputSample = buffer.getSample(nChannel, nSample);
            double dInputSample = (double) fInputSample;

            // de-normalise input sample
            fInputSample += fAntiDenormal;
            dInputSample += dAntiDenormal;

            // store de-normalised input sample
            arrInputSamples.set(nChannel, dInputSample);

            // store de-normalised input sample in buffer for input
            // meter
            MeterInputBuffer.setSample(nChannel, nMeterBufferPosition, fInputSample);
        }

        // compressor is bypassed (or mix is set to 0 percent)
        if (bBypassCompressorCombined)
        {
            for (int nChannel = 0; nChannel < nChannels; nChannel++)
            {
                // store de-normalised (!) input sample in buffer for
                // output meter
                MeterOutputBuffer.copyFrom(nChannel, nMeterBufferPosition, MeterInputBuffer, nChannel, nSample, 1);

                // store gain reduction now and apply ballistics later
                arrGainReduction.set(nChannel, 0.0);
            }

            // update meter ballistics and increment buffer location
            updateMeterBallistics();

            // skip compressor
            continue;
        }

        // compress channels
        for (int nChannel = 0; nChannel < nChannels; nChannel++)
        {
            // feed-forward design
            if (bDesignFeedForward)
            {
                // stereo linking is off (save some processing time)
                if (dStereoLinkOriginal == 0.0)
                {
                    arrSidechainSamples.set(nChannel, arrInputSamples[nChannel]);
                }
                // stereo linking is on
                else
                {
                    // get ID of other channel
                    int nChannelOther = (nChannel == 0) ? 1 : 0;

                    // mix stereo input samples according to stereo
                    // link percentage
                    arrSidechainSamples.set(nChannel, (arrInputSamples[nChannel] * dStereoLinkOriginal) + (arrInputSamples[nChannelOther] * dStereoLinkOther));
                }

                // filter side-chain sample (the filter's output is
                // already de-normalised!)
                if (bSidechainFilterState)
                {
                    arrSidechainSamples.set(nChannel, p_arrSidechainFilter[nChannel]->filterSample(arrSidechainSamples[nChannel]) * dGainNormalise);
                }

                // calculate level of side-chain sample
                double dSidechainInputLevel = SideChain::level2decibel(fabs(arrSidechainSamples[nChannel]));

                // apply crest factor
                dSidechainInputLevel += dCrestFactor;

                // send current input sample to gain reduction unit
                p_arrSideChain[nChannel]->processSample(dSidechainInputLevel);
            }

            // store gain reduction now and apply ballistics later
            arrGainReduction.set(nChannel, p_arrSideChain[nChannel]->getGainReduction(false));

            // apply gain reduction to current input sample
            //
            //  feed-forward design:  current gain reduction
            //  feed-back design:     "old" gain reduction
            double dGainReduction = p_arrSideChain[nChannel]->getGainReduction(bAutoMakeupGain);

            // invert gain reduction for upward expansion
            if (bUpwardExpansion)
            {
                dGainReduction = -dGainReduction;
            }

            arrOutputSamples.set(nChannel, arrInputSamples[nChannel] / SideChain::decibel2level(dGainReduction));

            // apply make-up gain
            arrOutputSamples.set(nChannel, arrOutputSamples[nChannel] * dMakeupGain);
        }

        // post-processing for feed-back design
        if (!bDesignFeedForward)
        {
            // loop over channels
            for (int nChannel = 0; nChannel < nChannels; nChannel++)
            {
                // stereo linking is off (save some processing time)
                if (dStereoLinkOriginal == 0.0)
                {
                    arrSidechainSamples.set(nChannel, arrOutputSamples[nChannel]);
                }
                // stereo linking is on
                else
                {
                    // get ID of other channel
                    int nChannelOther = (nChannel == 0) ? 1 : 0;

                    // mix stereo output samples according to stereo
                    // link percentage
                    arrSidechainSamples.set(nChannel, (arrOutputSamples[nChannel] * dStereoLinkOriginal) + (arrOutputSamples[nChannelOther] * dStereoLinkOther));
                }

                // filter side-chain sample (the filter's output is
                // already de-normalised!)
                if (bSidechainFilterState)
                {
                    arrSidechainSamples.set(nChannel, p_arrSidechainFilter[nChannel]->filterSample(arrSidechainSamples[nChannel]) * dGainNormalise);
                }

                // calculate level of side-chain sample
                double dSidechainOutputLevel = SideChain::level2decibel(fabs(arrSidechainSamples[nChannel]));

                // apply crest factor
                dSidechainOutputLevel += dCrestFactor;

                // feed current output sample back to gain reduction unit
                p_arrSideChain[nChannel]->processSample(dSidechainOutputLevel);
            }
        }

        // loop over channels
        for (int nChannel = 0; nChannel < nChannels; nChannel++)
        {
            // listen to side-chain (already de-normalised)
            if (bSidechainListen)
            {
                // dither output to float
                float fOutput = dither.dither(arrSidechainSamples[nChannel]);
                buffer.setSample(nChannel, nSample, fOutput);
            }
            // listen to compressor's output (already de-normalised)
            else
            {
                // dry shall be mixed in (test to save some processing
                // time)
                if (nWetMix < 100)
                {
                    arrOutputSamples.set(nChannel, (arrOutputSamples[nChannel] * dWetMix) + (arrInputSamples[nChannel] * dDryMix));
                }

                // dither output to float
                float fOutput = dither.dither(arrOutputSamples[nChannel]);
                buffer.setSample(nChannel, nSample, fOutput);
            }

            // store output sample for metering (reflects real output,
            // so when the user listens to the side-chain, the output
            // meter will also display the side-chain's level!)
            MeterOutputBuffer.copyFrom(nChannel, nMeterBufferPosition, buffer, nChannel, nSample, 1);
        }

        // update meter ballistics and increment buffer location
        updateMeterBallistics();
    }
}


void Compressor::updateMeterBallistics()
{
    // update metering buffer position
    nMeterBufferPosition++;

    // meter will only be updated when buffer is full
    if (nMeterBufferPosition < nMeterBufferSize)
    {
        return;
    }

    // reset buffer location
    nMeterBufferPosition = 0;

    // loop over channels
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        // determine peak levels
        double dInputPeak = MeterInputBuffer.getMagnitude(nChannel, 0, nMeterBufferSize);
        double dOutputPeak = MeterOutputBuffer.getMagnitude(nChannel, 0, nMeterBufferSize);

        // convert peak meter levels from linear scale to decibels
        dInputPeak = SideChain::level2decibel(dInputPeak);
        dOutputPeak = SideChain::level2decibel(dOutputPeak);

        // update maximum meter levels
        if (dInputPeak > arrMaximumInputLevels[nChannel])
        {
            arrMaximumInputLevels.set(nChannel, dInputPeak);
        }

        if (dOutputPeak > arrMaximumOutputLevels[nChannel])
        {
            arrMaximumOutputLevels.set(nChannel, dOutputPeak);
        }

        // apply peak meter ballistics
        PeakMeterBallistics(dInputPeak, arrPeakMeterInputLevels.getReference(nChannel), arrPeakMeterPeakInputLevels.getReference(nChannel), arrPeakMeterPeakInputHoldTime.getReference(nChannel));
        PeakMeterBallistics(dOutputPeak, arrPeakMeterOutputLevels.getReference(nChannel), arrPeakMeterPeakOutputLevels.getReference(nChannel), arrPeakMeterPeakOutputHoldTime.getReference(nChannel));

        // apply peak gain reduction ballistics
        GainReductionMeterPeakBallistics(arrGainReduction[nChannel], arrGainReductionPeak.getReference(nChannel), arrGainReductionHoldTime.getReference(nChannel));

        // determine average levels
        double dInputRms = MeterInputBuffer.getRMSLevel(nChannel, 0, nMeterBufferSize);
        double dOutputRms = MeterOutputBuffer.getRMSLevel(nChannel, 0, nMeterBufferSize);

        // convert average meter levels from linear scale to
        // decibels
        dInputRms = SideChain::level2decibel(dInputRms);
        dOutputRms = SideChain::level2decibel(dOutputRms);

        // apply average meter ballistics
        AverageMeterBallistics(dInputRms, arrAverageMeterInputLevels.getReference(nChannel));
        AverageMeterBallistics(dOutputRms, arrAverageMeterOutputLevels.getReference(nChannel));
    }
}


void Compressor::PeakMeterBallistics(double dPeakLevelCurrent, double &dPeakLevelOld, double &dPeakMarkOld, double &dPeakHoldTime)
/*  Calculate ballistics for peak meter levels.

    dPeakLevelCurrent (double): current peak meter level in decibel

    dPeakLevelOld (double): old peak meter reading in decibel (will be
    overwritten!)

    dPeakMarkOld (double): old peak mark in decibel (will be
    overwritten!)

    dPeakHoldTime (double): time since last meter update (will be
    overwritten!)

    return value: none
*/
{
    // limit peak level
    if (dPeakLevelCurrent >= 0.0)
    {
        // immediate rise time, but limit current peak to top mark
        dPeakLevelOld = 0.0;
    }
    // apply rise time if peak level is above old level
    else if (dPeakLevelCurrent >= dPeakLevelOld)
    {
        // immediate rise time, so return current peak level as new
        // peak meter reading
        dPeakLevelOld = dPeakLevelCurrent;
    }
    // otherwise, apply fall time
    else
    {
        // apply fall time and return new peak meter reading (linear
        // fall time: 26 dB in 3 seconds)
        dPeakLevelOld -= dReleaseCoefLinear;

        // make sure that meter doesn't fall below current level
        if (dPeakLevelCurrent > dPeakLevelOld)
        {
            dPeakLevelOld = dPeakLevelCurrent;
        }
    }

    // peak marker: limit peak level
    if (dPeakLevelCurrent >= 0.0)
    {
        // immediate rise time, but limit current peak to top mark
        dPeakMarkOld = 0.0;

        // reset hold time
        dPeakHoldTime = 0.0;
    }
    // peak marker: apply rise time if peak level is above old level
    else if (dPeakLevelCurrent >= dPeakMarkOld)
    {
        // immediate rise time, so return current peak level as new
        // peak meter reading
        dPeakMarkOld = dPeakLevelCurrent;

        // reset hold time
        dPeakHoldTime = 0.0;
    }
    // peak marker: otherwise, apply fall time
    else
    {
        // hold peaks for 10 seconds
        if (dPeakHoldTime < 10.0)
        {
            // update hold time
            dPeakHoldTime += dBufferLength;
        }
        // hold time exceeded
        else
        {
            // apply fall time and return new peak reading (linear
            // fall time: 26 dB in 3 seconds)
            dPeakMarkOld -= dReleaseCoefLinear;

            // make sure that meter doesn't fall below current level
            if (dPeakLevelCurrent > dPeakMarkOld)
            {
                dPeakMarkOld = dPeakLevelCurrent;
            }
        }
    }
}


void Compressor::AverageMeterBallistics(double dAverageLevelCurrent, double &dAverageLevelOld)
/*  Calculate ballistics for average meter levels and update readout.

    dAverageLevelCurrent (double): current average meter level in
    decibel

    dAverageLevelOld (double): old average meter reading in decibel
    (will be overwritten!)

    return value: none
*/
{
    // the meter reaches 99% of the final reading in 300 ms
    // (logarithmic)
    LogMeterBallistics(0.300, dBufferLength, dAverageLevelCurrent, dAverageLevelOld);
}


void Compressor::GainReductionMeterPeakBallistics(double dGainReductionPeakCurrent, double &dGainReductionPeakOld, double &dGainReductionHoldTime)
/*  Calculate ballistics for peak gain reduction.

    dGainReductionPeakCurrent (double): current peak gain reduction in
    decibel

    dGainReductionPeakOld (double): old peak gain reduction in decibel
    (will be overwritten!)

    dGainReductionHoldTime (double): time since last meter update

    return value: none
*/
{
    // apply rise time if peak level is above old level
    if (dGainReductionPeakCurrent >= dGainReductionPeakOld)
    {
        // immediate rise time, so return current peak level as new
        // gain reduction peak reading
        dGainReductionPeakOld = dGainReductionPeakCurrent;

        // reset hold time
        dGainReductionHoldTime = 0.0;
    }
    // otherwise, apply fall time
    else
    {
        // hold gain reduction meter for 10 seconds
        if (dGainReductionHoldTime < 10.0)
        {
            // update hold time
            dGainReductionHoldTime += dBufferLength;
        }
        // hold time exceeded
        else
        {
            // apply fall time and return new gain reduction peak
            // reading (linear fall time: 26 dB in 3 seconds)
            dGainReductionPeakOld -= dReleaseCoefLinear;

            // make sure that meter doesn't fall below current level
            if (dGainReductionPeakCurrent > dGainReductionPeakOld)
            {
                dGainReductionPeakOld = dGainReductionPeakCurrent;
            }
        }
    }
}


void Compressor::LogMeterBallistics(double dMeterInertia, double dTimePassed, double dLevel, double &dReadout)
/*  Calculate logarithmic meter ballistics.

    dMeterInertia (double): time needed to reach 99% of the final
    readout (in fractional seconds)

    dTimePassed (double): time that has passed since last update (in
    fractional seconds)

    dLevel (double): new meter level

    dReadout (reference to double): old meter readout; this variable
    will be updated by this function

    return value: none
*/
{
    // we only have to calculate meter ballistics if meter level and
    // meter readout are not equal
    if (dLevel != dReadout)
    {
        // Thanks to Bram de Jong for the code snippet!
        // (http://www.musicdsp.org/showone.php?id=136)
        //
        // rise and fall: 99% of final reading in "fMeterInertia" seconds
        double dAttackReleaseCoef = pow(0.01, dTimePassed / dMeterInertia);
        dReadout = dAttackReleaseCoef * (dReadout - dLevel) + dLevel;
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
