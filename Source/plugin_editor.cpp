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

    nChannels = channels;

    ButtonBypass.setButtonText("Bypass");
    ButtonBypass.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonBypass.setColour(TextButton::buttonOnColourId, Colours::red);

    ButtonBypass.addListener(this);
    addAndMakeVisible(&ButtonBypass);


    ButtonDetectorRmsPeak.setButtonText("Peak");
    ButtonDetectorRmsPeak.setRadioGroupId(1);
    ButtonDetectorRmsPeak.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDetectorRmsPeak.setColour(TextButton::buttonOnColourId, Colours::yellow.withRotatedHue(+0.08f));

    ButtonDetectorRmsPeak.addListener(this);
    addAndMakeVisible(&ButtonDetectorRmsPeak);


    ButtonDetectorRmsFast.setButtonText("Fast");
    ButtonDetectorRmsFast.setRadioGroupId(1);
    ButtonDetectorRmsFast.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDetectorRmsFast.setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonDetectorRmsFast.addListener(this);
    addAndMakeVisible(&ButtonDetectorRmsFast);


    ButtonDetectorRmsMedium.setButtonText("Medium");
    ButtonDetectorRmsMedium.setRadioGroupId(1);
    ButtonDetectorRmsMedium.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDetectorRmsMedium.setColour(TextButton::buttonOnColourId, Colours::yellow.withRotatedHue(-0.08f));

    ButtonDetectorRmsMedium.addListener(this);
    addAndMakeVisible(&ButtonDetectorRmsMedium);


    ButtonDetectorRmsSlow.setButtonText("Slow");
    ButtonDetectorRmsSlow.setRadioGroupId(1);
    ButtonDetectorRmsSlow.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDetectorRmsSlow.setColour(TextButton::buttonOnColourId, Colours::yellow.withRotatedHue(-0.16f));

    ButtonDetectorRmsSlow.addListener(this);
    addAndMakeVisible(&ButtonDetectorRmsSlow);


    ButtonDesignFeedForward.setButtonText("F.Forw");
    ButtonDesignFeedForward.setRadioGroupId(2);
    ButtonDesignFeedForward.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDesignFeedForward.setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonDesignFeedForward.addListener(this);
    addAndMakeVisible(&ButtonDesignFeedForward);


    ButtonDesignFeedBack.setButtonText("F.Back");
    ButtonDesignFeedBack.setRadioGroupId(2);
    ButtonDesignFeedBack.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDesignFeedBack.setColour(TextButton::buttonOnColourId, Colours::yellow.withRotatedHue(-0.08f));

    ButtonDesignFeedBack.addListener(this);
    addAndMakeVisible(&ButtonDesignFeedBack);


    ButtonDetectorLinear.setButtonText("Linear");
    ButtonDetectorLinear.setRadioGroupId(3);
    ButtonDetectorLinear.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDetectorLinear.setColour(TextButton::buttonOnColourId, Colours::yellow.withRotatedHue(+0.08f));

    ButtonDetectorLinear.addListener(this);
    addAndMakeVisible(&ButtonDetectorLinear);


    ButtonDetectorSmoothDecoupled.setButtonText("S-Curve");
    ButtonDetectorSmoothDecoupled.setRadioGroupId(3);
    ButtonDetectorSmoothDecoupled.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDetectorSmoothDecoupled.setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonDetectorSmoothDecoupled.addListener(this);
    addAndMakeVisible(&ButtonDetectorSmoothDecoupled);


    ButtonDetectorSmoothBranching.setButtonText("Log");
    ButtonDetectorSmoothBranching.setRadioGroupId(3);
    ButtonDetectorSmoothBranching.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDetectorSmoothBranching.setColour(TextButton::buttonOnColourId, Colours::yellow.withRotatedHue(-0.08f));

    ButtonDetectorSmoothBranching.addListener(this);
    addAndMakeVisible(&ButtonDetectorSmoothBranching);


    ButtonGainStageFET.setButtonText("FET");
    ButtonGainStageFET.setRadioGroupId(4);
    ButtonGainStageFET.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonGainStageFET.setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonGainStageFET.addListener(this);
    addAndMakeVisible(&ButtonGainStageFET);


    ButtonGainStageOptical.setButtonText("Opto");
    ButtonGainStageOptical.setRadioGroupId(4);
    ButtonGainStageOptical.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonGainStageOptical.setColour(TextButton::buttonOnColourId, Colours::yellow.withRotatedHue(-0.08f));

    ButtonGainStageOptical.addListener(this);
    addAndMakeVisible(&ButtonGainStageOptical);


    int nIndex = SqueezerPluginParameters::selThreshold;
    int nIndexSwitch = SqueezerPluginParameters::selThresholdSwitch;
    String strName = parameters->getName(nIndex);
    SliderThresholdCombined = new SliderCombined(strName, parameters, nIndex, nIndexSwitch);
    SliderThresholdCombined->setSliderColour(Colours::purple.brighter(0.2f));

    SliderThresholdCombined->addListener(this);
    SliderThresholdCombined->addButtonListener(this);
    addAndMakeVisible(SliderThresholdCombined);


    nIndex = SqueezerPluginParameters::selRatio;
    nIndexSwitch = SqueezerPluginParameters::selRatioSwitch;
    strName = parameters->getName(nIndex);
    SliderRatioCombined = new SliderCombined(strName, parameters, nIndex, nIndexSwitch);
    SliderRatioCombined->setSliderColour(Colours::purple.brighter(0.2f));

    SliderRatioCombined->addListener(this);
    SliderRatioCombined->addButtonListener(this);
    addAndMakeVisible(SliderRatioCombined);

    nIndex = SqueezerPluginParameters::selKneeWidth;
    nIndexSwitch = SqueezerPluginParameters::selKneeWidthSwitch;
    strName = parameters->getName(nIndex);
    SliderKneeWidthCombined = new SliderCombined(strName, parameters, nIndex, nIndexSwitch);
    SliderKneeWidthCombined->setSliderColour(Colours::purple.brighter(0.2f));

    SliderKneeWidthCombined->addListener(this);
    SliderKneeWidthCombined->addButtonListener(this);
    addAndMakeVisible(SliderKneeWidthCombined);


    nIndex = SqueezerPluginParameters::selAttackRate;
    nIndexSwitch = SqueezerPluginParameters::selAttackRateSwitch;
    strName = parameters->getName(nIndex);
    SliderAttackRateCombined = new SliderCombined(strName, parameters, nIndex, nIndexSwitch);
    SliderAttackRateCombined->setSliderColour(Colours::yellow);

    SliderAttackRateCombined->addListener(this);
    SliderAttackRateCombined->addButtonListener(this);
    addAndMakeVisible(SliderAttackRateCombined);


    nIndex = SqueezerPluginParameters::selReleaseRate;
    nIndexSwitch = SqueezerPluginParameters::selReleaseRateSwitch;
    strName = parameters->getName(nIndex);
    SliderReleaseRateCombined = new SliderCombined(strName, parameters, nIndex, nIndexSwitch);
    SliderReleaseRateCombined->setSliderColour(Colours::yellow);

    SliderReleaseRateCombined->addListener(this);
    SliderReleaseRateCombined->addButtonListener(this);
    addAndMakeVisible(SliderReleaseRateCombined);


