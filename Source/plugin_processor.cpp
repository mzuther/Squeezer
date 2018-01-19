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

#ifdef SQUEEZER_MONO
#if JucePlugin_Build_VST

SqueezerAudioProcessor::SqueezerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
                     .withInput("Main / SC In",
                                AudioChannelSet::discreteChannels(2))
                     .withOutput("Main Out",
                                 AudioChannelSet::discreteChannels(1))
                     .withInput("Disabled In",
                                AudioChannelSet::disabled()))
#endif

#else

SqueezerAudioProcessor::SqueezerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
                     .withInput("Main In",
                                AudioChannelSet::mono())
                     .withOutput("Main Out",
                                 AudioChannelSet::mono())
                     .withInput("Sidechain In",
                                AudioChannelSet::mono()))
#endif

#endif
#else
#if JucePlugin_Build_VST

SqueezerAudioProcessor::SqueezerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
                     .withInput("Main / SC In",
                                AudioChannelSet::discreteChannels(4))
                     .withOutput("Main Out",
                                 AudioChannelSet::discreteChannels(2))
                     .withInput("Disabled In",
                                AudioChannelSet::disabled()))
#endif

#else

SqueezerAudioProcessor::SqueezerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
                     .withInput("Main In",
                                AudioChannelSet::stereo())
                     .withOutput("Main Out",
                                 AudioChannelSet::stereo())
                     .withInput("Sidechain In",
                                AudioChannelSet::stereo()))
#endif

#endif
#endif
{
    frut::Frut::printVersionNumbers();

    bSampleRateIsValid = false;
    hasSideChain = false;

    setLatencySamples(0);
}


SqueezerAudioProcessor::~SqueezerAudioProcessor()
{
    removeAllActionListeners();
}


//==============================================================================

#ifndef JucePlugin_PreferredChannelConfigurations
bool SqueezerAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
    // main bus: do not allow disabling channels
    if (layouts.getMainInputChannelSet().isDisabled())
    {
        return false;
    }

#ifdef SQUEEZER_MONO

    // main bus with mono output --> okay
    if (layouts.getMainOutputChannelSet() == AudioChannelSet::mono())
    {
        if (layouts.getMainInputChannelSet() == AudioChannelSet::mono())
        {
            // main bus and side chain with mono input --> okay
            if (layouts.getChannelSet(true, 1) == AudioChannelSet::mono())
            {
                return true;
            }

            // main bus with mono input and no side chain --> okay
            if (layouts.getChannelSet(true, 1) == AudioChannelSet::disabled())
            {
                return true;
            }
        }

        // main bus with two inputs and no side chain --> okay
        if (layouts.getMainInputChannelSet().size() == 2)
        {
            if (layouts.getChannelSet(true, 1) == AudioChannelSet::disabled())
            {
                return true;
            }
        }
    }
    // main bus with two outputs --> okay
    else if (layouts.getMainOutputChannelSet().size() == 2)
    {
        // main bus with two inputs --> okay
        if (layouts.getMainInputChannelSet().size() == 2)
        {
            return true;
        }
    }

#else

    // main bus with stereo output --> okay
    if (layouts.getMainOutputChannelSet() == AudioChannelSet::stereo())
    {
        if (layouts.getMainInputChannelSet() == AudioChannelSet::stereo())
        {
            // main bus and side chain with stereo input --> okay
            if (layouts.getChannelSet(true, 1) == AudioChannelSet::stereo())
            {
                return true;
            }

            // main bus with stereo input and no side chain --> okay
            if (layouts.getChannelSet(true, 1) == AudioChannelSet::disabled())
            {
                return true;
            }
        }

        // main bus with four inputs and no side chain --> okay
        if (layouts.getMainInputChannelSet().size() == 4)
        {
            if (layouts.getChannelSet(true, 1) == AudioChannelSet::disabled())
            {
                return true;
            }
        }
    }
    // main bus with four outputs --> okay
    else if (layouts.getMainOutputChannelSet().size() == 4)
    {
        // main bus with four inputs --> okay
        if (layouts.getMainInputChannelSet().size() == 4)
        {
            return true;
        }
    }

