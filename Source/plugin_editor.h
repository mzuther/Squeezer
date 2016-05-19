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

#ifndef __PLUGINEDITOR_SQUEEZER_H__
#define __PLUGINEDITOR_SQUEEZER_H__

#include "FrutHeader.h"
#include "compressor.h"
#include "meter_bar_gain_reduction.h"
#include "meter_bar_level.h"
#include "plugin_processor.h"


//==============================================================================
/**
*/
class SqueezerAudioProcessorEditor : public AudioProcessorEditor, public ButtonListener, public SliderListener, public ActionListener
{
public:
    SqueezerAudioProcessorEditor(SqueezerAudioProcessor *ownerFilter, SqueezerPluginParameters *parameters, int channels);
    ~SqueezerAudioProcessorEditor();

    void buttonClicked(Button *button);
    void sliderValueChanged(Slider *slider);

    void actionListenerCallback(const String &strMessage);
    void updateParameter(int nIndex);

    void windowAboutCallback(int modalResult);
    void windowSettingsCallback(int modalResult);

    //==============================================================================
    // This is just a standard Juce paint method...
    void paint(Graphics &g);
    void resized();

private:
    JUCE_LEAK_DETECTOR(SqueezerAudioProcessorEditor);

    void resizeEditor();

    int nChannels;
    int nHeight;
    int nWidth;

    int nInputChannels;

    SqueezerAudioProcessor *pProcessor;

    OwnedArray<MeterBarLevel> p_arrInputLevelMeters;
    OwnedArray<MeterBarLevel> p_arrOutputLevelMeters;
    OwnedArray<MeterBarGainReduction> p_arrGainReductionMeters;

    TextButton ButtonDetectorRms;
    TextButton ButtonDesignFeedBack;
    TextButton ButtonGainStageOptical;

    TextButton ButtonDetectorLinear;
    TextButton ButtonDetectorSmoothDecoupled;
    TextButton ButtonDetectorSmoothBranching;

    ScopedPointer<frut::widget::SliderCombined> SliderThresholdCombined;
    ScopedPointer<frut::widget::SliderCombined> SliderRatioCombined;
    ScopedPointer<frut::widget::SliderCombined> SliderKneeWidthCombined;

    ScopedPointer<frut::widget::SliderCombined> SliderAttackRateCombined;
    ScopedPointer<frut::widget::SliderCombined> SliderReleaseRateCombined;

#ifdef SQUEEZER_STEREO
    ScopedPointer<frut::widget::SliderCombined> SliderStereoLinkCombined;
#endif

    ScopedPointer<frut::widget::SliderCombined> SliderInputGainCombined;
    TextButton ButtonAutoMakeupGain;
    ScopedPointer<frut::widget::SliderCombined> SliderMakeupGainCombined;
    ScopedPointer<frut::widget::SliderCombined> SliderWetMixCombined;

    TextButton ButtonSidechainFilterState;
    ScopedPointer<frut::widget::SliderCombined> SliderSidechainFilterCutoffCombined;
    ScopedPointer<frut::widget::SliderSwitchLinearBar> SliderSidechainFilterGain;
    TextButton ButtonSidechainListen;

    TextButton ButtonBypass;
    TextButton ButtonResetMeters;
    TextButton ButtonSettings;
    TextButton ButtonAbout;

    Label LabelDebug;
};


#endif  // __PLUGINEDITOR_SQUEEZER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