#ifdef SQUEEZER_STEREO
    nIndex = SqueezerPluginParameters::selStereoLink;
    nIndexSwitch = SqueezerPluginParameters::selStereoLinkSwitch;
    strName = parameters->getName(nIndex);
    SliderStereoLinkCombined = new SliderCombined(strName, parameters, nIndex, nIndexSwitch);
    SliderStereoLinkCombined->setSliderColour(Colours::purple.brighter(0.2f));

    SliderStereoLinkCombined->addListener(this);
    SliderStereoLinkCombined->addButtonListener(this);
    addAndMakeVisible(SliderStereoLinkCombined);
#endif


    ButtonAutoMakeupGain.setButtonText("Auto MU");
    ButtonAutoMakeupGain.setColour(TextButton::buttonColourId, Colours::grey);
    ButtonAutoMakeupGain.setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonAutoMakeupGain.addListener(this);
    addAndMakeVisible(&ButtonAutoMakeupGain);


    nIndex = SqueezerPluginParameters::selMakeupGain;
    nIndexSwitch = SqueezerPluginParameters::selMakeupGainSwitch;
    strName = parameters->getName(nIndex);
    SliderMakeupGainCombined = new SliderCombined(strName, parameters, nIndex, nIndexSwitch);
    SliderMakeupGainCombined->setSliderColour(Colours::blue.brighter(0.4f));

    SliderMakeupGainCombined->addListener(this);
    SliderMakeupGainCombined->addButtonListener(this);
    addAndMakeVisible(SliderMakeupGainCombined);


    nIndex = SqueezerPluginParameters::selWetMix;
    nIndexSwitch = SqueezerPluginParameters::selWetMixSwitch;
    strName = parameters->getName(nIndex);
    SliderWetMixCombined = new SliderCombined(strName, parameters, nIndex, nIndexSwitch);
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
    strName = parameters->getName(nIndex);
    SliderSidechainFilterCutoffCombined = new SliderCombined(strName, parameters, nIndex, nIndexSwitch);
    SliderSidechainFilterCutoffCombined->setSliderColour(Colours::green.brighter(0.1f));

    SliderSidechainFilterCutoffCombined->addListener(this);
    SliderSidechainFilterCutoffCombined->addButtonListener(this);
    addAndMakeVisible(SliderSidechainFilterCutoffCombined);


    nIndex = SqueezerPluginParameters::selSidechainFilterGain;
    strName = parameters->getName(nIndex);
    SliderSidechainFilterGain = new SliderSwitchLinearBar(strName, parameters, nIndex);
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
    addAndMakeVisible(&LabelDebug);
