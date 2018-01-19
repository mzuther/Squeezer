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

#include "plugin_editor.h"


static void window_about_callback(
    int ModalResult,
    SqueezerAudioProcessorEditor *Editor)
{
    if (Editor != nullptr)
    {
        Editor->windowAboutCallback(ModalResult);
    }
}


static void window_settings_callback(
    int ModalResult,
    SqueezerAudioProcessorEditor *Editor)
{
    if (Editor != nullptr)
    {
        Editor->windowSettingsCallback(ModalResult);
    }
}


static void window_skin_callback(
    int ModalResult,
    SqueezerAudioProcessorEditor *Editor)
{
    if (Editor != nullptr)
    {
        Editor->windowSkinCallback(ModalResult);
    }
}


SqueezerAudioProcessorEditor::SqueezerAudioProcessorEditor(
    SqueezerAudioProcessor *OwnerFilter,
    SqueezerPluginParameters *PluginParameters,
    int NumberOfChannels)
    : AudioProcessorEditor(OwnerFilter)
{
    // the editor window does not have any transparent areas
    // (increases performance on redrawing)
    setOpaque(true);

    // prevent meter updates during initialisation
    IsInitialising_ = true;

    // The plug-in editor's size as well as the location of buttons
    // and labels will be set later on in this constructor.

    PluginProcessor_ = OwnerFilter;
    PluginProcessor_->addActionListener(this);

    NumberOfChannels_ = NumberOfChannels;

    ButtonBypass_.setButtonText("Bypass");
    ButtonBypass_.setColour(TextButton::buttonColourId,
                            Colours::grey);
    ButtonBypass_.setColour(TextButton::buttonOnColourId,
                            Colours::red);

    ButtonBypass_.addListener(this);
    addAndMakeVisible(&ButtonBypass_);


    ButtonDetectorRms_.setButtonText("RMS");
    ButtonDetectorRms_.setColour(TextButton::buttonColourId,
                                 Colours::grey);
    ButtonDetectorRms_.setColour(TextButton::buttonOnColourId,
                                 Colours::yellow);

    ButtonDetectorRms_.addListener(this);
    addAndMakeVisible(&ButtonDetectorRms_);


    ButtonDesignFeedback_.setButtonText("F.Back");
    ButtonDesignFeedback_.setColour(TextButton::buttonColourId,
                                    Colours::grey);
    ButtonDesignFeedback_.setColour(TextButton::buttonOnColourId,
                                    Colours::yellow);

    ButtonDesignFeedback_.addListener(this);
    addAndMakeVisible(&ButtonDesignFeedback_);


    ButtonGainStageOptical_.setButtonText("Opto");
    ButtonGainStageOptical_.setColour(TextButton::buttonColourId,
                                      Colours::grey);
    ButtonGainStageOptical_.setColour(TextButton::buttonOnColourId,
                                      Colours::yellow);

    ButtonGainStageOptical_.addListener(this);
    addAndMakeVisible(&ButtonGainStageOptical_);


    ButtonDetectorLinear_.setButtonText("Linear");
    ButtonDetectorLinear_.setRadioGroupId(1);
    ButtonDetectorLinear_.setColour(TextButton::buttonColourId,
                                    Colours::grey);
    ButtonDetectorLinear_.setColour(TextButton::buttonOnColourId,
                                    Colours::yellow.withRotatedHue(-0.08f));

    ButtonDetectorLinear_.addListener(this);
    addAndMakeVisible(&ButtonDetectorLinear_);


    ButtonDetectorSmoothDecoupled_.setButtonText("S-Curve");
    ButtonDetectorSmoothDecoupled_.setRadioGroupId(1);
    ButtonDetectorSmoothDecoupled_.setColour(TextButton::buttonColourId,
            Colours::grey);
    ButtonDetectorSmoothDecoupled_.setColour(TextButton::buttonOnColourId,
            Colours::yellow.withRotatedHue(-0.08f));

    ButtonDetectorSmoothDecoupled_.addListener(this);
    addAndMakeVisible(&ButtonDetectorSmoothDecoupled_);


    ButtonDetectorSmoothBranching_.setButtonText("Log");
    ButtonDetectorSmoothBranching_.setRadioGroupId(1);
    ButtonDetectorSmoothBranching_.setColour(TextButton::buttonColourId,
            Colours::grey);
    ButtonDetectorSmoothBranching_.setColour(TextButton::buttonOnColourId,
            Colours::yellow.withRotatedHue(-0.08f));

    ButtonDetectorSmoothBranching_.addListener(this);
    addAndMakeVisible(&ButtonDetectorSmoothBranching_);


    ButtonKneeHard_.setButtonText("Hard");
    ButtonKneeHard_.setRadioGroupId(2);
    ButtonKneeHard_.setColour(TextButton::buttonColourId,
                              Colours::grey);
    ButtonKneeHard_.setColour(TextButton::buttonOnColourId,
                              Colours::yellow.withRotatedHue(+0.08f));

    ButtonKneeHard_.addListener(this);
    addAndMakeVisible(&ButtonKneeHard_);

    ButtonKneeMedium_.setButtonText("Medium");
    ButtonKneeMedium_.setRadioGroupId(2);
    ButtonKneeMedium_.setColour(TextButton::buttonColourId,
                                Colours::grey);
    ButtonKneeMedium_.setColour(TextButton::buttonOnColourId,
                                Colours::yellow.withRotatedHue(+0.08f));

    ButtonKneeMedium_.addListener(this);
    addAndMakeVisible(&ButtonKneeMedium_);

    ButtonKneeSoft_.setButtonText("Soft");
    ButtonKneeSoft_.setRadioGroupId(2);
    ButtonKneeSoft_.setColour(TextButton::buttonColourId,
                              Colours::grey);
    ButtonKneeSoft_.setColour(TextButton::buttonOnColourId,
                              Colours::yellow.withRotatedHue(+0.08f));

    ButtonKneeSoft_.addListener(this);
    addAndMakeVisible(&ButtonKneeSoft_);

    int Index = SqueezerPluginParameters::selThreshold;
    int IndexSwitch = SqueezerPluginParameters::selThresholdSwitch;
    SliderThreshold_ = new frut::widget::SliderCombined(
        PluginParameters, Index, IndexSwitch);
    SliderThreshold_->setSliderColour(Colours::purple.brighter(0.2f));

    SliderThreshold_->addListener(this);
    SliderThreshold_->addButtonListener(this);
    addAndMakeVisible(SliderThreshold_);


    Index = SqueezerPluginParameters::selRatio;
    IndexSwitch = SqueezerPluginParameters::selRatioSwitch;
    SliderRatio_ = new frut::widget::SliderCombined(
        PluginParameters, Index, IndexSwitch);
    SliderRatio_->setSliderColour(Colours::purple.brighter(0.2f));

    SliderRatio_->addListener(this);
    SliderRatio_->addButtonListener(this);
    addAndMakeVisible(SliderRatio_);

    Index = SqueezerPluginParameters::selAttackRate;
    IndexSwitch = SqueezerPluginParameters::selAttackRateSwitch;
    SliderAttackRate_ = new frut::widget::SliderCombined(
        PluginParameters, Index, IndexSwitch);
    SliderAttackRate_->setSliderColour(Colours::yellow);

    SliderAttackRate_->addListener(this);
    SliderAttackRate_->addButtonListener(this);
    addAndMakeVisible(SliderAttackRate_);


    Index = SqueezerPluginParameters::selReleaseRate;
    IndexSwitch = SqueezerPluginParameters::selReleaseRateSwitch;
    SliderReleaseRate_ = new frut::widget::SliderCombined(
        PluginParameters, Index, IndexSwitch);
    SliderReleaseRate_->setSliderColour(Colours::yellow);

    SliderReleaseRate_->addListener(this);
    SliderReleaseRate_->addButtonListener(this);
    addAndMakeVisible(SliderReleaseRate_);


    ButtonAutoMakeupGain_.setButtonText("Auto MU");
    ButtonAutoMakeupGain_.setColour(TextButton::buttonColourId,
                                    Colours::grey);
    ButtonAutoMakeupGain_.setColour(TextButton::buttonOnColourId,
                                    Colours::yellow);

    ButtonAutoMakeupGain_.addListener(this);
    addAndMakeVisible(&ButtonAutoMakeupGain_);


    Index = SqueezerPluginParameters::selMakeupGain;
    IndexSwitch = SqueezerPluginParameters::selMakeupGainSwitch;
    SliderMakeupGain_ = new frut::widget::SliderCombined(
        PluginParameters, Index, IndexSwitch);
    SliderMakeupGain_->setSliderColour(Colours::blue.brighter(0.4f));

    SliderMakeupGain_->addListener(this);
    SliderMakeupGain_->addButtonListener(this);
    addAndMakeVisible(SliderMakeupGain_);


    Index = SqueezerPluginParameters::selWetMix;
    IndexSwitch = SqueezerPluginParameters::selWetMixSwitch;
    SliderWetMix_ = new frut::widget::SliderCombined(
        PluginParameters, Index, IndexSwitch);
    SliderWetMix_->setSliderColour(Colours::blue.brighter(0.4f));

    SliderWetMix_->addListener(this);
    SliderWetMix_->addButtonListener(this);
    addAndMakeVisible(SliderWetMix_);


    ButtonSidechainInput_.setButtonText("Ext");
    ButtonSidechainInput_.setColour(TextButton::buttonColourId,
                                    Colours::grey);
    ButtonSidechainInput_.setColour(TextButton::buttonOnColourId,
                                    Colours::red);

    ButtonSidechainInput_.addListener(this);
    addAndMakeVisible(&ButtonSidechainInput_);


    Index = SqueezerPluginParameters::selSidechainHPFCutoff;
    IndexSwitch = SqueezerPluginParameters::selSidechainHPFCutoffSwitch;
    SliderSidechainHPFCutoff_ = new frut::widget::SliderCombined(
        PluginParameters, Index, IndexSwitch);
    SliderSidechainHPFCutoff_->setSliderColour(Colours::green.brighter(0.1f));

    SliderSidechainHPFCutoff_->addListener(this);
    SliderSidechainHPFCutoff_->addButtonListener(this);
    addAndMakeVisible(SliderSidechainHPFCutoff_);


    Index = SqueezerPluginParameters::selSidechainLPFCutoff;
    IndexSwitch = SqueezerPluginParameters::selSidechainLPFCutoffSwitch;
    SliderSidechainLPFCutoff_ = new frut::widget::SliderCombined(
        PluginParameters, Index, IndexSwitch);
    SliderSidechainLPFCutoff_->setSliderColour(Colours::green.brighter(0.1f));

    SliderSidechainLPFCutoff_->addListener(this);
    SliderSidechainLPFCutoff_->addButtonListener(this);
    addAndMakeVisible(SliderSidechainLPFCutoff_);


    ButtonSidechainListen_.setButtonText("List");
    ButtonSidechainListen_.setColour(TextButton::buttonColourId,
                                     Colours::grey);
    ButtonSidechainListen_.setColour(TextButton::buttonOnColourId,
                                     Colours::red);

    ButtonSidechainListen_.addListener(this);
    addAndMakeVisible(&ButtonSidechainListen_);


#ifdef DEBUG
    LabelDebug_.setText("dbg", dontSendNotification);
    LabelDebug_.setColour(Label::textColourId,
                          Colours::red);
    LabelDebug_.setJustificationType(Justification::centred);
#endif


    ButtonSkin_.setButtonText("Skin");
    ButtonSkin_.setColour(TextButton::buttonColourId,
                          Colours::grey);
    ButtonSkin_.setColour(TextButton::buttonOnColourId,
                          Colours::yellow);

    ButtonSkin_.addListener(this);
    addAndMakeVisible(&ButtonSkin_);


    ButtonReset_.setButtonText("Reset");
    ButtonReset_.setColour(TextButton::buttonColourId,
                           Colours::grey);
    ButtonReset_.setColour(TextButton::buttonOnColourId,
                           Colours::red);

    ButtonReset_.addListener(this);
    addAndMakeVisible(&ButtonReset_);


    ButtonSettings_.setButtonText("Settings");
    ButtonSettings_.setColour(TextButton::buttonColourId,
                              Colours::grey);
    ButtonSettings_.setColour(TextButton::buttonOnColourId,
                              Colours::yellow);

    ButtonSettings_.addListener(this);
    addAndMakeVisible(&ButtonSettings_);


    ButtonAbout_.setButtonText("About");
    ButtonAbout_.setColour(TextButton::buttonColourId,
                           Colours::grey);
    ButtonAbout_.setColour(TextButton::buttonOnColourId,
                           Colours::yellow);

    ButtonAbout_.addListener(this);
    addAndMakeVisible(&ButtonAbout_);


#ifdef DEBUG
    // moves debug label to the back of the editor's z-plane to that
    // it doesn't overlay (and thus block) any other components
    addAndMakeVisible(LabelDebug_, 0);
#endif


    // prevent unnecessary redrawing of plugin editor
    BackgroundImage_.setOpaque(true);
    // moves background image to the back of the editor's z-plane to
    // that it doesn't overlay (and thus block) any other components
    addAndMakeVisible(BackgroundImage_, 0);

    updateParameter(SqueezerPluginParameters::selBypass);

    updateParameter(SqueezerPluginParameters::selDetectorRmsFilter);
    updateParameter(SqueezerPluginParameters::selDesign);
    updateParameter(SqueezerPluginParameters::selDetector);
    updateParameter(SqueezerPluginParameters::selGainStage);
    updateParameter(SqueezerPluginParameters::selKneeWidth);

    updateParameter(SqueezerPluginParameters::selThresholdSwitch);
    updateParameter(SqueezerPluginParameters::selThreshold);
    updateParameter(SqueezerPluginParameters::selRatioSwitch);
    updateParameter(SqueezerPluginParameters::selRatio);

    updateParameter(SqueezerPluginParameters::selAttackRateSwitch);
    updateParameter(SqueezerPluginParameters::selAttackRate);
    updateParameter(SqueezerPluginParameters::selReleaseRateSwitch);
    updateParameter(SqueezerPluginParameters::selReleaseRate);

    updateParameter(SqueezerPluginParameters::selAutoMakeupGain);
    updateParameter(SqueezerPluginParameters::selMakeupGainSwitch);
    updateParameter(SqueezerPluginParameters::selMakeupGain);
    updateParameter(SqueezerPluginParameters::selWetMixSwitch);
    updateParameter(SqueezerPluginParameters::selWetMix);

    updateParameter(SqueezerPluginParameters::selSidechainInput);
    updateParameter(SqueezerPluginParameters::selSidechainHPFCutoffSwitch);
    updateParameter(SqueezerPluginParameters::selSidechainHPFCutoff);
    updateParameter(SqueezerPluginParameters::selSidechainLPFCutoffSwitch);
    updateParameter(SqueezerPluginParameters::selSidechainLPFCutoff);
    updateParameter(SqueezerPluginParameters::selSidechainListen);

    // locate directory containing the skins
    SkinDirectory_ = SqueezerPluginParameters::getSkinDirectory();

    // apply skin to plug-in editor
    CurrentSkinName_ = PluginProcessor_->getParameterSkinName();
    loadSkin_();
}


