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

#pragma once

#include "FrutHeader.h"
#include "compressor.h"
#include "plugin_parameters.h"


class SqueezerAudioProcessor : public AudioProcessor, public ActionBroadcaster
{
public:
    SqueezerAudioProcessor();
    ~SqueezerAudioProcessor();

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void reset() override;

    void processBlock(AudioBuffer<float> &buffer,
                      MidiBuffer &midiMessages) override;
    void processBlock(AudioBuffer<double> &buffer,
                      MidiBuffer &midiMessages) override;
    void process(AudioBuffer<double> &buffer);

    AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    int getNumParameters() override;
    const String getParameterName(int nIndex) override;
    const String getParameterText(int nIndex) override;

    String getParameters();
    float getParameter(int nIndex) override;
    void changeParameter(int nIndex, float fValue);
    void setParameter(int nIndex, float fValue) override;

    void clearChangeFlag(int nIndex);
    bool hasChanged(int nIndex);
    void updateParameters(bool bIncludeHiddenParameters);

    String getParameterSkinName();
    void setParameterSkinName(const String &strSkinName);

    void resetMeters();

    float getGainReduction(int nChannel);
    float getGainReductionMeterPeak(int nChannel);

    float getPeakMeterInputLevel(int nChannel);
    float getPeakMeterOutputLevel(int nChannel);

    float getPeakMeterPeakInputLevel(int nChannel);
    float getPeakMeterPeakOutputLevel(int nChannel);

    float getMaximumInputLevel(int nChannel);
    float getMaximumOutputLevel(int nChannel);

    float getAverageMeterInputLevel(int nChannel);
    float getAverageMeterOutputLevel(int nChannel);

    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;

    double getTailLengthSeconds() const override;

    int getNumPrograms() override;

    int getCurrentProgram() override;
    void setCurrentProgram(int nIndex) override;

    const String getProgramName(int nIndex) override;
    void changeProgramName(int nIndex, const String &newName) override;

    void getStateInformation(MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

private:
    JUCE_LEAK_DETECTOR(SqueezerAudioProcessor);

    static BusesProperties getBusesProperties();

    AudioBuffer<double> mainInput_;
    AudioBuffer<double> sideChainInput_;

    frut::dsp::Dither dither_;

    bool hasSideChain_;

    SqueezerPluginParameters pluginParameters_;
    ScopedPointer<Compressor> compressor_;

    bool sampleRateIsValid_;
};

AudioProcessor *JUCE_CALLTYPE createPluginFilter();


// Local Variables:
// ispell-local-dictionary: "british"
// End:
