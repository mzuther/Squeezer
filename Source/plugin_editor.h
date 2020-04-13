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

#ifndef SQUEEZER_PLUGIN_EDITOR_H
#define SQUEEZER_PLUGIN_EDITOR_H

#include "FrutHeader.h"
#include "compressor.h"
#include "meter_bar_gain_reduction.h"
#include "meter_bar_level.h"
#include "plugin_processor.h"
#include "skin.h"


class SqueezerAudioProcessorEditor :
    public AudioProcessorEditor,
    public Button::Listener,
    public Slider::Listener,
    public ActionListener
{
public:
    SqueezerAudioProcessorEditor(
        SqueezerAudioProcessor *OwnerFilter,
        SqueezerPluginParameters *PluginParameters,
        int NumberOfChannels);

    ~SqueezerAudioProcessorEditor();

    void buttonClicked(Button *Button);
    void sliderValueChanged(Slider *Slider);

    void actionListenerCallback(const String &Message);
    void updateParameter(int Index);

    void windowAboutCallback(int ModalResult);
    void windowSettingsCallback(int ModalResult);
    void windowSkinCallback(int ModalResult);

    void resized();

private:
    JUCE_LEAK_DETECTOR(SqueezerAudioProcessorEditor);

    void applySkin_();
    void loadSkin_();

    bool IsInitialising_;
    int NumberOfChannels_;

    SqueezerAudioProcessor *PluginProcessor_;

    File SkinDirectory_;
    Skin CurrentSkin_;
    String CurrentSkinName_;

    OwnedArray<MeterBarLevel> InputLevelMeters_;
    OwnedArray<MeterBarLevel> OutputLevelMeters_;
    OwnedArray<MeterBarGainReduction> GainReductionMeters_;

    frut::skin::LookAndFeel_Frut_V3 customLookAndFeel_;

    std::unique_ptr<frut::widgets::SliderCombined> SliderThreshold_;
    std::unique_ptr<frut::widgets::SliderCombined> SliderRatio_;

    std::unique_ptr<frut::widgets::SliderCombined> SliderAttackRate_;
    std::unique_ptr<frut::widgets::SliderCombined> SliderReleaseRate_;

    std::unique_ptr<frut::widgets::SliderCombined> SliderInputTrim_;
    std::unique_ptr<frut::widgets::SliderCombined> SliderMakeupGain_;
    std::unique_ptr<frut::widgets::SliderCombined> SliderStereoLink_;
    std::unique_ptr<frut::widgets::SliderCombined> SliderWetMix_;

    std::unique_ptr<frut::widgets::SliderCombined> SliderSidechainHPFCutoff_;
    std::unique_ptr<frut::widgets::SliderCombined> SliderSidechainLPFCutoff_;

    ImageButton ButtonRmsWindow_;
    ImageButton ButtonDesignFeedback_;
    ImageButton ButtonGainStageOptical_;

    ImageButton ButtonKneeHard_;
    ImageButton ButtonKneeMedium_;
    ImageButton ButtonKneeSoft_;

    ImageButton ButtonCurveLinear_;
    ImageButton ButtonCurveSmoothDecoupled_;
    ImageButton ButtonCurveSmoothBranching_;

    ImageButton ButtonAutoMakeupGain_;
    ImageButton ButtonSidechainExternal_;
    ImageButton ButtonSidechainListen_;

    ImageButton ButtonBypass_;
    ImageButton ButtonReset_;

    ImageButton ButtonSkin_;
    ImageButton ButtonSettings_;
    ImageButton ButtonAbout_;

    ImageComponent LabelDebug_;

    ImageComponent BackgroundImage_;
};

#endif  // SQUEEZER_PLUGIN_EDITOR_H
