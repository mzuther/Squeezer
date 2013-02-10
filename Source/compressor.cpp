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

#include "compressor.h"


//==============================================================================

Compressor::Compressor(int channels, int sample_rate)
{
    jassert((channels == 1) || (channels == 2));

    nChannels = channels;
    fCrestFactor = 20.0f;

    // sample buffer holds 50 ms worth of samples
    nMeterBufferSize = sample_rate / 20;

    pMeterInputBuffer = new AudioSampleBuffer(nChannels, nMeterBufferSize);
    pMeterOutputBuffer = new AudioSampleBuffer(nChannels, nMeterBufferSize);
    nMeterBufferPosition = 0;

    // allocate variables for meter levels (all audio input channels)
    fPeakMeterInputLevels = new float[nChannels];
    fPeakMeterOutputLevels = new float[nChannels];

    fAverageMeterInputLevels = new float[nChannels];
    fAverageMeterOutputLevels = new float[nChannels];

    float fMeterMinimumDecibel = -(70.01f + fCrestFactor);

    // loop through all audio channels
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        // set meter levels to meter minimum
        fPeakMeterInputLevels[nChannel] = fMeterMinimumDecibel;
        fPeakMeterOutputLevels[nChannel] = fMeterMinimumDecibel;

        fAverageMeterInputLevels[nChannel] = fMeterMinimumDecibel;
        fAverageMeterOutputLevels[nChannel] = fMeterMinimumDecibel;
    }

    pSideChain = NULL;

    nWetMix = 100;
    setBypass(false);
    setDesign(Compressor::DesignFeedForward);

    setStereoLink(100);

    setAutoMakeupGain(false);
    setMakeupGain(0.0f);
    setWetMix(100);

    pSideChain = new SideChain*[nChannels];

    pInputSamples = new float[nChannels];
    pOutputSamples = new float[nChannels];

    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        pSideChain[nChannel] = new SideChain(sample_rate);

        pInputSamples[nChannel] = 0.0f;
        pOutputSamples[nChannel] = 0.0f;
    }
}


Compressor::~Compressor()
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        delete pSideChain[nChannel];
        pSideChain[nChannel] = NULL;
    }

    delete pMeterInputBuffer;
    pMeterInputBuffer = NULL;

    delete pMeterOutputBuffer;
    pMeterOutputBuffer = NULL;

    delete[] fPeakMeterInputLevels;
    fPeakMeterInputLevels = NULL;

    delete[] fPeakMeterOutputLevels;
    fPeakMeterOutputLevels = NULL;

    delete[] fAverageMeterInputLevels;
    fAverageMeterInputLevels = NULL;

    delete[] fAverageMeterOutputLevels;
    fAverageMeterOutputLevels = NULL;

    delete[] pSideChain;
    pSideChain = NULL;

    delete[] pInputSamples;
    pInputSamples = NULL;

    delete[] pOutputSamples;
    pOutputSamples = NULL;
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


int Compressor::getLevelDetection()
/*  Get current level detection type.

    return value (integer): returns current current level detection
    type
 */
{
    return pSideChain[0]->getLevelDetection();
}


void Compressor::setLevelDetection(int nLevelDetectionTypeNew)
/*  Set new level detection type.

    nLevelDetectionTypeNew (integer): new level detection type

    return value: none
 */
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        pSideChain[nChannel]->setLevelDetection(nLevelDetectionTypeNew);
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


float Compressor::getThreshold()
/*  Get current threshold.

    return value (float): returns the current threshold in decibels
 */
{
    return pSideChain[0]->getThreshold();
}


void Compressor::setThreshold(float fThresholdNew)
/*  Set new threshold.

    fThresholdNew (float): new threshold in decibels

    return value: none
 */
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        pSideChain[nChannel]->setThreshold(fThresholdNew);
    }
}


float Compressor::getRatio()
/*  Get current compression ratio.

    return value (float): returns the current compression ratio
 */
{
    return pSideChain[0]->getRatio();
}


void Compressor::setRatio(float fRatioNew)
/*  Set new compression ratio.

    nRatioNew (float): new compression ratio

    return value: none
 */
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        pSideChain[nChannel]->setRatio(fRatioNew);
    }
}


