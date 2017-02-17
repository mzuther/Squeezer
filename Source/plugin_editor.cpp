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


static void window_about_callback(int modalResult, SqueezerAudioProcessorEditor *pEditor)
{
    if (pEditor != nullptr)
    {
        pEditor->windowAboutCallback(modalResult);
    }
}


static void window_settings_callback(int modalResult, SqueezerAudioProcessorEditor *pEditor)
{
    if (pEditor != nullptr)
    {
        pEditor->windowSettingsCallback(modalResult);
    }
}


static void window_skin_callback(int modalResult, SqueezerAudioProcessorEditor *pEditor)
{
    if (pEditor != nullptr)
    {
        pEditor->windowSkinCallback(modalResult);
    }
}


//==============================================================================
SqueezerAudioProcessorEditor::SqueezerAudioProcessorEditor(SqueezerAudioProcessor *ownerFilter, SqueezerPluginParameters *parameters, int channels)
    : AudioProcessorEditor(ownerFilter)
{
    // the editor window does not have any transparent areas
    // (increases performance on redrawing)
    setOpaque(true);

    // The plug-in editor's size as well as the location of buttons
    // and labels will be set later on in this constructor.

    pProcessor = ownerFilter;
    pProcessor->addActionListener(this);

    NumberOfChannels = channels;

    ButtonBypass.setButtonText("Bypass");
    ButtonBypass.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonBypass.setColour(TextButton::buttonOnColourId, Colours::red);

    ButtonBypass.addListener(this);
    addAndMakeVisible(&ButtonBypass);


    ButtonDetectorRms.setButtonText("RMS");
    ButtonDetectorRms.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDetectorRms.setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonDetectorRms.addListener(this);
    addAndMakeVisible(&ButtonDetectorRms);


    ButtonDesignFeedBack.setButtonText("F.Back");
    ButtonDesignFeedBack.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDesignFeedBack.setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonDesignFeedBack.addListener(this);
    addAndMakeVisible(&ButtonDesignFeedBack);


    ButtonGainStageOptical.setButtonText("Opto");
    ButtonGainStageOptical.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonGainStageOptical.setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonGainStageOptical.addListener(this);
    addAndMakeVisible(&ButtonGainStageOptical);


    ButtonDetectorLinear.setButtonText("Linear");
    ButtonDetectorLinear.setRadioGroupId(1);
    ButtonDetectorLinear.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDetectorLinear.setColour(TextButton::buttonOnColourId, Colours::yellow.withRotatedHue(-0.08f));

    ButtonDetectorLinear.addListener(this);
    addAndMakeVisible(&ButtonDetectorLinear);


    ButtonDetectorSmoothDecoupled.setButtonText("S-Curve");
    ButtonDetectorSmoothDecoupled.setRadioGroupId(1);
    ButtonDetectorSmoothDecoupled.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDetectorSmoothDecoupled.setColour(TextButton::buttonOnColourId, Colours::yellow.withRotatedHue(-0.08f));

    ButtonDetectorSmoothDecoupled.addListener(this);
    addAndMakeVisible(&ButtonDetectorSmoothDecoupled);


    ButtonDetectorSmoothBranching.setButtonText("Log");
    ButtonDetectorSmoothBranching.setRadioGroupId(1);
    ButtonDetectorSmoothBranching.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDetectorSmoothBranching.setColour(TextButton::buttonOnColourId, Colours::yellow.withRotatedHue(-0.08f));

    ButtonDetectorSmoothBranching.addListener(this);
    addAndMakeVisible(&ButtonDetectorSmoothBranching);


    ButtonKneeHard.setButtonText("Hard");
    ButtonKneeHard.setRadioGroupId(2);
    ButtonKneeHard.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonKneeHard.setColour(TextButton::buttonOnColourId, Colours::yellow.withRotatedHue(+0.08f));

    ButtonKneeHard.addListener(this);
    addAndMakeVisible(&ButtonKneeHard);

    ButtonKneeMedium.setButtonText("Medium");
    ButtonKneeMedium.setRadioGroupId(2);
    ButtonKneeMedium.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonKneeMedium.setColour(TextButton::buttonOnColourId, Colours::yellow.withRotatedHue(+0.08f));

    ButtonKneeMedium.addListener(this);
    addAndMakeVisible(&ButtonKneeMedium);

    ButtonKneeSoft.setButtonText("Soft");
    ButtonKneeSoft.setRadioGroupId(2);
    ButtonKneeSoft.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonKneeSoft.setColour(TextButton::buttonOnColourId, Colours::yellow.withRotatedHue(+0.08f));

    ButtonKneeSoft.addListener(this);
    addAndMakeVisible(&ButtonKneeSoft);

    int nIndex = SqueezerPluginParameters::selThreshold;
    int nIndexSwitch = SqueezerPluginParameters::selThresholdSwitch;
    SliderThresholdCombined = new frut::widget::SliderCombined(parameters, nIndex, nIndexSwitch);
    SliderThresholdCombined->setSliderColour(Colours::purple.brighter(0.2f));

    SliderThresholdCombined->addListener(this);
    SliderThresholdCombined->addButtonListener(this);
    addAndMakeVisible(SliderThresholdCombined);


    nIndex = SqueezerPluginParameters::selRatio;
    nIndexSwitch = SqueezerPluginParameters::selRatioSwitch;
    SliderRatioCombined = new frut::widget::SliderCombined(parameters, nIndex, nIndexSwitch);
    SliderRatioCombined->setSliderColour(Colours::purple.brighter(0.2f));

    SliderRatioCombined->addListener(this);
    SliderRatioCombined->addButtonListener(this);
    addAndMakeVisible(SliderRatioCombined);

    nIndex = SqueezerPluginParameters::selAttackRate;
    nIndexSwitch = SqueezerPluginParameters::selAttackRateSwitch;
    SliderAttackRateCombined = new frut::widget::SliderCombined(parameters, nIndex, nIndexSwitch);
    SliderAttackRateCombined->setSliderColour(Colours::yellow);

    SliderAttackRateCombined->addListener(this);
    SliderAttackRateCombined->addButtonListener(this);
    addAndMakeVisible(SliderAttackRateCombined);


    nIndex = SqueezerPluginParameters::selReleaseRate;
    nIndexSwitch = SqueezerPluginParameters::selReleaseRateSwitch;
    SliderReleaseRateCombined = new frut::widget::SliderCombined(parameters, nIndex, nIndexSwitch);
    SliderReleaseRateCombined->setSliderColour(Colours::yellow);

    SliderReleaseRateCombined->addListener(this);
    SliderReleaseRateCombined->addButtonListener(this);
    addAndMakeVisible(SliderReleaseRateCombined);


    ButtonAutoMakeupGain.setButtonText("Auto MU");
    ButtonAutoMakeupGain.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonAutoMakeupGain.setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonAutoMakeupGain.addListener(this);
    addAndMakeVisible(&ButtonAutoMakeupGain);


    nIndex = SqueezerPluginParameters::selMakeupGain;
    nIndexSwitch = SqueezerPluginParameters::selMakeupGainSwitch;
    SliderMakeupGainCombined = new frut::widget::SliderCombined(parameters, nIndex, nIndexSwitch);
    SliderMakeupGainCombined->setSliderColour(Colours::blue.brighter(0.4f));

    SliderMakeupGainCombined->addListener(this);
    SliderMakeupGainCombined->addButtonListener(this);
    addAndMakeVisible(SliderMakeupGainCombined);


    nIndex = SqueezerPluginParameters::selWetMix;
    nIndexSwitch = SqueezerPluginParameters::selWetMixSwitch;
    SliderWetMixCombined = new frut::widget::SliderCombined(parameters, nIndex, nIndexSwitch);
    SliderWetMixCombined->setSliderColour(Colours::blue.brighter(0.4f));

    SliderWetMixCombined->addListener(this);
    SliderWetMixCombined->addButtonListener(this);
    addAndMakeVisible(SliderWetMixCombined);


    ButtonSidechainFilterState.setButtonText("Filter");
    ButtonSidechainFilterState.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonSidechainFilterState.setColour(TextButton::buttonOnColourId, Colours::green);

    ButtonSidechainFilterState.addListener(this);
    addAndMakeVisible(&ButtonSidechainFilterState);


    nIndex = SqueezerPluginParameters::selSidechainFilterCutoff;
    nIndexSwitch = SqueezerPluginParameters::selSidechainFilterCutoffSwitch;
    SliderSidechainFilterCutoffCombined = new frut::widget::SliderCombined(parameters, nIndex, nIndexSwitch);
    SliderSidechainFilterCutoffCombined->setSliderColour(Colours::green.brighter(0.1f));

    SliderSidechainFilterCutoffCombined->addListener(this);
    SliderSidechainFilterCutoffCombined->addButtonListener(this);
    addAndMakeVisible(SliderSidechainFilterCutoffCombined);


    nIndex = SqueezerPluginParameters::selSidechainFilterGain;
    SliderSidechainFilterGain = new frut::widget::SliderSwitchLinearBar(parameters, nIndex);
    SliderSidechainFilterGain->setSliderColour(Colours::green.brighter(0.1f));

    SliderSidechainFilterGain->addListener(this);
    addAndMakeVisible(SliderSidechainFilterGain);


    ButtonSidechainListen.setButtonText("Listen");
    ButtonSidechainListen.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonSidechainListen.setColour(TextButton::buttonOnColourId, Colours::red);

    ButtonSidechainListen.addListener(this);
    addAndMakeVisible(&ButtonSidechainListen);


#ifdef DEBUG
    LabelDebug.setText("dbg", dontSendNotification);
    LabelDebug.setColour(Label::textColourId, Colours::red);
    LabelDebug.setJustificationType(Justification::centred);
#endif


    ButtonSkin.addListener(this);
    addAndMakeVisible(ButtonSkin);

    ButtonResetMeters.setButtonText("Reset");
    ButtonResetMeters.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonResetMeters.setColour(TextButton::buttonOnColourId, Colours::red);

    ButtonResetMeters.addListener(this);
    addAndMakeVisible(&ButtonResetMeters);


    ButtonSettings.setButtonText("Settings");
    ButtonSettings.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonSettings.setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonSettings.addListener(this);
    addAndMakeVisible(&ButtonSettings);


    ButtonAbout.setButtonText("About");
    ButtonAbout.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonAbout.setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonAbout.addListener(this);
    addAndMakeVisible(&ButtonAbout);

    int x = 510;
    int y = 20;

    int x_spacing;
    int width;

    if (NumberOfChannels == 1)
    {
        x_spacing = 20;
        width = 16;
    }
    else
    {
        x_spacing = 28;
        width = 12;
    }

    for (int nChannel = 0; nChannel < NumberOfChannels; ++nChannel)
    {
        Array<Colour> levelMeterColours;

        levelMeterColours.add(Colour(0.00f, 1.0f, 1.0f, 1.0f));  // overload
        levelMeterColours.add(Colour(0.18f, 1.0f, 1.0f, 1.0f));  // warning
        levelMeterColours.add(Colour(0.30f, 1.0f, 1.0f, 1.0f));  // fine


        Array<Colour> gainReductionColours;

        gainReductionColours.add(Colour(0.58f, 1.0f, 1.0f, 1.0f));  // normal
        gainReductionColours.add(Colour(0.18f, 1.0f, 1.0f, 1.0f));  // notify


        bool discreteMeter = true;

        MeterBarLevel *pMeterBarLevelInput = p_arrInputLevelMeters.add(
                new MeterBarLevel());
        pMeterBarLevelInput->create(20,
                                    frut::widget::Orientation::vertical,
                                    discreteMeter,
                                    5,
                                    width,
                                    levelMeterColours);

        pMeterBarLevelInput->setBounds(x, y, width, 0);
        addAndMakeVisible(pMeterBarLevelInput);

        MeterBarLevel *pMeterBarLevelOutput = p_arrOutputLevelMeters.add(
                new MeterBarLevel());
        pMeterBarLevelOutput->create(20,
                                     frut::widget::Orientation::vertical,
                                     discreteMeter,
                                     5,
                                     width,
                                     levelMeterColours);

        pMeterBarLevelOutput->setBounds(x + 2 * x_spacing, y, width, 0);
        addAndMakeVisible(pMeterBarLevelOutput);

        MeterBarGainReduction *pMeterBarGainReduction = p_arrGainReductionMeters.add(
                    new MeterBarGainReduction());
        pMeterBarGainReduction->create(
            frut::widget::Orientation::vertical,
            discreteMeter,
            5,
            width,
            gainReductionColours);

        pMeterBarGainReduction->setBounds(x + x_spacing, y, width, 0);
        addAndMakeVisible(pMeterBarGainReduction);

        x += width;
    }

#ifdef DEBUG
    // moves debug label to the back of the editor's z-plane to that
    // it doesn't overlay (and thus block) any other components
    addAndMakeVisible(LabelDebug, 0);
#endif

    // prevent unnecessary redrawing of plugin editor
    BackgroundImage.setOpaque(true);
    // moves background image to the back of the editor's z-plane to
    // that it doesn't overlay (and thus block) any other components
    addAndMakeVisible(BackgroundImage, 0);

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

    updateParameter(SqueezerPluginParameters::selSidechainFilterState);
    updateParameter(SqueezerPluginParameters::selSidechainFilterCutoffSwitch);
    updateParameter(SqueezerPluginParameters::selSidechainFilterCutoff);
    updateParameter(SqueezerPluginParameters::selSidechainFilterGain);
    updateParameter(SqueezerPluginParameters::selSidechainListen);

    // locate directory containing the skins
    skinDirectory = SqueezerPluginParameters::getSkinDirectory();

    // apply skin to plug-in editor
    currentSkinName = pProcessor->getParameterSkinName();
    loadSkin();
}


