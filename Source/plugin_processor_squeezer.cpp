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

#include "plugin_processor_squeezer.h"
#include "plugin_editor_squeezer.h"


//==============================================================================

SqueezerAudioProcessor::SqueezerAudioProcessor()
{
    bSampleRateIsValid = false;
    fCrestFactor = 20.0f;

    nNumInputChannels = 0;
    pGainReducer = NULL;

    setLatencySamples(0);
    pPluginParameters = new SqueezerPluginParameters();

    fProcessedSeconds = 0.0f;

    bDesignModern = false;
    fInputGain = 1.0f;
    fOutputGain = 1.0f;
}


SqueezerAudioProcessor::~SqueezerAudioProcessor()
{
    removeAllActionListeners();

    // call function "releaseResources()" by force to make sure all
    // allocated memory is freed
    releaseResources();

    delete pPluginParameters;
    pPluginParameters = NULL;
}


void SqueezerAudioProcessor::addActionListenerParameters(ActionListener* listener) throw()
{
    pPluginParameters->addActionListener(listener);
}


void SqueezerAudioProcessor::removeActionListenerParameters(ActionListener* listener) throw()
{
    pPluginParameters->removeActionListener(listener);
}


//==============================================================================

const String SqueezerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}


int SqueezerAudioProcessor::getNumParameters()
{
    return pPluginParameters->getNumParameters(false);
}


float SqueezerAudioProcessor::getParameter(int nIndex)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    return pPluginParameters->getValue(nIndex);
}


void SqueezerAudioProcessor::setParameter(int nIndex, float newValue)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    // Please use this method for non-automatable values only!

    pPluginParameters->setValue(nIndex, newValue);
}


const String SqueezerAudioProcessor::getParameterName(int nIndex)
{
    return pPluginParameters->getName(nIndex);
}


const String SqueezerAudioProcessor::getParameterText(int nIndex)
{
    return pPluginParameters->getText(nIndex);
}


void SqueezerAudioProcessor::changeParameter(int nIndex, float fNewValue)
{
    pPluginParameters->setValue(nIndex, fNewValue);

    if (pGainReducer)
    {
        float fRealValue = pPluginParameters->getRealValue(nIndex);

        switch (nIndex)
        {
        case SqueezerPluginParameters::selDesignSwitch:
        {
            int nDesign = roundf(fRealValue);
            bDesignModern = (nDesign == SqueezerPluginParameters::selDesignModern);

            for (int nChannel = 0; nChannel < nNumInputChannels; nChannel++)
            {
                pGainReducer[nChannel]->setDesign(nDesign);
            }
        }
        break;

        case SqueezerPluginParameters::selSensorSwitch:
        {
            int nSensor = roundf(fRealValue);

            for (int nChannel = 0; nChannel < nNumInputChannels; nChannel++)
            {
                pGainReducer[nChannel]->setSensor(nSensor);
            }
        }
        break;

        case SqueezerPluginParameters::selThresholdSwitch:

            for (int nChannel = 0; nChannel < nNumInputChannels; nChannel++)
            {
                pGainReducer[nChannel]->setThreshold(fRealValue);
            }

            break;

        case SqueezerPluginParameters::selRatioSwitch:

            for (int nChannel = 0; nChannel < nNumInputChannels; nChannel++)
            {
                pGainReducer[nChannel]->setRatio(fRealValue);
            }

            break;

        case SqueezerPluginParameters::selAttackRateSwitch:

            for (int nChannel = 0; nChannel < nNumInputChannels; nChannel++)
            {
                pGainReducer[nChannel]->setAttackRate(fRealValue);
            }

            break;

        case SqueezerPluginParameters::selReleaseRateSwitch:

            for (int nChannel = 0; nChannel < nNumInputChannels; nChannel++)
            {
                pGainReducer[nChannel]->setReleaseRate(fRealValue);
            }

            break;

        case SqueezerPluginParameters::selInputGainSwitch:
            fInputGain = GainReducer::decibel2level(fRealValue);
            break;

        case SqueezerPluginParameters::selOutputGainSwitch:
            fOutputGain = GainReducer::decibel2level(fRealValue);
            break;
        }
    }

    beginParameterChangeGesture(nIndex);
    setParameterNotifyingHost(nIndex, fNewValue);
    endParameterChangeGesture(nIndex);
}


void SqueezerAudioProcessor::clearChangeFlag(int nIndex)
{
    pPluginParameters->clearChangeFlag(nIndex);
}


void SqueezerAudioProcessor::setChangeFlag(int nIndex)
{
    pPluginParameters->setChangeFlag(nIndex);
}


bool SqueezerAudioProcessor::hasChanged(int nIndex)
{
    return pPluginParameters->hasChanged(nIndex);
}


void SqueezerAudioProcessor::updateParameters()
{
    int nNumParameters = pPluginParameters->getNumParameters(true);

    for (int nIndex = 0; nIndex < nNumParameters; nIndex++)
    {
        if (pPluginParameters->hasChanged(nIndex))
        {
            float fNewValue = pPluginParameters->getValue(nIndex);
            changeParameter(nIndex, fNewValue);
        }
    }
}


const String SqueezerAudioProcessor::getInputChannelName(int channelIndex) const
{
    return "Input " + String(channelIndex + 1);
}


const String SqueezerAudioProcessor::getOutputChannelName(int channelIndex) const
{
    return "Output " + String(channelIndex + 1);
}