float Compressor::getKneeWidth()
/*  Get current knee width.

    return value (float): returns the current knee width in decibels
 */
{
    return pSideChain[0]->getKneeWidth();
}


void Compressor::setKneeWidth(float fKneeWidthNew)
/*  Set new knee width.

    nKneeWidthNew (float): new knee width in decibels

    return value: none
 */
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        pSideChain[nChannel]->setKneeWidth(fKneeWidthNew);
    }

}


int Compressor::getAttackRate()
/*  Get current attack rate.

    return value (integer): returns the current attack rate in
    milliseconds
 */
{
    return pSideChain[0]->getAttackRate();
}


void Compressor::setAttackRate(int nAttackRateNew)
/*  Set new attack rate.

    nAttackRateNew (integer): new attack rate in milliseconds

    return value: none
 */
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        pSideChain[nChannel]->setAttackRate(nAttackRateNew);
    }
}


int Compressor::getReleaseRate()
/*  Get current release rate.

    return value (integer): returns the current release rate in
    milliseconds
 */
{
    return pSideChain[0]->getReleaseRate();
}


void Compressor::setReleaseRate(int nReleaseRateNew)
/*  Set new release rate.

    nReleaseRateNew (integer): new release rate in milliseconds

    return value: none
 */
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        pSideChain[nChannel]->setReleaseRate(nReleaseRateNew);
    }
}


int Compressor::getDetector()
/*  Get current compressor detector type.

    return value (integer): returns compressor detector type
 */
{
    return pSideChain[0]->getDetector();
}


void Compressor::setDetector(int nDetectorTypeNew)
/*  Set new compressor detector type.

    nDetectorTypeNew (integer): new compressor detector type

    return value: none
 */
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        pSideChain[nChannel]->setDetector(nDetectorTypeNew);
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
    fStereoLinkOther = nStereoLink / 200.0f;

    // amplification factor for original channel ranging from 1.0 (no
    // stereo linking) to 0.5 (full stereo linking)
    fStereoLinkOriginal = 1.0f - fStereoLinkOther;
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


float Compressor::getMakeupGain()
/*  Get current make-up gain.

    return value (float): returns the current make-up gain in decibels
 */
{
    return fMakeupGainDecibel;
}


void Compressor::setMakeupGain(float fMakeupGainNew)
/*  Set new make-up gain.

    nMakeupGainNew (float): new make-up gain in decibels

    return value: none
 */
{
    fMakeupGainDecibel = fMakeupGainNew;
    fMakeupGain = SideChain::decibel2level(fMakeupGainDecibel);
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

    fWetMix = nWetMix / 100.0f;
    fDryMix = 1.0f - fWetMix;

    bBypassCompressorCombined = (bBypassCompressor || (nWetMix == 0));
}


float Compressor::getGainReduction(int nChannel)
/*  Get current gain reduction.

    nChannel (integer): queried audio channel

    return value (float): returns the current gain reduction in
    decibel
 */
{
    jassert((nChannel >= 0) && (nChannel < nChannels));

    if (bBypassCompressorCombined)
    {
        return 0.0f;
    }
    else
    {
        return pSideChain[nChannel]->getGainReduction(false);
    }
}


float Compressor::getPeakMeterInputLevel(int nChannel)
/*  Get current input peak level.

    nChannel (integer): selected audio channel

    return value (float): returns current input peak level in decibel
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nChannels);

    return fPeakMeterInputLevels[nChannel] + fCrestFactor;
}


float Compressor::getPeakMeterOutputLevel(int nChannel)
/*  Get current output peak level.

    nChannel (integer): selected audio channel

    return value (float): returns current output peak level in decibel
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nChannels);

    return fPeakMeterOutputLevels[nChannel] + fCrestFactor;
}


float Compressor::getAverageMeterInputLevel(int nChannel)
/*  Get current input average level.

    nChannel (integer): selected audio channel

    return value (float): returns current input average level in
    decibel
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nChannels);

    return fAverageMeterInputLevels[nChannel] + fCrestFactor;
}


float Compressor::getAverageMeterOutputLevel(int nChannel)
/*  Get current output average level.

    nChannel (integer): selected audio channel

    return value (float): returns current output average level in
    decibel
*/
{
    jassert(nChannel >= 0);
    jassert(nChannel < nChannels);

    return fAverageMeterOutputLevels[nChannel] + fCrestFactor;
}


