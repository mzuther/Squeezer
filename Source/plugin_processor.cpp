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

#include "plugin_processor.h"
#include "plugin_editor.h"


/*==============================================================================

Flow of parameter processing:

  Editor:      buttonClicked(button) / sliderValueChanged(slider)
  Processor:   changeParameter(nIndex, fValue)
  Processor:   setParameter(nIndex, fValue)
  Parameters:  setFloat(nIndex, fValue)
  Editor:      actionListenerCallback(strMessage)
  Editor:      updateParameter(nIndex)

==============================================================================*/

SqueezerAudioProcessor::SqueezerAudioProcessor()
{
    bSampleRateIsValid = false;
    nNumInputChannels = 0;

    setLatencySamples(0);
}


SqueezerAudioProcessor::~SqueezerAudioProcessor()
{
    removeAllActionListeners();

    // call function "releaseResources()" by force to make sure all
    // allocated memory is freed
    releaseResources();
}


//==============================================================================

const String SqueezerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}


int SqueezerAudioProcessor::getNumParameters()
{
    return pluginParameters.getNumParameters(false);
}


const String SqueezerAudioProcessor::getParameterName(int nIndex)
{
    return pluginParameters.getName(nIndex);
}


const String SqueezerAudioProcessor::getParameterText(int nIndex)
{
    return pluginParameters.getText(nIndex);
}


String SqueezerAudioProcessor::getParameters()
{
    return pluginParameters.toString();
}


float SqueezerAudioProcessor::getParameter(int nIndex)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    return pluginParameters.getFloat(nIndex);
}


void SqueezerAudioProcessor::changeParameter(int nIndex, float fValue)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    // notify host of parameter change (this will automatically call
    // "setParameter"!)
    beginParameterChangeGesture(nIndex);
    setParameterNotifyingHost(nIndex, fValue);
    endParameterChangeGesture(nIndex);
}


void SqueezerAudioProcessor::setParameter(int nIndex, float fValue)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    // Please only call this method directly for non-automatable
    // values!

    // update signal processing unit
    switch (nIndex)
    {
    case SqueezerPluginParameters::selBypass:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            bool bBypassCompressor = pluginParameters.getBoolean(nIndex);
            pCompressor->setBypass(bBypassCompressor);
        }

        break;

    case SqueezerPluginParameters::selDetectorRmsFilter:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            float fDetectorRateMilliSeconds = pluginParameters.getRealFloat(nIndex);
            pCompressor->setDetectorRmsFilter(fDetectorRateMilliSeconds);
        }

        break;

    case SqueezerPluginParameters::selDesign:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            int nDesign = pluginParameters.getRealInteger(nIndex);
            pCompressor->setDesign(nDesign);
        }

        break;

    case SqueezerPluginParameters::selDetector:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            int nDetector = pluginParameters.getRealInteger(nIndex);
            pCompressor->setDetector(nDetector);
        }

        break;

    case SqueezerPluginParameters::selGainStage:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            int nGainStage = pluginParameters.getRealInteger(nIndex);
            pCompressor->setGainStage(nGainStage);
        }

        break;

    case SqueezerPluginParameters::selThreshold:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            float fThreshold = pluginParameters.getRealFloat(nIndex);
            pCompressor->setThreshold(fThreshold);
        }

        break;

    case SqueezerPluginParameters::selRatio:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            float fRatio = pluginParameters.getRealFloat(nIndex);
            pCompressor->setRatio(fRatio);
        }

        break;

    case SqueezerPluginParameters::selKneeWidth:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            float fKneeWidth = pluginParameters.getRealFloat(nIndex);
            pCompressor->setKneeWidth(fKneeWidth);
        }

        break;

    case SqueezerPluginParameters::selAttackRate:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            int nAttackRate = pluginParameters.getRealInteger(nIndex);
            pCompressor->setAttackRate(nAttackRate);
        }

        break;

    case SqueezerPluginParameters::selReleaseRate:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            int nReleaseRate = pluginParameters.getRealInteger(nIndex);
            pCompressor->setReleaseRate(nReleaseRate);
        }

        break;

#ifdef SQUEEZER_STEREO

    case SqueezerPluginParameters::selStereoLink:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            int nStereoLink = pluginParameters.getRealInteger(nIndex);
            pCompressor->setStereoLink(nStereoLink);
        }

        break;
