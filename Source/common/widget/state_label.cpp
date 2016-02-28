/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
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


/// Create a state label.
///
StateLabel::StateLabel()
{
    // this component blends in with the background
    setOpaque(false);

    // initialise meter state
    state_ = State::off;

    // initialise spacing of text within text label
    horizontalTextSpacing_ = 0;
    verticalTextSpacing_ = 0;

    // initialise widget for displaying text
    textLabel_.setName("Text label");
    textLabel_.setJustificationType(Justification::topRight);
    textLabel_.setBorderSize(BorderSize<int>(0, 0, 0, 0));
    addAndMakeVisible(textLabel_);

    // initialise widget for displaying the background image
    addAndMakeVisible(imageComponent_);

    // background images should be in the back -- I don't know why I'm
    // even writing this ... :)
    imageComponent_.toBack();
}


/// Called when this component's size has been changed.
///
void StateLabel::resized()
{
    // call base method
    Component::resized();

    // resize text label, taking text spacing into account
    textLabel_.setBounds(horizontalTextSpacing_,
                         verticalTextSpacing_,
                         getWidth() - 2 * horizontalTextSpacing_,
                         getHeight() - 2 * verticalTextSpacing_);

    // resize of background image
    imageComponent_.setBounds(0, 0, getWidth(), getHeight());
}


/// Set new state.
///
/// @param state new state (see StateLabel::Parameters)
///
/// @param forceUpdate update state regardless of current state
///
void StateLabel::setState(
    int state,
    bool forceUpdate)

{
    if ((state != state_) || forceUpdate)
    {
        state_ = state;
        updateState();
    }
}


/// Update background image and text colour according to current
/// state.
///
void StateLabel::updateState()
{
    // "active" state
    if (state_ == State::active)
    {
        imageComponent_.setImage(imageActive_);
        setLabelColour(textColourActive_);
    }
    // "on" state
    else if (state_ == State::on)
    {
        imageComponent_.setImage(imageOn_);
        setLabelColour(textColourOn_);
    }
    // "off" state
    else
    {
        imageComponent_.setImage(imageOff_);
    }
}


/// Set new images and text label settings.
///
/// @param imageOff image displayed in the "off" state
///
/// @param imageOn image displayed in the "on" state
///
/// @param imageActive image displayed in the "active" state
///
/// @param textColourOn HTML text colour for the "on" state
///        ("ff0000" is red)
///
/// @param textColourActive HTML text colour for the "active" state
///        ("ff0000" is red)
///
/// @param horizontalTextSpacing horizontal spacing of text in the
///        text label
///
/// @param verticalTextSpacing vertical spacing of text in the text
///        label
///
/// @param fontSize font size for text label
///
void StateLabel::setImages(
    const Image &imageOff,
    const Image &imageOn,
    const Image &imageActive,
    const String &textColourOn,
    const String &textColourActive,
    int horizontalTextSpacing,
    int verticalTextSpacing,
    float fontSize)

{
    // update text spacing in label
    horizontalTextSpacing_ = horizontalTextSpacing;
    verticalTextSpacing_ = verticalTextSpacing;

    // update font of text label (bold weight)
    textLabel_.setFont(Font(fontSize, Font::bold));

    // update colours of text label
    textColourOn_ = Colour::fromString("ff" + textColourOn);
    textColourActive_ = Colour::fromString("ff" + textColourActive);

    // update images (creates a copy of each image)
    imageOff_ = imageOff.createCopy();
    imageOn_ = imageOn.createCopy();
    imageActive_ = imageActive.createCopy();

    // assert that all images have the same size
    jassert(imageOff_.getBounds() == imageOn_.getBounds());
    jassert(imageOff_.getBounds() == imageActive_.getBounds());

    // update component
    updateState();
}


/// Changes the label's text colour.  This will be overwritten on the
/// next call of updateState().
///
/// @param textColour new text colour
///
void StateLabel::setLabelColour(
    const Colour &textColour)

{
    textLabel_.setColour(Label::textColourId, textColour);
}


/// Changes the label's text.
///
/// @param text new text string
///
void StateLabel::setLabelText(
    const String &text)

{
    textLabel_.setText(text, dontSendNotification);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
