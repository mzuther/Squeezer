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


    ButtonDesignModern = new TextButton("Modern");
    ButtonDesignModern->setRadioGroupId(1);
    ButtonDesignModern->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDesignModern->setColour(TextButton::buttonOnColourId, Colours::orange);

    ButtonDesignModern->addListener(this);
    addAndMakeVisible(ButtonDesignModern);


    ButtonDesignVintage = new TextButton("Vintage");
    ButtonDesignVintage->setRadioGroupId(1);
    ButtonDesignVintage->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonDesignVintage->setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonDesignVintage->addListener(this);
    addAndMakeVisible(ButtonDesignVintage);


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


    ButtonAttackCurveLinear = new TextButton("Linear");
    ButtonAttackCurveLinear->setRadioGroupId(2);
    ButtonAttackCurveLinear->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonAttackCurveLinear->setColour(TextButton::buttonOnColourId, Colours::orange);

    ButtonAttackCurveLinear->addListener(this);
    addAndMakeVisible(ButtonAttackCurveLinear);


    ButtonAttackCurveLogarithmic = new TextButton("Log");
    ButtonAttackCurveLogarithmic->setRadioGroupId(2);
    ButtonAttackCurveLogarithmic->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonAttackCurveLogarithmic->setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonAttackCurveLogarithmic->addListener(this);
    addAndMakeVisible(ButtonAttackCurveLogarithmic);


    ButtonReleaseCurveLinear = new TextButton("Linear");
    ButtonReleaseCurveLinear->setRadioGroupId(3);
    ButtonReleaseCurveLinear->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonReleaseCurveLinear->setColour(TextButton::buttonOnColourId, Colours::orange);

    ButtonReleaseCurveLinear->addListener(this);
    addAndMakeVisible(ButtonReleaseCurveLinear);


    ButtonReleaseCurveLogarithmic = new TextButton("Log");
    ButtonReleaseCurveLogarithmic->setRadioGroupId(3);
    ButtonReleaseCurveLogarithmic->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonReleaseCurveLogarithmic->setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonReleaseCurveLogarithmic->addListener(this);
    addAndMakeVisible(ButtonReleaseCurveLogarithmic);


    nIndex = SqueezerPluginParameters::selStereoLink;
    nIndexSwitch = SqueezerPluginParameters::selStereoLinkSwitch;
    strName = parameters->getName(nIndex);
    SliderStereoLinkCombined = new SliderCombined(strName, parameters, nIndex, nIndexSwitch);
    SliderStereoLinkCombined->setSliderColour(Colours::purple.brighter(0.2f));

    SliderStereoLinkCombined->addListener(this);
    SliderStereoLinkCombined->addButtonListener(this);
    addAndMakeVisible(SliderStereoLinkCombined);


    nIndex = SqueezerPluginParameters::selOutputGain;
    nIndexSwitch = SqueezerPluginParameters::selOutputGainSwitch;
    strName = parameters->getName(nIndex);
    SliderOutputGainCombined = new SliderCombined(strName, parameters, nIndex, nIndexSwitch);
    SliderOutputGainCombined->setSliderColour(Colours::blue.brighter(0.4f));

    SliderOutputGainCombined->addListener(this);
    SliderOutputGainCombined->addButtonListener(this);
    addAndMakeVisible(SliderOutputGainCombined);


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


    ButtonAbout = new TextButton("About");
    ButtonAbout->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonAbout->setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonAbout->addListener(this);
    addAndMakeVisible(ButtonAbout);


    pGainReductionMeters = new MeterBarGainReduction*[nChannels];
    int x = 490;
    int width = 12;

    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        pGainReductionMeters[nChannel] = new MeterBarGainReduction("Gain Reduction Meter #" + String(nChannel), x, 12, width, 13, 5);
        addAndMakeVisible(pGainReductionMeters[nChannel]);

        x += width - 1;
    }


    // This is where our plug-in editor's size is set.
    resizeEditor();

    updateParameter(SqueezerPluginParameters::selBypass);
    updateParameter(SqueezerPluginParameters::selDesign);

    updateParameter(SqueezerPluginParameters::selThresholdSwitch);
    updateParameter(SqueezerPluginParameters::selThreshold);
    updateParameter(SqueezerPluginParameters::selRatioSwitch);
    updateParameter(SqueezerPluginParameters::selRatio);

    updateParameter(SqueezerPluginParameters::selAttackRateSwitch);
    updateParameter(SqueezerPluginParameters::selAttackRate);
    updateParameter(SqueezerPluginParameters::selAttackCurve);
    updateParameter(SqueezerPluginParameters::selReleaseRateSwitch);
    updateParameter(SqueezerPluginParameters::selReleaseRate);
    updateParameter(SqueezerPluginParameters::selReleaseCurve);

    updateParameter(SqueezerPluginParameters::selStereoLinkSwitch);
    updateParameter(SqueezerPluginParameters::selStereoLink);
    updateParameter(SqueezerPluginParameters::selOutputGainSwitch);
    updateParameter(SqueezerPluginParameters::selOutputGain);
    updateParameter(SqueezerPluginParameters::selWetMixSwitch);
    updateParameter(SqueezerPluginParameters::selWetMix);
}