#endif


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

    int y = 20;

#ifdef SQUEEZER_STEREO
    int x = 610;
    int x_spacing = 28;
    int width = 12;
#else
    int x = 570;
    int x_spacing = 20;
    int width = 16;
#endif

    for (int nChannel = 0; nChannel < nChannels; ++nChannel)
    {
        bool discreteMeter = true;

        MeterBarLevel *pMeterBarLevelInput = p_arrInputLevelMeters.add(
                new MeterBarLevel());
        pMeterBarLevelInput->create(20,
                                    MeterBarLevel::orientationVertical,
                                    discreteMeter,
                                    5,
                                    12);

        pMeterBarLevelInput->setBounds(x, y, width, 0);
        addAndMakeVisible(pMeterBarLevelInput);

        MeterBarLevel *pMeterBarLevelOutput = p_arrOutputLevelMeters.add(
                new MeterBarLevel());
        pMeterBarLevelOutput->create(20,
                                     MeterBarLevel::orientationVertical,
                                     discreteMeter,
                                     5,
                                     12);

        pMeterBarLevelOutput->setBounds(x + 2 * x_spacing, y, width, 0);
        addAndMakeVisible(pMeterBarLevelOutput);

        MeterBarGainReduction *pMeterBarGainReduction = p_arrGainReductionMeters.add(
                    new MeterBarGainReduction());
        pMeterBarGainReduction->create(
            MeterBarLevel::orientationVertical,
            discreteMeter,
            5,
            12);

        pMeterBarGainReduction->setBounds(x + x_spacing, y, width, 0);
        addAndMakeVisible(pMeterBarGainReduction);

        x += width;
    }

    // This is where our plug-in editor's size is set.
    resizeEditor();

    updateParameter(SqueezerPluginParameters::selBypass);
    updateParameter(SqueezerPluginParameters::selDetectorRmsFilter);
    updateParameter(SqueezerPluginParameters::selDesign);
    updateParameter(SqueezerPluginParameters::selDetector);
    updateParameter(SqueezerPluginParameters::selGainStage);

    updateParameter(SqueezerPluginParameters::selThresholdSwitch);
    updateParameter(SqueezerPluginParameters::selThreshold);
    updateParameter(SqueezerPluginParameters::selRatioSwitch);
    updateParameter(SqueezerPluginParameters::selRatio);
    updateParameter(SqueezerPluginParameters::selKneeWidthSwitch);
    updateParameter(SqueezerPluginParameters::selKneeWidth);

    updateParameter(SqueezerPluginParameters::selAttackRateSwitch);
    updateParameter(SqueezerPluginParameters::selAttackRate);
    updateParameter(SqueezerPluginParameters::selReleaseRateSwitch);
    updateParameter(SqueezerPluginParameters::selReleaseRate);

#ifdef SQUEEZER_STEREO
    updateParameter(SqueezerPluginParameters::selStereoLinkSwitch);
    updateParameter(SqueezerPluginParameters::selStereoLink);
#endif

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
}


SqueezerAudioProcessorEditor::~SqueezerAudioProcessorEditor()
{
    pProcessor->removeActionListener(this);
}