SqueezerAudioProcessorEditor::~SqueezerAudioProcessorEditor()
{
    pProcessor->removeActionListener(this);
}


void SqueezerAudioProcessorEditor::loadSkin()
{
    File fileSkin = skinDirectory.getChildFile(currentSkinName + ".skin");

    if (!fileSkin.existsAsFile())
    {
        Logger::outputDebugString("[Skin] file \"" + fileSkin.getFileName() + "\" not found");

        currentSkinName = "Default";
        fileSkin = skinDirectory.getChildFile(currentSkinName + ".skin");
    }

    pProcessor->setParameterSkinName(currentSkinName);

    skin.loadSkin(fileSkin, NumberOfChannels);

    // moves background image to the back of the editor's z-plane
    applySkin();
}


void SqueezerAudioProcessorEditor::applySkin()
{
    // update skin
    skin.updateSkin(NumberOfChannels);

    // moves background image to the back of the editor's z-plane;
    // will also resize plug-in editor
    skin.setBackgroundImage(&BackgroundImage, this);

    skin.placeComponent(&ButtonDetectorLinear,
                        "button_detector_linear");
    skin.placeComponent(&ButtonDetectorSmoothDecoupled,
                        "button_detector_smooth_decoupled");
    skin.placeComponent(&ButtonDetectorSmoothBranching,
                        "button_detector_smooth_branching");

    skin.placeComponent(&ButtonDetectorRms,
                        "button_detector_rms");
    skin.placeComponent(&ButtonDesignFeedBack,
                        "button_design_feedback");
    skin.placeComponent(&ButtonGainStageOptical,
                        "button_gainstage_optical");

    skin.placeComponent(&ButtonKneeHard,
                        "button_knee_hard");
    skin.placeComponent(&ButtonKneeMedium,
                        "button_knee_medium");
    skin.placeComponent(&ButtonKneeSoft,
                        "button_knee_soft");

    skin.placeComponent(SliderThresholdCombined,
                        "slider_threshold");
    skin.placeComponent(SliderRatioCombined,
                        "slider_ratio");

    skin.placeComponent(SliderAttackRateCombined,
                        "slider_attack_rate");
    skin.placeComponent(SliderReleaseRateCombined,
                        "slider_release_rate");

    skin.placeComponent(SliderSidechainFilterCutoffCombined,
                        "slider_sidechain_filter_cutoff");
    skin.placeComponent(SliderSidechainFilterGain,
                        "slider_sidechain_filter_gain");
    skin.placeComponent(&ButtonSidechainFilterState,
                        "button_sidechain_filter_state");
    skin.placeComponent(&ButtonSidechainListen,
                        "button_sidechain_listen");

    skin.placeComponent(SliderMakeupGainCombined,
                        "slider_makeup_gain");
    skin.placeComponent(SliderWetMixCombined,
                        "slider_wet_mix");

    skin.placeComponent(&ButtonAutoMakeupGain,
                        "button_auto_makeup_gain");
    skin.placeComponent(&ButtonAbout,
                        "button_about");
    skin.placeComponent(&ButtonSettings,
                        "button_settings");
    skin.placeComponent(&ButtonBypass,
                        "button_bypass");

    skin.placeComponent(&ButtonResetMeters,
                        "button_reset");

#ifdef DEBUG
    skin.placeComponent(&LabelDebug,
                        "label_debug");
#endif
}


