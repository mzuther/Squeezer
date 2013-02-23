/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

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

#include "plugin_editor_squeezer.h"

//==============================================================================
SqueezerAudioProcessorEditor::SqueezerAudioProcessorEditor(SqueezerAudioProcessor* ownerFilter, SqueezerPluginParameters* parameters)
    : AudioProcessorEditor(ownerFilter)
{
    // the editor window does not have any transparent areas
    // (increases performance on redrawing)
    setOpaque(true);

    // The plug-in editor's size as well as the location of buttons
    // and labels will be set later on in this constructor.

    pProcessor = ownerFilter;
    pProcessor->addActionListener(this);

    nChannels = 2;

    ButtonBypass = new TextButton("Bypass");
    ButtonBypass->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonBypass->setColour(TextButton::buttonOnColourId, Colours::red);

    ButtonBypass->addListener(this);
    addAndMakeVisible(ButtonBypass);


    ButtonLevelDetectionPeak = new TextButton("Peak");
    ButtonLevelDetectionPeak->setRadioGroupId(1);
    ButtonLevelDetectionPeak->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonLevelDetectionPeak->setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonLevelDetectionPeak->addListener(this);
    addAndMakeVisible(ButtonLevelDetectionPeak);


    ButtonLevelDetectionRms = new TextButton("RMS");
    ButtonLevelDetectionRms->setRadioGroupId(1);
    ButtonLevelDetectionRms->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonLevelDetectionRms->setColour(TextButton::buttonOnColourId, Colours::yellow.withRotatedHue(-0.05f));

    ButtonLevelDetectionRms->addListener(this);
    addAndMakeVisible(ButtonLevelDetectionRms);


    ButtonDesignFeedForward = new TextButton("F.Forw");
    ButtonDesignFeedForward->setRadioGroupId(2);
    ButtonDesignFeedForward->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDesignFeedForward->setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonDesignFeedForward->addListener(this);
    addAndMakeVisible(ButtonDesignFeedForward);


    ButtonDesignFeedBack = new TextButton("F.Back");
    ButtonDesignFeedBack->setRadioGroupId(2);
    ButtonDesignFeedBack->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDesignFeedBack->setColour(TextButton::buttonOnColourId, Colours::yellow.withRotatedHue(-0.05f));

    ButtonDesignFeedBack->addListener(this);
    addAndMakeVisible(ButtonDesignFeedBack);


    ButtonDetectorLinear = new TextButton("Linear");
    ButtonDetectorLinear->setRadioGroupId(3);
    ButtonDetectorLinear->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDetectorLinear->setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonDetectorLinear->addListener(this);
    addAndMakeVisible(ButtonDetectorLinear);


    ButtonDetectorSmoothDecoupled = new TextButton("S-Curve");
    ButtonDetectorSmoothDecoupled->setRadioGroupId(3);
    ButtonDetectorSmoothDecoupled->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDetectorSmoothDecoupled->setColour(TextButton::buttonOnColourId, Colours::yellow.withRotatedHue(-0.05f));

    ButtonDetectorSmoothDecoupled->addListener(this);
    addAndMakeVisible(ButtonDetectorSmoothDecoupled);


    ButtonDetectorSmoothBranching = new TextButton("Log");
    ButtonDetectorSmoothBranching->setRadioGroupId(3);
    ButtonDetectorSmoothBranching->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDetectorSmoothBranching->setColour(TextButton::buttonOnColourId, Colours::yellow.withRotatedHue(-0.10f));

    ButtonDetectorSmoothBranching->addListener(this);
    addAndMakeVisible(ButtonDetectorSmoothBranching);


    ButtonDetectorOptical = new TextButton("Opto");
    ButtonDetectorOptical->setRadioGroupId(3);
    ButtonDetectorOptical->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDetectorOptical->setColour(TextButton::buttonOnColourId, Colours::yellow.withRotatedHue(-0.15f));

    ButtonDetectorOptical->addListener(this);
    addAndMakeVisible(ButtonDetectorOptical);


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


    nIndex = SqueezerPluginParameters::selStereoLink;
    nIndexSwitch = SqueezerPluginParameters::selStereoLinkSwitch;
    strName = parameters->getName(nIndex);
    SliderStereoLinkCombined = new SliderCombined(strName, parameters, nIndex, nIndexSwitch);
    SliderStereoLinkCombined->setSliderColour(Colours::purple.brighter(0.2f));

    SliderStereoLinkCombined->addListener(this);
    SliderStereoLinkCombined->addButtonListener(this);
    addAndMakeVisible(SliderStereoLinkCombined);


    ButtonAutoMakeupGain = new TextButton("Auto");
    ButtonAutoMakeupGain->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonAutoMakeupGain->setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonAutoMakeupGain->addListener(this);
    addAndMakeVisible(ButtonAutoMakeupGain);


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


#ifdef DEBUG
    LabelDebug = new Label("Debug Notification", "DEBUG");
    LabelDebug->setColour(Label::textColourId, Colours::red);
    LabelDebug->setJustificationType(Justification::centred);
    addAndMakeVisible(LabelDebug);
#else
    LabelDebug = NULL;
#endif


    ButtonResetMeters = new TextButton("Reset");
    ButtonResetMeters->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonResetMeters->setColour(TextButton::buttonOnColourId, Colours::red);

    ButtonResetMeters->addListener(this);
    addAndMakeVisible(ButtonResetMeters);


    ButtonSettings = new TextButton("Settings");
    ButtonSettings->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonSettings->setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonSettings->addListener(this);
    addAndMakeVisible(ButtonSettings);


    ButtonAbout = new TextButton("About");
    ButtonAbout->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonAbout->setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonAbout->addListener(this);
    addAndMakeVisible(ButtonAbout);


    pInputLevelMeters = new MeterBarLevel*[nChannels];
    pOutputLevelMeters = new MeterBarLevel*[nChannels];
    pGainReductionMeters = new MeterBarGainReduction*[nChannels];

    int x = 550;
    int width = 12;

    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        pInputLevelMeters[nChannel] = new MeterBarLevel("Input Level Meter #" + String(nChannel), x, 12, width, 24, 5);
        addAndMakeVisible(pInputLevelMeters[nChannel]);

        pOutputLevelMeters[nChannel] = new MeterBarLevel("Output Level Meter #" + String(nChannel), x + 60, 12, width, 24, 5);
        addAndMakeVisible(pOutputLevelMeters[nChannel]);

        pGainReductionMeters[nChannel] = new MeterBarGainReduction("Gain Reduction Meter #" + String(nChannel), x + 30, 12, width, 24, 5);
        addAndMakeVisible(pGainReductionMeters[nChannel]);

        x += width;
    }


    // This is where our plug-in editor's size is set.
    resizeEditor();

    updateParameter(SqueezerPluginParameters::selBypass);
    updateParameter(SqueezerPluginParameters::selLevelDetection);
    updateParameter(SqueezerPluginParameters::selDesign);
    updateParameter(SqueezerPluginParameters::selDetector);

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

    updateParameter(SqueezerPluginParameters::selStereoLinkSwitch);
    updateParameter(SqueezerPluginParameters::selStereoLink);
    updateParameter(SqueezerPluginParameters::selAutoMakeupGain);
    updateParameter(SqueezerPluginParameters::selMakeupGainSwitch);
    updateParameter(SqueezerPluginParameters::selMakeupGain);
    updateParameter(SqueezerPluginParameters::selWetMixSwitch);
    updateParameter(SqueezerPluginParameters::selWetMix);
}