void SqueezerAudioProcessorEditor::resizeEditor()
{
    nHeight = 188;

#ifdef SQUEEZER_STEREO
    nWidth = 710;
#else
    nWidth = 646;
#endif

    int x = 15;
    int y1 = 18;
    int y2 = y1 + 78;

    setSize(nWidth, nHeight);

    ButtonDetectorRmsPeak.setBounds(x + 37, y2, 52, 20);
    ButtonDetectorRmsFast.setBounds(x + 97, y2, 52, 20);
    ButtonDetectorRmsMedium.setBounds(x + 157, y2, 52, 20);
    ButtonDetectorRmsSlow.setBounds(x + 217, y2, 52, 20);

    ButtonDesignFeedForward.setBounds(x + 22, y2 + 25, 52, 20);
    ButtonDesignFeedBack.setBounds(x + 82, y2 + 25, 52, 20);

    ButtonGainStageFET.setBounds(x + 172, y2 + 25, 52, 20);
    ButtonGainStageOptical.setBounds(x + 232, y2 + 25, 52, 20);

    ButtonDetectorLinear.setBounds(x + 67, y2 + 50, 52, 20);
    ButtonDetectorSmoothDecoupled.setBounds(x + 127, y2 + 50, 52, 20);
    ButtonDetectorSmoothBranching.setBounds(x + 187, y2 + 50, 52, 20);

    SliderThresholdCombined->setBounds(x, y1, 52, 60);
    SliderRatioCombined->setBounds(x + 60, y1, 52, 60);
    SliderKneeWidthCombined->setBounds(x + 120, y1, 52, 60);

    SliderAttackRateCombined->setBounds(x + 195, y1, 52, 60);
    SliderReleaseRateCombined->setBounds(x + 255, y1, 52, 60);

    SliderSidechainFilterCutoffCombined->setBounds(x + 330, y1, 52, 60);
    SliderSidechainFilterGain->setBounds(x + 330, y2 + 26, 52, 18);
    ButtonSidechainFilterState.setBounds(x + 330, y2, 52, 20);
    ButtonSidechainListen.setBounds(x + 330, y2 + 50, 52, 20);

#ifdef SQUEEZER_STEREO
    SliderStereoLinkCombined->setBounds(x + 405, y1, 52, 60);

    SliderMakeupGainCombined->setBounds(x + 465, y1, 52, 60);
    SliderWetMixCombined->setBounds(x + 525, y1, 52, 60);

    ButtonAutoMakeupGain.setBounds(x + 465, y2, 52, 20);
    ButtonAbout.setBounds(x + 435, y2 + 25, 52, 20);
    ButtonSettings.setBounds(x + 495, y2 + 25, 52, 20);
    ButtonBypass.setBounds(x + 465, y2 + 50, 52, 20);

    ButtonResetMeters.setBounds(x + 611, y2 + 50, 52, 20);

#else
    SliderMakeupGainCombined->setBounds(x + 415, y1, 52, 60);
    SliderWetMixCombined->setBounds(x + 475, y1, 52, 60);

    ButtonAutoMakeupGain.setBounds(x + 445, y2, 52, 20);
    ButtonAbout.setBounds(x + 415, y2 + 25, 52, 20);
    ButtonSettings.setBounds(x + 475, y2 + 25, 52, 20);
    ButtonBypass.setBounds(x + 445, y2 + 50, 52, 20);

    ButtonResetMeters.setBounds(x + 557, y2 + 50, 52, 20);

#endif

#ifdef DEBUG
    LabelDebug.setBounds(x + 127, y2 + 26, 52, 16);
#endif
}


void SqueezerAudioProcessorEditor::windowAboutCallback(int modalResult)
{
    // manually deactivate about button
    ButtonAbout.setToggleState(false, dontSendNotification);
}


