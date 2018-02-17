/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013-2018 Martin Zuther (http://www.mzuther.de/)

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
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(getBusesProperties())
#endif
{
    frut::Frut::printVersionNumbers();

    sampleRateIsValid_ = false;
    hasSideChain_ = false;

    setLatencySamples(0);
}


SqueezerAudioProcessor::~SqueezerAudioProcessor()
{
    removeAllActionListeners();
}


AudioProcessor::BusesProperties SqueezerAudioProcessor::getBusesProperties()
{
#ifdef SQUEEZER_MONO

#if JucePlugin_Build_VST

    return BusesProperties()
           .withInput("Main / SC In",
                      AudioChannelSet::discreteChannels(2))
           .withOutput("Main Out",
                       AudioChannelSet::discreteChannels(1))
           .withInput("Disabled In",
                      AudioChannelSet::disabled());

#else

    return BusesProperties()
           .withInput("Main In",
                      AudioChannelSet::mono())
           .withOutput("Main Out",
                       AudioChannelSet::mono())
           .withInput("Side-Chain In",
                      AudioChannelSet::mono());

#endif

#else

#if JucePlugin_Build_VST

    return BusesProperties()
           .withInput("Main / SC In",
                      AudioChannelSet::discreteChannels(4))
           .withOutput("Main Out",
                       AudioChannelSet::discreteChannels(2))
           .withInput("Disabled In",
                      AudioChannelSet::disabled());

#else

    return BusesProperties()
           .withInput("Main In",
                      AudioChannelSet::stereo())
           .withOutput("Main Out",
                       AudioChannelSet::stereo())
           .withInput("Side-Chain In",
                      AudioChannelSet::stereo());

#endif

#endif
}


#ifndef JucePlugin_PreferredChannelConfigurations
bool SqueezerAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const
{
    // main bus: do not allow disabling of input channels
    if (layouts.getMainInputChannelSet().isDisabled())
    {
        return false;
    }

#ifdef SQUEEZER_MONO

    // main output must be mono
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono())
    {
        return false;
    }

    // main input is mono
    if (layouts.getMainInputChannelSet() == AudioChannelSet::mono())
    {
        // side chain input is mono --> okay
        if (layouts.getChannelSet(true, 1) == AudioChannelSet::mono())
        {
            return true;
        }

        // no side chain input --> okay
        if (layouts.getChannelSet(true, 1) == AudioChannelSet::disabled())
        {
            return true;
        }
    }

    // main input has two channels
    if (layouts.getMainInputChannelSet().size() == 2)
    {
        // no side chain input --> okay
        if (layouts.getChannelSet(true, 1) == AudioChannelSet::disabled())
        {
            return true;
        }
    }

#else

    // main output must be stereo
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
    {
        return false;
    }

    // main input is stereo
    if (layouts.getMainInputChannelSet() == AudioChannelSet::stereo())
    {
        // side chain input is stereo --> okay
        if (layouts.getChannelSet(true, 1) == AudioChannelSet::stereo())
        {
            return true;
        }

        // no side chain input --> okay
        if (layouts.getChannelSet(true, 1) == AudioChannelSet::disabled())
        {
            return true;
        }
    }

    // main input has four channels
    if (layouts.getMainInputChannelSet().size() == 4)
    {
        // no side chain input --> okay
        if (layouts.getChannelSet(true, 1) == AudioChannelSet::disabled())
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
    return pluginParameters_.getNumParameters(false);
}


const String SqueezerAudioProcessor::getParameterName(
    int nIndex)
{
    return pluginParameters_.getName(nIndex);
}


const String SqueezerAudioProcessor::getParameterText(
    int nIndex)
{
    return pluginParameters_.getText(nIndex);
}


String SqueezerAudioProcessor::getParameters()
{
    return pluginParameters_.toString();
}


float SqueezerAudioProcessor::getParameter(
    int nIndex)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    return pluginParameters_.getFloat(nIndex);
}


void SqueezerAudioProcessor::changeParameter(
    int nIndex,
    float fValue)
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


void SqueezerAudioProcessor::setParameter(
    int nIndex,
    float fValue)
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

        pluginParameters_.setFloat(nIndex, fValue);

        if (compressor_)
        {
            bool bBypassCompressor = pluginParameters_.getBoolean(nIndex);
            compressor_->setBypass(bBypassCompressor);
        }

        break;

    case SqueezerPluginParameters::selDetectorRmsFilter:

        pluginParameters_.setFloat(nIndex, fValue);

        if (compressor_)
        {
            float fDetectorRateMilliSeconds = pluginParameters_.getRealFloat(nIndex);
            compressor_->setDetectorRmsFilter(fDetectorRateMilliSeconds);
        }

        break;

    case SqueezerPluginParameters::selDesign:

        pluginParameters_.setFloat(nIndex, fValue);

        if (compressor_)
        {
            int nDesign = pluginParameters_.getRealInteger(nIndex);
            compressor_->setDesign(nDesign);
        }

        break;

    case SqueezerPluginParameters::selDetector:

        pluginParameters_.setFloat(nIndex, fValue);

        if (compressor_)
        {
            int nDetector = pluginParameters_.getRealInteger(nIndex);
            compressor_->setDetector(nDetector);
        }

        break;

    case SqueezerPluginParameters::selGainStage:

        pluginParameters_.setFloat(nIndex, fValue);

        if (compressor_)
        {
            int nGainStage = pluginParameters_.getRealInteger(nIndex);
            compressor_->setGainStage(nGainStage);
        }

        break;

    case SqueezerPluginParameters::selThreshold:

        pluginParameters_.setFloat(nIndex, fValue);

        if (compressor_)
        {
            float fThreshold = pluginParameters_.getRealFloat(nIndex);
            compressor_->setThreshold(fThreshold);
        }

        break;

    case SqueezerPluginParameters::selRatio:

        pluginParameters_.setFloat(nIndex, fValue);

        if (compressor_)
        {
            float fRatio = pluginParameters_.getRealFloat(nIndex);
            compressor_->setRatio(fRatio);
        }

        break;

    case SqueezerPluginParameters::selKneeWidth:

        pluginParameters_.setFloat(nIndex, fValue);

        if (compressor_)
        {
            float fKneeWidth = pluginParameters_.getRealFloat(nIndex);
            compressor_->setKneeWidth(fKneeWidth);
        }

        break;

    case SqueezerPluginParameters::selAttackRate:

        pluginParameters_.setFloat(nIndex, fValue);

        if (compressor_)
        {
            float fAttackRate = pluginParameters_.getRealFloat(nIndex);
            compressor_->setAttackRate(fAttackRate);
        }

        break;

    case SqueezerPluginParameters::selReleaseRate:

        pluginParameters_.setFloat(nIndex, fValue);

        if (compressor_)
        {
            int nReleaseRate = pluginParameters_.getRealInteger(nIndex);
            compressor_->setReleaseRate(nReleaseRate);
        }

        break;

    case SqueezerPluginParameters::selAutoMakeupGain:

        pluginParameters_.setFloat(nIndex, fValue);

        if (compressor_)
        {
            bool bAutoMakeupGain = pluginParameters_.getBoolean(nIndex);
            compressor_->setAutoMakeupGain(bAutoMakeupGain);
        }

        break;

    case SqueezerPluginParameters::selMakeupGain:

        pluginParameters_.setFloat(nIndex, fValue);

        if (compressor_)
        {
            float fMakeupGain = pluginParameters_.getRealFloat(nIndex);
            compressor_->setMakeupGain(fMakeupGain);
        }

        break;

    case SqueezerPluginParameters::selWetMix:

        pluginParameters_.setFloat(nIndex, fValue);

        if (compressor_)
        {
            int nWetMix = pluginParameters_.getRealInteger(nIndex);
            compressor_->setWetMix(nWetMix);
        }

        break;

    case SqueezerPluginParameters::selSidechainInput:

        pluginParameters_.setFloat(nIndex, fValue);

        if (compressor_)
        {
            bool bSidechainInput = pluginParameters_.getBoolean(nIndex);
            compressor_->setSidechainInput(bSidechainInput);
        }

        break;

    case SqueezerPluginParameters::selSidechainHPFCutoff:

        pluginParameters_.setFloat(nIndex, fValue);

        if (compressor_)
        {
            int nSidechainHPFCutoff = pluginParameters_.getRealInteger(nIndex);
            compressor_->setSidechainHPFCutoff(nSidechainHPFCutoff);
        }

        break;

    case SqueezerPluginParameters::selSidechainLPFCutoff:

        pluginParameters_.setFloat(nIndex, fValue);

        if (compressor_)
        {
            int nSidechainLPFCutoff = pluginParameters_.getRealInteger(nIndex);
            compressor_->setSidechainLPFCutoff(nSidechainLPFCutoff);
        }

        break;

    case SqueezerPluginParameters::selSidechainListen:

        pluginParameters_.setFloat(nIndex, fValue);

        if (compressor_)
        {
            bool bSidechainListen = pluginParameters_.getBoolean(nIndex);
            compressor_->setSidechainListen(bSidechainListen);
        }

        break;

    default:
    {
        frut::parameter::ParCombined *pCombined = dynamic_cast<frut::parameter::ParCombined *>(pluginParameters_.getPluginParameter(nIndex + 1));

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
    if (pluginParameters_.hasChanged(nIndex))
    {
        // for visible parameters, notify the editor of changes (this
        // will also clear the change flag)
        if (nIndex < pluginParameters_.getNumParameters(false))
        {
            // "PC" --> parameter changed, followed by a hash and the
            // parameter's ID
            sendActionMessage("PC#" + String(nIndex));
        }
        // for hidden parameters, we only have to clear the change
        // flag
        else
        {
            pluginParameters_.clearChangeFlag(nIndex);
        }
    }
}


