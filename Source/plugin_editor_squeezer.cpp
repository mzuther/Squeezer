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
    String strName = parameters->getName(nIndex);
    SliderThresholdSwitch = new SliderSwitch(strName, parameters, nIndex);
    SliderThresholdSwitch->setSliderColour(Colours::purple.brighter(0.2f));

    SliderThresholdSwitch->addListener(this);
    addAndMakeVisible(SliderThresholdSwitch);


    nIndex = SqueezerPluginParameters::selRatio;
    strName = parameters->getName(nIndex);
    SliderRatioSwitch = new SliderSwitch(strName, parameters, nIndex);
    SliderRatioSwitch->setSliderColour(Colours::purple.brighter(0.2f));

    SliderRatioSwitch->addListener(this);
    addAndMakeVisible(SliderRatioSwitch);


    nIndex = SqueezerPluginParameters::selAttackRate;
    strName = parameters->getName(nIndex);
    SliderAttackRateCombined = new SliderCombined(strName, parameters, nIndex);
    SliderAttackRateCombined->setSliderColour(Colours::yellow);

    SliderAttackRateCombined->addListener(this);
    addAndMakeVisible(SliderAttackRateCombined);


    nIndex = SqueezerPluginParameters::selReleaseRate;
    strName = parameters->getName(nIndex);
    SliderReleaseRateCombined = new SliderCombined(strName, parameters, nIndex);
    SliderReleaseRateCombined->setSliderColour(Colours::yellow);

    SliderReleaseRateCombined->addListener(this);
    addAndMakeVisible(SliderReleaseRateCombined);


    ButtonAttackModeLinear = new TextButton("Linear");
    ButtonAttackModeLinear->setRadioGroupId(2);
    ButtonAttackModeLinear->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonAttackModeLinear->setColour(TextButton::buttonOnColourId, Colours::orange);

    ButtonAttackModeLinear->addListener(this);
    addAndMakeVisible(ButtonAttackModeLinear);


    ButtonAttackModeLogarithmic = new TextButton("Log");
    ButtonAttackModeLogarithmic->setRadioGroupId(2);
    ButtonAttackModeLogarithmic->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonAttackModeLogarithmic->setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonAttackModeLogarithmic->addListener(this);
    addAndMakeVisible(ButtonAttackModeLogarithmic);


    ButtonReleaseModeLinear = new TextButton("Linear");
    ButtonReleaseModeLinear->setRadioGroupId(3);
    ButtonReleaseModeLinear->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonReleaseModeLinear->setColour(TextButton::buttonOnColourId, Colours::orange);

    ButtonReleaseModeLinear->addListener(this);
    addAndMakeVisible(ButtonReleaseModeLinear);


    ButtonReleaseModeLogarithmic = new TextButton("Log");
    ButtonReleaseModeLogarithmic->setRadioGroupId(3);
    ButtonReleaseModeLogarithmic->setColour(TextButton::buttonColourId, Colours::grey);
    ButtonReleaseModeLogarithmic->setColour(TextButton::buttonOnColourId, Colours::yellow);

    ButtonReleaseModeLogarithmic->addListener(this);
    addAndMakeVisible(ButtonReleaseModeLogarithmic);


    nIndex = SqueezerPluginParameters::selStereoLink;
    strName = parameters->getName(nIndex);
    SliderStereoLinkSwitch = new SliderSwitch(strName, parameters, nIndex);
    SliderStereoLinkSwitch->setSliderColour(Colours::purple.brighter(0.2f));

    SliderStereoLinkSwitch->addListener(this);
    addAndMakeVisible(SliderStereoLinkSwitch);


    nIndex = SqueezerPluginParameters::selInputGain;
    strName = parameters->getName(nIndex);
    SliderInputGainSwitch = new SliderSwitch(strName, parameters, nIndex);
    SliderInputGainSwitch->setSliderColour(Colours::blue.brighter(0.4f));

    SliderInputGainSwitch->addListener(this);
    addAndMakeVisible(SliderInputGainSwitch);


    nIndex = SqueezerPluginParameters::selOutputGain;
    strName = parameters->getName(nIndex);
    SliderOutputGainSwitch = new SliderSwitch(strName, parameters, nIndex);
    SliderOutputGainSwitch->setSliderColour(Colours::blue.brighter(0.4f));

    SliderOutputGainSwitch->addListener(this);
    addAndMakeVisible(SliderOutputGainSwitch);


    nIndex = SqueezerPluginParameters::selWetMix;
    strName = parameters->getName(nIndex);
    SliderWetMixSwitch = new SliderSwitch(strName, parameters, nIndex);
    SliderWetMixSwitch->setSliderColour(Colours::blue.brighter(0.4f));

    SliderWetMixSwitch->addListener(this);
    addAndMakeVisible(SliderWetMixSwitch);


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

    pProcessor->addActionListenerParameters(this);

    nIndex = SqueezerPluginParameters::selBypass;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selDesign;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selThreshold;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selRatio;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selAttackMode;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selReleaseMode;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selAttackRate;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selReleaseRate;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selStereoLink;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selInputGain;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selOutputGain;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selWetMix;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));
}