#endif

    case SqueezerPluginParameters::selAutoMakeupGain:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            bool bAutoMakeupGain = pluginParameters.getBoolean(nIndex);
            pCompressor->setAutoMakeupGain(bAutoMakeupGain);
        }

        break;

    case SqueezerPluginParameters::selMakeupGain:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            float fMakeupGain = pluginParameters.getRealFloat(nIndex);
            pCompressor->setMakeupGain(fMakeupGain);
        }

        break;

    case SqueezerPluginParameters::selWetMix:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            int nWetMix = pluginParameters.getRealInteger(nIndex);
            pCompressor->setWetMix(nWetMix);
        }

        break;

    case SqueezerPluginParameters::selSidechainFilterState:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            bool bSidechainFilterState = pluginParameters.getBoolean(nIndex);
            pCompressor->setSidechainFilterState(bSidechainFilterState);
        }

        break;

    case SqueezerPluginParameters::selSidechainFilterCutoff:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            int nSidechainFilterCutoff = pluginParameters.getRealInteger(nIndex);
            pCompressor->setSidechainFilterCutoff(nSidechainFilterCutoff);
        }

        break;

    case SqueezerPluginParameters::selSidechainFilterGain:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            float fSidechainFilterGain = pluginParameters.getRealFloat(nIndex);
            pCompressor->setSidechainFilterGain(fSidechainFilterGain);
        }

        break;

    case SqueezerPluginParameters::selSidechainListen:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            bool bSidechainListen = pluginParameters.getBoolean(nIndex);
            pCompressor->setSidechainListen(bSidechainListen);
        }

        break;

    default:
    {
        WrappedParameterCombined *pCombined = dynamic_cast<WrappedParameterCombined *>(pluginParameters.getWrappedParameter(nIndex + 1));

        if (pCombined)
        {
            switch (nIndex)
            {
            case SqueezerPluginParameters::selThresholdSwitch:
            case SqueezerPluginParameters::selRatioSwitch:
            case SqueezerPluginParameters::selKneeWidthSwitch:

            case SqueezerPluginParameters::selAttackRateSwitch:
            case SqueezerPluginParameters::selReleaseRateSwitch:

#ifdef SQUEEZER_STEREO
            case SqueezerPluginParameters::selStereoLinkSwitch:
#endif

            case SqueezerPluginParameters::selMakeupGainSwitch:
            case SqueezerPluginParameters::selWetMixSwitch:

            case SqueezerPluginParameters::selSidechainFilterCutoffSwitch:

                pCombined->setMode(fValue != 0.0f);
                break;

            default:
                DBG("[Squeezer] processor::setParameter --> invalid combined index");
                break;
            }
        }
        else
        {
            DBG("[Squeezer] processor::setParameter --> invalid index");
        }
    }
    break;
    }

    // notify plug-in editor of parameter change
    if (pluginParameters.hasChanged(nIndex))
    {
        // for visible parameters, notify the editor of changes (this
        // will also clear the change flag)
        if (nIndex < pluginParameters.getNumParameters(false))
        {
            // "PC" --> parameter changed, followed by a hash and the
            // parameter's ID
            sendActionMessage("PC#" + String(nIndex));
        }
        // for hidden parameters, we only have to clear the change
        // flag
        else
        {
            pluginParameters.clearChangeFlag(nIndex);
        }
    }
}


void SqueezerAudioProcessor::clearChangeFlag(int nIndex)
{
    pluginParameters.clearChangeFlag(nIndex);
}


void SqueezerAudioProcessor::setChangeFlag(int nIndex)
{
    pluginParameters.setChangeFlag(nIndex);
}


bool SqueezerAudioProcessor::hasChanged(int nIndex)
{
    return pluginParameters.hasChanged(nIndex);
}


void SqueezerAudioProcessor::updateParameters(bool bIncludeHiddenParameters)
{
    int nNumParameters = pluginParameters.getNumParameters(false);

    for (int nIndex = 0; nIndex < nNumParameters; nIndex++)
    {
        if (pluginParameters.hasChanged(nIndex))
        {
            float fValue = pluginParameters.getFloat(nIndex);
            changeParameter(nIndex, fValue);
        }
    }

    if (bIncludeHiddenParameters)
    {
        // handle hidden parameters here!
    }
}