SqueezerAudioProcessorEditor::~SqueezerAudioProcessorEditor()
{
    PluginProcessor_->removeActionListener(this);
}


void SqueezerAudioProcessorEditor::loadSkin_()
{
    File FileSkin = SkinDirectory_.getChildFile(CurrentSkinName_ + ".skin");

    if (!FileSkin.existsAsFile())
    {
        Logger::outputDebugString("[Skin] file \"" + FileSkin.getFileName() +
                                  "\" not found");

        CurrentSkinName_ = "Default";
        FileSkin = SkinDirectory_.getChildFile(CurrentSkinName_ + ".skin");
    }

    PluginProcessor_->setParameterSkinName(CurrentSkinName_);

    CurrentSkin_.loadSkin(FileSkin, NumberOfChannels_);

    // moves background image to the back of the editor's z-plane
    applySkin_();
}


void SqueezerAudioProcessorEditor::applySkin_()
{
    // update skin
    CurrentSkin_.updateSkin(NumberOfChannels_);

    // moves background image to the back of the editor's z-plane;
    // will also resize plug-in editor
    CurrentSkin_.setBackgroundImage(&BackgroundImage_, this);

    CurrentSkin_.placeComponent(&ButtonDetectorLinear_,
                                "button_detector_linear");
    CurrentSkin_.placeComponent(&ButtonDetectorSmoothDecoupled_,
                                "button_detector_smooth_decoupled");
    CurrentSkin_.placeComponent(&ButtonDetectorSmoothBranching_,
                                "button_detector_smooth_branching");

    CurrentSkin_.placeComponent(&ButtonDetectorRms_,
                                "button_detector_rms");
    CurrentSkin_.placeComponent(&ButtonDesignFeedback_,
                                "button_design_feedback");
    CurrentSkin_.placeComponent(&ButtonGainStageOptical_,
                                "button_gainstage_optical");

    CurrentSkin_.placeComponent(&ButtonKneeHard_,
                                "button_knee_hard");
    CurrentSkin_.placeComponent(&ButtonKneeMedium_,
                                "button_knee_medium");
    CurrentSkin_.placeComponent(&ButtonKneeSoft_,
                                "button_knee_soft");

    CurrentSkin_.placeComponent(SliderThreshold_,
                                "slider_threshold");
    CurrentSkin_.placeComponent(SliderRatio_,
                                "slider_ratio");

    CurrentSkin_.placeComponent(SliderAttackRate_,
                                "slider_attack_rate");
    CurrentSkin_.placeComponent(SliderReleaseRate_,
                                "slider_release_rate");

    CurrentSkin_.placeComponent(SliderSidechainHPFCutoff_,
                                "slider_sidechain_hpf_cutoff");
    CurrentSkin_.placeComponent(SliderSidechainLPFCutoff_,
                                "slider_sidechain_lpf_cutoff");
    CurrentSkin_.placeComponent(&ButtonSidechainInput_,
                                "button_sidechain_input");
    CurrentSkin_.placeComponent(&ButtonSidechainListen_,
                                "button_sidechain_listen");

    CurrentSkin_.placeComponent(SliderMakeupGain_,
                                "slider_makeup_gain");
    CurrentSkin_.placeComponent(SliderWetMix_,
                                "slider_wet_mix");

    CurrentSkin_.placeComponent(&ButtonAutoMakeupGain_,
                                "button_auto_makeup_gain");
    CurrentSkin_.placeComponent(&ButtonAbout_,
                                "button_about");
    CurrentSkin_.placeComponent(&ButtonSettings_,
                                "button_settings");
    CurrentSkin_.placeComponent(&ButtonSkin_,
                                "button_skin");
    CurrentSkin_.placeComponent(&ButtonBypass_,
                                "button_bypass");

    CurrentSkin_.placeComponent(&ButtonReset_,
                                "button_reset");

#ifdef DEBUG
    CurrentSkin_.placeComponent(&LabelDebug_,
                                "label_debug");
#endif

    // allow meter updates from now on
    IsInitialising_ = false;

    InputLevelMeters_.clear(true);
    OutputLevelMeters_.clear(true);
    GainReductionMeters_.clear(true);


    Array<Colour> ColoursLevelMeter;

    Colour ColourHigh = CurrentSkin_.getColourSetting(
                            "meter_colour_high",
                            0.00f);

    Colour ColourMedium = CurrentSkin_.getColourSetting(
                              "meter_colour_medium",
                              0.18f);

    Colour ColourLow = CurrentSkin_.getColourSetting(
                           "meter_colour_low",
                           0.30f);

    ColoursLevelMeter.add(ColourHigh);    // overload
    ColoursLevelMeter.add(ColourMedium);  // warning
    ColoursLevelMeter.add(ColourLow);     // fine


    Array<Colour> ColoursGainReduction;

    Colour ColourReductionNormal = CurrentSkin_.getColourSetting(
                                       "gain_reduction_meter_normal",
                                       0.58f);

    Colour ColourReductionSpecial = CurrentSkin_.getColourSetting(
                                        "gain_reduction_meter_special",
                                        0.18f);

    ColoursGainReduction.add(ColourReductionNormal);   // normal
    ColoursGainReduction.add(ColourReductionSpecial);  // special


    int SegmentHeight = CurrentSkin_.getIntegerSetting(
                            "meter_segment",
                            "height",
                            5);

    bool IsDiscreteMeter = true;
    int CrestFactor = 20;
    frut::widget::Orientation MeterOrientation =
        frut::widget::Orientation::vertical;

    for (int Channel = 0; Channel < NumberOfChannels_; ++Channel)
    {
        MeterBarLevel *InputLevelMeter = InputLevelMeters_.add(
                                             new MeterBarLevel());

        InputLevelMeter->create(CrestFactor,
                                MeterOrientation,
                                IsDiscreteMeter,
                                SegmentHeight,
                                ColoursLevelMeter);

        addAndMakeVisible(InputLevelMeter);

        MeterBarLevel *OutputLevelMeter = OutputLevelMeters_.add(
                                              new MeterBarLevel());

        OutputLevelMeter->create(CrestFactor,
                                 MeterOrientation,
                                 IsDiscreteMeter,
                                 SegmentHeight,
                                 ColoursLevelMeter);

        addAndMakeVisible(OutputLevelMeter);


        MeterBarGainReduction *GainReductionMeter = GainReductionMeters_.add(
                    new MeterBarGainReduction());

        GainReductionMeter->create(
            MeterOrientation,
            IsDiscreteMeter,
            SegmentHeight,
            ColoursGainReduction);

        addAndMakeVisible(GainReductionMeter);
    }

    if (NumberOfChannels_ == 1)
    {
        CurrentSkin_.placeMeterBar(InputLevelMeters_[0],
                                   "input_meter");

        CurrentSkin_.placeMeterBar(OutputLevelMeters_[0],
                                   "output_meter");

        CurrentSkin_.placeMeterBar(GainReductionMeters_[0],
                                   "gain_reduction_meter");
    }
    else
    {
        CurrentSkin_.placeMeterBar(InputLevelMeters_[0],
                                   "input_meter_left");
        CurrentSkin_.placeMeterBar(InputLevelMeters_[1],
                                   "input_meter_right");

        CurrentSkin_.placeMeterBar(OutputLevelMeters_[0],
                                   "output_meter_left");
        CurrentSkin_.placeMeterBar(OutputLevelMeters_[1],
                                   "output_meter_right");

        CurrentSkin_.placeMeterBar(GainReductionMeters_[0],
                                   "gain_reduction_meter_left");
        CurrentSkin_.placeMeterBar(GainReductionMeters_[1],
                                   "gain_reduction_meter_right");
    }
}


void SqueezerAudioProcessorEditor::windowAboutCallback(
    int ModalResult)
{
    ignoreUnused(ModalResult);

    // manually deactivate about button
    ButtonAbout_.setToggleState(false, dontSendNotification);
}


void SqueezerAudioProcessorEditor::windowSettingsCallback(
    int ModalResult)
{
    ignoreUnused(ModalResult);

    // manually deactivate about button
    ButtonSettings_.setToggleState(false, dontSendNotification);
}


void SqueezerAudioProcessorEditor::windowSkinCallback(
    int ModalResult)
{
    // manually deactivate skin button
    ButtonSkin_.setToggleState(false, dontSendNotification);

    // user has selected a skin
    if (ModalResult > 0)
    {
        // apply skin to plug-in editor
        loadSkin_();
    }
}


void SqueezerAudioProcessorEditor::actionListenerCallback(
    const String &Message)
{
    // "PC" --> parameter changed, followed by a hash and the
    // parameter's ID
    if (Message.startsWith("PC#"))
    {
        String StringIndex = Message.substring(3);
        int Index = StringIndex.getIntValue();
        jassert(Index >= 0);
        jassert(Index < PluginProcessor_->getNumParameters());

        if (PluginProcessor_->hasChanged(Index))
        {
            updateParameter(Index);
        }
    }
    // "UM" --> update meters
    else if (!Message.compare("UM"))
    {
        // prevent meter updates during initialisation
        if (!IsInitialising_)
        {
            for (int Channel = 0; Channel < NumberOfChannels_; ++Channel)
            {
                float AverageInputLevel =
                    PluginProcessor_->getAverageMeterInputLevel(Channel);
                float MaximumInputLevel =
                    PluginProcessor_->getMaximumInputLevel(Channel);

                float PeakInputLevel =
                    PluginProcessor_->getPeakMeterInputLevel(Channel);
                float PeakInputPeakLevel =
                    PluginProcessor_->getPeakMeterPeakInputLevel(Channel);

                InputLevelMeters_[Channel]->setLevels(
                    AverageInputLevel, MaximumInputLevel,
                    PeakInputLevel, PeakInputPeakLevel);

                float AverageOutputLevel =
                    PluginProcessor_->getAverageMeterOutputLevel(Channel);
                float MaximumOutputLevel =
                    PluginProcessor_->getMaximumOutputLevel(Channel);

                float PeakOutputLevel =
                    PluginProcessor_->getPeakMeterOutputLevel(Channel);
                float PeakOutputPeakLevel =
                    PluginProcessor_->getPeakMeterPeakOutputLevel(Channel);

                OutputLevelMeters_[Channel]->setLevels(
                    AverageOutputLevel, MaximumOutputLevel,
                    PeakOutputLevel, PeakOutputPeakLevel);

                float GainReduction =
                    PluginProcessor_->getGainReduction(Channel);
                float GainReductionMeterPeak =
                    PluginProcessor_->getGainReductionMeterPeak(Channel);

                // make sure gain reduction meter doesn't show anything
                // while there is no gain reduction
                GainReduction -= 0.01f;
                GainReductionMeterPeak -= 0.01f;

                GainReductionMeters_[Channel]->setNormalLevels(
                    GainReduction, GainReductionMeterPeak);
            }
        }
    }
    else
    {
        DBG("[Squeezer] Received unknown action message \"" + Message + "\".");
    }
}