#endif

    // current channel layout is not allowed
    return false;
}
#endif


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
            float fAttackRate = pluginParameters.getRealFloat(nIndex);
            pCompressor->setAttackRate(fAttackRate);
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

    case SqueezerPluginParameters::selSidechainInput:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            bool bSidechainInput = pluginParameters.getBoolean(nIndex);
            pCompressor->setSidechainInput(bSidechainInput);
        }

        break;

    case SqueezerPluginParameters::selSidechainHPFCutoff:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            int nSidechainHPFCutoff = pluginParameters.getRealInteger(nIndex);
            pCompressor->setSidechainHPFCutoff(nSidechainHPFCutoff);
        }

        break;

    case SqueezerPluginParameters::selSidechainLPFCutoff:

        pluginParameters.setFloat(nIndex, fValue);

        if (pCompressor)
        {
            int nSidechainLPFCutoff = pluginParameters.getRealInteger(nIndex);
            pCompressor->setSidechainLPFCutoff(nSidechainLPFCutoff);
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
        frut::parameter::ParCombined *pCombined = dynamic_cast<frut::parameter::ParCombined *>(pluginParameters.getPluginParameter(nIndex + 1));

        if (pCombined)
        {
            switch (nIndex)
            {
            case SqueezerPluginParameters::selThresholdSwitch:
            case SqueezerPluginParameters::selRatioSwitch:

            case SqueezerPluginParameters::selAttackRateSwitch:
            case SqueezerPluginParameters::selReleaseRateSwitch:

            case SqueezerPluginParameters::selMakeupGainSwitch:
            case SqueezerPluginParameters::selWetMixSwitch:

            case SqueezerPluginParameters::selSidechainHPFCutoffSwitch:
            case SqueezerPluginParameters::selSidechainLPFCutoffSwitch:

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


bool SqueezerAudioProcessor::hasChanged(int nIndex)
{
    return pluginParameters.hasChanged(nIndex);
}


void SqueezerAudioProcessor::updateParameters(bool bIncludeHiddenParameters)
{
    int nNumParameters = pluginParameters.getNumParameters(false);

    for (int nIndex = 0; nIndex < nNumParameters; ++nIndex)
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

        // the following parameters need no updating:
        //
        // * selSkinName
    }
}


String SqueezerAudioProcessor::getParameterSkinName()
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    return pluginParameters.getSkinName();
}


void SqueezerAudioProcessor::setParameterSkinName(const String &strSkinName)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    pluginParameters.setSkinName(strSkinName);
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


float SqueezerAudioProcessor::getGainReductionMeterPeak(int nChannel)
{
    if (pCompressor)
    {
        return (float) pCompressor->getGainReductionMeterPeak(nChannel);
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


double SqueezerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
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
    ignoreUnused(nIndex);
}


const String SqueezerAudioProcessor::getProgramName(int nIndex)
{
    ignoreUnused(nIndex);

    return String::empty;
}


void SqueezerAudioProcessor::changeProgramName(int nIndex, const String &newName)
{
    ignoreUnused(nIndex, newName);
}


//==============================================================================

void SqueezerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    ignoreUnused(samplesPerBlock);

    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    Logger::outputDebugString("[Squeezer] preparing to play");

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

    Logger::outputDebugString("[Squeezer] number of main/aux input channels:  " + String(getMainBusNumInputChannels()) + "/" + String(getTotalNumInputChannels() - getMainBusNumInputChannels()));
    Logger::outputDebugString("[Squeezer] number of main/aux output channels: " + String(getMainBusNumOutputChannels()) + "/" + String(getTotalNumOutputChannels() - getMainBusNumOutputChannels()));

    bool bBypassCompressor = pluginParameters.getBoolean(SqueezerPluginParameters::selBypass);
    float fDetectorRateMilliSeconds = pluginParameters.getRealFloat(SqueezerPluginParameters::selDetectorRmsFilter);
    int nDesign = pluginParameters.getRealInteger(SqueezerPluginParameters::selDesign);
    int nDetector = pluginParameters.getRealInteger(SqueezerPluginParameters::selDetector);
    int nGainStage = pluginParameters.getRealInteger(SqueezerPluginParameters::selGainStage);

    float fThreshold = pluginParameters.getRealFloat(SqueezerPluginParameters::selThreshold);
    float fRatio = pluginParameters.getRealFloat(SqueezerPluginParameters::selRatio);
    float fKneeWidth = pluginParameters.getRealFloat(SqueezerPluginParameters::selKneeWidth);

    float fAttackRate = pluginParameters.getRealFloat(SqueezerPluginParameters::selAttackRate);
    int nReleaseRate = pluginParameters.getRealInteger(SqueezerPluginParameters::selReleaseRate);

    bool bAutoMakeupGain = pluginParameters.getBoolean(SqueezerPluginParameters::selAutoMakeupGain);
    float fMakeupGain = pluginParameters.getRealFloat(SqueezerPluginParameters::selMakeupGain);
    int nWetMix = pluginParameters.getRealInteger(SqueezerPluginParameters::selWetMix);

    bool bSidechainInput = pluginParameters.getBoolean(SqueezerPluginParameters::selSidechainInput);
    int nSidechainHPFCutoff = pluginParameters.getRealInteger(SqueezerPluginParameters::selSidechainHPFCutoff);
    int nSidechainLPFCutoff = pluginParameters.getRealInteger(SqueezerPluginParameters::selSidechainLPFCutoff);
    bool bSidechainListen = pluginParameters.getBoolean(SqueezerPluginParameters::selSidechainListen);

#ifdef SQUEEZER_MONO
    pCompressor = new Compressor(1, (int) sampleRate);
#else
    pCompressor = new Compressor(2, (int) sampleRate);
#endif

    pCompressor->setBypass(bBypassCompressor);
    pCompressor->setDetectorRmsFilter(fDetectorRateMilliSeconds);
    pCompressor->setDesign(nDesign);
    pCompressor->setDetector(nDetector);
    pCompressor->setGainStage(nGainStage);

    pCompressor->setThreshold(fThreshold);
    pCompressor->setRatio(fRatio);
    pCompressor->setKneeWidth(fKneeWidth);

    pCompressor->setAttackRate(fAttackRate);
    pCompressor->setReleaseRate(nReleaseRate);

    pCompressor->setAutoMakeupGain(bAutoMakeupGain);
    pCompressor->setMakeupGain(fMakeupGain);
    pCompressor->setWetMix(nWetMix);

    pCompressor->setSidechainInput(bSidechainInput);
    pCompressor->setSidechainHPFCutoff(nSidechainHPFCutoff);
    pCompressor->setSidechainLPFCutoff(nSidechainLPFCutoff);
    pCompressor->setSidechainListen(bSidechainListen);
}


void SqueezerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free
    // up any spare memory, etc.

    Logger::outputDebugString("[Squeezer] releasing resources");
    Logger::outputDebugString("");
}


