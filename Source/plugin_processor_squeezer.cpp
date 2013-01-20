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

#include "plugin_processor_squeezer.h"
#include "plugin_editor_squeezer.h"


//==============================================================================

SqueezerAudioProcessor::SqueezerAudioProcessor()
{
    bSampleRateIsValid = false;
    nNumInputChannels = 0;

    pCompressor = NULL;

    setLatencySamples(0);
    pPluginParameters = new SqueezerPluginParameters();
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


const String SqueezerAudioProcessor::getParameterName(int nIndex)
{
    return pPluginParameters->getName(nIndex);
}


const String SqueezerAudioProcessor::getParameterText(int nIndex)
{
    return pPluginParameters->getText(nIndex);
}


float SqueezerAudioProcessor::getParameter(int nIndex)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    return pPluginParameters->getFloat(nIndex);
}


void SqueezerAudioProcessor::setParameter(int nIndex, float fNewValue)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    // Please use this method for non-automatable values only!

    pPluginParameters->setFloat(nIndex, fNewValue);

    if (pCompressor)
    {
        switch (nIndex)
        {
        case SqueezerPluginParameters::selBypassSwitch:
        {
            bool bBypassCompressor = pPluginParameters->getBoolean(nIndex);
            pCompressor->setBypass(bBypassCompressor);
            break;
        }

        case SqueezerPluginParameters::selDesignSwitch:
        {
            int nDesign = pPluginParameters->getRealInteger(nIndex);
            pCompressor->setDesign(nDesign);
            break;
        }

        case SqueezerPluginParameters::selThresholdSwitch:
        {
            float fThreshold = pPluginParameters->getRealFloat(nIndex);
            pCompressor->setThreshold(fThreshold);
            break;
        }

        case SqueezerPluginParameters::selRatioSwitch:
        {
            float fRatio = pPluginParameters->getRealFloat(nIndex);
            pCompressor->setRatio(fRatio);
            break;
        }

        case SqueezerPluginParameters::selAttackRateCombined:
        {
            int nAttackRate = pPluginParameters->getRealInteger(nIndex);
            pCompressor->setAttackRate(nAttackRate);
            break;
        }

        case SqueezerPluginParameters::selReleaseRateCombined:
        {
            int nReleaseRate = pPluginParameters->getRealInteger(nIndex);
            pCompressor->setReleaseRate(nReleaseRate);
            break;
        }

        case SqueezerPluginParameters::selAttackModeSwitch:
        {
            bool bLogarithmicAttack = pPluginParameters->getBoolean(nIndex);
            pCompressor->setLogarithmicAttack(bLogarithmicAttack);
            break;
        }

        case SqueezerPluginParameters::selReleaseModeSwitch:
        {
            bool bLogarithmicRelease = pPluginParameters->getBoolean(nIndex);
            pCompressor->setLogarithmicRelease(bLogarithmicRelease);
            break;
        }

        case SqueezerPluginParameters::selStereoLinkSwitch:
        {
            int nStereoLink = pPluginParameters->getRealInteger(nIndex);
            pCompressor->setStereoLink(nStereoLink);
            break;
        }

        case SqueezerPluginParameters::selInputGainSwitch:
        {
            float fInputGain = pPluginParameters->getRealFloat(nIndex);
            pCompressor->setInputGain(fInputGain);
            break;
        }

        case SqueezerPluginParameters::selOutputGainSwitch:
        {
            float fOutputGain = pPluginParameters->getRealFloat(nIndex);
            pCompressor->setOutputGain(fOutputGain);
            break;
        }

        case SqueezerPluginParameters::selWetMixSwitch:
        {
            int nWetMix = pPluginParameters->getRealInteger(nIndex);
            pCompressor->setWetMix(nWetMix);
            break;
        }
        }
    }
}


void SqueezerAudioProcessor::changeParameter(int nIndex, float fNewValue)
{
    setParameter(nIndex, fNewValue);

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
            float fNewValue = pPluginParameters->getFloat(nIndex);
            changeParameter(nIndex, fNewValue);
        }
    }
}


float SqueezerAudioProcessor::getGainReduction(int nChannel)
{
    return pCompressor->getGainReduction(nChannel);
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
    DBG("[Squeezer] number of input channels: " + String(nNumInputChannels));

    bool bBypassCompressor = pPluginParameters->getBoolean(SqueezerPluginParameters::selBypassSwitch);

    int nDesign = pPluginParameters->getRealInteger(SqueezerPluginParameters::selDesignSwitch);

    float fThreshold = pPluginParameters->getRealFloat(SqueezerPluginParameters::selThresholdSwitch);
    float fRatio = pPluginParameters->getRealFloat(SqueezerPluginParameters::selRatioSwitch);

    int nAttackRate = pPluginParameters->getRealInteger(SqueezerPluginParameters::selAttackRateCombined);
    int nReleaseRate = pPluginParameters->getRealInteger(SqueezerPluginParameters::selReleaseRateCombined);

    bool bLogarithmicAttack = pPluginParameters->getBoolean(SqueezerPluginParameters::selAttackModeSwitch);
    bool bLogarithmicRelease = pPluginParameters->getBoolean(SqueezerPluginParameters::selReleaseModeSwitch);

    int nStereoLink = pPluginParameters->getRealInteger(SqueezerPluginParameters::selStereoLinkSwitch);

    float fInputGain = pPluginParameters->getRealFloat(SqueezerPluginParameters::selInputGainSwitch);
    float fOutputGain = pPluginParameters->getRealFloat(SqueezerPluginParameters::selOutputGainSwitch);
    int nWetMix = pPluginParameters->getRealInteger(SqueezerPluginParameters::selWetMixSwitch);

    pCompressor = new Compressor(nNumInputChannels, (int) sampleRate);

    pCompressor->setBypass(bBypassCompressor);
    pCompressor->setDesign(nDesign);

    pCompressor->setThreshold(fThreshold);
    pCompressor->setRatio(fRatio);

    pCompressor->setAttackRate(nAttackRate);
    pCompressor->setReleaseRate(nReleaseRate);

    pCompressor->setLogarithmicAttack(bLogarithmicAttack);
    pCompressor->setLogarithmicRelease(bLogarithmicRelease);

    pCompressor->setStereoLink(nStereoLink);

    pCompressor->setInputGain(fInputGain);
    pCompressor->setOutputGain(fOutputGain);
    pCompressor->setWetMix(nWetMix);
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

    if (pCompressor != NULL)
    {
        delete pCompressor;
        pCompressor = NULL;
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

    pCompressor->processBlock(buffer);

    // "UM" --> update meters
    sendActionMessage("UM");
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
