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

#include "compressor.h"


//==============================================================================

Compressor::Compressor(int channels, int sample_rate)
{
    jassert((channels == 1) || (channels == 2));

    nChannels = channels;
    fCrestFactor = 20.0f;

    pGainReducer = NULL;

    setBypass(false);
    bDesignModern = true;

    setStereoLink(100);

    setInputGain(1.0f);
    setOutputGain(1.0f);

    pGainReducer = new GainReducer*[nChannels];

    pInputSamples = new float[nChannels];
    pOutputSamples = new float[nChannels];

    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        pGainReducer[nChannel] = new GainReducer(sample_rate);

        pInputSamples[nChannel] = 0.0f;
        pOutputSamples[nChannel] = 0.0f;
    }
}


Compressor::~Compressor()
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        delete pGainReducer[nChannel];
        pGainReducer[nChannel] = NULL;
    }

    delete[] pGainReducer;
    pGainReducer = NULL;

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
}


int Compressor::getDesign()
/*  Get current compressor design.

    return value (integer): returns the current compressor design
 */
{
    return pGainReducer[0]->getDesign();
}


void Compressor::setDesign(int nDesignNew)
/*  Set new compressor design.

    nDesignNew (integer): new compressor design

    return value: none
 */
{
    bDesignModern = (nDesignNew == SqueezerPluginParameters::selDesignModern);

    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        pGainReducer[nChannel]->setDesign(nDesignNew);
    }
}


int Compressor::getSensor()
/*  Get current compressor sensor.

    return value (integer): returns the current compressor sensor
 */
{
    return pGainReducer[0]->getSensor();
}


void Compressor::setSensor(int nSensorNew)
/*  Set new compressor sensor.

    nSensorNew (integer): new compressor sensor

    return value: none
 */
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        pGainReducer[nChannel]->setSensor(nSensorNew);
    }
}


float Compressor::getThreshold()
/*  Get current threshold.

    return value (float): returns the current threshold in decibels
 */
{
    return pGainReducer[0]->getThreshold();
}


void Compressor::setThreshold(float fThresholdNew)
/*  Set new threshold.

    fThresholdNew (float): new threshold in decibels

    return value: none
 */
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        pGainReducer[nChannel]->setThreshold(fThresholdNew);
    }
}


float Compressor::getRatio()
/*  Get current compression ratio.

    return value (float): returns the current compression ratio
 */
{
    return pGainReducer[0]->getRatio();
}


void Compressor::setRatio(float fRatioNew)
/*  Set new compression ratio.

    nRatioNew (float): new compression ratio

    return value: none
 */
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        pGainReducer[nChannel]->setRatio(fRatioNew);
    }
}


int Compressor::getAttackRate()
/*  Get current attack rate.

    return value (integer): returns the current attack rate in
    milliseconds
 */
{
    return pGainReducer[0]->getAttackRate();
}


void Compressor::setAttackRate(int nAttackRateNew)
/*  Set new attack rate.

    nAttackRateNew (integer): new attack rate in milliseconds

    return value: none
 */
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        pGainReducer[nChannel]->setAttackRate(nAttackRateNew);
    }
}


int Compressor::getReleaseRate()
/*  Get current release rate.

    return value (integer): returns the current release rate in
    milliseconds
 */
{
    return pGainReducer[0]->getReleaseRate();
}