SqueezerAudioProcessorEditor::~SqueezerAudioProcessorEditor()
{
    pProcessor->removeActionListener(this);
    pProcessor->removeActionListenerParameters(this);

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

    ButtonBypass->setBounds(360, 90, 52, 20);

    ButtonDesignModern->setBounds(20, 90, 52, 20);
    ButtonDesignVintage->setBounds(20, 115, 52, 20);

    SliderThresholdSwitch->setBounds(20, 15, 52, 60);
    SliderRatioSwitch->setBounds(80, 15, 52, 60);

    SliderAttackRateCombined->setBounds(160, 15, 52, 60);
    SliderReleaseRateCombined->setBounds(220, 15, 52, 60);

    ButtonAttackModeLinear->setBounds(160, 90, 52, 20);
    ButtonAttackModeLogarithmic->setBounds(160, 115, 52, 20);
    ButtonReleaseModeLinear->setBounds(220, 90, 52, 20);
    ButtonReleaseModeLogarithmic->setBounds(220, 115, 52, 20);

    SliderStereoLinkSwitch->setBounds(300, 15, 52, 60);

    SliderInputGainSwitch->setBounds(360, 15, 52, 60);
    SliderOutputGainSwitch->setBounds(420, 15, 52, 60);
    SliderWetMixSwitch->setBounds(420, 85, 52, 60);

    ButtonAbout->setBounds(nRightColumnStart, nHeight - 31, 60, 20);

    if (LabelDebug)
    {
        LabelDebug->setBounds(nRightColumnStart, nHeight - 82, 60, 16);
    }
}


void SqueezerAudioProcessorEditor::actionListenerCallback(const String& message)
{
    // "PC" --> parameter changed, followed by a hash and the
    // parameter's ID
    if (message.startsWith("PC#"))
    {
        String strIndex = message.substring(3);
        int nIndex = strIndex.getIntValue();
        jassert(nIndex >= 0);
        jassert(nIndex < pProcessor->getNumParameters());

        changeParameter(nIndex);
    }
    // "UM" --> update meters
    else if (!message.compare("UM"))
    {
        for (int nChannel = 0; nChannel < nChannels; nChannel++)
        {
            float fGainReduction = pProcessor->getGainReduction(nChannel);
            pGainReductionMeters[nChannel]->setGainReduction(fGainReduction);
        }
    }
    else
    {
        DBG("[Squeezer] Received unknown action message \"" + message + "\".");
    }
}


void SqueezerAudioProcessorEditor::changeParameter(int nIndex)
{
    if (pProcessor->hasChanged(nIndex))
    {
        float fValue = pProcessor->getParameter(nIndex);

        changeParameter(nIndex, fValue);
        pProcessor->clearChangeFlag(nIndex);
    }
}


void SqueezerAudioProcessorEditor::changeParameter(int nIndex, float fValue)
{
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
    case SqueezerPluginParameters::selThreshold:
        SliderThresholdSwitch->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selRatio:
        SliderRatioSwitch->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selAttackRate:
        SliderAttackRateCombined->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selReleaseRate:
        SliderReleaseRateCombined->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selAttackMode:

        if (fValue == SqueezerPluginParameters::selAttackModeLinear)
        {
            ButtonAttackModeLinear->setToggleState(true, false);
        }
        else
        {
            ButtonAttackModeLogarithmic->setToggleState(true, false);
        }

        break;
    case SqueezerPluginParameters::selReleaseMode:

        if (fValue == SqueezerPluginParameters::selReleaseModeLinear)
        {
            ButtonReleaseModeLinear->setToggleState(true, false);
        }
        else
        {
            ButtonReleaseModeLogarithmic->setToggleState(true, false);
        }

        break;
    case SqueezerPluginParameters::selStereoLink:
        SliderStereoLinkSwitch->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selInputGain:
        SliderInputGainSwitch->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selOutputGain:
        SliderOutputGainSwitch->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selWetMix:
        SliderWetMixSwitch->setValue(fValue, false);
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
    else if (button == ButtonAttackModeLinear)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selAttackMode, SqueezerPluginParameters::selAttackModeLinear);
    }
    else if (button == ButtonAttackModeLogarithmic)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selAttackMode, SqueezerPluginParameters::selAttackModeLogarithmic);
    }
    else if (button == ButtonReleaseModeLinear)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selReleaseMode, SqueezerPluginParameters::selReleaseModeLinear);
    }
    else if (button == ButtonReleaseModeLogarithmic)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selReleaseMode, SqueezerPluginParameters::selReleaseModeLogarithmic);
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
}


void SqueezerAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    float fValue = (float) slider->getValue();

    if (slider == SliderThresholdSwitch)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selThreshold, fValue);
    }
    else if (slider == SliderRatioSwitch)
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
    else if (slider == SliderStereoLinkSwitch)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selStereoLink, fValue);
    }
    else if (slider == SliderInputGainSwitch)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selInputGain, fValue);
    }
    else if (slider == SliderOutputGainSwitch)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selOutputGain, fValue);
    }
    else if (slider == SliderWetMixSwitch)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selWetMix, fValue);
    }
}


void SqueezerAudioProcessorEditor::resized()
{
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