void SqueezerAudioProcessorEditor::windowSettingsCallback(int modalResult)
{
    // manually deactivate about button
    ButtonSettings.setToggleState(false, dontSendNotification);
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
        for (int nChannel = 0; nChannel < nChannels; ++nChannel)
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
            float gainReductionPeak = pProcessor->getGainReductionPeak(nChannel);

            // make sure gain reduction meter doesn't show anything
            // while there is no gain reduction
            gainReduction -= 0.01f;
            gainReductionPeak -= 0.01f;

            p_arrGainReductionMeters[nChannel]->setNormalLevels(
                gainReduction, gainReductionPeak);
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

        if (fValue == (Compressor::DetectorRmsFilterPeak / float(Compressor::NumberOfDetectorRmsFilters - 1)))
        {
            ButtonDetectorRmsPeak.setToggleState(true, dontSendNotification);
        }
        else if (fValue == (Compressor::DetectorRmsFilterFast / float(Compressor::NumberOfDetectorRmsFilters - 1)))
        {
            ButtonDetectorRmsFast.setToggleState(true, dontSendNotification);
        }
        else if (fValue == (Compressor::DetectorRmsFilterMedium / float(Compressor::NumberOfDetectorRmsFilters - 1)))
        {
            ButtonDetectorRmsMedium.setToggleState(true, dontSendNotification);
        }
        else
        {
            ButtonDetectorRmsSlow.setToggleState(true, dontSendNotification);
        }

        break;

    case SqueezerPluginParameters::selDesign:

        if (fValue == Compressor::DesignFeedForward)
        {
            ButtonDesignFeedForward.setToggleState(true, dontSendNotification);
        }
        else
        {
            ButtonDesignFeedBack.setToggleState(true, dontSendNotification);
        }

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

    case SqueezerPluginParameters::selGainStage:

        if (fValue == (Compressor::GainStageFET / float(Compressor::NumberOfGainStages - 1)))
        {
            ButtonGainStageFET.setToggleState(true, dontSendNotification);
        }
        else
        {
            ButtonGainStageOptical.setToggleState(true, dontSendNotification);
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

            for (int nChannel = 0; nChannel < nChannels; ++nChannel)
            {
                p_arrGainReductionMeters[nChannel]->setUpwardExpansion(bUpwardExpansion);
            }
        }

        break;

    case SqueezerPluginParameters::selKneeWidthSwitch:
        SliderKneeWidthCombined->updateMode();
        break;

    case SqueezerPluginParameters::selKneeWidth:
        SliderKneeWidthCombined->setValue(fValue, dontSendNotification);
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

#ifdef SQUEEZER_STEREO

    case SqueezerPluginParameters::selStereoLinkSwitch:
        SliderStereoLinkCombined->updateMode();
        break;

    case SqueezerPluginParameters::selStereoLink:
        SliderStereoLinkCombined->setValue(fValue, dontSendNotification);
        break;
#endif

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

    g.setColour(Colours::grey.darker(0.0f));
    g.fillRect(x      , y1, 317, 168);
    g.fillRect(x + 320, y1,  82, 168);
#ifdef SQUEEZER_STEREO
    g.fillRect(x + 405, y1, 182, 168);
    g.fillRect(x + 590, y1, 100, 168);
#else
    g.fillRect(x + 405, y1, 142, 168);
    g.fillRect(x + 550, y1,  76, 168);
#endif

    g.setColour(Colours::darkgrey);
    g.drawRect(x      , y1, 317, 168);
    g.drawRect(x + 320, y1,  82, 168);
#ifdef SQUEEZER_STEREO
    g.drawRect(x + 405, y1, 182, 168);
    g.drawRect(x + 590, y1, 100, 168);
#else
    g.drawRect(x + 405, y1, 142, 168);
    g.drawRect(x + 550, y1,  76, 168);
#endif

    g.setColour(Colours::lightgrey.darker(0.2f));
    g.fillRect(x +   5, y2, 307,  85);
    g.fillRect(x + 325, y2,  72,  85);
#ifdef SQUEEZER_STEREO
    g.fillRect(x + 410, y2, 172,  85);
    g.fillRect(x + 595, y1 + 5,  90, 158);
#else
    g.fillRect(x + 410, y2, 132,  85);
    g.fillRect(x + 555, y1 + 5,  66, 158);
#endif

    g.setColour(Colours::grey.darker(0.2f));
    g.drawRect(x +   5, y2, 307,  85);
    g.drawRect(x + 325, y2,  72,  85);
#ifdef SQUEEZER_STEREO
    g.drawRect(x + 410, y2, 172,  85);
    g.drawRect(x + 595, y1 + 5,  90, 158);
#else
    g.drawRect(x + 410, y2, 132,  85);
    g.drawRect(x + 555, y1 + 5,  66, 158);
#endif
}