void SqueezerAudioProcessorEditor::updateParameter(
    int Index)
{
    float FloatValue = PluginProcessor_->getParameter(Index);
    PluginProcessor_->clearChangeFlag(Index);

    switch (Index)
    {
    case SqueezerPluginParameters::selBypass:
        ButtonBypass_.setToggleState(FloatValue != 0.0f,
                                     dontSendNotification);
        break;

    case SqueezerPluginParameters::selDetectorRmsFilter:

        ButtonDetectorRms_.setToggleState(FloatValue != 0.0f,
                                          dontSendNotification);
        break;

    case SqueezerPluginParameters::selDesign:

        ButtonDesignFeedback_.setToggleState(FloatValue != 0.0f,
                                             dontSendNotification);
        break;

    case SqueezerPluginParameters::selGainStage:

        ButtonGainStageOptical_.setToggleState(FloatValue != 0.0f,
                                               dontSendNotification);
        break;

    case SqueezerPluginParameters::selDetector:

        if (FloatValue == (Compressor::DetectorLinear /
                           float(Compressor::NumberOfDetectors - 1)))
        {
            ButtonDetectorLinear_.setToggleState(true,
                                                 dontSendNotification);
        }
        else if (FloatValue == (Compressor::DetectorSmoothDecoupled /
                                float(Compressor::NumberOfDetectors - 1)))
        {
            ButtonDetectorSmoothDecoupled_.setToggleState(true,
                    dontSendNotification);
        }
        else
        {
            ButtonDetectorSmoothBranching_.setToggleState(true,
                    dontSendNotification);
        }

        break;

    case SqueezerPluginParameters::selKneeWidth:

        if (FloatValue == (Compressor::KneeHard /
                           float(Compressor::NumberOfKneeSettings - 1)))
        {
            ButtonKneeHard_.setToggleState(true,
                                           dontSendNotification);
        }
        else if (FloatValue == (Compressor::KneeMedium /
                                float(Compressor::NumberOfKneeSettings - 1)))
        {
            ButtonKneeMedium_.setToggleState(true,
                                             dontSendNotification);
        }
        else
        {
            ButtonKneeSoft_.setToggleState(true,
                                           dontSendNotification);
        }

        break;

    case SqueezerPluginParameters::selThresholdSwitch:
        SliderThreshold_->updateMode();
        break;

    case SqueezerPluginParameters::selThreshold:
        SliderThreshold_->setValue(FloatValue,
                                   dontSendNotification);
        break;

    case SqueezerPluginParameters::selRatioSwitch:
        SliderRatio_->updateMode();
        break;

    case SqueezerPluginParameters::selRatio:
        SliderRatio_->setValue(FloatValue,
                               dontSendNotification);

        {
            // prevent meter updates during initialisation
            if (!IsInitialising_)
            {
                float RealValue = SliderRatio_->getRealFloat();
                bool UseUpwardExpansion = (RealValue < 1.0f);

                for (int Channel = 0; Channel < NumberOfChannels_; ++Channel)
                {
                    GainReductionMeters_[Channel]->setUpwardExpansion(
                        UseUpwardExpansion);
                }
            }
        }

        break;

    case SqueezerPluginParameters::selAttackRateSwitch:
        SliderAttackRate_->updateMode();
        break;

    case SqueezerPluginParameters::selAttackRate:
        SliderAttackRate_->setValue(FloatValue,
                                    dontSendNotification);
        break;

    case SqueezerPluginParameters::selReleaseRateSwitch:
        SliderReleaseRate_->updateMode();
        break;

    case SqueezerPluginParameters::selReleaseRate:
        SliderReleaseRate_->setValue(FloatValue,
                                     dontSendNotification);
        break;

    case SqueezerPluginParameters::selAutoMakeupGain:
        ButtonAutoMakeupGain_.setToggleState(FloatValue != 0.0f,
                                             dontSendNotification);
        break;

    case SqueezerPluginParameters::selMakeupGainSwitch:
        SliderMakeupGain_->updateMode();
        break;

    case SqueezerPluginParameters::selMakeupGain:
        SliderMakeupGain_->setValue(FloatValue,
                                    dontSendNotification);
        break;

    case SqueezerPluginParameters::selWetMixSwitch:
        SliderWetMix_->updateMode();
        break;

    case SqueezerPluginParameters::selWetMix:
        SliderWetMix_->setValue(FloatValue,
                                dontSendNotification);
        break;

    case SqueezerPluginParameters::selSidechainInput:
        ButtonSidechainInput_.setToggleState(FloatValue == 0.0f,
                                             dontSendNotification);
        break;

    case SqueezerPluginParameters::selSidechainHPFCutoffSwitch:
        SliderSidechainHPFCutoff_->updateMode();
        break;

    case SqueezerPluginParameters::selSidechainHPFCutoff:
        SliderSidechainHPFCutoff_->setValue(FloatValue,
                                            dontSendNotification);
        break;

    case SqueezerPluginParameters::selSidechainLPFCutoffSwitch:
        SliderSidechainLPFCutoff_->updateMode();
        break;

    case SqueezerPluginParameters::selSidechainLPFCutoff:
        SliderSidechainLPFCutoff_->setValue(FloatValue,
                                            dontSendNotification);
        break;

    case SqueezerPluginParameters::selSidechainListen:
        ButtonSidechainListen_.setToggleState(FloatValue != 0.0f,
                                              dontSendNotification);
        break;

    default:
        DBG("[Squeezer] editor::updateParameter --> invalid index");
        break;
    }
}