void Compressor::processBlock(AudioSampleBuffer& buffer)
{
    int nNumSamples = buffer.getNumSamples();

    // loop over input buffer samples
    for (int nSample = 0; nSample < nNumSamples; nSample++)
    {
        if (bBypassCompressorCombined)
        {
            continue;
        }

        // get and prepare input samples (all channels have to be
        // prepared before any processing can take place!)
        for (int nChannel = 0; nChannel < nChannels; nChannel++)
        {
            // get current input sample
            pInputSamples[nChannel] = *buffer.getSampleData(nChannel, nSample);

            // store input sample for metering
            pMeterInputBuffer->copyFrom(nChannel, nMeterBufferPosition, buffer, nChannel, nSample, 1);
        }

        // compress channels
        for (int nChannel = 0; nChannel < nChannels; nChannel++)
        {
            // feed-forward design
            if (bDesignFeedForward)
            {
                float fInputLevel;

                // stereo linking is off (save some processing time)
                if (fStereoLinkOriginal == 0.0f)
                {
                    fInputLevel = SideChain::level2decibel(fabs(pInputSamples[nChannel]));
                }
                // stereo linking is on
                else
                {
                    // get ID of other channel
                    int nChannelOther = (nChannel == 0) ? 1 : 0;

                    // mix stereo input samples according to stereo
                    // link percentage
                    fInputLevel = SideChain::level2decibel(fabs(pInputSamples[nChannel]) * fStereoLinkOriginal + fabs(pInputSamples[nChannelOther]) * fStereoLinkOther);
                }

                // apply crest factor
                fInputLevel += fCrestFactor;

                // send current input sample to gain reduction unit
                pSideChain[nChannel]->processSample(fInputLevel);
            }

            // apply gain reduction to current input sample
            //
            //  feed-forward design:  current gain reduction
            //  feed-back design:     "old" gain reduction
            float fGainReduction = pSideChain[nChannel]->getGainReduction(bAutoMakeupGain);
            pOutputSamples[nChannel] = pInputSamples[nChannel] / SideChain::decibel2level(fGainReduction);

            // apply make-up gain
            pOutputSamples[nChannel] *= fMakeupGain;

            // dry shall be mixed in (test to save some processing time)
            if (nWetMix < 100)
            {
                pOutputSamples[nChannel] *= fWetMix;
                pOutputSamples[nChannel] += pInputSamples[nChannel] * fDryMix;
            }

            // save current output sample
            buffer.copyFrom(nChannel, nSample, &pOutputSamples[nChannel], 1);

            // store output sample for metering
            pMeterOutputBuffer->copyFrom(nChannel, nMeterBufferPosition, buffer, nChannel, nSample, 1);
        }

        // post-processing for feed-back design
        if (!bDesignFeedForward)
        {
            float fOutputLevel;

            // loop over channels
            for (int nChannel = 0; nChannel < nChannels; nChannel++)
            {
                // stereo linking is off (save some processing time)
                if (fStereoLinkOriginal == 0.0f)
                {
                    fOutputLevel = SideChain::level2decibel(fabs(pOutputSamples[nChannel]));
                }
                // stereo linking is on
                else
                {
                    // get ID of other channel
                    int nChannelOther = (nChannel == 0) ? 1 : 0;

                    // mix stereo output samples according to stereo
                    // link percentage
                    fOutputLevel = SideChain::level2decibel(fabs(pOutputSamples[nChannel]) * fStereoLinkOriginal + fabs(pOutputSamples[nChannelOther]) * fStereoLinkOther);
                }

                // apply crest factor
                fOutputLevel += fCrestFactor;

                // feed current output sample back to gain reduction unit
                pSideChain[nChannel]->processSample(fOutputLevel);
            }
        }

        // update metering buffer position
        nMeterBufferPosition++;

        if (nMeterBufferPosition >= nMeterBufferSize)
        {
            nMeterBufferPosition = 0;
            // update meters

            // loop over channels
            for (int nChannel = 0; nChannel < nChannels; nChannel++)
            {
                // determine peak levels
                float fInputPeak = pMeterInputBuffer->getMagnitude(nChannel, 0, nMeterBufferSize);
                float fOutputPeak = pMeterOutputBuffer->getMagnitude(nChannel, 0, nMeterBufferSize);

                // convert peak meter levels from linear scale to
                // decibels
                fInputPeak = SideChain::level2decibel(fInputPeak);
                fOutputPeak = SideChain::level2decibel(fOutputPeak);

                // apply peak meter ballistics
                PeakMeterBallistics(fInputPeak, fPeakMeterInputLevels[nChannel]);
                PeakMeterBallistics(fOutputPeak, fPeakMeterOutputLevels[nChannel]);

                // determine average levels
                float fInputRms = pMeterInputBuffer->getRMSLevel(nChannel, 0, nMeterBufferSize);
                float fOutputRms = pMeterOutputBuffer->getRMSLevel(nChannel, 0, nMeterBufferSize);

                // convert average meter levels from linear scale to
                // decibels
                fInputRms = SideChain::level2decibel(fInputRms);
                fOutputRms = SideChain::level2decibel(fOutputRms);

                // apply average meter ballistics
                AverageMeterBallistics(fInputRms, fAverageMeterInputLevels[nChannel]);
                AverageMeterBallistics(fOutputRms, fAverageMeterOutputLevels[nChannel]);
            }
        }
    }
}


void Compressor::PeakMeterBallistics(float fPeakLevelCurrent, float& fPeakLevelOld)
/*  Calculate ballistics for peak meter levels.

    fPeakLevelCurrent (float): current peak meter level in decibel

    fPeakLevelOld (float): old peak meter reading in decibel (will be
    overwritten!)

    return value: none
*/
{
    // apply rise time if peak level is above old level
    if (fPeakLevelCurrent >= fPeakLevelOld)
    {
        // immediate rise time, so return current peak level as new
        // peak meter reading
        fPeakLevelOld = fPeakLevelCurrent;
    }
    // otherwise, apply fall time
    else
    {
        // time that has passed since last update (50 ms)
        float fTimePassed = 0.050f;

        // fall time: 26 dB in 3 seconds (linear)
        float fReleaseCoef = 26.0f * fTimePassed / 3.0f;

        // apply fall time and return new peak meter reading
        fPeakLevelOld -= fReleaseCoef;

        // make sure that meter doesn't fall below current level
        if (fPeakLevelCurrent > fPeakLevelOld)
        {
            fPeakLevelOld = fPeakLevelCurrent;
        }
    }
}


void Compressor::AverageMeterBallistics(float fAverageLevelCurrent, float& fAverageLevelOld)
/*  Calculate ballistics for average meter levels and update readout.

    fAverageLevelCurrent (float): current average meter level in
    decibel

    fAverageLevelOld (float): old average meter reading in decibel
    (will be overwritten!)

    return value: none
*/
{
    // time that has passed since last update (50 ms)
    float fTimePassed = 0.050f;

    // the meter reaches 99% of the final reading in 300 ms
    // (logarithmic)
    LogMeterBallistics(0.300f, fTimePassed, fAverageLevelCurrent, fAverageLevelOld);
}


void Compressor::LogMeterBallistics(float fMeterInertia, float fTimePassed, float fLevel, float& fReadout)
/*  Calculate logarithmic meter ballistics.

    fMeterInertia (float): time needed to reach 99% of the final
    readout (in fractional seconds)

    fTimePassed (float): time that has passed since last update (in
    fractional seconds)

    fLevel (float): new meter level

    fReadout (reference to float): old meter readout; this variable
    will be updated by this function

    return value: none
*/
{
    // we only have to calculate meter ballistics if meter level and
    // meter readout are not equal
    if (fLevel != fReadout)
    {
        // Thanks to Bram from Smartelectronix for the code snippet!
        // (http://www.musicdsp.org/showone.php?id=136)
        //
        // rise and fall: 99% of final reading in "fMeterInertia" seconds
        float fAttackReleaseCoef = powf(0.01f, fTimePassed / fMeterInertia);
        fReadout = fAttackReleaseCoef * (fReadout - fLevel) + fLevel;
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
