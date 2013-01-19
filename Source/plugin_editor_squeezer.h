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

#ifndef __PLUGINEDITOR_SQUEEZER_H__
#define __PLUGINEDITOR_SQUEEZER_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "meter_bar_gain_reduction.h"
#include "plugin_processor_squeezer.h"
#include "slider_continuous.h"
#include "slider_switch.h"
#include "window_about.h"


//==============================================================================
/**
*/
class SqueezerAudioProcessorEditor : public AudioProcessorEditor, public ButtonListener, public SliderListener, public ActionListener
{
public:
    SqueezerAudioProcessorEditor(SqueezerAudioProcessor* ownerFilter, SqueezerPluginParameters* parameters);
    ~SqueezerAudioProcessorEditor();

    void buttonClicked(Button* button);
    void sliderValueChanged(Slider* slider);

    void actionListenerCallback(const String& message);
    void changeParameter(int nIndex);
    void changeParameter(int nIndex, float fValue);

    //==============================================================================
    // This is just a standard Juce paint method...
    void paint(Graphics& g);
    void resized();

private:
    JUCE_LEAK_DETECTOR(SqueezerAudioProcessorEditor);

    void resizeEditor();

    int nChannels;
    int nRightColumnStart;
    int nHeight;

    int nInputChannels;

    SqueezerAudioProcessor* pProcessor;

    MeterBarGainReduction** pGainReductionMeters;

    TextButton* ButtonDesignModern;
    TextButton* ButtonDesignVintage;

    SliderSwitch* SliderThresholdSwitch;
    SliderSwitch* SliderRatioSwitch;

    TextButton* ButtonEnvelopeTypeLinear;
    TextButton* ButtonEnvelopeTypeLogarithmic;

    SliderSwitch* SliderAttackRateSwitch;
    SliderSwitch* SliderReleaseRateSwitch;

    SliderContinuous* SliderReleaseRateContinuous;

    SliderSwitch* SliderStereoLinkSwitch;

    SliderSwitch* SliderInputGainSwitch;
    SliderSwitch* SliderOutputGainSwitch;
    SliderSwitch* SliderWetMixSwitch;

    TextButton* ButtonBypass;
    TextButton* ButtonAbout;

    Label* LabelDebug;
};


#endif  // __PLUGINEDITOR_SQUEEZER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