SqueezerAudioProcessorEditor::~SqueezerAudioProcessorEditor()
{
    pProcessor->removeActionListener(this);

    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        delete pInputLevelMeters[nChannel];
        pInputLevelMeters[nChannel] = NULL;

        delete pOutputLevelMeters[nChannel];
        pOutputLevelMeters[nChannel] = NULL;

        delete pGainReductionMeters[nChannel];
        pGainReductionMeters[nChannel] = NULL;
    }

    delete[] pInputLevelMeters;
    pInputLevelMeters = NULL;

    delete[] pOutputLevelMeters;
    pOutputLevelMeters = NULL;

    delete[] pGainReductionMeters;
    pGainReductionMeters = NULL;

    deleteAllChildren();
}


void SqueezerAudioProcessorEditor::resizeEditor()
{
    nHeight = 180;
    nWidth = 660;

    setSize(nWidth, nHeight);

    ButtonBypass->setBounds(480, 90, 52, 20);
    ButtonLevelDetectionPeak->setBounds(20, 90, 52, 20);
    ButtonLevelDetectionRms->setBounds(80, 90, 52, 20);
    ButtonDesignFeedForward->setBounds(20, 115, 52, 20);
    ButtonDesignFeedBack->setBounds(80, 115, 52, 20);

    ButtonDetectorLinear->setBounds(20, 140, 52, 20);
    ButtonDetectorSmoothDecoupled->setBounds(80, 140, 52, 20);
    ButtonDetectorSmoothBranching->setBounds(140, 140, 52, 20);
    ButtonDetectorOptical->setBounds(200, 140, 52, 20);

    SliderThresholdCombined->setBounds(20, 15, 52, 60);
    SliderRatioCombined->setBounds(80, 15, 52, 60);
    SliderKneeWidthCombined->setBounds(140, 15, 52, 60);

    SliderAttackRateCombined->setBounds(220, 15, 52, 60);
    SliderReleaseRateCombined->setBounds(280, 15, 52, 60);

    SliderStereoLinkCombined->setBounds(360, 15, 52, 60);
    ButtonAutoMakeupGain->setBounds(420, 90, 52, 20);
    SliderMakeupGainCombined->setBounds(420, 15, 52, 60);
    SliderWetMixCombined->setBounds(480, 15, 52, 60);

    ButtonResetMeters->setBounds(565, 140, 52, 20);
    ButtonSettings->setBounds(420, 140, 52, 20);
    ButtonAbout->setBounds(480, 140, 52, 20);

    if (LabelDebug)
    {
        LabelDebug->setBounds(480, 117, 52, 16);
    }
}


