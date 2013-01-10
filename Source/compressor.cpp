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
    bBypassCompressor = false;
    bDesignModern = false;

    fInputGain = 1.0f;
    fOutputGain = 1.0f;

    pGainReducer = new GainReducer*[nChannels];

    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        pGainReducer[nChannel] = new GainReducer(sample_rate);
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


float Compressor::getGainReduction(int nChannel, bool useGainCompensation)
/*  Get current gain reduction.

    nChannel (integer): queried audio channel

    useGainCompensation (boolean): determines whether the gain
    reduction should be level-compensated or not

    return value (float): returns the current gain reduction in
    decibel
 */
{
    jassert((nChannel == 1) || (nChannel == 2));

    return pGainReducer[nChannel]->getGainReduction(useGainCompensation);
}


void Compressor::processBlock(AudioSampleBuffer& buffer)
{
    int nNumSamples = buffer.getNumSamples();

    for (int nSample = 0; nSample < nNumSamples; nSample++)
    {
        if (bBypassCompressor)
        {
            continue;
        }

        for (int nChannel = 0; nChannel < nChannels; nChannel++)
        {
            // get current input sample
            float fInputSample = *buffer.getSampleData(nChannel, nSample);
            float fOutputSample;

            // apply input gain
            if (fInputGain != 1.0f)
            {
                fInputSample = fInputSample * fInputGain;
            }

            // "modern" (feed-forward) design
            if (bDesignModern)
            {
                // send current input sample to gain reduction unit
                float fInputLevel = GainReducer::level2decibel(fabs(fInputSample)) + fCrestFactor;
                pGainReducer[nChannel]->processSample(fInputLevel);

                // apply gain reduction to current input sample
                float fGainReduction = pGainReducer[nChannel]->getGainReduction(true);
                fOutputSample = fInputSample * GainReducer::decibel2level(fGainReduction);
            }
            // "vintage" (feed-back) design
            else
            {
                // get "old" gain reduction and apply it to current
                // input sample
                float fGainReduction = pGainReducer[nChannel]->getGainReduction(true);
                fOutputSample = fInputSample * GainReducer::decibel2level(fGainReduction);

                // feed current output sample back to gain reduction unit
                float fOutputLevel = GainReducer::level2decibel(fabs(fOutputSample)) + fCrestFactor;
                pGainReducer[nChannel]->processSample(fOutputLevel);
            }

            // apply output gain
            fOutputSample = fOutputSample * fOutputGain;

            // save current output sample
            buffer.copyFrom(nChannel, nSample, &fOutputSample, 1);
        }

        if (nSample % 2000 == 0)
        {
            String strTemp;

            for (int i = -3 * pGainReducer[0]->getGainReduction(false); i > 0; i--)
            {
                strTemp += "**";
            }

            DBG(strTemp);
        }
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