void SqueezerAudioProcessorEditor::windowAboutCallback(int modalResult)
{
    UNUSED(modalResult);

    // manually deactivate about button
    ButtonAbout.setToggleState(false, dontSendNotification);
}


void SqueezerAudioProcessorEditor::windowSettingsCallback(int modalResult)
{
    UNUSED(modalResult);

    // manually deactivate about button
    ButtonSettings.setToggleState(false, dontSendNotification);
}


void SqueezerAudioProcessorEditor::windowSkinCallback(int modalResult)
{
    // manually deactivate skin button
    ButtonSkin.setToggleState(false, dontSendNotification);

    // user has selected a skin
    if (modalResult > 0)
    {
        // apply skin to plug-in editor
        loadSkin();
    }
}


void SqueezerAudioProcessorEditor::actionListenerCallback(const String &strMessage)
{
    // "PC" --> parameter changed, followed by a hash and the
    // parameter's ID
    if (strMessage.startsWith("PC#"))
    {
        String strIndex = strMessage.substring(3);
        int nIndex = strIndex.getIntValue();
        jassert(nIndex >= 0);
        jassert(nIndex < pProcessor->getNumParameters());

        if (pProcessor->hasChanged(nIndex))
        {
            updateParameter(nIndex);
        }
    }
    // "UM" --> update meters
    else if (!strMessage.compare("UM"))
    {
        for (int nChannel = 0; nChannel < NumberOfChannels; ++nChannel)
        {
            float averageInputLevel = pProcessor->getAverageMeterInputLevel(nChannel);
            float maximumInputLevel = pProcessor->getMaximumInputLevel(nChannel);

            float peakInputLevel = pProcessor->getPeakMeterInputLevel(nChannel);
            float peakInputPeakLevel = pProcessor->getPeakMeterPeakInputLevel(nChannel);

            p_arrInputLevelMeters[nChannel]->setLevels(
                averageInputLevel, maximumInputLevel,
                peakInputLevel, peakInputPeakLevel);

            float averageOutputLevel = pProcessor->getAverageMeterOutputLevel(nChannel);
            float maximumOutputLevel = pProcessor->getMaximumOutputLevel(nChannel);

            float peakOutputLevel = pProcessor->getPeakMeterOutputLevel(nChannel);
            float peakOutputPeakLevel = pProcessor->getPeakMeterPeakOutputLevel(nChannel);

            p_arrOutputLevelMeters[nChannel]->setLevels(
                averageOutputLevel, maximumOutputLevel,
                peakOutputLevel, peakOutputPeakLevel);

            float gainReduction = pProcessor->getGainReduction(nChannel);
            float gainReductionMeterPeak = pProcessor->getGainReductionMeterPeak(nChannel);

            // make sure gain reduction meter doesn't show anything
            // while there is no gain reduction
            gainReduction -= 0.01f;
            gainReductionMeterPeak -= 0.01f;

            p_arrGainReductionMeters[nChannel]->setNormalLevels(
                gainReduction, gainReductionMeterPeak);
        }
    }
    else
    {
        DBG("[Squeezer] Received unknown action message \"" + strMessage + "\".");
    }
}