void SqueezerAudioProcessorEditor::actionListenerCallback(const String& strMessage)
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
        for (int nChannel = 0; nChannel < nChannels; nChannel++)
        {
            float fPeakInputLevel = pProcessor->getPeakMeterInputLevel(nChannel);
            float fAverageInputLevel = pProcessor->getAverageMeterInputLevel(nChannel);
            float fPeakInputPeakLevel = pProcessor->getPeakMeterPeakInputLevel(nChannel);
            pInputLevelMeters[nChannel]->setLevel(fPeakInputLevel, fAverageInputLevel, fPeakInputPeakLevel);

            float fPeakOutputLevel = pProcessor->getPeakMeterOutputLevel(nChannel);
            float fAverageOutputLevel = pProcessor->getAverageMeterOutputLevel(nChannel);
            float fPeakOutputPeakLevel = pProcessor->getPeakMeterPeakOutputLevel(nChannel);
            pOutputLevelMeters[nChannel]->setLevel(fPeakOutputLevel, fAverageOutputLevel, fPeakOutputPeakLevel);

            float fGainReduction = pProcessor->getGainReduction(nChannel);
            float fGainReductionPeak = pProcessor->getGainReductionPeak(nChannel);
            pGainReductionMeters[nChannel]->setGainReduction(fGainReduction, fGainReductionPeak);
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
        ButtonBypass->setToggleState(fValue != 0.0f, false);
        break;
    case SqueezerPluginParameters::selLevelDetection:

        if (fValue == 0.0f)
        {
            ButtonLevelDetectionPeak->setToggleState(true, false);
        }
        else
        {
            ButtonLevelDetectionRms->setToggleState(true, false);
        }

        break;
    case SqueezerPluginParameters::selDesign:

        if (fValue == Compressor::DesignFeedForward)
        {
            ButtonDesignFeedForward->setToggleState(true, false);
        }
        else
        {
            ButtonDesignFeedBack->setToggleState(true, false);
        }

        break;
    case SqueezerPluginParameters::selDetector:

        if (fValue == (Compressor::DetectorLinear / float(Compressor::NumberOfDetectors - 1)))
        {
            ButtonDetectorLinear->setToggleState(true, false);
        }
        else if (fValue == (Compressor::DetectorSmoothDecoupled / float(Compressor::NumberOfDetectors - 1)))
        {
            ButtonDetectorSmoothDecoupled->setToggleState(true, false);
        }
        else if (fValue == (Compressor::DetectorSmoothBranching / float(Compressor::NumberOfDetectors - 1)))
        {
            ButtonDetectorSmoothBranching->setToggleState(true, false);
        }
        else
        {
            ButtonDetectorOptical->setToggleState(true, false);
        }

        break;
    case SqueezerPluginParameters::selThresholdSwitch:
        SliderThresholdCombined->updateMode();
        break;
    case SqueezerPluginParameters::selThreshold:
        SliderThresholdCombined->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selRatioSwitch:
        SliderRatioCombined->updateMode();
        break;
    case SqueezerPluginParameters::selRatio:
        SliderRatioCombined->setValue(fValue, false);

        {
            float fRealValue = SliderRatioCombined->getRealFloat();
            bool bUpwardExpansion = (fRealValue < 1.0f);

            for (int nChannel = 0; nChannel < nChannels; nChannel++)
            {
                pGainReductionMeters[nChannel]->setUpwardExpansion(bUpwardExpansion);
            }
        }

        break;
    case SqueezerPluginParameters::selKneeWidthSwitch:
        SliderKneeWidthCombined->updateMode();
        break;
    case SqueezerPluginParameters::selKneeWidth:
        SliderKneeWidthCombined->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selAttackRateSwitch:
        SliderAttackRateCombined->updateMode();
        break;
    case SqueezerPluginParameters::selAttackRate:
        SliderAttackRateCombined->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selReleaseRateSwitch:
        SliderReleaseRateCombined->updateMode();
        break;
    case SqueezerPluginParameters::selReleaseRate:
        SliderReleaseRateCombined->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selStereoLinkSwitch:
        SliderStereoLinkCombined->updateMode();
        break;
    case SqueezerPluginParameters::selStereoLink:
        SliderStereoLinkCombined->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selAutoMakeupGain:
        ButtonAutoMakeupGain->setToggleState(fValue != 0.0f, false);
        break;
    case SqueezerPluginParameters::selMakeupGainSwitch:
        SliderMakeupGainCombined->updateMode();
        break;
    case SqueezerPluginParameters::selMakeupGain:
        SliderMakeupGainCombined->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selWetMixSwitch:
        SliderWetMixCombined->updateMode();
        break;
    case SqueezerPluginParameters::selWetMix:
        SliderWetMixCombined->setValue(fValue, false);
        break;
    default:
        DBG("[Squeezer] editor::updateParameter --> invalid index");
        break;
    }
}


