/* ----------------------------------------------------------------------------

   MZ common JUCE
   ==============
   Common classes for use with the JUCE library

   Copyright (c) 2010-2015 Martin Zuther (http://www.mzuther.de/)

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

#include "generic_state_label.h"

GenericStateLabel::GenericStateLabel(const String &componentName)
{
    setName(componentName);

    // this component blends in with the background
    setOpaque(false);

    nSpacingLeft = 0;
    nSpacingTop = 0;
    nState = stateOff;

    imageOff = Image();
    imageOn = Image();

    pLabel = new Label("Label", "");
    pLabel->setBorderSize(BorderSize<int>(0, 0, 0, 0));
    pLabel->setJustificationType(Justification::topRight);
    addAndMakeVisible(pLabel);

    pBackgroundImage = new ImageComponent("Background Image");
    addAndMakeVisible(pBackgroundImage);

    pBackgroundImage->toBack();
}


GenericStateLabel::~GenericStateLabel()
{
    delete pLabel;
    pLabel = nullptr;

    delete pBackgroundImage;
    pBackgroundImage = nullptr;
}


void GenericStateLabel::resized()
{
    pLabel->setBounds(nSpacingLeft, nSpacingTop, getWidth() - 2 * nSpacingLeft, getHeight() - 2 * nSpacingTop);
    pBackgroundImage->setBounds(0, 0, getWidth(), getHeight());
}


void GenericStateLabel::setState(int nStateNew, bool bForceUpdate)
{
    if ((nState != nStateNew) || bForceUpdate)
    {
        nState = nStateNew;
        updateState();
    }
}


void GenericStateLabel::updateState()
{
    if (nState == stateActive)
    {
        pBackgroundImage->setImage(imageActive);
        pLabel->setColour(Label::textColourId, colActive);
    }
    else if (nState == stateOn)
    {
        pBackgroundImage->setImage(imageOn);
        pLabel->setColour(Label::textColourId, colOn);
    }
    else
    {
        pBackgroundImage->setImage(imageOff);
    }
}


void GenericStateLabel::setImages(Image &imageOffNew, Image &imageOnNew, Image &imageActiveNew, String &strColourOn, String &strColourActive, int nSpacingLeftNew, int nSpacingTopNew, int nFontSize)
{
    nSpacingLeft = nSpacingLeftNew;
    nSpacingTop = nSpacingTopNew;
    pLabel->setFont(Font((float) nFontSize, Font::bold));

    colOn = Colour::fromString("ff" + strColourOn);
    colActive = Colour::fromString("ff" + strColourActive);

    pLabel->setColour(Label::textColourId, colOn);

    imageOff = Image(imageOffNew);
    imageOn = Image(imageOnNew);
    imageActive = Image(imageActiveNew);

    jassert(imageOff.getBounds() == imageOn.getBounds());
    jassert(imageOff.getBounds() == imageActive.getBounds());

    updateState();
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