void SqueezerAudioProcessorEditor::updateParameter(int nIndex)
{
    float fValue = pProcessor->getParameter(nIndex);
    pProcessor->clearChangeFlag(nIndex);

    switch (nIndex)
    {
    case SqueezerPluginParameters::selBypass:
        ButtonBypass.setToggleState(fValue != 0.0f, dontSendNotification);
        break;

    case SqueezerPluginParameters::selDetectorRmsFilter:

        ButtonDetectorRms.setToggleState(fValue != 0.0f, dontSendNotification);
        break;

    case SqueezerPluginParameters::selDesign:

        ButtonDesignFeedBack.setToggleState(fValue != 0.0f, dontSendNotification);
        break;

    case SqueezerPluginParameters::selGainStage:

        ButtonGainStageOptical.setToggleState(fValue != 0.0f, dontSendNotification);
        break;

    case SqueezerPluginParameters::selDetector:

        if (fValue == (Compressor::DetectorLinear / float(Compressor::NumberOfDetectors - 1)))
        {
            ButtonDetectorLinear.setToggleState(true, dontSendNotification);
        }
        else if (fValue == (Compressor::DetectorSmoothDecoupled / float(Compressor::NumberOfDetectors - 1)))
        {
            ButtonDetectorSmoothDecoupled.setToggleState(true, dontSendNotification);
        }
        else
        {
            ButtonDetectorSmoothBranching.setToggleState(true, dontSendNotification);
        }

        break;

    case SqueezerPluginParameters::selKneeWidth:

        if (fValue == (Compressor::KneeHard / float(Compressor::NumberOfKneeSettings - 1)))
        {
            ButtonKneeHard.setToggleState(true, dontSendNotification);
        }
        else if (fValue == (Compressor::KneeMedium / float(Compressor::NumberOfKneeSettings - 1)))
        {
            ButtonKneeMedium.setToggleState(true, dontSendNotification);
        }
        else
        {
            ButtonKneeSoft.setToggleState(true, dontSendNotification);
        }

        break;

    case SqueezerPluginParameters::selThresholdSwitch:
        SliderThresholdCombined->updateMode();
        break;

    case SqueezerPluginParameters::selThreshold:
        SliderThresholdCombined->setValue(fValue, dontSendNotification);
        break;

    case SqueezerPluginParameters::selRatioSwitch:
        SliderRatioCombined->updateMode();
        break;

    case SqueezerPluginParameters::selRatio:
        SliderRatioCombined->setValue(fValue, dontSendNotification);

        {
            float fRealValue = SliderRatioCombined->getRealFloat();
            bool bUpwardExpansion = (fRealValue < 1.0f);

            for (int nChannel = 0; nChannel < NumberOfChannels; ++nChannel)
            {
                p_arrGainReductionMeters[nChannel]->setUpwardExpansion(bUpwardExpansion);
            }
        }

        break;

    case SqueezerPluginParameters::selAttackRateSwitch:
        SliderAttackRateCombined->updateMode();
        break;

    case SqueezerPluginParameters::selAttackRate:
        SliderAttackRateCombined->setValue(fValue, dontSendNotification);
        break;

    case SqueezerPluginParameters::selReleaseRateSwitch:
        SliderReleaseRateCombined->updateMode();
        break;

    case SqueezerPluginParameters::selReleaseRate:
        SliderReleaseRateCombined->setValue(fValue, dontSendNotification);
        break;

    case SqueezerPluginParameters::selAutoMakeupGain:
        ButtonAutoMakeupGain.setToggleState(fValue != 0.0f, dontSendNotification);
        break;

    case SqueezerPluginParameters::selMakeupGainSwitch:
        SliderMakeupGainCombined->updateMode();
        break;

    case SqueezerPluginParameters::selMakeupGain:
        SliderMakeupGainCombined->setValue(fValue, dontSendNotification);
        break;

    case SqueezerPluginParameters::selWetMixSwitch:
        SliderWetMixCombined->updateMode();
        break;

    case SqueezerPluginParameters::selWetMix:
        SliderWetMixCombined->setValue(fValue, dontSendNotification);
        break;

    case SqueezerPluginParameters::selSidechainFilterState:
        ButtonSidechainFilterState.setToggleState(fValue != 0.0f, dontSendNotification);
        break;

    case SqueezerPluginParameters::selSidechainFilterCutoffSwitch:
        SliderSidechainFilterCutoffCombined->updateMode();
        break;

    case SqueezerPluginParameters::selSidechainFilterCutoff:
        SliderSidechainFilterCutoffCombined->setValue(fValue, dontSendNotification);
        break;

    case SqueezerPluginParameters::selSidechainFilterGain:
        SliderSidechainFilterGain->setValue(fValue, dontSendNotification);
        break;

    case SqueezerPluginParameters::selSidechainListen:
        ButtonSidechainListen.setToggleState(fValue != 0.0f, dontSendNotification);
        break;

    default:
        DBG("[Squeezer] editor::updateParameter --> invalid index");
        break;
    }
}


