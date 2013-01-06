/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose compressor with a touch of lemon

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
    SliderThresholdSwitch = new SliderStepped(strName, 50, parameters, nIndex);
    SliderThresholdSwitch->setSliderColour(Colours::purple.brighter(0.2f));

    SliderThresholdSwitch->addListener(this);
    addAndMakeVisible(SliderThresholdSwitch);


    nIndex = SqueezerPluginParameters::selRatioSwitch;
    strName = parameters->getName(nIndex);
    SliderRatioSwitch = new SliderStepped(strName, 50, parameters, nIndex);
    SliderRatioSwitch->setSliderColour(Colours::purple.brighter(0.2f));

    SliderRatioSwitch->addListener(this);
    addAndMakeVisible(SliderRatioSwitch);


    nIndex = SqueezerPluginParameters::selAttackRateSwitch;
    strName = parameters->getName(nIndex);
    SliderAttackRateSwitch = new SliderStepped(strName, 50, parameters, nIndex);
    SliderAttackRateSwitch->setSliderColour(Colours::yellow);

    SliderAttackRateSwitch->addListener(this);
    addAndMakeVisible(SliderAttackRateSwitch);


    nIndex = SqueezerPluginParameters::selReleaseRateSwitch;
    strName = parameters->getName(nIndex);
    SliderReleaseRateSwitch = new SliderStepped(strName, 50, parameters, nIndex);
    SliderReleaseRateSwitch->setSliderColour(Colours::yellow);

    SliderReleaseRateSwitch->addListener(this);
    addAndMakeVisible(SliderReleaseRateSwitch);


    nIndex = SqueezerPluginParameters::selInputGainSwitch;
    strName = parameters->getName(nIndex);
    SliderInputGainSwitch = new SliderStepped(strName, 50, parameters, nIndex);
    SliderInputGainSwitch->setSliderColour(Colours::blue.brighter(0.4f));

    SliderInputGainSwitch->addListener(this);
    addAndMakeVisible(SliderInputGainSwitch);


    nIndex = SqueezerPluginParameters::selOutputGainSwitch;
    strName = parameters->getName(nIndex);
    SliderOutputGainSwitch = new SliderStepped(strName, 50, parameters, nIndex);
    SliderOutputGainSwitch->setSliderColour(Colours::blue.brighter(0.4f));

    SliderOutputGainSwitch->addListener(this);
    addAndMakeVisible(SliderOutputGainSwitch);


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


    // This is where our plug-in editor's size is set.
    resizeEditor();

    pProcessor->addActionListenerParameters(this);

    nIndex = SqueezerPluginParameters::selDesignSwitch;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selThresholdSwitch;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selRatioSwitch;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selAttackRateSwitch;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selReleaseRateSwitch;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selInputGainSwitch;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));

    nIndex = SqueezerPluginParameters::selOutputGainSwitch;
    changeParameter(nIndex, pProcessor->getParameter(nIndex));
}


SqueezerAudioProcessorEditor::~SqueezerAudioProcessorEditor()
{
    pProcessor->removeActionListener(this);
    pProcessor->removeActionListenerParameters(this);

    deleteAllChildren();
}


void SqueezerAudioProcessorEditor::resizeEditor()
{
    nHeight = 150;
    nRightColumnStart = 450;

    setSize(nRightColumnStart + 70, nHeight);

    ButtonDesignModern->setBounds(10, 10, 60, 20);
    ButtonDesignVintage->setBounds(10, 35, 60, 20);

    SliderThresholdSwitch->setBounds(10, 70, 70, 60);
    SliderRatioSwitch->setBounds(70, 70, 70, 60);

    SliderAttackRateSwitch->setBounds(150, 70, 70, 60);
    SliderReleaseRateSwitch->setBounds(210, 70, 70, 60);

    SliderInputGainSwitch->setBounds(290, 70, 70, 60);
    SliderOutputGainSwitch->setBounds(350, 70, 70, 60);

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
        // MeterBallistics* pMeterBallistics = pProcessor->getLevels();

        // if (pMeterBallistics)
        // {
        //     squeezer->setLevels(pMeterBallistics);
        // }
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
        SliderReleaseRateSwitch->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selInputGainSwitch:
        SliderInputGainSwitch->setValue(fValue, false);
        break;
    case SqueezerPluginParameters::selOutputGainSwitch:
        SliderOutputGainSwitch->setValue(fValue, false);
        break;
    }
}


//==============================================================================
void SqueezerAudioProcessorEditor::paint(Graphics& g)
{
    g.setGradientFill(ColourGradient(Colours::darkgrey.darker(0.8f), 0, 0, Colours::darkgrey.darker(1.4f), 0, (float) getHeight(), false));
    g.fillAll();
}


void SqueezerAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == ButtonDesignModern)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDesignSwitch, SqueezerPluginParameters::selDesignModern);
    }
    else if (button == ButtonDesignVintage)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selDesignSwitch, SqueezerPluginParameters::selDesignVintage);
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
    float fValue = slider->getValue();

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
    else if (slider == SliderReleaseRateSwitch)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selReleaseRateSwitch, fValue);
    }
    else if (slider == SliderInputGainSwitch)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selInputGainSwitch, fValue);
    }
    else if (slider == SliderOutputGainSwitch)
    {
        pProcessor->changeParameter(SqueezerPluginParameters::selOutputGainSwitch, fValue);
    }
}


void SqueezerAudioProcessorEditor::resized()
{
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