SqueezerAudioProcessorEditor::~SqueezerAudioProcessorEditor()
{
    pProcessor->removeActionListener(this);

    for (int nChannel = 0; nChannel < nChannels; nChannel++)
    {
        delete pGainReductionMeters[nChannel];
        pGainReductionMeters[nChannel] = NULL;
    }

    delete[] pGainReductionMeters;
    pGainReductionMeters = NULL;

    deleteAllChildren();
}


void SqueezerAudioProcessorEditor::resizeEditor()
{
    nHeight = 160;
    nRightColumnStart = 530;

    setSize(nRightColumnStart + 70, nHeight);

    ButtonBypass->setBounds(20, 90, 52, 20);
    ButtonDesignModern->setBounds(80, 115, 52, 20);
    ButtonDesignVintage->setBounds(80, 90, 52, 20);

    SliderThresholdCombined->setBounds(20, 15, 52, 60);
    SliderRatioCombined->setBounds(80, 15, 52, 60);

    SliderAttackRateCombined->setBounds(160, 15, 52, 60);
    SliderReleaseRateCombined->setBounds(220, 15, 52, 60);

    ButtonAttackCurveLinear->setBounds(160, 115, 52, 20);
    ButtonAttackCurveLogarithmic->setBounds(160, 90, 52, 20);
    ButtonReleaseCurveLinear->setBounds(220, 115, 52, 20);
    ButtonReleaseCurveLogarithmic->setBounds(220, 90, 52, 20);

    SliderStereoLinkCombined->setBounds(300, 15, 52, 60);

    SliderOutputGainCombined->setBounds(360, 15, 52, 60);
    SliderWetMixCombined->setBounds(420, 15, 52, 60);

    ButtonAbout->setBounds(nRightColumnStart, nHeight - 31, 60, 20);

    if (LabelDebug)
    {
        LabelDebug->setBounds(nRightColumnStart, nHeight - 82, 60, 16);
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
            float fGainReduction = pProcessor->getGainReduction(nChannel);
            pGainReductionMeters[nChannel]->setGainReduction(fGainReduction);
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
    case SqueezerPluginParameters::selDesign:

        if (fValue == SqueezerPluginParameters::selDesignModern)
        {
            ButtonDesignModern->setToggleState(true, false);
        }
        else
        {
            ButtonDesignVintage->setToggleState(true, false);
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
    case SqueezerPluginParameters::selAttackCurve:

        if (fValue == SqueezerPluginParameters::selAttackCurveLinear)
        {
            ButtonAttackCurveLinear->setToggleState(true, false);
        }
        else
        {
            ButtonAttackCurveLogarithmic->setToggleState(true, false);
        }

        break;
    case SqueezerPluginParameters::selReleaseCurve:

        if (fValue == SqueezerPluginParameters::selReleaseCurveLinear)
        {
            ButtonReleaseCurveLinear->setToggleState(true, false);
        }
        else
        {
            ButtonReleaseCurveLogarithmic->setToggleState(true, false);
        }

        break;
    case SqueezerPluginParameters::selStereoLinkSwitch:
        SliderStereoLinkCombined->updateMode();
        break;
    case SqueezerPluginParameters::selStereoLink:
        SliderStereoLinkCombined->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selOutputGainSwitch:
        SliderOutputGainCombined->updateMode();
        break;
    case SqueezerPluginParameters::selOutputGain:
        SliderOutputGainCombined->setValue(fValue, false);
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
    else if (button == ButtonDesignModern)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDesign, SqueezerPluginParameters::selDesignModern);
    }
    else if (button == ButtonDesignVintage)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDesign, SqueezerPluginParameters::selDesignVintage);
    }
    else if (button == ButtonAttackCurveLinear)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selAttackCurve, SqueezerPluginParameters::selAttackCurveLinear);
    }
    else if (button == ButtonAttackCurveLogarithmic)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selAttackCurve, SqueezerPluginParameters::selAttackCurveLogarithmic);
    }
    else if (button == ButtonReleaseCurveLinear)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selReleaseCurve, SqueezerPluginParameters::selReleaseCurveLinear);
    }
    else if (button == ButtonReleaseCurveLogarithmic)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selReleaseCurve, SqueezerPluginParameters::selReleaseCurveLogarithmic);
    }
    else if (button == ButtonAbout)
    {
        WindowAbout* windowAbout = new WindowAbout(getWidth(), getHeight());
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
        else if (slider == SliderOutputGainCombined)
        {
            pProcessor->changeParameter(SqueezerPluginParameters::selOutputGainSwitch, fValue);
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
    else if (slider == SliderOutputGainCombined)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selOutputGain, fValue);
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
