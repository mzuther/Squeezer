/* ----------------------------------------------------------------------------

   MZ common JUCE
   ==============
   Common classes for use with the JUCE library

   Copyright (c) 2010-2016 Martin Zuther (http://www.mzuther.de/)

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

/// Create a state label.
///
/// @param componentName String containing the component's name
///
GenericStateLabel::GenericStateLabel(const String &componentName)
{
    // set component name
    setName(componentName);

    // this component blends in with the background
    setOpaque(false);

    // initialise meter state
    currentState = stateOff;

    // initialise spacing of text label
    horizontalTextSpacing = 0;
    verticalTextSpacing = 0;

    // initialise widget for displaying text
    textLabel.setName("Text label");
    textLabel.setJustificationType(Justification::topRight);
    textLabel.setBorderSize(BorderSize<int>(0, 0, 0, 0));
    addAndMakeVisible(textLabel);

    // initialise widget for displaying the background image
    addAndMakeVisible(imageComponent);

    // background images should be in the back -- I don't know why I'm
    // even writing this ... :)
    imageComponent.toBack();
}


/// Destructor.
///
GenericStateLabel::~GenericStateLabel()
{
}


/// Called when this component's size has been changed.
///
void GenericStateLabel::resized()
{
    // call base method
    Component::resized();

    // set size of text label, taking text spacing into account
    textLabel.setBounds(horizontalTextSpacing,
                        verticalTextSpacing,
                        getWidth() - 2 * horizontalTextSpacing,
                        getHeight() - 2 * verticalTextSpacing);

    // set size of background image
    imageComponent.setBounds(0, 0, getWidth(), getHeight());
}


/// Set new state.
///
/// @param newState new state (see GenericStateLabel::Parameters)
///
/// @param forceUpdate update state regardless of current state
///
void GenericStateLabel::setState(int newState, bool forceUpdate)
{
    if ((newState != currentState) || forceUpdate)
    {
        currentState = newState;
        updateState();
    }
}


/// Update background image and text colour according to current
/// state.
///
void GenericStateLabel::updateState()
{
    // "active" state
    if (currentState == stateActive)
    {
        imageComponent.setImage(imageActive);
        setLabelColour(colActive);
    }
    // "on" state
    else if (currentState == stateOn)
    {
        imageComponent.setImage(imageOn);
        setLabelColour(colOn);
    }
    // "off" state
    else
    {
        imageComponent.setImage(imageOff);
    }
}


/// Set new images and text label settings.
///
/// @param imageOffNew image displayed in the "off" state
///
/// @param imageOnNew image displayed in the "on" state
///
/// @param imageActiveNew image displayed in the "active" state
///
/// @param colourStringOn HTML text colour for the "on" state
///        ("ff0000" is red)
///
/// @param colourStringActive HTML text colour for the "active" state
///        ("ff0000" is red)
///
/// @param horizontalTextSpacingNew horizontal spacing of text label
///
/// @param verticalTextSpacingNew vertical spacing of text label
///
/// @param fontSize font size for text label
///
void GenericStateLabel::setImages(Image &imageOffNew, Image &imageOnNew, Image &imageActiveNew, String &colourStringOn, String &colourStringActive, int horizontalTextSpacingNew, int verticalTextSpacingNew, float fontSize)
{
    // update spacing of text label
    horizontalTextSpacing = horizontalTextSpacingNew;
    verticalTextSpacing = verticalTextSpacingNew;

    // update font of text label (bold weight)
    textLabel.setFont(Font(fontSize, Font::bold));

    // update colours of text label
    colOn = Colour::fromString("ff" + colourStringOn);
    colActive = Colour::fromString("ff" + colourStringActive);

    // update images (creates a copy of each image)
    imageOff = imageOffNew.createCopy();
    imageOn = imageOnNew.createCopy();
    imageActive = imageActiveNew.createCopy();

    // assert that all images have the same size
    jassert(imageOff.getBounds() == imageOn.getBounds());
    jassert(imageOff.getBounds() == imageActive.getBounds());

    // update component
    updateState();
}


/// Changes the label's text colour.  This will be overwritten on the
/// next call of updateState().
///
/// @param newColour new text colour
///
void GenericStateLabel::setLabelColour(const Colour &newColour)
{
    textLabel.setColour(Label::textColourId, newColour);
}


/// Changes the label's text.
///
/// @param newText new text string
///
void GenericStateLabel::setLabelText(const String &newText)
{
    textLabel.setText(newText, dontSendNotification);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
