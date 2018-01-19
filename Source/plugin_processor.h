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

#ifndef __SQUEEZER_PLUGINPROCESSOR_H__
#define __SQUEEZER_PLUGINPROCESSOR_H__

#include "FrutHeader.h"
#include "compressor.h"
#include "plugin_parameters.h"


//============================================================================
class SqueezerAudioProcessor : public AudioProcessor, public ActionBroadcaster
{
public:
    //==========================================================================

    SqueezerAudioProcessor();
    ~SqueezerAudioProcessor();

    //==========================================================================
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock(AudioBuffer<float> &buffer, MidiBuffer &midiMessages);

    //==========================================================================
    AudioProcessorEditor *createEditor();
    bool hasEditor() const;

    //==========================================================================
    int getNumParameters();
    const String getParameterName(int nIndex);
    const String getParameterText(int nIndex);

    String getParameters();
    float getParameter(int nIndex);
    void changeParameter(int nIndex, float fValue);
    void setParameter(int nIndex, float fValue);

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

    //==========================================================================
    const String getName() const;

    bool acceptsMidi() const;
    bool producesMidi() const;

    double getTailLengthSeconds() const;

    //==========================================================================
    int getNumPrograms();

    int getCurrentProgram();
    void setCurrentProgram(int nIndex);

    const String getProgramName(int nIndex);
    void changeProgramName(int nIndex, const String &newName);

    //==========================================================================
    void getStateInformation(MemoryBlock &destData);
    void setStateInformation(const void *data, int sizeInBytes);

    //==========================================================================

private:
    JUCE_LEAK_DETECTOR(SqueezerAudioProcessor);

    AudioBuffer<float> MainInput;
    AudioBuffer<float> SideChainInput;

    bool hasSideChain;

    SqueezerPluginParameters pluginParameters;
    ScopedPointer<Compressor> pCompressor;

    bool bSampleRateIsValid;
};

AudioProcessor *JUCE_CALLTYPE createPluginFilter();

#endif  // __SQUEEZER_PLUGINPROCESSOR_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