void SqueezerAudioProcessor::clearChangeFlag(
    int nIndex)
{
    pluginParameters_.clearChangeFlag(nIndex);
}


bool SqueezerAudioProcessor::hasChanged(
    int nIndex)
{
    return pluginParameters_.hasChanged(nIndex);
}


void SqueezerAudioProcessor::updateParameters(
    bool bIncludeHiddenParameters)
{
    int nNumParameters = pluginParameters_.getNumParameters(false);

    for (int nIndex = 0; nIndex < nNumParameters; ++nIndex)
    {
        if (pluginParameters_.hasChanged(nIndex))
        {
            float fValue = pluginParameters_.getFloat(nIndex);
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

    return pluginParameters_.getSkinName();
}


void SqueezerAudioProcessor::setParameterSkinName(
    const String &strSkinName)
{
    // This method will be called by the host, probably on the audio
    // thread, so it's absolutely time-critical. Don't use critical
    // sections or anything GUI-related, or anything at all that may
    // block in any way!

    pluginParameters_.setSkinName(strSkinName);
}


void SqueezerAudioProcessor::resetMeters()
{
    if (compressor_)
    {
        compressor_->resetMeters();
    }

    // "UM" --> update meters
    sendActionMessage("UM");
}


float SqueezerAudioProcessor::getGainReduction(
    int nChannel)
{
    if (compressor_)
    {
        return (float) compressor_->getGainReduction(nChannel);
    }
    else
    {
        return -1.0f;
    }
}


float SqueezerAudioProcessor::getGainReductionMeterPeak(
    int nChannel)
{
    if (compressor_)
    {
        return (float) compressor_->getGainReductionMeterPeak(nChannel);
    }
    else
    {
        return -1.0f;
    }
}


float SqueezerAudioProcessor::getPeakMeterInputLevel(
    int nChannel)
{
    if (compressor_)
    {
        return (float) compressor_->getPeakMeterInputLevel(nChannel);
    }
    else
    {
        return -1.0f;
    }
}


float SqueezerAudioProcessor::getPeakMeterOutputLevel(
    int nChannel)
{
    if (compressor_)
    {
        return (float) compressor_->getPeakMeterOutputLevel(nChannel);
    }
    else
    {
        return -1.0f;
    }
}


float SqueezerAudioProcessor::getPeakMeterPeakInputLevel(
    int nChannel)
{
    if (compressor_)
    {
        return (float) compressor_->getPeakMeterPeakInputLevel(nChannel);
    }
    else
    {
        return -1.0f;
    }
}


float SqueezerAudioProcessor::getPeakMeterPeakOutputLevel(
    int nChannel)
{
    if (compressor_)
    {
        return (float) compressor_->getPeakMeterPeakOutputLevel(nChannel);
    }
    else
    {
        return -1.0f;
    }
}


float SqueezerAudioProcessor::getMaximumInputLevel(
    int nChannel)
{
    if (compressor_)
    {
        return (float) compressor_->getMaximumInputLevel(nChannel);
    }
    else
    {
        return -1.0f;
    }
}


float SqueezerAudioProcessor::getMaximumOutputLevel(
    int nChannel)
{
    if (compressor_)
    {
        return (float) compressor_->getMaximumOutputLevel(nChannel);
    }
    else
    {
        return -1.0f;
    }
}


float SqueezerAudioProcessor::getAverageMeterInputLevel(
    int nChannel)
{
    if (compressor_)
    {
        return (float) compressor_->getAverageMeterInputLevel(nChannel);
    }
    else
    {
        return -1.0f;
    }
}


float SqueezerAudioProcessor::getAverageMeterOutputLevel(
    int nChannel)
{
    if (compressor_)
    {
        return (float) compressor_->getAverageMeterOutputLevel(nChannel);
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


void SqueezerAudioProcessor::setCurrentProgram(
    int nIndex)
{
    ignoreUnused(nIndex);
}


const String SqueezerAudioProcessor::getProgramName(
    int nIndex)
{
    ignoreUnused(nIndex);

    return String::empty;
}


void SqueezerAudioProcessor::changeProgramName(
    int nIndex,
    const String &newName)
{
    ignoreUnused(nIndex, newName);
}


void SqueezerAudioProcessor::prepareToPlay(
    double sampleRate,
    int samplesPerBlock)
{
    ignoreUnused(samplesPerBlock);

    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    Logger::outputDebugString("[Squeezer] preparing to play");

    if ((sampleRate < 44100) || (sampleRate > 192000))
    {
        Logger::outputDebugString("[Squeezer] WARNING: sample rate of " +
                                  String(sampleRate) + " Hz not supported");
        sampleRateIsValid_ = false;
        return;
    }
    else
    {
        sampleRateIsValid_ = true;
    }

    Logger::outputDebugString("[Squeezer] number of main/aux input channels:  " + String(getMainBusNumInputChannels()) + "/" + String(getTotalNumInputChannels() - getMainBusNumInputChannels()));
    Logger::outputDebugString("[Squeezer] number of main/aux output channels: " + String(getMainBusNumOutputChannels()) + "/" + String(getTotalNumOutputChannels() - getMainBusNumOutputChannels()));

    bool bBypassCompressor = pluginParameters_.getBoolean(
                                 SqueezerPluginParameters::selBypass);
    float fDetectorRateMilliSeconds = pluginParameters_.getRealFloat(
                                          SqueezerPluginParameters::selDetectorRmsFilter);
    int nDesign = pluginParameters_.getRealInteger(
                      SqueezerPluginParameters::selDesign);
    int nDetector = pluginParameters_.getRealInteger(
                        SqueezerPluginParameters::selDetector);
    int nGainStage = pluginParameters_.getRealInteger(
                         SqueezerPluginParameters::selGainStage);

    float fThreshold = pluginParameters_.getRealFloat(
                           SqueezerPluginParameters::selThreshold);
    float fRatio = pluginParameters_.getRealFloat(
                       SqueezerPluginParameters::selRatio);
    float fKneeWidth = pluginParameters_.getRealFloat(
                           SqueezerPluginParameters::selKneeWidth);

    float fAttackRate = pluginParameters_.getRealFloat(
                            SqueezerPluginParameters::selAttackRate);
    int nReleaseRate = pluginParameters_.getRealInteger(
                           SqueezerPluginParameters::selReleaseRate);

    bool bAutoMakeupGain = pluginParameters_.getBoolean(
                               SqueezerPluginParameters::selAutoMakeupGain);
    float fMakeupGain = pluginParameters_.getRealFloat(
                            SqueezerPluginParameters::selMakeupGain);
    int nWetMix = pluginParameters_.getRealInteger(
                      SqueezerPluginParameters::selWetMix);

    bool bSidechainInput = pluginParameters_.getBoolean(
                               SqueezerPluginParameters::selSidechainInput);
    int nSidechainHPFCutoff = pluginParameters_.getRealInteger(
                                  SqueezerPluginParameters::selSidechainHPFCutoff);
    int nSidechainLPFCutoff = pluginParameters_.getRealInteger(
                                  SqueezerPluginParameters::selSidechainLPFCutoff);
    bool bSidechainListen = pluginParameters_.getBoolean(
                                SqueezerPluginParameters::selSidechainListen);

#ifdef SQUEEZER_MONO
    int numberOfChannels = 1;
#else
    int numberOfChannels = 2;
#endif

    dither_.initialise(jmax(getMainBusNumInputChannels(),
                            getMainBusNumOutputChannels()),
                       24);

    compressor_ = new Compressor(numberOfChannels, (int) sampleRate);

    compressor_->setBypass(bBypassCompressor);
    compressor_->setDetectorRmsFilter(fDetectorRateMilliSeconds);
    compressor_->setDesign(nDesign);
    compressor_->setDetector(nDetector);
    compressor_->setGainStage(nGainStage);

    compressor_->setThreshold(fThreshold);
    compressor_->setRatio(fRatio);
    compressor_->setKneeWidth(fKneeWidth);

    compressor_->setAttackRate(fAttackRate);
    compressor_->setReleaseRate(nReleaseRate);

    compressor_->setAutoMakeupGain(bAutoMakeupGain);
    compressor_->setMakeupGain(fMakeupGain);
    compressor_->setWetMix(nWetMix);

    compressor_->setSidechainInput(bSidechainInput);
    compressor_->setSidechainHPFCutoff(nSidechainHPFCutoff);
    compressor_->setSidechainLPFCutoff(nSidechainLPFCutoff);
    compressor_->setSidechainListen(bSidechainListen);
}


void SqueezerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free
    // up any spare memory, etc.

    Logger::outputDebugString("[Squeezer] releasing resources");
    Logger::outputDebugString("");
}


void SqueezerAudioProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers,
    // etc, as it means there's been a break in the audio's
    // continuity.
}


void SqueezerAudioProcessor::processBlock(
    AudioBuffer<float> &buffer,
    MidiBuffer &midiMessages)
{
    jassert(!isUsingDoublePrecision());
    ignoreUnused(midiMessages);

    int NumberOfChannels = buffer.getNumChannels();
    int NumberOfSamples = buffer.getNumSamples();

    AudioBuffer<double> processBuffer(NumberOfChannels, NumberOfSamples);

    // convert input to float and de-normalize samples
    dither_.denormalizeToDouble(buffer, processBuffer);

    // process input samples
    process(processBuffer);

    // dither output to float
    dither_.ditherToFloat(processBuffer, buffer);
}


void SqueezerAudioProcessor::processBlock(
    AudioBuffer<double> &buffer,
    MidiBuffer &midiMessages)
{
    jassert(isUsingDoublePrecision());
    ignoreUnused(midiMessages);

    // de-normalize samples
    dither_.denormalize(buffer);

    // process input samples
    process(buffer);
}


void SqueezerAudioProcessor::process(
    AudioBuffer<double> &buffer)
{
    int nNumSamples = buffer.getNumSamples();

    if (!sampleRateIsValid_)
    {
        buffer.clear();
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

    mainInput_ = AudioBuffer<double>(1, nNumSamples);
    sideChainInput_ = AudioBuffer<double>(1, nNumSamples);

    if (getChannelLayoutOfBus(true, 0) == AudioChannelSet::mono())
    {
        mainInput_ = getBusBuffer(buffer, true, 0);

        if (getChannelLayoutOfBus(true, 1) == AudioChannelSet::mono())
        {
            hasSideChain_ = true;
            sideChainInput_ = getBusBuffer(buffer, true, 1);
        }
        else
        {
            hasSideChain_ = false;
            sideChainInput_ = getBusBuffer(buffer, true, 0);
        }
    }
    else if (getChannelLayoutOfBus(true, 0).size() == 2)
    {
        hasSideChain_ = true;
        mainInput_.copyFrom(0, 0, buffer,
                            0, 0, nNumSamples);

        sideChainInput_.copyFrom(0, 0, buffer,
                                 1, 0, nNumSamples);
    }
    else
    {
        hasSideChain_ = false;
        mainInput_.clear();
        sideChainInput_.clear();
    }

#else

    mainInput_ = AudioBuffer<double>(2, nNumSamples);
    sideChainInput_ = AudioBuffer<double>(2, nNumSamples);

    if (getChannelLayoutOfBus(true, 0) == AudioChannelSet::stereo())
    {
        mainInput_ = getBusBuffer(buffer, true, 0);

        if (getChannelLayoutOfBus(true, 1) == AudioChannelSet::stereo())
        {
            hasSideChain_ = true;
            sideChainInput_ = getBusBuffer(buffer, true, 1);
        }
        else
        {
            hasSideChain_ = false;
            sideChainInput_ = getBusBuffer(buffer, true, 0);
        }
    }
    else if (getChannelLayoutOfBus(true, 0).size() == 4)
    {
        hasSideChain_ = true;
        mainInput_.copyFrom(0, 0, buffer,
                            0, 0, nNumSamples);
        mainInput_.copyFrom(1, 0, buffer,
                            1, 0, nNumSamples);

        sideChainInput_.copyFrom(0, 0, buffer,
                                 2, 0, nNumSamples);
        sideChainInput_.copyFrom(1, 0, buffer,
                                 3, 0, nNumSamples);
    }
    else
    {
        DBG("clearing main input and side chain");

        hasSideChain_ = false;
        mainInput_.clear();
        sideChainInput_.clear();
    }

#endif

    compressor_->process(mainInput_, sideChainInput_);

#ifdef SQUEEZER_MONO

    buffer.copyFrom(0, 0, mainInput_,
                    0, 0, nNumSamples);

#else

    buffer.copyFrom(0, 0, mainInput_,
                    0, 0, nNumSamples);
    buffer.copyFrom(1, 0, mainInput_,
                    1, 0, nNumSamples);

#endif

    // "UM" --> update meters
    sendActionMessage("UM");
}


AudioProcessorEditor *SqueezerAudioProcessor::createEditor()
{
#ifdef SQUEEZER_MONO
    return new SqueezerAudioProcessorEditor(this, &pluginParameters_, 1);
#else
    return new SqueezerAudioProcessorEditor(this, &pluginParameters_, 2);
#endif
}


bool SqueezerAudioProcessor::hasEditor() const
{
    return true;
}


void SqueezerAudioProcessor::getStateInformation(
    MemoryBlock &destData)
{
    XmlElement xmlParameters = pluginParameters_.storeAsXml();

    DBG("[Squeezer]");
    DBG("[Squeezer] storing plug-in parameters:");
    DBG("[Squeezer]");
    DBG(String("[Squeezer]   ") + xmlParameters.createDocument("").replace(
            "\n", "\n[Squeezer]   "));

    copyXmlToBinary(xmlParameters, destData);
}


void SqueezerAudioProcessor::setStateInformation(
    const void *data,
    int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlParameters(getXmlFromBinary(data, sizeInBytes));

    DBG("[Squeezer] loading plug-in parameters:");
    DBG("[Squeezer]");
    DBG(String("[Squeezer]   ") + xmlParameters->createDocument("").replace(
            "\n", "\n[Squeezer]   "));

    pluginParameters_.loadFromXml(xmlParameters);
    updateParameters(true);
}


// This creates new instances of the plug-in.
AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new SqueezerAudioProcessor();
}