bool SqueezerAudioProcessor::isInputChannelStereoPair(int nIndex) const
{
    return true;
}


bool SqueezerAudioProcessor::isOutputChannelStereoPair(int nIndex) const
{
    return true;
}


bool SqueezerAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}


bool SqueezerAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}


int SqueezerAudioProcessor::getNumChannels()
{
    return nNumInputChannels;
}


int SqueezerAudioProcessor::getNumPrograms()
{
    return 0;
}


int SqueezerAudioProcessor::getCurrentProgram()
{
    return 0;
}


void SqueezerAudioProcessor::setCurrentProgram(int nIndex)
{
}


const String SqueezerAudioProcessor::getProgramName(int nIndex)
{
    return String::empty;
}


void SqueezerAudioProcessor::changeProgramName(int nIndex, const String& newName)
{
}

//==============================================================================

void SqueezerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    DBG("[Squeezer] in method SqueezerAudioProcessor::prepareToPlay()");

    if ((sampleRate < 44100) || (sampleRate > 192000))
    {
        Logger::outputDebugString("[Squeezer] WARNING: sample rate of " + String(sampleRate) + " Hz not supported");
        bSampleRateIsValid = false;
        return;
    }
    else
    {
        bSampleRateIsValid = true;
    }

    nNumInputChannels = getNumInputChannels();
    isStereo = (nNumInputChannels == 2);

    DBG("[Squeezer] number of input channels: " + String(nNumInputChannels));

    int nDesign = roundf(pPluginParameters->getRealValue(SqueezerPluginParameters::selDesignSwitch));
    int nSensor = roundf(pPluginParameters->getRealValue(SqueezerPluginParameters::selSensorSwitch));

    float fThreshold = pPluginParameters->getRealValue(SqueezerPluginParameters::selThresholdSwitch);
    float fRatio = pPluginParameters->getRealValue(SqueezerPluginParameters::selRatioSwitch);

    float fAttackRate = pPluginParameters->getRealValue(SqueezerPluginParameters::selAttackRateSwitch);
    float fReleaseRate = pPluginParameters->getRealValue(SqueezerPluginParameters::selReleaseRateSwitch);

    pGainReducer = new GainReducer*[nNumInputChannels];

    for (int nChannel = 0; nChannel < nNumInputChannels; nChannel++)
    {
        pGainReducer[nChannel] = new GainReducer((int) sampleRate);

        pGainReducer[nChannel]->setDesign(nDesign);
        pGainReducer[nChannel]->setSensor(nSensor);

        pGainReducer[nChannel]->setThreshold(fThreshold);
        pGainReducer[nChannel]->setRatio(fRatio);

        pGainReducer[nChannel]->setAttackRate(fAttackRate);
        pGainReducer[nChannel]->setReleaseRate(fReleaseRate);
    }

    nSamplesInBuffer = 0;
}


void SqueezerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free
    // up any spare memory, etc.

    DBG("[Squeezer] in method SqueezerAudioProcessor::releaseResources()");

    if (!bSampleRateIsValid)
    {
        return;
    }

    if (pGainReducer != NULL)
    {
        for (int nChannel = 0; nChannel < nNumInputChannels; nChannel++)
        {
            delete pGainReducer[nChannel];
            pGainReducer[nChannel] = NULL;
        }

        delete[] pGainReducer;
        pGainReducer = NULL;
    }
}


void SqueezerAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // This is the place where you'd normally do the guts of your
    // plug-in's audio processing...

    int nNumSamples = buffer.getNumSamples();

    if (!bSampleRateIsValid)
    {
        // In case we have more outputs than inputs, we'll clear any
        // output channels that didn't contain input data, because these
        // aren't guaranteed to be empty -- they may contain garbage.

        if (getNumOutputChannels() > nNumInputChannels)
        {
            nNumInputChannels = getNumOutputChannels();
        }

        for (int nChannel = 0; nChannel < nNumInputChannels; nChannel++)
        {
            buffer.clear(nChannel, 0, nNumSamples);
        }

        return;
    }

    if (nNumInputChannels < 1)
    {
        DBG("[Squeezer] nNumInputChannels < 1");
        return;
    }

    // In case we have more outputs than inputs, we'll clear any
    // output channels that didn't contain input data, because these
    // aren't guaranteed to be empty -- they may contain garbage.

    for (int nChannel = nNumInputChannels; nChannel < getNumOutputChannels(); nChannel++)
    {
        buffer.clear(nChannel, 0, nNumSamples);
    }

    for (int nSample = 0; nSample < nNumSamples; nSample++)
    {
        for (int nChannel = 0; nChannel < nNumInputChannels; nChannel++)
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


//==============================================================================

AudioProcessorEditor* SqueezerAudioProcessor::createEditor()
{
    return new SqueezerAudioProcessorEditor(this, pPluginParameters);
}


bool SqueezerAudioProcessor::hasEditor() const
{
    return true;
}


//==============================================================================

void SqueezerAudioProcessor::getStateInformation(MemoryBlock& destData)
{
    copyXmlToBinary(pPluginParameters->storeAsXml(), destData);
}


void SqueezerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    pPluginParameters->loadFromXml(xml);

    updateParameters();
}

//==============================================================================

// This creates new instances of the plug-in..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SqueezerAudioProcessor();
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