void SqueezerAudioProcessorEditor::buttonClicked(
    Button *Button)
{
    if (Button == &ButtonBypass_)
    {
        PluginProcessor_->changeParameter(SqueezerPluginParameters::selBypass,
                                          !Button->getToggleState());
    }
    else if (Button == &ButtonDetectorRms_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selDetectorRmsFilter,
            !Button->getToggleState());
    }
    else if (Button == &ButtonDesignFeedback_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selDesign,
            !Button->getToggleState());
    }
    else if (Button == &ButtonGainStageOptical_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selGainStage,
            !Button->getToggleState());
    }
    else if (Button == &ButtonDetectorLinear_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selDetector,
            Compressor::DetectorLinear /
            float(Compressor::NumberOfDetectors - 1));
    }
    else if (Button == &ButtonDetectorSmoothDecoupled_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selDetector,
            Compressor::DetectorSmoothDecoupled /
            float(Compressor::NumberOfDetectors - 1));
    }
    else if (Button == &ButtonDetectorSmoothBranching_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selDetector,
            Compressor::DetectorSmoothBranching /
            float(Compressor::NumberOfDetectors - 1));
    }
    else if (Button == &ButtonKneeHard_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selKneeWidth,
            Compressor::KneeHard /
            float(Compressor::NumberOfKneeSettings - 1));
    }
    else if (Button == &ButtonKneeMedium_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selKneeWidth,
            Compressor::KneeMedium /
            float(Compressor::NumberOfKneeSettings - 1));
    }
    else if (Button == &ButtonKneeSoft_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selKneeWidth,
            Compressor::KneeSoft /
            float(Compressor::NumberOfKneeSettings - 1));
    }
    else if (Button == &ButtonAutoMakeupGain_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selAutoMakeupGain,
            !Button->getToggleState());
    }
    else if (Button == &ButtonSidechainInput_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selSidechainInput,
            Button->getToggleState());
    }
    else if (Button == &ButtonSidechainListen_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selSidechainListen,
            !Button->getToggleState());
    }
    else if (Button == &ButtonReset_)
    {
        PluginProcessor_->resetMeters();

        // apply skin to plug-in editor
        loadSkin_();
    }
    else if (Button == &ButtonSettings_)
    {
        // manually activate button
        Button->setToggleState(true,
                               dontSendNotification);

        int Width = 440;
        int Height = 155;
        String PluginSettings = PluginProcessor_->getParameters().trim();

        // prepare and launch dialog window
        DialogWindow *windowSettings =
            frut::widget::WindowSettingsContent::createDialogWindow(
                this, Width, Height, PluginSettings);

        // attach callback to dialog window
        ModalComponentManager::getInstance()->attachCallback(
            windowSettings, ModalCallbackFunction::forComponent(
                window_settings_callback, this));

        // manually deactivate button
        Button->setToggleState(false, dontSendNotification);
    }
    else if (Button == &ButtonSkin_)
    {
        // manually activate button (will be deactivated in dialog
        // window callback)
        Button->setToggleState(true, dontSendNotification);

        // prepare and launch dialog window
        DialogWindow *WindowSkin =
            frut::widget::WindowSkinContent::createDialogWindow(
                this, &CurrentSkinName_, SkinDirectory_);

        // attach callback to dialog window
        ModalComponentManager::getInstance()->attachCallback(
            WindowSkin, ModalCallbackFunction::forComponent(
                window_skin_callback, this));
    }
    else if (Button == &ButtonAbout_)
    {
        // manually activate button (will be deactivated in dialog
        // window callback)
        Button->setToggleState(true,
                               dontSendNotification);

        StringPairArray Chapters;

        String pluginNameAndVersion = String(ProjectInfo::projectName);
        pluginNameAndVersion += " v";
        pluginNameAndVersion += JucePlugin_VersionString;

#if JucePlugin_Build_AU
        pluginNameAndVersion += " (Audio Unit)";
#endif

#if JucePlugin_Build_LV2
        pluginNameAndVersion += " (LV2)";
#endif

#if JucePlugin_Build_VST
        pluginNameAndVersion += " (VST)";
#endif

        Chapters.set(
            pluginNameAndVersion,
            String(JucePlugin_Desc) + ".\n");

        Chapters.set(
            "Copyright",
            "(c) 2013-2016 Martin Zuther\n");

        Chapters.set(
            "Contributors",
            L"Dimitrios Giannoulis et al.\n"
            L"Filipe Coelho\n"
            L"Paul Kellet\n"
            L"Bram de Jong\n");

        Chapters.set(
            "Thanks",
            L"I want to thank all contributors and beta testers "
            L"and the open source community at large!\n\n"
            L"Thank you for using free software!\n");

        Chapters.set(
            "Libraries",
#ifdef LINUX
            L"ALSA\n"
#endif
#ifdef LINUX
            L"FreeType\n"
            L"JACK\n"
#endif
            L"JUCE\n"
#if JucePlugin_Build_LV2
            L"LV2\n"
#endif
#ifdef LINUX
            L"POSIX Threads\n"
            L"Xlib\n"
            L"Xext\n"
#endif
        );

#if JucePlugin_Build_VST
        // display trademarks (but only when necessary)
        Chapters.set(
            "Trademarks",
            L"VST PlugIn Technology by Steinberg Media Technologies\n");
#endif

#if JUCE_ASIO
        // display trademarks (but only when necessary)
        Chapters.set(
            "Trademarks",
            L"ASIO Driver Technology by Steinberg Media Technologies\n");
#endif

        Chapters.set(
            "License",
            L"This program is free software: you can redistribute it "
            L"and/or modify it under the terms of the GNU General "
            L"Public License as published by the Free Software "
            L"Foundation, either version 3 of the License, or (at "
            L"your option) any later version.\n\n"

            L"This program is distributed in the hope that it will "
            L"be useful, but WITHOUT ANY WARRANTY; without even "
            L"the implied warranty of MERCHANTABILITY or FITNESS "
            L"FOR A PARTICULAR PURPOSE.  See the GNU General Public "
            L"License for more details.\n\n"

            L"You should have received a copy of the GNU General "
            L"Public License along with this program.  If not, "
            L"see <http://www.gnu.org/licenses/>.\n\n"

            L"Thank you for using free software!");

        // prepare and launch dialog window
        int Width = 270;
        int Height = 540;

        DialogWindow *windowAbout =
            frut::widget::WindowAboutContent::createDialogWindow(
                this, Width, Height, Chapters);

        // attach callback to dialog window
        ModalComponentManager::getInstance()->attachCallback(
            windowAbout, ModalCallbackFunction::forComponent(
                window_about_callback, this));
    }
    else
    {
        float FloatValue = Button->getToggleState() ? 1.0f : 0.0f;
        frut::widget::SliderCombined *Slider =
            dynamic_cast<frut::widget::SliderCombined *>(
                Button->getParentComponent());

        if (Slider == SliderThreshold_)
        {
            PluginProcessor_->changeParameter(
                SqueezerPluginParameters::selThresholdSwitch,
                FloatValue);
        }
        else if (Slider == SliderRatio_)
        {
            PluginProcessor_->changeParameter(
                SqueezerPluginParameters::selRatioSwitch,
                FloatValue);
        }
        else if (Slider == SliderAttackRate_)
        {
            PluginProcessor_->changeParameter(
                SqueezerPluginParameters::selAttackRateSwitch,
                FloatValue);
        }
        else if (Slider == SliderReleaseRate_)
        {
            PluginProcessor_->changeParameter(
                SqueezerPluginParameters::selReleaseRateSwitch,
                FloatValue);
        }
        else if (Slider == SliderMakeupGain_)
        {
            PluginProcessor_->changeParameter(
                SqueezerPluginParameters::selMakeupGainSwitch,
                FloatValue);
        }
        else if (Slider == SliderWetMix_)
        {
            PluginProcessor_->changeParameter(
                SqueezerPluginParameters::selWetMixSwitch,
                FloatValue);
        }
        else if (Slider == SliderSidechainHPFCutoff_)
        {
            PluginProcessor_->changeParameter(
                SqueezerPluginParameters::selSidechainHPFCutoffSwitch,
                FloatValue);
        }
        else if (Slider == SliderSidechainLPFCutoff_)
        {
            PluginProcessor_->changeParameter(
                SqueezerPluginParameters::selSidechainLPFCutoffSwitch,
                FloatValue);
        }
        else
        {
            DBG("[Squeezer] editor::buttonClicked --> invalid button");
        }
    }
}


void SqueezerAudioProcessorEditor::sliderValueChanged(
    Slider *Slider)
{
    float FloatValue = (float) Slider->getValue();

    if (Slider == SliderThreshold_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selThreshold,
            FloatValue);
    }
    else if (Slider == SliderRatio_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selRatio,
            FloatValue);
    }
    else if (Slider == SliderAttackRate_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selAttackRate,
            FloatValue);
    }
    else if (Slider == SliderReleaseRate_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selReleaseRate,
            FloatValue);
    }
    else if (Slider == SliderMakeupGain_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selMakeupGain,
            FloatValue);
    }
    else if (Slider == SliderWetMix_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selWetMix,
            FloatValue);
    }
    else if (Slider == SliderSidechainHPFCutoff_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selSidechainHPFCutoff,
            FloatValue);
    }
    else if (Slider == SliderSidechainLPFCutoff_)
    {
        PluginProcessor_->changeParameter(
            SqueezerPluginParameters::selSidechainLPFCutoff,
            FloatValue);
    }
    else
    {
        DBG("[Squeezer] editor::sliderValueChanged --> invalid slider");
    }
}


void SqueezerAudioProcessorEditor::resized()
{
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
