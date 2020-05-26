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
    SqueezerAudioProcessor &processor,
    SqueezerPluginParameters &PluginParameters,
    int NumberOfChannels)
    : AudioProcessorEditor(&processor),
      PluginProcessor_(processor),

      ButtonRmsWindow_("Rms Window", DrawableButton::ButtonStyle::ImageRaw),
      ButtonDesignFeedback_("Design Feedback", DrawableButton::ButtonStyle::ImageRaw),
      ButtonGainStageOptical_("Gain Stage Optical", DrawableButton::ButtonStyle::ImageRaw),

      ButtonKneeHard_("Knee Hard", DrawableButton::ButtonStyle::ImageRaw),
      ButtonKneeMedium_("Knee Medium", DrawableButton::ButtonStyle::ImageRaw),
      ButtonKneeSoft_("Knee Soft", DrawableButton::ButtonStyle::ImageRaw),

      ButtonCurveLinear_("Curve Linear", DrawableButton::ButtonStyle::ImageRaw),
      ButtonCurveSmoothDecoupled_("Curve Smooth Decoupled", DrawableButton::ButtonStyle::ImageRaw),
      ButtonCurveSmoothBranching_("Curve Smooth Branching", DrawableButton::ButtonStyle::ImageRaw),

      ButtonAutoMakeupGain_("Auto Makeup Gain", DrawableButton::ButtonStyle::ImageRaw),
      ButtonSidechainExternal_("Sidechain External", DrawableButton::ButtonStyle::ImageRaw),
      ButtonSidechainListen_("Sidechain Listen", DrawableButton::ButtonStyle::ImageRaw),

      ButtonBypass_("Bypass", DrawableButton::ButtonStyle::ImageRaw),
      ButtonReset_("Reset", DrawableButton::ButtonStyle::ImageRaw),

      ButtonSkin_("Skin", DrawableButton::ButtonStyle::ImageRaw),
      ButtonSettings_("Settings", DrawableButton::ButtonStyle::ImageRaw),
      ButtonAbout_("About", DrawableButton::ButtonStyle::ImageRaw)
{
    // load look and feel
    setLookAndFeel(&customLookAndFeel_);

    // the editor window does not have any transparent areas
    // (increases performance on redrawing)
    setOpaque(true);

    // prevent meter updates during initialisation
    IsInitialising_ = true;

    // The plug-in editor's size as well as the location of buttons
    // and labels will be set later on in this constructor.

    PluginProcessor_.addActionListener(this);

    NumberOfChannels_ = NumberOfChannels;

    SliderThreshold_ = std::make_unique<frut::widgets::SliderCombined>(
                           PluginParameters,
                           SqueezerPluginParameters::selThreshold,
                           SqueezerPluginParameters::selThresholdSwitch);

    SliderThreshold_->addListener(this);
    SliderThreshold_->addButtonListener(this);
    addAndMakeVisible(SliderThreshold_.get());


    SliderRatio_ = std::make_unique<frut::widgets::SliderCombined>(
                       PluginParameters,
                       SqueezerPluginParameters::selRatio,
                       SqueezerPluginParameters::selRatioSwitch);

    SliderRatio_->addListener(this);
    SliderRatio_->addButtonListener(this);
    addAndMakeVisible(SliderRatio_.get());


    SliderAttackRate_ = std::make_unique<frut::widgets::SliderCombined>(
                            PluginParameters,
                            SqueezerPluginParameters::selAttackRate,
                            SqueezerPluginParameters::selAttackRateSwitch);

    SliderAttackRate_->addListener(this);
    SliderAttackRate_->addButtonListener(this);
    addAndMakeVisible(SliderAttackRate_.get());


    SliderReleaseRate_ = std::make_unique<frut::widgets::SliderCombined>(
                             PluginParameters,
                             SqueezerPluginParameters::selReleaseRate,
                             SqueezerPluginParameters::selReleaseRateSwitch);

    SliderReleaseRate_->addListener(this);
    SliderReleaseRate_->addButtonListener(this);
    addAndMakeVisible(SliderReleaseRate_.get());


    SliderInputTrim_ = std::make_unique<frut::widgets::SliderCombined>(
                           PluginParameters,
                           SqueezerPluginParameters::selInputTrim,
                           SqueezerPluginParameters::selInputTrimSwitch);

    SliderInputTrim_->addListener(this);
    SliderInputTrim_->addButtonListener(this);
    addAndMakeVisible(SliderInputTrim_.get());


    SliderMakeupGain_ = std::make_unique<frut::widgets::SliderCombined>(
                            PluginParameters,
                            SqueezerPluginParameters::selMakeupGain,
                            SqueezerPluginParameters::selMakeupGainSwitch);

    SliderMakeupGain_->addListener(this);
    SliderMakeupGain_->addButtonListener(this);
    addAndMakeVisible(SliderMakeupGain_.get());


    SliderStereoLink_ = std::make_unique<frut::widgets::SliderCombined>(
                            PluginParameters,
                            SqueezerPluginParameters::selStereoLink,
                            SqueezerPluginParameters::selStereoLinkSwitch);

    SliderStereoLink_->addListener(this);
    SliderStereoLink_->addButtonListener(this);
    addAndMakeVisible(SliderStereoLink_.get());


    SliderWetMix_ = std::make_unique<frut::widgets::SliderCombined>(
                        PluginParameters,
                        SqueezerPluginParameters::selWetMix,
                        SqueezerPluginParameters::selWetMixSwitch);

    SliderWetMix_->addListener(this);
    SliderWetMix_->addButtonListener(this);
    addAndMakeVisible(SliderWetMix_.get());


    SliderSidechainHPFCutoff_ = std::make_unique<frut::widgets::SliderCombined>(
                                    PluginParameters,
                                    SqueezerPluginParameters::selSidechainHPFCutoff,
                                    SqueezerPluginParameters::selSidechainHPFCutoffSwitch);

    SliderSidechainHPFCutoff_->addListener(this);
    SliderSidechainHPFCutoff_->addButtonListener(this);
    addAndMakeVisible(SliderSidechainHPFCutoff_.get());


    SliderSidechainLPFCutoff_ = std::make_unique<frut::widgets::SliderCombined>(
                                    PluginParameters,
                                    SqueezerPluginParameters::selSidechainLPFCutoff,
                                    SqueezerPluginParameters::selSidechainLPFCutoffSwitch);

    SliderSidechainLPFCutoff_->addListener(this);
    SliderSidechainLPFCutoff_->addButtonListener(this);
    addAndMakeVisible(SliderSidechainLPFCutoff_.get());


    ButtonBypass_.addListener(this);
    addAndMakeVisible(&ButtonBypass_);

    ButtonRmsWindow_.addListener(this);
    addAndMakeVisible(&ButtonRmsWindow_);

    ButtonDesignFeedback_.addListener(this);
    addAndMakeVisible(&ButtonDesignFeedback_);

    ButtonGainStageOptical_.addListener(this);
    addAndMakeVisible(&ButtonGainStageOptical_);

    ButtonCurveLinear_.setRadioGroupId(1);
    ButtonCurveLinear_.addListener(this);
    addAndMakeVisible(&ButtonCurveLinear_);

    ButtonCurveSmoothDecoupled_.setRadioGroupId(1);
    ButtonCurveSmoothDecoupled_.addListener(this);
    addAndMakeVisible(&ButtonCurveSmoothDecoupled_);

    ButtonCurveSmoothBranching_.setRadioGroupId(1);
    ButtonCurveSmoothBranching_.addListener(this);
    addAndMakeVisible(&ButtonCurveSmoothBranching_);

    ButtonKneeHard_.setRadioGroupId(2);
    ButtonKneeHard_.addListener(this);
    addAndMakeVisible(&ButtonKneeHard_);

    ButtonKneeMedium_.setRadioGroupId(2);
    ButtonKneeMedium_.addListener(this);
    addAndMakeVisible(&ButtonKneeMedium_);

    ButtonKneeSoft_.setRadioGroupId(2);
    ButtonKneeSoft_.addListener(this);
    addAndMakeVisible(&ButtonKneeSoft_);

    ButtonAutoMakeupGain_.addListener(this);
    addAndMakeVisible(&ButtonAutoMakeupGain_);

    ButtonSidechainExternal_.addListener(this);
    addAndMakeVisible(&ButtonSidechainExternal_);

    ButtonSidechainListen_.addListener(this);
    addAndMakeVisible(&ButtonSidechainListen_);

    ButtonSkin_.addListener(this);
    addAndMakeVisible(&ButtonSkin_);

    ButtonReset_.addListener(this);
    addAndMakeVisible(&ButtonReset_);

    ButtonSettings_.addListener(this);
    addAndMakeVisible(&ButtonSettings_);

    ButtonAbout_.addListener(this);
    addAndMakeVisible(&ButtonAbout_);

#ifdef DEBUG
    // moves debug label to the back of the editor's z-plane to that
    // it doesn't overlay (and thus block) any other components
    addAndMakeVisible(LabelDebug_.get(), 0);
#endif

    // moves background image to the back of the editor's z-plane so
    // that it doesn't overlay (and thus block) any other components
    addAndMakeVisible(DrawableBackground_.get(), 0);

    updateParameter(SqueezerPluginParameters::selBypass);

    updateParameter(SqueezerPluginParameters::selThresholdSwitch);
    updateParameter(SqueezerPluginParameters::selThreshold);
    updateParameter(SqueezerPluginParameters::selRatioSwitch);
    updateParameter(SqueezerPluginParameters::selRatio);

    updateParameter(SqueezerPluginParameters::selAttackRateSwitch);
    updateParameter(SqueezerPluginParameters::selAttackRate);
    updateParameter(SqueezerPluginParameters::selReleaseRateSwitch);
    updateParameter(SqueezerPluginParameters::selReleaseRate);

    updateParameter(SqueezerPluginParameters::selInputTrimSwitch);
    updateParameter(SqueezerPluginParameters::selInputTrim);
    updateParameter(SqueezerPluginParameters::selMakeupGainSwitch);
    updateParameter(SqueezerPluginParameters::selMakeupGain);

    updateParameter(SqueezerPluginParameters::selStereoLinkSwitch);
    updateParameter(SqueezerPluginParameters::selStereoLink);
    updateParameter(SqueezerPluginParameters::selWetMixSwitch);
    updateParameter(SqueezerPluginParameters::selWetMix);

    updateParameter(SqueezerPluginParameters::selSidechainHPFCutoffSwitch);
    updateParameter(SqueezerPluginParameters::selSidechainHPFCutoff);
    updateParameter(SqueezerPluginParameters::selSidechainLPFCutoffSwitch);
    updateParameter(SqueezerPluginParameters::selSidechainLPFCutoff);

    updateParameter(SqueezerPluginParameters::selRmsWindowSize);
    updateParameter(SqueezerPluginParameters::selDesign);
    updateParameter(SqueezerPluginParameters::selCurveType);
    updateParameter(SqueezerPluginParameters::selGainStage);
    updateParameter(SqueezerPluginParameters::selKneeWidth);

    updateParameter(SqueezerPluginParameters::selAutoMakeupGain);
    updateParameter(SqueezerPluginParameters::selSidechainInput);
    updateParameter(SqueezerPluginParameters::selSidechainListen);

    // locate directory containing the skins
    SkinDirectory_ = SqueezerPluginParameters::getSkinDirectory();

    // apply skin to plug-in editor
    CurrentSkinName_ = PluginProcessor_.getParameterSkinName();
    loadSkin_();
}