void SqueezerAudioProcessorEditor::buttonClicked(Button *button)
{
    if (button == &ButtonBypass)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selBypass, !button->getToggleState());
    }
    else if (button == &ButtonDetectorRmsPeak)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDetectorRmsFilter, Compressor::DetectorRmsFilterPeak / float(Compressor::NumberOfDetectorRmsFilters - 1));
    }
    else if (button == &ButtonDetectorRmsFast)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDetectorRmsFilter, Compressor::DetectorRmsFilterFast / float(Compressor::NumberOfDetectorRmsFilters - 1));
    }
    else if (button == &ButtonDetectorRmsMedium)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDetectorRmsFilter, Compressor::DetectorRmsFilterMedium / float(Compressor::NumberOfDetectorRmsFilters - 1));
    }
    else if (button == &ButtonDetectorRmsSlow)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDetectorRmsFilter, Compressor::DetectorRmsFilterSlow / float(Compressor::NumberOfDetectorRmsFilters - 1));
    }
    else if (button == &ButtonDesignFeedForward)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDesign, Compressor::DesignFeedForward);
    }
    else if (button == &ButtonDesignFeedBack)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDesign, Compressor::DesignFeedBack);
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
    else if (button == &ButtonGainStageFET)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selGainStage, Compressor::GainStageFET / float(Compressor::NumberOfGainStages - 1));
    }
    else if (button == &ButtonGainStageOptical)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selGainStage, Compressor::GainStageOptical / float(Compressor::NumberOfGainStages - 1));
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
        DialogWindow *windowSettings = GenericWindowSettingsContent::createDialogWindow(
                                           this, width, height, pluginSettings);

        // attach callback to dialog window
        ModalComponentManager::getInstance()->attachCallback(windowSettings, ModalCallbackFunction::forComponent(window_settings_callback, this));

        // manually deactivate button
        button->setToggleState(false, dontSendNotification);
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
#if (KMETER_LV2_PLUGIN != 0)
            L"LV2\n"
#endif
#ifdef LINUX
            L"POSIX Threads\n"
            L"Xlib\n"
            L"Xext\n"
#endif
        );

#if (JUCE_USE_VSTSDK_2_4 != 0)
        // display trademarks (but only when necessary)
        arrChapters.set(
            "Trademarks",
            L"VST PlugIn Technology by Steinberg\n");
#endif

#if (JUCE_ASIO != 0)
        // display trademarks (but only when necessary)
        arrChapters.set(
            "Trademarks",
            L"ASIO Technology by Steinberg Media Technologies GmbH\n");
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

        DialogWindow *windowAbout = GenericWindowAboutContent::createDialogWindow(
                                        this, width, height, arrChapters);

        // attach callback to dialog window
        ModalComponentManager::getInstance()->attachCallback(windowAbout, ModalCallbackFunction::forComponent(window_about_callback, this));
    }
    else
    {
        float fValue = button->getToggleState() ? 1.0f : 0.0f;
        SliderCombined *slider = dynamic_cast<SliderCombined *>(button->getParentComponent());

        if (slider == SliderThresholdCombined)
        {
            pProcessor->changeParameter(SqueezerPluginParameters::selThresholdSwitch, fValue);
        }
        else if (slider == SliderRatioCombined)
        {
            pProcessor->changeParameter(SqueezerPluginParameters::selRatioSwitch, fValue);
        }
        else if (slider == SliderKneeWidthCombined)
        {
            pProcessor->changeParameter(SqueezerPluginParameters::selKneeWidthSwitch, fValue);
        }
        else if (slider == SliderAttackRateCombined)
        {
            pProcessor->changeParameter(SqueezerPluginParameters::selAttackRateSwitch, fValue);
        }
        else if (slider == SliderReleaseRateCombined)
        {
            pProcessor->changeParameter(SqueezerPluginParameters::selReleaseRateSwitch, fValue);
        }

#ifdef SQUEEZER_STEREO
        else if (slider == SliderStereoLinkCombined)
        {
            pProcessor->changeParameter(SqueezerPluginParameters::selStereoLinkSwitch, fValue);
        }

#endif
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
    else if (slider == SliderKneeWidthCombined)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selKneeWidth, fValue);
    }
    else if (slider == SliderAttackRateCombined)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selAttackRate, fValue);
    }
    else if (slider == SliderReleaseRateCombined)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selReleaseRate, fValue);
    }

#ifdef SQUEEZER_STEREO
    else if (slider == SliderStereoLinkCombined)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selStereoLink, fValue);
    }

#endif
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
