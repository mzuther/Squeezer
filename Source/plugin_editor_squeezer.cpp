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


    int nIndex = SqueezerPluginParameters::selThresholdSwitch;
    String strName = parameters->getName(nIndex);
    int nLabelWidth = 50;
    SliderThresholdSwitch = new SliderSwitch(strName, nLabelWidth, parameters, nIndex);
    SliderThresholdSwitch->setSliderColour(Colours::purple.brighter(0.2f));

    SliderThresholdSwitch->addListener(this);
    addAndMakeVisible(SliderThresholdSwitch);


    nIndex = SqueezerPluginParameters::selRatioSwitch;
    strName = parameters->getName(nIndex);
    nLabelWidth = 50;
    SliderRatioSwitch = new SliderSwitch(strName, nLabelWidth, parameters, nIndex);
    SliderRatioSwitch->setSliderColour(Colours::purple.brighter(0.2f));

    SliderRatioSwitch->addListener(this);
    addAndMakeVisible(SliderRatioSwitch);


    nIndex = SqueezerPluginParameters::selAttackRateSwitch;
    strName = parameters->getName(nIndex);
    nLabelWidth = 50;
    SliderAttackRateSwitch = new SliderSwitch(strName, nLabelWidth, parameters, nIndex);
    SliderAttackRateSwitch->setSliderColour(Colours::yellow);

    SliderAttackRateSwitch->addListener(this);
    addAndMakeVisible(SliderAttackRateSwitch);


    nIndex = SqueezerPluginParameters::selReleaseRateSwitch;
    strName = parameters->getName(nIndex);
    nLabelWidth = 50;
    SliderReleaseRateCombined = new SliderCombined(strName, nLabelWidth, parameters, nIndex);
    SliderReleaseRateCombined->setSliderColour(Colours::yellow);

    SliderReleaseRateCombined->addListener(this);
    addAndMakeVisible(SliderReleaseRateCombined);


    nIndex = SqueezerPluginParameters::selReleaseRateContinuous;
    strName = parameters->getName(nIndex);
    nLabelWidth = 50;
    SliderReleaseRateContinuous = new SliderContinuous(strName, nLabelWidth, parameters, nIndex);
    SliderReleaseRateContinuous->setSliderColour(Colours::yellow);

    SliderReleaseRateContinuous->addListener(this);
    addAndMakeVisible(SliderReleaseRateContinuous);


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


    nIndex = SqueezerPluginParameters::selStereoLinkSwitch;
    strName = parameters->getName(nIndex);
    nLabelWidth = 50;
    SliderStereoLinkSwitch = new SliderSwitch(strName, nLabelWidth, parameters, nIndex);
    SliderStereoLinkSwitch->setSliderColour(Colours::purple.brighter(0.2f));

    SliderStereoLinkSwitch->addListener(this);
    addAndMakeVisible(SliderStereoLinkSwitch);


    nIndex = SqueezerPluginParameters::selInputGainSwitch;
    strName = parameters->getName(nIndex);
    nLabelWidth = 50;
    SliderInputGainSwitch = new SliderSwitch(strName, nLabelWidth, parameters, nIndex);
    SliderInputGainSwitch->setSliderColour(Colours::blue.brighter(0.4f));

    SliderInputGainSwitch->addListener(this);
    addAndMakeVisible(SliderInputGainSwitch);


    nIndex = SqueezerPluginParameters::selOutputGainSwitch;
    strName = parameters->getName(nIndex);
    nLabelWidth = 50;
    SliderOutputGainSwitch = new SliderSwitch(strName, nLabelWidth, parameters, nIndex);
    SliderOutputGainSwitch->setSliderColour(Colours::blue.brighter(0.4f));

    SliderOutputGainSwitch->addListener(this);
    addAndMakeVisible(SliderOutputGainSwitch);


    nIndex = SqueezerPluginParameters::selWetMixSwitch;
    strName = parameters->getName(nIndex);
    nLabelWidth = 50;
    SliderWetMixSwitch = new SliderSwitch(strName, nLabelWidth, parameters, nIndex);
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

    nIndex = SqueezerPluginParameters::selBypassSwitch;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selDesignSwitch;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selThresholdSwitch;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selRatioSwitch;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selAttackModeSwitch;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selReleaseModeSwitch;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selAttackRateSwitch;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selReleaseRateSwitch;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selReleaseRateContinuous;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selStereoLinkSwitch;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selInputGainSwitch;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selOutputGainSwitch;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selWetMixSwitch;
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

    ButtonBypass->setBounds(360, 90, 50, 20);

    ButtonDesignModern->setBounds(20, 90, 50, 20);
    ButtonDesignVintage->setBounds(20, 115, 50, 20);

    SliderThresholdSwitch->setBounds(10, 15, 70, 60);
    SliderRatioSwitch->setBounds(70, 15, 70, 60);

    SliderAttackRateSwitch->setBounds(150, 15, 70, 60);
    SliderReleaseRateCombined->setBounds(210, 15, 70, 60);
    SliderReleaseRateContinuous->setBounds(280, 85, 70, 60);

    ButtonAttackModeLinear->setBounds(160, 90, 50, 20);
    ButtonAttackModeLogarithmic->setBounds(160, 115, 50, 20);
    ButtonReleaseModeLinear->setBounds(220, 90, 50, 20);
    ButtonReleaseModeLogarithmic->setBounds(220, 115, 50, 20);

    SliderStereoLinkSwitch->setBounds(290, 15, 70, 60);

    SliderInputGainSwitch->setBounds(350, 15, 70, 60);
    SliderOutputGainSwitch->setBounds(410, 15, 70, 60);
    SliderWetMixSwitch->setBounds(410, 85, 70, 60);

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
    case SqueezerPluginParameters::selBypassSwitch:
        ButtonBypass->setToggleState(fValue != 0.0f, false);
        break;
    case SqueezerPluginParameters::selDesignSwitch:

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
        SliderThresholdSwitch->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selRatioSwitch:
        SliderRatioSwitch->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selAttackRateSwitch:
        SliderAttackRateSwitch->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selReleaseRateSwitch:
        SliderReleaseRateCombined->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selReleaseRateContinuous:
        SliderReleaseRateContinuous->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selAttackModeSwitch:

        if (fValue == SqueezerPluginParameters::selAttackModeLinear)
        {
            ButtonAttackModeLinear->setToggleState(true, false);
        }
        else
        {
            ButtonAttackModeLogarithmic->setToggleState(true, false);
        }

        break;
    case SqueezerPluginParameters::selReleaseModeSwitch:

        if (fValue == SqueezerPluginParameters::selReleaseModeLinear)
        {
            ButtonReleaseModeLinear->setToggleState(true, false);
        }
        else
        {
            ButtonReleaseModeLogarithmic->setToggleState(true, false);
        }

        break;
    case SqueezerPluginParameters::selStereoLinkSwitch:
        SliderStereoLinkSwitch->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selInputGainSwitch:
        SliderInputGainSwitch->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selOutputGainSwitch:
        SliderOutputGainSwitch->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selWetMixSwitch:
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
        pProcessor->changeParameter(SqueezerPluginParameters::selBypassSwitch, !button->getToggleState());
    }
    else if (button == ButtonDesignModern)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDesignSwitch, SqueezerPluginParameters::selDesignModern);
    }
    else if (button == ButtonDesignVintage)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDesignSwitch, SqueezerPluginParameters::selDesignVintage);
    }
    else if (button == ButtonAttackModeLinear)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selAttackModeSwitch, SqueezerPluginParameters::selAttackModeLinear);
    }
    else if (button == ButtonAttackModeLogarithmic)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selAttackModeSwitch, SqueezerPluginParameters::selAttackModeLogarithmic);
    }
    else if (button == ButtonReleaseModeLinear)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selReleaseModeSwitch, SqueezerPluginParameters::selReleaseModeLinear);
    }
    else if (button == ButtonReleaseModeLogarithmic)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selReleaseModeSwitch, SqueezerPluginParameters::selReleaseModeLogarithmic);
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
        pProcessor->changeParameter(SqueezerPluginParameters::selThresholdSwitch, fValue);
    }
    else if (slider == SliderRatioSwitch)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selRatioSwitch, fValue);
    }
    else if (slider == SliderAttackRateSwitch)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selAttackRateSwitch, fValue);
    }
    else if (slider == SliderReleaseRateCombined)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selReleaseRateSwitch, fValue);
    }
    else if (slider == SliderReleaseRateContinuous)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selReleaseRateContinuous, fValue);
    }
    else if (slider == SliderStereoLinkSwitch)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selStereoLinkSwitch, fValue);
    }
    else if (slider == SliderInputGainSwitch)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selInputGainSwitch, fValue);
    }
    else if (slider == SliderOutputGainSwitch)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selOutputGainSwitch, fValue);
    }
    else if (slider == SliderWetMixSwitch)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selWetMixSwitch, fValue);
    }
}


void SqueezerAudioProcessorEditor::resized()
{
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