SqueezerAudioProcessorEditor::~SqueezerAudioProcessorEditor()
{
    PluginProcessor_.removeActionListener(this);

    // release look and feel
    setLookAndFeel(nullptr);
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

    PluginProcessor_.setParameterSkinName(CurrentSkinName_);

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
    CurrentSkin_.setBackground(DrawableBackground_, this);

    CurrentSkin_.placeAndSkinSlider("slider_threshold",
                                    SliderThreshold_.get());
    CurrentSkin_.placeAndSkinSlider("slider_ratio",
                                    SliderRatio_.get());
    CurrentSkin_.placeAndSkinSlider("slider_attack_rate",
                                    SliderAttackRate_.get());
    CurrentSkin_.placeAndSkinSlider("slider_release_rate",
                                    SliderReleaseRate_.get());
    CurrentSkin_.placeAndSkinSlider("slider_sidechain_hpf_cutoff",
                                    SliderSidechainHPFCutoff_.get());
    CurrentSkin_.placeAndSkinSlider("slider_sidechain_lpf_cutoff",
                                    SliderSidechainLPFCutoff_.get());

    CurrentSkin_.placeAndSkinSlider("slider_input_trim",
                                    SliderInputTrim_.get());
    CurrentSkin_.placeAndSkinSlider("slider_makeup_gain",
                                    SliderMakeupGain_.get());

    CurrentSkin_.placeAndSkinSlider("slider_stereo_link",
                                    SliderStereoLink_.get());
    CurrentSkin_.placeAndSkinSlider("slider_wet_mix",
                                    SliderWetMix_.get());

    CurrentSkin_.placeAndSkinButton("button_curve_linear",
                                    &ButtonCurveLinear_);
    CurrentSkin_.placeAndSkinButton("button_curve_smooth_decoupled",
                                    &ButtonCurveSmoothDecoupled_);
    CurrentSkin_.placeAndSkinButton("button_curve_smooth_branching",
                                    &ButtonCurveSmoothBranching_);

    CurrentSkin_.placeAndSkinButton("button_rms_window",
                                    &ButtonRmsWindow_);
    CurrentSkin_.placeAndSkinButton("button_design_feedback",
                                    &ButtonDesignFeedback_);
    CurrentSkin_.placeAndSkinButton("button_gainstage_optical",
                                    &ButtonGainStageOptical_);

    CurrentSkin_.placeAndSkinButton("button_knee_hard",
                                    &ButtonKneeHard_);
    CurrentSkin_.placeAndSkinButton("button_knee_medium",
                                    &ButtonKneeMedium_);
    CurrentSkin_.placeAndSkinButton("button_knee_soft",
                                    &ButtonKneeSoft_);

    CurrentSkin_.placeAndSkinButton("button_sidechain_external",
                                    &ButtonSidechainExternal_);
    CurrentSkin_.placeAndSkinButton("button_sidechain_listen",
                                    &ButtonSidechainListen_);

    CurrentSkin_.placeAndSkinButton("button_auto_makeup_gain",
                                    &ButtonAutoMakeupGain_);
    CurrentSkin_.placeAndSkinButton("button_about",
                                    &ButtonAbout_);
    CurrentSkin_.placeAndSkinButton("button_settings",
                                    &ButtonSettings_);
    CurrentSkin_.placeAndSkinButton("button_skin",
                                    &ButtonSkin_);
    CurrentSkin_.placeAndSkinButton("button_bypass",
                                    &ButtonBypass_);

    CurrentSkin_.placeAndSkinButton("button_reset",
                                    &ButtonReset_);

#ifdef DEBUG
    CurrentSkin_.placeAndSkinLabel("label_debug",
                                   LabelDebug_,
                                   this);
#endif

    // allow meter updates from now on
    IsInitialising_ = false;

    InputLevelMeters_.clear(true);
    OutputLevelMeters_.clear(true);
    GainReductionMeters_.clear(true);


    Array<Colour> ColoursLevelMeter;

    XmlElement *xmlSetting = CurrentSkin_.getSetting("meter_colour_high");
    Colour ColourHigh = CurrentSkin_.getColour(xmlSetting,
                        Colour(0.00f, 1.0f, 1.0f, 1.0f));

    xmlSetting = CurrentSkin_.getSetting("meter_colour_medium");
    Colour ColourMedium = CurrentSkin_.getColour(xmlSetting,
                          Colour(0.18f, 1.0f, 1.0f, 1.0f));

    xmlSetting = CurrentSkin_.getSetting("meter_colour_low");
    Colour ColourLow = CurrentSkin_.getColour(xmlSetting,
                       Colour(0.30f, 1.0f, 1.0f, 1.0f));

    ColoursLevelMeter.add(ColourHigh);    // overload
    ColoursLevelMeter.add(ColourMedium);  // warning
    ColoursLevelMeter.add(ColourLow);     // fine

    xmlSetting = CurrentSkin_.getSetting("meter_gain_reduction_normal");
    Colour ColourReduction = CurrentSkin_.getColour(xmlSetting,
                             Colour(0.58f, 1.0f, 1.0f, 1.0f));

    xmlSetting = CurrentSkin_.getSetting("meter_segment");
    int SegmentHeight = CurrentSkin_.getInteger(xmlSetting,
                        "height",
                        5);

    bool IsDiscreteMeter = true;
    int CrestFactor = 0;

    frut::widgets::Orientation MeterOrientation =
        frut::widgets::Orientation::vertical;

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
            ColourReduction);

        addAndMakeVisible(GainReductionMeter);
    }

    if (NumberOfChannels_ == 1)
    {
        CurrentSkin_.placeMeterBar("meter_input",
                                   InputLevelMeters_[0]);

        CurrentSkin_.placeMeterBar("meter_output",
                                   OutputLevelMeters_[0]);

        CurrentSkin_.placeMeterBar("meter_gain_reduction",
                                   GainReductionMeters_[0]);

        SliderStereoLink_->setEnabled(false);
    }
    else
    {
        CurrentSkin_.placeMeterBar("meter_input_left",
                                   InputLevelMeters_[0]);
        CurrentSkin_.placeMeterBar("meter_input_right",
                                   InputLevelMeters_[1]);

        CurrentSkin_.placeMeterBar("meter_output_left",
                                   OutputLevelMeters_[0]);
        CurrentSkin_.placeMeterBar("meter_output_right",
                                   OutputLevelMeters_[1]);

        CurrentSkin_.placeMeterBar("meter_gain_reduction_left",
                                   GainReductionMeters_[0]);
        CurrentSkin_.placeMeterBar("meter_gain_reduction_right",
                                   GainReductionMeters_[1]);
    }