//==============================================================================
void SqueezerAudioProcessorEditor::paint(Graphics& g)
{
    g.setColour(Colours::darkgrey.darker(2.5f));
    g.fillAll();
}


void SqueezerAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == ButtonBypass)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selBypass, !button->getToggleState());
    }
    else if (button == ButtonLevelDetectionPeak)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selLevelDetection, 0.0f);
    }
    else if (button == ButtonLevelDetectionRms)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selLevelDetection, 1.0f);
    }
    else if (button == ButtonDesignFeedForward)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDesign, Compressor::DesignFeedForward);
    }
    else if (button == ButtonDesignFeedBack)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDesign, Compressor::DesignFeedBack);
    }
    else if (button == ButtonDetectorLinear)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDetector, Compressor::DetectorLinear / float(Compressor::NumberOfDetectors - 1));
    }
    else if (button == ButtonDetectorSmoothDecoupled)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDetector, Compressor::DetectorSmoothDecoupled / float(Compressor::NumberOfDetectors - 1));
    }
    else if (button == ButtonDetectorSmoothBranching)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDetector, Compressor::DetectorSmoothBranching / float(Compressor::NumberOfDetectors - 1));
    }
    else if (button == ButtonDetectorOptical)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDetector, Compressor::DetectorOptical / float(Compressor::NumberOfDetectors - 1));
    }
    else if (button == ButtonAutoMakeupGain)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selAutoMakeupGain, !button->getToggleState());
    }
    else if (button == ButtonResetMeters)
    {
        pProcessor->resetMeters();
    }
    else if (button == ButtonSettings)
    {
        String strPluginSettings = pProcessor->getParameters();

        WindowSettings* windowSettings = new WindowSettings(getWidth() - 184, getHeight(), strPluginSettings);
        addAndMakeVisible(windowSettings);

        windowSettings->runModalLoop();

        removeChildComponent(windowSettings);
        delete windowSettings;
        windowSettings = NULL;
    }
    else if (button == ButtonAbout)
    {
        WindowAbout* windowAbout = new WindowAbout(getWidth() - 120, getHeight());
        addAndMakeVisible(windowAbout);

        windowAbout->runModalLoop();

        removeChildComponent(windowAbout);
        delete windowAbout;
        windowAbout = NULL;
    }
    else
    {
        float fValue = button->getToggleState() ? 1.0f : 0.0f;
        SliderCombined* slider = dynamic_cast<SliderCombined*>(button->getParentComponent());

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
        else if (slider == SliderStereoLinkCombined)
        {
            pProcessor->changeParameter(SqueezerPluginParameters::selStereoLinkSwitch, fValue);
        }
        else if (slider == SliderMakeupGainCombined)
        {
            pProcessor->changeParameter(SqueezerPluginParameters::selMakeupGainSwitch, fValue);
        }
        else if (slider == SliderWetMixCombined)
        {
            pProcessor->changeParameter(SqueezerPluginParameters::selWetMixSwitch, fValue);
        }
        else
        {
            DBG("[Squeezer] editor::buttonClicked --> invalid button");
        }
    }
}


void SqueezerAudioProcessorEditor::sliderValueChanged(Slider* slider)
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
    else if (slider == SliderStereoLinkCombined)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selStereoLink, fValue);
    }
    else if (slider == SliderMakeupGainCombined)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selMakeupGain, fValue);
    }
    else if (slider == SliderWetMixCombined)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selWetMix, fValue);
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