void SqueezerAudioProcessor::resetMeters()
{
    if (pCompressor)
    {
        pCompressor->resetMeters();
    }

    // "UM" --> update meters
    sendActionMessage("UM");
}


float SqueezerAudioProcessor::getGainReduction(int nChannel)
{
    if (pCompressor)
    {
        return (float) pCompressor->getGainReduction(nChannel);
    }
    else
    {
        return -1.0f;
    }
}


float SqueezerAudioProcessor::getGainReductionPeak(int nChannel)
{
    if (pCompressor)
    {
        return (float) pCompressor->getGainReductionPeak(nChannel);
    }
    else
    {
        return -1.0f;
    }
}


float SqueezerAudioProcessor::getPeakMeterInputLevel(int nChannel)
{
    if (pCompressor)
    {
        return (float) pCompressor->getPeakMeterInputLevel(nChannel);
    }
    else
    {
        return -1.0f;
    }
}


float SqueezerAudioProcessor::getPeakMeterOutputLevel(int nChannel)
{
    if (pCompressor)
    {
        return (float) pCompressor->getPeakMeterOutputLevel(nChannel);
    }
    else
    {
        return -1.0f;
    }
}


float SqueezerAudioProcessor::getPeakMeterPeakInputLevel(int nChannel)
{
    if (pCompressor)
    {
        return (float) pCompressor->getPeakMeterPeakInputLevel(nChannel);
    }
    else
    {
        return -1.0f;
    }
}


float SqueezerAudioProcessor::getPeakMeterPeakOutputLevel(int nChannel)
{
    if (pCompressor)
    {
        return (float) pCompressor->getPeakMeterPeakOutputLevel(nChannel);
    }
    else
    {
        return -1.0f;
    }
}


float SqueezerAudioProcessor::getMaximumInputLevel(int nChannel)
{
    if (pCompressor)
    {
        return (float) pCompressor->getMaximumInputLevel(nChannel);
    }
    else
    {
        return -1.0f;
    }
}


float SqueezerAudioProcessor::getMaximumOutputLevel(int nChannel)
{
    if (pCompressor)
    {
        return (float) pCompressor->getMaximumOutputLevel(nChannel);
    }
    else
    {
        return -1.0f;
    }
}


float SqueezerAudioProcessor::getAverageMeterInputLevel(int nChannel)
{
    if (pCompressor)
    {
        return (float) pCompressor->getAverageMeterInputLevel(nChannel);
    }
    else
    {
        return -1.0f;
    }
}