//==============================================================================
void SqueezerAudioProcessorEditor::paint(Graphics &g)
{
    g.setColour(Colours::lightgrey.darker(0.1f));
    g.fillAll();

    int x = 10;
    int y1 = 10;
    int y2 = 88;

    g.setColour(Colours::grey);
    g.fillRect(x      , y1, 257, 168);
    g.fillRect(x + 260, y1,  82, 168);
    g.fillRect(x + 345, y1, 142, 168);

    if (NumberOfChannels == 1)
    {
        g.fillRect(x + 490, y1,  76, 168);
    }
    else
    {
        g.fillRect(x + 490, y1, 100, 168);
    }

    g.setColour(Colours::darkgrey);
    g.drawRect(x      , y1, 257, 168);
    g.drawRect(x + 260, y1,  82, 168);
    g.drawRect(x + 345, y1, 142, 168);

    if (NumberOfChannels == 1)
    {
        g.drawRect(x + 490, y1,  76, 168);
    }
    else
    {
        g.drawRect(x + 490, y1, 100, 168);
    }

    g.setColour(Colours::lightgrey.darker(0.2f));
    g.fillRect(x +   5, y2, 247,  85);
    g.fillRect(x + 265, y2,  72,  85);
    g.fillRect(x + 350, y2, 132,  85);

    if (NumberOfChannels == 1)
    {
        g.fillRect(x + 495, y1 + 5,  66, 158);
    }
    else
    {
        g.fillRect(x + 495, y1 + 5,  90, 158);
    }

    g.setColour(Colours::grey.darker(0.2f));
    g.drawRect(x +   5, y2, 247,  85);
    g.drawRect(x + 265, y2,  72,  85);
    g.drawRect(x + 350, y2, 132,  85);

    if (NumberOfChannels == 1)
    {
        g.drawRect(x + 495, y1 + 5,  66, 158);
    }
    else
    {
        g.drawRect(x + 495, y1 + 5,  90, 158);
    }
}