void SqueezerAudioProcessor::processBlock(AudioBuffer<float> &buffer, MidiBuffer &midiMessages)
{
    ignoreUnused(midiMessages);

    // This is the place where you'd normally do the guts of your
    // plug-in's audio processing...

    int nNumSamples = buffer.getNumSamples();

    if (!bSampleRateIsValid)
    {
        for (int nChannel = 0; nChannel < getMainBusNumOutputChannels(); ++nChannel)
        {
            buffer.clear(nChannel, 0, nNumSamples);
        }

        return;
    }

    if (getMainBusNumInputChannels() < 1)
    {
        Logger::outputDebugString("[Squeezer] no input channels!");
        return;
    }

    // In case we have more main outputs than inputs, we'll clear any
    // output channels that didn't contain input data, because these
    // aren't guaranteed to be empty -- they may contain garbage.

    for (int nChannel = getMainBusNumInputChannels(); nChannel < getMainBusNumOutputChannels(); ++nChannel)
    {
        buffer.clear(nChannel, 0, nNumSamples);
    }

#ifdef SQUEEZER_MONO

    MainInput = AudioBuffer<float>(1, nNumSamples);
    SideChainInput = AudioBuffer<float>(1, nNumSamples);

    if (getChannelLayoutOfBus(true, 0) == AudioChannelSet::mono())
    {
        MainInput = getBusBuffer(buffer, true, 0);

        if (getChannelLayoutOfBus(true, 1) == AudioChannelSet::mono())
        {
            hasSideChain = true;
            SideChainInput = getBusBuffer(buffer, true, 1);
        }
        else
        {
            hasSideChain = false;
            SideChainInput = getBusBuffer(buffer, true, 0);
        }
    }
    else if (getChannelLayoutOfBus(true, 0).size() == 2)
    {
        hasSideChain = true;
        MainInput.copyFrom(0, 0, buffer,
                           0, 0, nNumSamples);

        SideChainInput.copyFrom(0, 0, buffer,
                                1, 0, nNumSamples);
    }
    else
    {
        hasSideChain = false;
        MainInput.clear();
        SideChainInput.clear();
    }

#else

    MainInput = AudioBuffer<float>(2, nNumSamples);
    SideChainInput = AudioBuffer<float>(2, nNumSamples);

    if (getChannelLayoutOfBus(true, 0) == AudioChannelSet::stereo())
    {
        MainInput = getBusBuffer(buffer, true, 0);

        if (getChannelLayoutOfBus(true, 1) == AudioChannelSet::stereo())
        {
            hasSideChain = true;
            SideChainInput = getBusBuffer(buffer, true, 1);
        }
        else
        {
            hasSideChain = false;
            SideChainInput = getBusBuffer(buffer, true, 0);
        }
    }
    else if (getChannelLayoutOfBus(true, 0).size() == 4)
    {
        hasSideChain = true;
        MainInput.copyFrom(0, 0, buffer,
                           0, 0, nNumSamples);
        MainInput.copyFrom(1, 0, buffer,
                           1, 0, nNumSamples);

        SideChainInput.copyFrom(0, 0, buffer,
                                2, 0, nNumSamples);
        SideChainInput.copyFrom(1, 0, buffer,
                                3, 0, nNumSamples);
    }
    else
    {
        DBG("clearing main input and side chain");

        hasSideChain = false;
        MainInput.clear();
        SideChainInput.clear();
    }

#endif

    // compressor's output is already de-normalised
    pCompressor->processBlock(MainInput, SideChainInput);

#ifdef SQUEEZER_MONO

    buffer.copyFrom(0, 0, MainInput,
                    0, 0, nNumSamples);

#else

    buffer.copyFrom(0, 0, MainInput,
                    0, 0, nNumSamples);
    buffer.copyFrom(1, 0, MainInput,
                    1, 0, nNumSamples);

#endif

    // "UM" --> update meters
    sendActionMessage("UM");
}


