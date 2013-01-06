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

#ifndef __SQUEEZER_PLUGINPROCESSOR_H__
#define __SQUEEZER_PLUGINPROCESSOR_H__

class SqueezerAudioProcessor;
class GainReducer;
class SqueezerPluginParameters;

#include "../JuceLibraryCode/JuceHeader.h"
#include "gain_reducer.h"
#include "plugin_parameters_squeezer.h"


//============================================================================
class SqueezerAudioProcessor  : public AudioProcessor, public ActionBroadcaster
{
public:
    //==========================================================================

    SqueezerAudioProcessor();
    ~SqueezerAudioProcessor();

    void addActionListenerParameters(ActionListener* listener) throw();
    void removeActionListenerParameters(ActionListener* listener) throw();

    //==========================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==========================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;

    //==========================================================================
    int getNumParameters();

    float getParameter(int nIndex);

    void setParameter(int nIndex, float newValue);

    const String getParameterName(int nIndex);
    const String getParameterText(int nIndex);

    void changeParameter(int nIndex, float fNewValue);

    void clearChangeFlag(int nIndex);
    bool hasChanged(int nIndex);

    //==========================================================================
    const String getName() const;

    const String getInputChannelName(int channelIndex) const;
    const String getOutputChannelName(int channelIndex) const;
    bool isInputChannelStereoPair(int nIndex) const;
    bool isOutputChannelStereoPair(int nIndex) const;

    bool acceptsMidi() const;
    bool producesMidi() const;

    //==========================================================================
    int getNumPrograms();
    int getNumChannels();
    int getCurrentProgram();
    void setCurrentProgram(int nIndex);
    const String getProgramName(int nIndex);
    void changeProgramName(int nIndex, const String& newName);

    //==========================================================================
    void getStateInformation(MemoryBlock& destData);
    void setStateInformation(const void* data, int sizeInBytes);

    //==========================================================================
    juce_UseDebuggingNewOperator

private:
    JUCE_LEAK_DETECTOR(SqueezerAudioProcessor);

    SqueezerPluginParameters* pPluginParameters;

    int nNumInputChannels;
    bool isStereo;
    bool bSampleRateIsValid;

    GainReducer** pGainReducer;

    int nSamplesInBuffer;
    float fProcessedSeconds;
};

#endif  // __SQUEEZER_PLUGINPROCESSOR_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