void Compressor::setReleaseRate(int nReleaseRateNew)
/*  Set new release rate.

    nReleaseRateNew (integer): new release rate in milliseconds

    return value: none
 */
{
    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        pGainReducer[nChannel]->setReleaseRate(nReleaseRateNew);
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


float Compressor::getInputGain()
/*  Get current input gain.

    return value (float): returns the current input gain
 */
{
    return fInputGain;
}


void Compressor::setInputGain(float fInputGainNew)
/*  Set new input gain.

    nInputGainNew (float): new input gain

    return value: none
 */
{
    fInputGain = fInputGainNew;
}


float Compressor::getOutputGain()
/*  Get current output gain.

    return value (float): returns the current output gain
 */
{
    return fOutputGain;
}


void Compressor::setOutputGain(float fOutputGainNew)
/*  Set new output gain.

    nOutputGainNew (float): new output gain

    return value: none
 */
{
    fOutputGain = fOutputGainNew;
}


float Compressor::getGainReduction(int nChannel)
/*  Get current gain reduction.

    nChannel (integer): queried audio channel

    return value (float): returns the current gain reduction in
    decibel
 */
{
    jassert((nChannel >= 0) && (nChannel < nChannels));

    return pGainReducer[nChannel]->getGainReduction(false);
}


void Compressor::processBlock(AudioSampleBuffer& buffer)
{
    int nNumSamples = buffer.getNumSamples();

    // loop over input buffer samples
    for (int nSample = 0; nSample < nNumSamples; nSample++)
    {
        if (bBypassCompressor)
        {
            continue;
        }

        // get and prepare input samples (all channels have to be
        // prepared before any processing can take place!)
        for (int nChannel = 0; nChannel < nChannels; nChannel++)
        {
            // get current input sample
            pInputSamples[nChannel] = *buffer.getSampleData(nChannel, nSample);

            // apply input gain
            if (fInputGain != 1.0f)
            {
                pInputSamples[nChannel] *= fInputGain;
            }
        }

        // compress channels
        for (int nChannel = 0; nChannel < nChannels; nChannel++)
        {
            // "modern" (feed-forward) design
            if (bDesignModern)
            {
                float fInputLevel;

                // stereo linking is off (save some processing time)
                if (fStereoLinkOriginal == 0.0f)
                {
                    fInputLevel = GainReducer::level2decibel(fabs(pInputSamples[nChannel]));
                }
                // stereo linking is on
                else
                {
                    // get ID of other channel
                    int nChannelOther = (nChannel == 0) ? 1 : 0;

                    // mix stereo input samples according to stereo
                    // link percentage
                    fInputLevel = GainReducer::level2decibel(fabs(pInputSamples[nChannel]) * fStereoLinkOriginal + fabs(pInputSamples[nChannelOther]) * fStereoLinkOther);
                }

                // apply crest factor
                fInputLevel += fCrestFactor;

                // send current input sample to gain reduction unit
                pGainReducer[nChannel]->processSample(fInputLevel);
            }

            // apply gain reduction to current input sample
            //
            //  "modern" (feed-forward) design:  current gain reduction
            //  "vintage" (feed-back) design:  "old" gain reduction
            float fGainReduction = pGainReducer[nChannel]->getGainReduction(true);
            pOutputSamples[nChannel] = pInputSamples[nChannel] / GainReducer::decibel2level(fGainReduction);

            // apply output gain
            pOutputSamples[nChannel] *= fOutputGain;

            // save current output sample
            buffer.copyFrom(nChannel, nSample, &pOutputSamples[nChannel], 1);
        }

        // post-processing for "vintage" (feed-back) design
        if (!bDesignModern)
        {
            float fOutputLevel;

            // loop over channels
            for (int nChannel = 0; nChannel < nChannels; nChannel++)
            {
                // stereo linking is off (save some processing time)
                if (fStereoLinkOriginal == 0.0f)
                {
                    fOutputLevel = GainReducer::level2decibel(fabs(pOutputSamples[nChannel]));
                }
                // stereo linking is on
                else
                {
                    // get ID of other channel
                    int nChannelOther = (nChannel == 0) ? 1 : 0;

                    // mix stereo output samples according to stereo
                    // link percentage
                    fOutputLevel = GainReducer::level2decibel(fabs(pOutputSamples[nChannel]) * fStereoLinkOriginal + fabs(pOutputSamples[nChannelOther]) * fStereoLinkOther);
                }

                // apply crest factor
                fOutputLevel += fCrestFactor;

                // feed current output sample back to gain reduction unit
                pGainReducer[nChannel]->processSample(fOutputLevel);
            }
        }
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