//==============================================================================

AudioProcessorEditor *SqueezerAudioProcessor::createEditor()
{
#ifdef SQUEEZER_MONO
    return new SqueezerAudioProcessorEditor(this, &pluginParameters, 1);
#else
    return new SqueezerAudioProcessorEditor(this, &pluginParameters, 2);
#endif
}


bool SqueezerAudioProcessor::hasEditor() const
{
    return true;
}


//==============================================================================

void SqueezerAudioProcessor::getStateInformation(MemoryBlock &destData)
{
    XmlElement xmlParameters = pluginParameters.storeAsXml();

    DBG("[Squeezer]");
    DBG("[Squeezer] storing plug-in parameters:");
    DBG("[Squeezer]");
    DBG(String("[Squeezer]   ") + xmlParameters.createDocument("").replace(
            "\n", "\n[Squeezer]   "));

    copyXmlToBinary(xmlParameters, destData);
}


void SqueezerAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlParameters(getXmlFromBinary(data, sizeInBytes));

    DBG("[Squeezer] loading plug-in parameters:");
    DBG("[Squeezer]");
    DBG(String("[Squeezer]   ") + xmlParameters->createDocument("").replace(
            "\n", "\n[Squeezer]   "));

    pluginParameters.loadFromXml(xmlParameters);
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