#if SQUEEZER_EXTERNAL_SIDECHAIN == 0
    ButtonSidechainExternal_.setEnabled(false);
#endif // SQUEEZER_EXTERNAL_SIDECHAIN == 1
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
    // "PC" ==> parameter changed, followed by a hash and the
    // parameter's ID
    if (Message.startsWith("PC#"))
    {
        String StringIndex = Message.substring(3);
        int Index = StringIndex.getIntValue();
        jassert(Index >= 0);
        jassert(Index < PluginProcessor_.getNumParameters());

        if (PluginProcessor_.hasChanged(Index))
        {
            updateParameter(Index);
        }
    }
    // "UM" ==> update meters
    else if (!Message.compare("UM"))
    {
        // prevent meter updates during initialisation
        if (!IsInitialising_)
        {
            for (int Channel = 0; Channel < NumberOfChannels_; ++Channel)
            {
                float NoPeakDisplay = -100.0;

                float AverageInputLevel =
                    PluginProcessor_.getAverageMeterInputLevel(Channel);
                float PeakInputLevel =
                    PluginProcessor_.getPeakMeterInputLevel(Channel);

                InputLevelMeters_[Channel]->setLevels(
                    AverageInputLevel, NoPeakDisplay,
                    PeakInputLevel, NoPeakDisplay);

                float AverageOutputLevel =
                    PluginProcessor_.getAverageMeterOutputLevel(Channel);
                float PeakOutputLevel =
                    PluginProcessor_.getPeakMeterOutputLevel(Channel);

                OutputLevelMeters_[Channel]->setLevels(
                    AverageOutputLevel, NoPeakDisplay,
                    PeakOutputLevel, NoPeakDisplay);

                float GainReduction =
                    PluginProcessor_.getGainReduction(Channel);

                // make sure gain reduction meter doesn't show anything
                // while there is no gain reduction
                GainReduction -= 0.01f;

                GainReductionMeters_[Channel]->setNormalLevels(
                    GainReduction, NoPeakDisplay);
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
    float FloatValue = PluginProcessor_.getParameter(Index);
    PluginProcessor_.clearChangeFlag(Index);

    switch (Index)
    {
    case SqueezerPluginParameters::selBypass:
        ButtonBypass_.setToggleState(FloatValue != 0.0f,
                                     dontSendNotification);
        break;

    case SqueezerPluginParameters::selRmsWindowSize:

        ButtonRmsWindow_.setToggleState(FloatValue != 0.0f,
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

    case SqueezerPluginParameters::selCurveType:

        if (FloatValue == (SideChain::CurveLogLin /
                           float(SideChain::NumberOfCurves - 1)))
        {
            ButtonCurveLinear_.setToggleState(true,
                                              dontSendNotification);
        }
        else if (FloatValue == (SideChain::CurveLogSmoothDecoupled /
                                float(SideChain::NumberOfCurves - 1)))
        {
            ButtonCurveSmoothDecoupled_.setToggleState(true,
                    dontSendNotification);
        }
        else
        {
            ButtonCurveSmoothBranching_.setToggleState(true,
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

    case SqueezerPluginParameters::selInputTrimSwitch:
        SliderInputTrim_->updateMode();
        break;

    case SqueezerPluginParameters::selInputTrim:
        SliderInputTrim_->setValue(FloatValue,
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

    case SqueezerPluginParameters::selStereoLinkSwitch:
        SliderStereoLink_->updateMode();
        break;

    case SqueezerPluginParameters::selStereoLink:
        SliderStereoLink_->setValue(FloatValue,
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
        ButtonSidechainExternal_.setToggleState(FloatValue != 0.0f,
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
        DBG("[Squeezer] editor::updateParameter ==> invalid index");
        break;
    }
}


void SqueezerAudioProcessorEditor::buttonClicked(
    Button *Button)
{
    if (Button == &ButtonBypass_)
    {
        PluginProcessor_.changeParameter(SqueezerPluginParameters::selBypass,
                                         !Button->getToggleState());
    }
    else if (Button == &ButtonRmsWindow_)
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selRmsWindowSize,
            !Button->getToggleState());
    }
    else if (Button == &ButtonDesignFeedback_)
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selDesign,
            !Button->getToggleState());
    }
    else if (Button == &ButtonGainStageOptical_)
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selGainStage,
            !Button->getToggleState());
    }
    else if (Button == &ButtonCurveLinear_)
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selCurveType,
            SideChain::CurveLogLin /
            float(SideChain::NumberOfCurves - 1));
    }
    else if (Button == &ButtonCurveSmoothDecoupled_)
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selCurveType,
            SideChain::CurveLogSmoothDecoupled /
            float(SideChain::NumberOfCurves - 1));
    }
    else if (Button == &ButtonCurveSmoothBranching_)
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selCurveType,
            SideChain::CurveLogSmoothBranching /
            float(SideChain::NumberOfCurves - 1));
    }
    else if (Button == &ButtonKneeHard_)
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selKneeWidth,
            Compressor::KneeHard /
            float(Compressor::NumberOfKneeSettings - 1));
    }
    else if (Button == &ButtonKneeMedium_)
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selKneeWidth,
            Compressor::KneeMedium /
            float(Compressor::NumberOfKneeSettings - 1));
    }
    else if (Button == &ButtonKneeSoft_)
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selKneeWidth,
            Compressor::KneeSoft /
            float(Compressor::NumberOfKneeSettings - 1));
    }
    else if (Button == &ButtonAutoMakeupGain_)
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selAutoMakeupGain,
            !Button->getToggleState());
    }
    else if (Button == &ButtonSidechainExternal_)
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selSidechainInput,
            !Button->getToggleState());
    }
    else if (Button == &ButtonSidechainListen_)
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selSidechainListen,
            !Button->getToggleState());
    }
    else if (Button == &ButtonReset_)
    {
        PluginProcessor_.resetMeters();

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
        String PluginSettings = PluginProcessor_.getParameters().trim();

        // prepare and launch dialog window
        DialogWindow *windowSettings =
            frut::widgets::WindowSettingsContent::createDialogWindow(
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
            frut::widgets::WindowSkinContent::createDialogWindow(
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

#if JucePlugin_Build_VST
        pluginNameAndVersion += " (VST2)";
#endif

#if JucePlugin_Build_VST3
        pluginNameAndVersion += " (VST3)";
#endif

        Chapters.set(
            pluginNameAndVersion,
            String(JucePlugin_Desc) + ".\n");

        Chapters.set(
            "Copyright",
            "(c) 2013-2020 Martin Zuther\n");

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
#ifdef LINUX
            L"POSIX Threads\n"
#endif
#if JucePlugin_Build_VST || JucePlugin_Build_VST3
            L"VST\n"
#endif
#ifdef LINUX
            L"Xlib\n"
            L"Xext\n"
#endif
        );

#if JucePlugin_Build_VST || JucePlugin_Build_VST3
        // display trademarks (but only when necessary)
        Chapters.set(
            "Trademarks",
            L"VST is a trademark of Steinberg Media Technologies GmbH, "
            L"registered in Europe and other countries.\n");
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
            frut::widgets::WindowAboutContent::createDialogWindow(
                this, Width, Height, Chapters);

        // attach callback to dialog window
        ModalComponentManager::getInstance()->attachCallback(
            windowAbout, ModalCallbackFunction::forComponent(
                window_about_callback, this));
    }
    else
    {
        float FloatValue = Button->getToggleState() ? 1.0f : 0.0f;
        frut::widgets::SliderCombined *Slider =
            dynamic_cast<frut::widgets::SliderCombined *>(
                Button->getParentComponent());

        if (Slider == SliderThreshold_.get())
        {
            PluginProcessor_.changeParameter(
                SqueezerPluginParameters::selThresholdSwitch,
                FloatValue);
        }
        else if (Slider == SliderRatio_.get())
        {
            PluginProcessor_.changeParameter(
                SqueezerPluginParameters::selRatioSwitch,
                FloatValue);
        }
        else if (Slider == SliderAttackRate_.get())
        {
            PluginProcessor_.changeParameter(
                SqueezerPluginParameters::selAttackRateSwitch,
                FloatValue);
        }
        else if (Slider == SliderReleaseRate_.get())
        {
            PluginProcessor_.changeParameter(
                SqueezerPluginParameters::selReleaseRateSwitch,
                FloatValue);
        }
        else if (Slider == SliderInputTrim_.get())
        {
            PluginProcessor_.changeParameter(
                SqueezerPluginParameters::selInputTrimSwitch,
                FloatValue);
        }
        else if (Slider == SliderMakeupGain_.get())
        {
            PluginProcessor_.changeParameter(
                SqueezerPluginParameters::selMakeupGainSwitch,
                FloatValue);
        }
        else if (Slider == SliderStereoLink_.get())
        {
            PluginProcessor_.changeParameter(
                SqueezerPluginParameters::selStereoLinkSwitch,
                FloatValue);
        }
        else if (Slider == SliderWetMix_.get())
        {
            PluginProcessor_.changeParameter(
                SqueezerPluginParameters::selWetMixSwitch,
                FloatValue);
        }
        else if (Slider == SliderSidechainHPFCutoff_.get())
        {
            PluginProcessor_.changeParameter(
                SqueezerPluginParameters::selSidechainHPFCutoffSwitch,
                FloatValue);
        }
        else if (Slider == SliderSidechainLPFCutoff_.get())
        {
            PluginProcessor_.changeParameter(
                SqueezerPluginParameters::selSidechainLPFCutoffSwitch,
                FloatValue);
        }
        else
        {
            DBG("[Squeezer] editor::buttonClicked ==> invalid button");
        }
    }
}


