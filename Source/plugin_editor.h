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

#include "build_id.h"

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
        SqueezerAudioProcessor &processor,
        SqueezerPluginParameters &PluginParameters,
        int NumberOfChannels);

    ~SqueezerAudioProcessorEditor();

    void buttonClicked(Button *Button) override;
    void sliderValueChanged(Slider *Slider) override;

    void actionListenerCallback(const String &Message) override;
    void updateParameter(int Index);

    void windowAboutCallback(int ModalResult);
    void windowSettingsCallback(int ModalResult);
    void windowSkinCallback(int ModalResult);

    virtual void paint(Graphics &g) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SqueezerAudioProcessorEditor);

    void applySkin_();
    void loadSkin_();

    bool IsInitialising_;
    int NumberOfChannels_;

    SqueezerAudioProcessor &PluginProcessor_;
    Skin skin_;

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

    DrawableButton ButtonRmsWindow_;
    DrawableButton ButtonDesignFeedback_;
    DrawableButton ButtonGainStageOptical_;

    DrawableButton ButtonKneeHard_;
    DrawableButton ButtonKneeMedium_;
    DrawableButton ButtonKneeSoft_;

    DrawableButton ButtonCurveLinear_;
    DrawableButton ButtonCurveSmoothDecoupled_;
    DrawableButton ButtonCurveSmoothBranching_;

    DrawableButton ButtonAutoMakeupGain_;
    DrawableButton ButtonSidechainExternal_;
    DrawableButton ButtonSidechainListen_;

    DrawableButton ButtonBypass_;
    DrawableButton ButtonReset_;

    DrawableButton ButtonSkin_;
    DrawableButton ButtonSettings_;
    DrawableButton ButtonAbout_;

#ifdef DEBUG
    DrawableComposite LabelDebug_;
#endif // DEBUG

    DrawableComposite DrawableBackground_;
};

#endif  // SQUEEZER_PLUGIN_EDITOR_H
