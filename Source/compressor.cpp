/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of citrus

   Copyright (c) 2013-2020 Martin Zuther (http://www.mzuther.de/)

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


Compressor::Compressor(int channels, int sample_rate) :
    // the meter's sample buffer holds 50 ms worth of samples
    BufferLength(0.050),
    NumberOfChannels(channels),
    SampleRate(sample_rate),
    MeterBufferSize((int)(SampleRate * BufferLength)),
    MeterInputBuffer(NumberOfChannels, MeterBufferSize),
    MeterOutputBuffer(NumberOfChannels, MeterBufferSize)
{
    jassert((NumberOfChannels == 1) || (NumberOfChannels == 2));

    CrestFactor = 20.0;
    UseUpwardExpansion = false;

    // fall time: 26 dB in 3 seconds (linear)
    ReleaseCoefLinear = 26.0 * BufferLength / 3.0;

    MeterBufferPosition = 0;

    // reset meter and set up array members
    resetMeters();

    WetMixPercentage = 100;
    setBypass(false);
    setDesign(Compressor::DesignFeedForward);

    setInputTrim(0.0);
    setAutoMakeupGain(false);
    setMakeupGain(0.0);

    setStereoLink(100);
    setWetMix(100);

    for (int CurrentChannel = 0; CurrentChannel < NumberOfChannels; ++CurrentChannel)
    {
        SideChainProcessor.add(new SideChain(SampleRate));

        InputSamples.add(0.0);
        SidechainSamples.add(0.0);
        OutputSamples.add(0.0);

        // initialise side-chain high-pass filter
        SidechainFilter_HPF.add(
            new frut::dsp::IirFilterBox(
                NumberOfChannels, SampleRate));

        // initialise side-chain low-pass filter
        SidechainFilter_LPF.add(
            new frut::dsp::IirFilterBox(
                NumberOfChannels, SampleRate));
    }

    // disable external side-chain
    setSidechainInput(false);
    setSidechainListen(false);

    // bypass side-chain filters
    setSidechainHPFCutoff(0);
    setSidechainLPFCutoff(0);
}


void Compressor::resetMeters()
{
    double MeterMinimumDecibel = -(70.01 + CrestFactor);

    // loop through all audio channels
    for (int CurrentChannel = 0; CurrentChannel < NumberOfChannels; ++CurrentChannel)
    {
        // set gain reduction to zero
        GainReduction.set(CurrentChannel, 0.0);
        GainReductionWithMakeup.set(CurrentChannel, 0.0);

        // set peak meter levels to meter minimum
        PeakMeterInputLevels.set(CurrentChannel, MeterMinimumDecibel);
        PeakMeterOutputLevels.set(CurrentChannel, MeterMinimumDecibel);

        // set average meter levels to meter minimum
        AverageMeterInputLevels.set(CurrentChannel, MeterMinimumDecibel);
        AverageMeterOutputLevels.set(CurrentChannel, MeterMinimumDecibel);
    }
}


bool Compressor::getBypass()
/*  Get current bypass state.

    return value (boolean): returns current bypass state
 */
{
    return CompressorIsBypassed;
}


void Compressor::setBypass(bool CompressorIsBypassedNew)
/*  Set new bypass state.

    CompressorIsBypassedNew (boolean): new bypass state

    return value: none
 */
{
    CompressorIsBypassed = CompressorIsBypassedNew;
    CompressorIsBypassedCombined = (CompressorIsBypassed || (WetMixPercentage == 0));
}


double Compressor::getRmsWindowSize()
/*  Get current detector RMS window size.

    return value (double): returns current current detector RMS window
    size
*/
{
    return SideChainProcessor[0]->getRmsWindowSize();
}


void Compressor::setRmsWindowSize(double RmsWindowSizeMilliSecondsNew)
/*  Set new detector RMS window size.

    RmsWindowSizeMilliSecondsNew (double): new detector RMS window size

    return value: none
*/
{
    for (int CurrentChannel = 0; CurrentChannel < NumberOfChannels; ++CurrentChannel)
    {
        SideChainProcessor[CurrentChannel]->setRmsWindowSize(RmsWindowSizeMilliSecondsNew);
    }
}


int Compressor::getDesign()
/*  Get current compressor design.

    return value (integer): returns the current compressor design
 */
{
    return CompressorDesign;
}


void Compressor::setDesign(int CompressorDesignNew)
/*  Set new compressor design.

    CompressorDesignNew (integer): new compressor design

    return value: none
 */
{
    CompressorDesign = CompressorDesignNew;
    DesignIsFeedForward = (CompressorDesign == Compressor::DesignFeedForward);
}


double Compressor::getThreshold()
/*  Get current threshold.

    return value (double): returns the current threshold in decibels
 */
{
    return SideChainProcessor[0]->getThreshold();
}


void Compressor::setThreshold(double ThresholdNew)
/*  Set new threshold.

    ThresholdNew (double): new threshold in decibels

    return value: none
 */
{
    for (int CurrentChannel = 0; CurrentChannel < NumberOfChannels; ++CurrentChannel)
    {
        SideChainProcessor[CurrentChannel]->setThreshold(ThresholdNew);
    }
}


double Compressor::getRatio()
/*  Get current compression ratio.

    return value (double): returns the current compression ratio
 */
{
    double RatioNew = SideChainProcessor[0]->getRatio();

    if (UseUpwardExpansion)
    {
        return 1.0 / RatioNew;
    }
    else
    {
        return RatioNew;
    }
}


void Compressor::setRatio(double RatioNew)
/*  Set new compression ratio.

    RatioNew (double): new compression ratio

    return value: none
 */
{
    if (RatioNew < 1.0)
    {
        UseUpwardExpansion = true;
        RatioNew = 1.0 / RatioNew;
    }
    else
    {
        UseUpwardExpansion = false;
    }

    for (int CurrentChannel = 0; CurrentChannel < NumberOfChannels; ++CurrentChannel)
    {
        SideChainProcessor[CurrentChannel]->setRatio(RatioNew);
    }
}


double Compressor::getKneeWidth()
/*  Get current knee width.

    return value (double): returns the current knee width in decibels
 */
{
    return SideChainProcessor[0]->getKneeWidth();
}


void Compressor::setKneeWidth(double KneeWidthNew)
/*  Set new knee width.

    KneeWidthNew (double): new knee width in decibels

    return value: none
 */
{
    for (int CurrentChannel = 0; CurrentChannel < NumberOfChannels; ++CurrentChannel)
    {
        SideChainProcessor[CurrentChannel]->setKneeWidth(KneeWidthNew);
    }

}


double Compressor::getAttackRate()
/*  Get current attack rate.

    return value (double): returns the current attack rate in
    milliseconds
 */
{
    return SideChainProcessor[0]->getAttackRate();
}


void Compressor::setAttackRate(double AttackRateNew)
/*  Set new attack rate.

    AttackRateNew (double): new attack rate in milliseconds

    return value: none
 */
{
    for (int CurrentChannel = 0; CurrentChannel < NumberOfChannels; ++CurrentChannel)
    {
        SideChainProcessor[CurrentChannel]->setAttackRate(AttackRateNew);
    }
}


int Compressor::getReleaseRate()
/*  Get current release rate.

    return value (integer): returns the current release rate in
    milliseconds
 */
{
    return SideChainProcessor[0]->getReleaseRate();
}


void Compressor::setReleaseRate(int ReleaseRateNew)
/*  Set new release rate.

    ReleaseRateNew (integer): new release rate in milliseconds

    return value: none
 */
{
    for (int CurrentChannel = 0; CurrentChannel < NumberOfChannels; ++CurrentChannel)
    {
        SideChainProcessor[CurrentChannel]->setReleaseRate(ReleaseRateNew);
    }
}


int Compressor::getCurve()
/*  Get current compressor curve type.

    return value (integer): returns compressor curve type
 */
{
    return SideChainProcessor[0]->getCurve();
}


void Compressor::setCurve(int CurveTypeNew)
/*  Set new compressor curve type.

    CurveTypeNew (integer): new compressor curve type

    return value: none
 */
{
    for (int CurrentChannel = 0; CurrentChannel < NumberOfChannels; ++CurrentChannel)
    {
        SideChainProcessor[CurrentChannel]->setCurve(CurveTypeNew);
    }
}


int Compressor::getGainStage()
/*  Get current compressor gain stage type.

    return value (integer): returns compressor gain stage type
 */
{
    return SideChainProcessor[0]->getGainStage();
}


void Compressor::setGainStage(int GainStageTypeNew)
/*  Set new compressor gain stage type.

    GainStageTypeNew (integer): new compressor gain stage type

    return value: none
 */
{
    for (int CurrentChannel = 0; CurrentChannel < NumberOfChannels; ++CurrentChannel)
    {
        SideChainProcessor[CurrentChannel]->setGainStage(GainStageTypeNew);
    }
}


int Compressor::getStereoLink()
/*  Get current stereo link percentage.

    return value (integer): returns the current stereo link percentage
    (0 to 100)
 */
{
    return StereoLinkPercentage;
}


void Compressor::setStereoLink(int StereoLinkPercentageNew)
/*  Set new stereo link percentage.

    StereoLinkPercentageNew (integer): new stereo link percentage (0 to 100)

    return value: none
 */
{
    StereoLinkPercentage = StereoLinkPercentageNew;

    // amplification factor for other channel ranging from 0.0 (no
    // stereo linking) to 0.5 (full stereo linking)
    StereoLinkWeightOther = StereoLinkPercentage / 200.0;

    // amplification factor for original channel ranging from 1.0 (no
    // stereo linking) to 0.5 (full stereo linking)
    StereoLinkWeight = 1.0 - StereoLinkWeightOther;
}


double Compressor::getInputTrim()
/*  Get current input trim gain.

    return value (double): returns the current input trim gain in
    decibels
 */
{
    return InputTrim;
}


void Compressor::setInputTrim(double InputTrimNew)
/*  Set new input trim gain.

    InputTrimNew (double): new input trim gain in decibels

    return value: none
 */
{
    InputTrim = InputTrimNew;
}


bool Compressor::getAutoMakeupGain()
/*  Get current auto make-up gain state.

    return value (boolean): returns current auto make-up gain state
 */
{
    return UseAutoMakeupGain;
}


void Compressor::setAutoMakeupGain(bool UseAutoMakeupGainNew)
/*  Set new auto make-up gain state.

    UseAutoMakeupGainNew (boolean): new auto make-up gain state

    return value: none
 */
{
    UseAutoMakeupGain = UseAutoMakeupGainNew;
}


double Compressor::getMakeupGain()
/*  Get current make-up gain.

    return value (double): returns the current make-up gain in decibels
 */
{
    return MakeupGainDecibel;
}


void Compressor::setMakeupGain(double MakeupGainNew)
/*  Set new make-up gain.

    nMakeupGainNew (double): new make-up gain in decibels

    return value: none
 */
{
    MakeupGainDecibel = MakeupGainNew;
    MakeupGain = SideChain::decibel2level(MakeupGainDecibel);
}


int Compressor::getWetMix()
/*  Get current wet mix percentage.

    return value (integer): returns the current wet mix percentage
    (0 to 100)
 */
{
    return WetMixPercentage;
}


void Compressor::setWetMix(int WetMixPercentageNew)
/*  Set new wet mix percentage.

    WetMixPercentageNew (integer): new wet mix percentage (0 to 100)

    return value: none
 */
{
    WetMixPercentage = WetMixPercentageNew;

    WetMix = WetMixPercentage / 100.0;
    DryMix = 1.0 - WetMix;

    CompressorIsBypassedCombined = (CompressorIsBypassed || (WetMixPercentage == 0));
}


bool Compressor::getSidechainInput()
/*  Get current side-chain input.

    return value (boolean): returns current side-chain input (true ==
    external input)
 */
{
    return EnableExternalInput;
}


void Compressor::setSidechainInput(bool EnableExternalInputNew)
/*  Set new side-chain input.

    EnableExternalInputNew (boolean): new side-chain input (true ==
    external input)

    return value: none
 */
{
    EnableExternalInput = EnableExternalInputNew;
}


int Compressor::getSidechainHPFCutoff()
/*  Get current side-chain high-pass filter cutoff frequency.

    return value (integer): side-chain high-pass filter cutoff
    frequency (in Hertz)
 */
{
    return SidechainHPFCutoff;
}


void Compressor::setSidechainHPFCutoff(int SidechainHPFCutoffNew)
/*  Set new side-chain high-pass filter cutoff frequency.

    SidechainHPFCutoff (integer): new side-chain high-pass filter
    cutoff frequency (in Hertz)

    return value: none
 */
{
    SidechainHPFCutoff = SidechainHPFCutoffNew;

    bool IsHPFEnabledOld = IsHPFEnabled;
    IsHPFEnabled = (SidechainHPFCutoff > 20);

    for (int CurrentChannel = 0; CurrentChannel < NumberOfChannels; ++CurrentChannel)
    {
        if (IsHPFEnabled != IsHPFEnabledOld)
        {
            SidechainFilter_HPF[CurrentChannel]->resetDelays();
        }

        SidechainFilter_HPF[CurrentChannel]->passFilterSecondOrder(
            SidechainHPFCutoff, 0.707, false);
    }
}


int Compressor::getSidechainLPFCutoff()
/*  Get current side-chain low-pass filter cutoff frequency.

    return value (integer): side-chain low-pass filter cutoff
    frequency (in Hertz)
 */
{
    return SidechainLPFCutoff;
}


void Compressor::setSidechainLPFCutoff(int SidechainLPFCutoffNew)
/*  Set new side-chain low-pass filter cutoff frequency.

    SidechainLPFCutoff (integer): new side-chain low-pass filter
    cutoff frequency (in Hertz)

    return value: none
 */
{
    SidechainLPFCutoff = SidechainLPFCutoffNew;

    bool IsLPFEnabledOld = IsLPFEnabled;
    IsLPFEnabled = (SidechainLPFCutoff < 15000);

    for (int CurrentChannel = 0; CurrentChannel < NumberOfChannels; ++CurrentChannel)
    {
        if (IsLPFEnabled != IsLPFEnabledOld)
        {
            SidechainFilter_LPF[CurrentChannel]->resetDelays();
        }

        SidechainFilter_LPF[CurrentChannel]->passFilterSecondOrder(
            SidechainLPFCutoff, 0.707, true);
    }
}


bool Compressor::getSidechainListen()
/*  Get current side-chain listen state.

    return value (boolean): returns current side-chain listen state
 */
{
    return ListenToSidechain;
}


void Compressor::setSidechainListen(bool ListenToSidechainNew)
/*  Set new side-chain listen state.

    ListenToSidechainNew (boolean): new side-chain listen state

    return value: none
 */
{
    ListenToSidechain = ListenToSidechainNew;
}


double Compressor::getGainReduction(int CurrentChannel)
/*  Get current gain reduction.

    CurrentChannel (integer): queried audio channel

    return value (double): returns the current gain reduction in
    decibel
 */
{
    jassert(CurrentChannel >= 0);
    jassert(CurrentChannel < NumberOfChannels);

    if (CompressorIsBypassedCombined)
    {
        return 0.0;
    }
    else
    {
        return GainReduction[CurrentChannel];
    }
}


double Compressor::getPeakMeterInputLevel(int CurrentChannel)
/*  Get current input peak level.

    CurrentChannel (integer): selected audio channel

    return value (double): returns current input peak level in decibel
*/
{
    jassert(CurrentChannel >= 0);
    jassert(CurrentChannel < NumberOfChannels);

    return PeakMeterInputLevels[CurrentChannel] + CrestFactor;
}


double Compressor::getPeakMeterOutputLevel(int CurrentChannel)
/*  Get current output peak level.

    CurrentChannel (integer): selected audio channel

    return value (double): returns current output peak level in decibel
*/
{
    jassert(CurrentChannel >= 0);
    jassert(CurrentChannel < NumberOfChannels);

    return PeakMeterOutputLevels[CurrentChannel] + CrestFactor;
}


double Compressor::getAverageMeterInputLevel(int CurrentChannel)
/*  Get current input average level.

    CurrentChannel (integer): selected audio channel

    return value (double): returns current input average level in
    decibel
*/
{
    jassert(CurrentChannel >= 0);
    jassert(CurrentChannel < NumberOfChannels);

    return AverageMeterInputLevels[CurrentChannel] + CrestFactor;
}


double Compressor::getAverageMeterOutputLevel(int CurrentChannel)
/*  Get current output average level.

    CurrentChannel (integer): selected audio channel

    return value (double): returns current output average level in
    decibel
*/
{
    jassert(CurrentChannel >= 0);
    jassert(CurrentChannel < NumberOfChannels);

    return AverageMeterOutputLevels[CurrentChannel] + CrestFactor;
}


void Compressor::process(
    AudioBuffer<double> &MainBuffer,
    AudioBuffer<double> &SideChainBuffer)
{
    jassert(MainBuffer.getNumSamples() == SideChainBuffer.getNumSamples());

    int nNumSamples = MainBuffer.getNumSamples();

    for (int nSample = 0; nSample < nNumSamples; ++nSample)
    {
        // get and prepare input samples (all channels have to be
        // prepared before any processing can take place!)
        for (int CurrentChannel = 0; CurrentChannel < NumberOfChannels; ++CurrentChannel)
        {
            // get current input sample
            double InputSample = MainBuffer.getSample(CurrentChannel, nSample);

            // store de-normalised input sample
            InputSamples.set(CurrentChannel, InputSample);

            // store input sample in buffer for input meter
            MeterInputBuffer.setSample(
                CurrentChannel, MeterBufferPosition, InputSample);
        }

        // compressor is bypassed (or mix is set to 0 percent)
        if (CompressorIsBypassedCombined)
        {
            for (int CurrentChannel = 0; CurrentChannel < NumberOfChannels; ++CurrentChannel)
            {
                // store de-normalised (!) input sample in buffer for
                // output meter
                MeterOutputBuffer.copyFrom(
                    CurrentChannel, MeterBufferPosition, MeterInputBuffer,
                    CurrentChannel, nSample, 1);

                // store gain reduction now and apply ballistics later
                GainReduction.set(CurrentChannel, 0.0);
                GainReductionWithMakeup.set(CurrentChannel, 0.0);
            }

            // update meter ballistics and increment buffer location
            updateMeterBallistics();

            // get next sample and thus skip compression
            continue;
        }

        // process side chain
        for (int CurrentChannel = 0; CurrentChannel < NumberOfChannels; ++CurrentChannel)
        {
            double SideChainSample;

            // compress channels (feed-forward design)
            if (DesignIsFeedForward)
            {
                // side chain is fed from *input* channel
                if (EnableExternalInput)
                {
                    // feed side chain from external input
                    SideChainSample = (double) SideChainBuffer.getSample(
                                          CurrentChannel, nSample);
                }
                else
                {
                    // feed side chain from main input
                    SideChainSample = (double) MainBuffer.getSample(
                                          CurrentChannel, nSample);
                }
            }
            // compress channels (feed-back design)
            else
            {
                // only the alternative feed-back mode supports
                // external side chain, so use it when external side
                // chain is enabled
                bool UseAlternativeFeedbackMode = EnableExternalInput;

                if (UseAlternativeFeedbackMode)
                {
                    // feed side chain from external input
                    SideChainSample = (double) SideChainBuffer.getSample(
                                          CurrentChannel, nSample);

                    // retrieve last gain reduction
                    double LastGainReduction = -GainReductionWithMakeup[CurrentChannel];

                    // apply feedback-loop
                    SideChainSample *= SideChain::decibel2level(LastGainReduction);
                }
                // "normal" feed-back mode (external side
                // chain not supported)
                else
                {
                    // side chain is fed from *output* channel
                    SideChainSample = OutputSamples[CurrentChannel];
                }
            }

            // filter side-chain sample (the filter's output is
            // already de-normalised!)
            if (IsHPFEnabled)
            {
                SidechainFilter_HPF[CurrentChannel]->processSample(
                    SideChainSample, CurrentChannel);
            }

            if (IsLPFEnabled)
            {
                SidechainFilter_LPF[CurrentChannel]->processSample(
                    SideChainSample, CurrentChannel);
            }

            SidechainSamples.set(CurrentChannel, SideChainSample);
        }

        // all channels of side chain have been processed; now we can
        // calculate the side chain level
        for (int CurrentChannel = 0; CurrentChannel < NumberOfChannels; ++CurrentChannel)
        {
            double SideChainLevel;

            // stereo linking is off (save some processing time)
            if (StereoLinkPercentage == 0)
            {
                SideChainLevel = fabs(SidechainSamples[CurrentChannel]);
            }
            // stereo linking is on
            else
            {
                // get ID of other stereo channel
                int OtherChannel = (CurrentChannel == 0) ? 1 : 0;

                // mix side chain according to stereo link percentage;
                // getting the absolute value of each channel
                // *separately* allows for a certain kind of M/S
                // compression
                SideChainLevel = fabs(SidechainSamples[CurrentChannel] * StereoLinkWeight) + fabs(SidechainSamples[OtherChannel] * StereoLinkWeightOther);
            }

            // convert side chain level to decibels
            SideChainLevel = SideChain::level2decibel(SideChainLevel);

            // apply crest factor
            SideChainLevel += CrestFactor;

            // send current trim-adjusted input sample to gain
            // reduction unit
            SideChainProcessor[CurrentChannel]->processSample(SideChainLevel + InputTrim);
        }

        // apply gain reduction and save output sample
        for (int CurrentChannel = 0; CurrentChannel < NumberOfChannels; ++CurrentChannel)
        {
            // store gain reduction now and apply ballistics later
            GainReduction.set(CurrentChannel, SideChainProcessor[CurrentChannel]->getGainReduction(false));
            GainReductionWithMakeup.set(CurrentChannel, SideChainProcessor[CurrentChannel]->getGainReduction(true));

            // apply gain reduction to current input sample
            //
            //  feed-forward design:  current gain reduction
            //  feed-back design:     "old" gain reduction
            double CurrentGainReduction;

            if (UseAutoMakeupGain)
            {
                CurrentGainReduction = -GainReductionWithMakeup[CurrentChannel];
            }
            else
            {
                CurrentGainReduction = -GainReduction[CurrentChannel];
            }

            // invert gain reduction for upward expansion
            if (UseUpwardExpansion)
            {
                CurrentGainReduction = -CurrentGainReduction;
            }

            // retrieve input sample
            double InputSample = InputSamples[CurrentChannel];
            double OutputSample = InputSample;

            // apply gain reduction
            OutputSample *= SideChain::decibel2level(CurrentGainReduction);

            // apply make-up gain
            OutputSample *= MakeupGain;

            // store output sample (used by side chain in alternative
            // feedback mode)
            OutputSamples.set(CurrentChannel, OutputSample);

            // listen to side-chain (already de-normalised)
            if (ListenToSidechain)
            {
                OutputSample = SidechainSamples[CurrentChannel];
            }
            // listen to compressor's output (already de-normalised)
            else
            {
                // dry shall be mixed in (test to save some processing
                // time)
                if (WetMixPercentage < 100)
                {
                    OutputSample = OutputSample * WetMix +
                                   InputSample * DryMix;
                }
            }

            // write output sample to main buffer
            MainBuffer.setSample(CurrentChannel, nSample, OutputSample);

            // store output sample for metering (reflects real output,
            // so when the user listens to the side-chain, the output
            // meter will also display the side-chain's level!)
            MeterOutputBuffer.copyFrom(CurrentChannel, MeterBufferPosition, MainBuffer, CurrentChannel, nSample, 1);
        }

        // update meter ballistics and increment buffer location
        updateMeterBallistics();
    }
}


void Compressor::updateMeterBallistics()
{
    // update metering buffer position
    ++MeterBufferPosition;

    // meter will only be updated when buffer is full
    if (MeterBufferPosition < MeterBufferSize)
    {
        return;
    }

    // reset buffer location
    MeterBufferPosition = 0;

    // loop over channels
    for (int CurrentChannel = 0; CurrentChannel < NumberOfChannels; ++CurrentChannel)
    {
        // determine peak levels
        double InputPeak = MeterInputBuffer.getMagnitude(CurrentChannel, 0, MeterBufferSize);
        double OutputPeak = MeterOutputBuffer.getMagnitude(CurrentChannel, 0, MeterBufferSize);

        // convert peak meter levels from linear scale to decibels
        InputPeak = SideChain::level2decibel(InputPeak);
        OutputPeak = SideChain::level2decibel(OutputPeak);

        // apply peak meter ballistics
        peakMeterBallistics(InputPeak, PeakMeterInputLevels.getReference(CurrentChannel));
        peakMeterBallistics(OutputPeak, PeakMeterOutputLevels.getReference(CurrentChannel));

        // note: due to the compressor's attack and release times,
        // there is no need to apply peak gain reduction ballistics

        // determine average levels
        double InputRms = MeterInputBuffer.getRMSLevel(CurrentChannel, 0, MeterBufferSize);
        double OutputRms = MeterOutputBuffer.getRMSLevel(CurrentChannel, 0, MeterBufferSize);

        // convert average meter levels from linear scale to
        // decibels
        InputRms = SideChain::level2decibel(InputRms);
        OutputRms = SideChain::level2decibel(OutputRms);

        // apply average meter ballistics
        averageMeterBallistics(InputRms, AverageMeterInputLevels.getReference(CurrentChannel));
        averageMeterBallistics(OutputRms, AverageMeterOutputLevels.getReference(CurrentChannel));
    }
}


void Compressor::peakMeterBallistics(double PeakLevelCurrent, double &PeakLevelOld)
/*  Calculate ballistics for peak meter levels.

    PeakLevelCurrent (double): current peak meter level in decibel

    PeakLevelOld (double): old peak meter reading in decibel (will be
    overwritten!)

    return value: none
*/
{
    // limit peak level
    if (PeakLevelCurrent >= 0.0)
    {
        // immediate rise time, but limit current peak to top mark
        PeakLevelOld = 0.0;
    }
    // apply rise time if peak level is above old level
    else if (PeakLevelCurrent >= PeakLevelOld)
    {
        // immediate rise time, so return current peak level as new
        // peak meter reading
        PeakLevelOld = PeakLevelCurrent;
    }
    // otherwise, apply fall time
    else
    {
        // apply fall time and return new peak meter reading (linear
        // fall time: 26 dB in 3 seconds)
        PeakLevelOld -= ReleaseCoefLinear;

        // make sure that meter doesn't fall below current level
        if (PeakLevelCurrent > PeakLevelOld)
        {
            PeakLevelOld = PeakLevelCurrent;
        }
    }
}


void Compressor::averageMeterBallistics(double AverageLevelCurrent, double &AverageLevelOld)
/*  Calculate ballistics for average meter levels and update readout.

    AverageLevelCurrent (double): current average meter level in
    decibel

    AverageLevelOld (double): old average meter reading in decibel
    (will be overwritten!)

    return value: none
*/
{
    // the meter reaches 99% of the final reading in 300 ms
    // (logarithmic)
    logMeterBallistics(0.300, BufferLength, AverageLevelCurrent, AverageLevelOld);
}


void Compressor::logMeterBallistics(double MeterInertia, double TimePassed, double Level, double &Readout)
/*  Calculate logarithmic meter ballistics.

    MeterInertia (double): time needed to reach 99% of the final
    readout (in fractional seconds)

    TimePassed (double): time that has passed since last update (in
    fractional seconds)

    Level (double): new meter level

    Readout (reference to double): old meter readout; this variable
    will be updated by this function

    return value: none
*/
{
    // we only have to calculate meter ballistics if meter level and
    // meter readout are not equal
    if (Level != Readout)
    {
        // Thanks to Bram de Jong for the code snippet!
        // (http://www.musicdsp.org/showone.php?id=136)
        //
        // rise and fall: 99% of final reading in "fMeterInertia" seconds
        double AttackReleaseCoef = pow(0.01, TimePassed / MeterInertia);
        Readout = AttackReleaseCoef * (Readout - Level) + Level;
    }
}