void SqueezerAudioProcessorEditor::buttonClicked(Button *button)
{
    if (button == &ButtonBypass)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selBypass, !button->getToggleState());
    }
    else if (button == &ButtonDetectorRms)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDetectorRmsFilter, !button->getToggleState());
    }
    else if (button == &ButtonDesignFeedBack)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDesign, !button->getToggleState());
    }
    else if (button == &ButtonGainStageOptical)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selGainStage, !button->getToggleState());
    }
    else if (button == &ButtonDetectorLinear)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDetector, Compressor::DetectorLinear / float(Compressor::NumberOfDetectors - 1));
    }
    else if (button == &ButtonDetectorSmoothDecoupled)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDetector, Compressor::DetectorSmoothDecoupled / float(Compressor::NumberOfDetectors - 1));
    }
    else if (button == &ButtonDetectorSmoothBranching)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDetector, Compressor::DetectorSmoothBranching / float(Compressor::NumberOfDetectors - 1));
    }
    else if (button == &ButtonKneeHard)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selKneeWidth, Compressor::KneeHard / float(Compressor::NumberOfKneeSettings - 1));
    }
    else if (button == &ButtonKneeMedium)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selKneeWidth, Compressor::KneeMedium / float(Compressor::NumberOfKneeSettings - 1));
    }
    else if (button == &ButtonKneeSoft)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selKneeWidth, Compressor::KneeSoft / float(Compressor::NumberOfKneeSettings - 1));
    }
    else if (button == &ButtonAutoMakeupGain)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selAutoMakeupGain, !button->getToggleState());
    }
    else if (button == &ButtonSidechainFilterState)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selSidechainFilterState, !button->getToggleState());
    }
    else if (button == &ButtonSidechainListen)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selSidechainListen, !button->getToggleState());
    }
    else if (button == &ButtonResetMeters)
    {
        pProcessor->resetMeters();
    }
    else if (button == &ButtonSettings)
    {
        // manually activate button
        button->setToggleState(true, dontSendNotification);

        int width = 440;
        int height = 155;
        String pluginSettings = pProcessor->getParameters().trim();

        // prepare and launch dialog window
        DialogWindow *windowSettings = frut::widget::WindowSettingsContent::createDialogWindow(
                                           this, width, height, pluginSettings);

        // attach callback to dialog window
        ModalComponentManager::getInstance()->attachCallback(windowSettings, ModalCallbackFunction::forComponent(window_settings_callback, this));

        // manually deactivate button
        button->setToggleState(false, dontSendNotification);
    }
    else if (button == &ButtonSkin)
    {
        // manually activate button (will be deactivated in dialog
        // window callback)
        button->setToggleState(true, dontSendNotification);

        // prepare and launch dialog window
        DialogWindow *windowSkin = frut::widget::WindowSkinContent::createDialogWindow(
                                       this, &currentSkinName, skinDirectory);

        // attach callback to dialog window
        ModalComponentManager::getInstance()->attachCallback(windowSkin, ModalCallbackFunction::forComponent(window_skin_callback, this));
    }
    else if (button == &ButtonAbout)
    {
        // manually activate button (will be deactivated in dialog
        // window callback)
        button->setToggleState(true, dontSendNotification);

        StringPairArray arrChapters;

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

        arrChapters.set(
            pluginNameAndVersion,
            String(JucePlugin_Desc) + ".\n");

        arrChapters.set(
            "Copyright",
            "(c) 2013-2016 Martin Zuther\n");

        arrChapters.set(
            "Contributors",
            L"Dimitrios Giannoulis et al.\n"
            L"Filipe Coelho\n"
            L"Paul Kellet\n"
            L"Bram de Jong\n");

        arrChapters.set(
            "Thanks",
            L"I want to thank all contributors and beta testers "
            L"and the open source community at large!\n\n"
            L"Thank you for using free software!\n");

        arrChapters.set(
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
        arrChapters.set(
            "Trademarks",
            L"VST PlugIn Technology by Steinberg Media Technologies\n");
#endif

#if JUCE_ASIO
        // display trademarks (but only when necessary)
        arrChapters.set(
            "Trademarks",
            L"ASIO Driver Technology by Steinberg Media Technologies\n");
#endif

        arrChapters.set(
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
        int width = 270;
        int height = 540;

        DialogWindow *windowAbout = frut::widget::WindowAboutContent::createDialogWindow(
                                        this, width, height, arrChapters);

        // attach callback to dialog window
        ModalComponentManager::getInstance()->attachCallback(windowAbout, ModalCallbackFunction::forComponent(window_about_callback, this));
    }
    else
    {
        float fValue = button->getToggleState() ? 1.0f : 0.0f;
        frut::widget::SliderCombined *slider = dynamic_cast<frut::widget::SliderCombined *>(button->getParentComponent());

        if (slider == SliderThresholdCombined)
        {
            pProcessor->changeParameter(SqueezerPluginParameters::selThresholdSwitch, fValue);
        }
        else if (slider == SliderRatioCombined)
        {
            pProcessor->changeParameter(SqueezerPluginParameters::selRatioSwitch, fValue);
        }
        else if (slider == SliderAttackRateCombined)
        {
            pProcessor->changeParameter(SqueezerPluginParameters::selAttackRateSwitch, fValue);
        }
        else if (slider == SliderReleaseRateCombined)
        {
            pProcessor->changeParameter(SqueezerPluginParameters::selReleaseRateSwitch, fValue);
        }
        else if (slider == SliderMakeupGainCombined)
        {
            pProcessor->changeParameter(SqueezerPluginParameters::selMakeupGainSwitch, fValue);
        }
        else if (slider == SliderWetMixCombined)
        {
            pProcessor->changeParameter(SqueezerPluginParameters::selWetMixSwitch, fValue);
        }
        else if (slider == SliderSidechainFilterCutoffCombined)
        {
            pProcessor->changeParameter(SqueezerPluginParameters::selSidechainFilterCutoffSwitch, fValue);
        }
        else
        {
            DBG("[Squeezer] editor::buttonClicked --> invalid button");
        }
    }
}


void SqueezerAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    float fValue = (float) slider->getValue();

    if (slider == SliderThresholdCombined)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selThreshold, fValue);
    }
    else if (slider == SliderRatioCombined)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selRatio, fValue);
    }
    else if (slider == SliderAttackRateCombined)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selAttackRate, fValue);
    }
    else if (slider == SliderReleaseRateCombined)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selReleaseRate, fValue);
    }
    else if (slider == SliderMakeupGainCombined)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selMakeupGain, fValue);
    }
    else if (slider == SliderWetMixCombined)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selWetMix, fValue);
    }
    else if (slider == SliderSidechainFilterCutoffCombined)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selSidechainFilterCutoff, fValue);
    }
    else if (slider == SliderSidechainFilterGain)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selSidechainFilterGain, fValue);
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