float SqueezerAudioProcessor::getAverageMeterOutputLevel(int nChannel)
{
    if (pCompressor)
    {
        return (float) pCompressor->getAverageMeterOutputLevel(nChannel);
    }
    else
    {
        return -1.0f;
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


bool SqueezerAudioProcessor::silenceInProducesSilenceOut() const
{
    return true;
}


double SqueezerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
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


void SqueezerAudioProcessor::changeProgramName(int nIndex, const String &newName)
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

    if (nNumInputChannels < 1)
    {
        nNumInputChannels = JucePlugin_MaxNumInputChannels;
        DBG("[K-Meter] no input channels detected, correcting this");
    }

    DBG("[Squeezer] number of input channels: " + String(nNumInputChannels));

    bool bBypassCompressor = pluginParameters.getBoolean(SqueezerPluginParameters::selBypass);
    float fDetectorRateMilliSeconds = pluginParameters.getRealFloat(SqueezerPluginParameters::selDetectorRmsFilter);
    int nDesign = pluginParameters.getRealInteger(SqueezerPluginParameters::selDesign);
    int nDetector = pluginParameters.getRealInteger(SqueezerPluginParameters::selDetector);
    int nGainStage = pluginParameters.getRealInteger(SqueezerPluginParameters::selGainStage);

    float fThreshold = pluginParameters.getRealFloat(SqueezerPluginParameters::selThreshold);
    float fRatio = pluginParameters.getRealFloat(SqueezerPluginParameters::selRatio);
    float fKneeWidth = pluginParameters.getRealFloat(SqueezerPluginParameters::selKneeWidth);

    int nAttackRate = pluginParameters.getRealInteger(SqueezerPluginParameters::selAttackRate);
    int nReleaseRate = pluginParameters.getRealInteger(SqueezerPluginParameters::selReleaseRate);

#ifdef SQUEEZER_STEREO
    int nStereoLink = pluginParameters.getRealInteger(SqueezerPluginParameters::selStereoLink);
#endif

    bool bAutoMakeupGain = pluginParameters.getBoolean(SqueezerPluginParameters::selAutoMakeupGain);
    float fMakeupGain = pluginParameters.getRealFloat(SqueezerPluginParameters::selMakeupGain);
    int nWetMix = pluginParameters.getRealInteger(SqueezerPluginParameters::selWetMix);

    bool bSidechainFilterState = pluginParameters.getBoolean(SqueezerPluginParameters::selSidechainFilterState);
    int nSidechainFilterCutoff = pluginParameters.getRealInteger(SqueezerPluginParameters::selSidechainFilterCutoff);
    float fSidechainFilterGain = pluginParameters.getRealFloat(SqueezerPluginParameters::selSidechainFilterGain);
    bool bSidechainListen = pluginParameters.getBoolean(SqueezerPluginParameters::selSidechainListen);

    pCompressor = new Compressor(nNumInputChannels, (int) sampleRate);

    pCompressor->setBypass(bBypassCompressor);
    pCompressor->setDetectorRmsFilter(fDetectorRateMilliSeconds);
    pCompressor->setDesign(nDesign);
    pCompressor->setDetector(nDetector);
    pCompressor->setGainStage(nGainStage);

    pCompressor->setThreshold(fThreshold);
    pCompressor->setRatio(fRatio);
    pCompressor->setKneeWidth(fKneeWidth);

    pCompressor->setAttackRate(nAttackRate);
    pCompressor->setReleaseRate(nReleaseRate);

#ifdef SQUEEZER_STEREO
    pCompressor->setStereoLink(nStereoLink);
#endif

    pCompressor->setAutoMakeupGain(bAutoMakeupGain);
    pCompressor->setMakeupGain(fMakeupGain);
    pCompressor->setWetMix(nWetMix);

    pCompressor->setSidechainFilterState(bSidechainFilterState);
    pCompressor->setSidechainFilterCutoff(nSidechainFilterCutoff);
    pCompressor->setSidechainFilterGain(fSidechainFilterGain);
    pCompressor->setSidechainListen(bSidechainListen);
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
}


void SqueezerAudioProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages)
{
    // This is the place where you'd normally do the guts of your
    // plug-in's audio processing...

    int nNumSamples = buffer.getNumSamples();

    if (!bSampleRateIsValid)
    {
        for (int nChannel = 0; nChannel < getNumOutputChannels(); nChannel++)
        {
            buffer.clear(nChannel, 0, nNumSamples);
        }

        return;
    }

    if (nNumInputChannels < 1)
    {
        Logger::outputDebugString("[Squeezer] nNumInputChannels < 1");
        return;
    }

    // In case we have more outputs than inputs, we'll clear any
    // output channels that didn't contain input data, because these
    // aren't guaranteed to be empty -- they may contain garbage.

    for (int nChannel = nNumInputChannels; nChannel < getNumOutputChannels(); nChannel++)
    {
        buffer.clear(nChannel, 0, nNumSamples);
    }

    // compressor's output is already de-normalised
    pCompressor->processBlock(buffer);

    // "UM" --> update meters
    sendActionMessage("UM");
}


//==============================================================================

AudioProcessorEditor *SqueezerAudioProcessor::createEditor()
{
    return new SqueezerAudioProcessorEditor(this, &pluginParameters, nNumInputChannels);
}


bool SqueezerAudioProcessor::hasEditor() const
{
    return true;
}


//==============================================================================

void SqueezerAudioProcessor::getStateInformation(MemoryBlock &destData)
{
    copyXmlToBinary(pluginParameters.storeAsXml(), destData);
}


void SqueezerAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    pluginParameters.loadFromXml(xml);

    updateParameters(true);
}

//==============================================================================

// This creates new instances of the plug-in.
AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new SqueezerAudioProcessor();
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