void SqueezerAudioProcessorEditor::sliderValueChanged(
    Slider *Slider)
{
    float FloatValue = (float) Slider->getValue();

    if (Slider == SliderThreshold_.get())
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selThreshold,
            FloatValue);
    }
    else if (Slider == SliderRatio_.get())
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selRatio,
            FloatValue);
    }
    else if (Slider == SliderAttackRate_.get())
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selAttackRate,
            FloatValue);
    }
    else if (Slider == SliderReleaseRate_.get())
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selReleaseRate,
            FloatValue);
    }
    else if (Slider == SliderInputTrim_.get())
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selInputTrim,
            FloatValue);
    }
    else if (Slider == SliderMakeupGain_.get())
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selMakeupGain,
            FloatValue);
    }
    else if (Slider == SliderStereoLink_.get())
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selStereoLink,
            FloatValue);
    }
    else if (Slider == SliderWetMix_.get())
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selWetMix,
            FloatValue);
    }
    else if (Slider == SliderSidechainHPFCutoff_.get())
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selSidechainHPFCutoff,
            FloatValue);
    }
    else if (Slider == SliderSidechainLPFCutoff_.get())
    {
        PluginProcessor_.changeParameter(
            SqueezerPluginParameters::selSidechainLPFCutoff,
            FloatValue);
    }
    else
    {
        DBG("[Squeezer] editor::sliderValueChanged ==> invalid slider");
    }
}


void SqueezerAudioProcessorEditor::paint(Graphics &g)
{
    g.fillAll(Colours::green);
}


void SqueezerAudioProcessorEditor::resized()
{
}
