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

#include "generic_window_validation_content.h"


/// Create content component for dialog window with validation settings.
///
/// ### Exit values
///
/// | %Value | %Result                                        |
/// | :----: | ---------------------------------------------- |
/// | 0      | window has been closed "by force" or by using the "Cancel" button |
/// | 1      | window has been closed to start validation     |
///
GenericWindowValidationContent::GenericWindowValidationContent()
{
}


/// Initialise dialog window components.
///
/// @param width width of content component
///
/// @param height height of content component
///
/// @param numberOfInputChannels current number of audio input
///        channels
///
/// @param sampleRate current audio sample rate
///
/// @param selectedChannel current audio channel used for validation
///        (starting at 0; -1 designates all channels)
///
/// @param validationFileNew current audio file used for validation
///
void GenericWindowValidationContent::initialise(int width, int height,
        int numberOfInputChannels, int sampleRate, int selectedChannel,
        const File &validationFileNew)
{
    // set content component dimensions
    setSize(width, height);

    // set audio file used for validation
    validationFile = validationFileNew;

    // initialise label that displays the name of the validation file
    LabelFileSelection.setText(validationFile.getFileName(),
                               dontSendNotification);
    addAndMakeVisible(LabelFileSelection);

    // initialise button for validation file selection
    ButtonFileSelection.setButtonText("...");
    addAndMakeVisible(ButtonFileSelection);
    ButtonFileSelection.addListener(this);

    // initialise helper label
    LabelSampleRate.setText("Host SR: ",
                            dontSendNotification);
    addAndMakeVisible(LabelSampleRate);

    // format sample rate
    String formattedSampleRate(sampleRate);
    formattedSampleRate = formattedSampleRate.dropLastCharacters(3) + " " +
                          formattedSampleRate.getLastCharacters(3) + " Hz";

    // initialise label for displaying the sample rate
    LabelSampleRateValue.setText(formattedSampleRate,
                                 dontSendNotification);
    addAndMakeVisible(LabelSampleRateValue);

    // initialise helper label
    LabelDumpSelectedChannel.setText("Channel: ",
                                     dontSendNotification);
    addAndMakeVisible(LabelDumpSelectedChannel);

    // initialise label for displaying the current audio channel
    SliderDumpSelectedChannel.setNumberOfChannels(numberOfInputChannels);
    SliderDumpSelectedChannel.setValue(selectedChannel,
                                       dontSendNotification);
    addAndMakeVisible(SliderDumpSelectedChannel);

    // initialise "validation" button
    ButtonValidation.setButtonText("Validate");
    addAndMakeVisible(ButtonValidation);
    ButtonValidation.addListener(this);

    // initialise "cancel" button
    ButtonCancel.setButtonText("Cancel");
    addAndMakeVisible(ButtonCancel);
    ButtonCancel.addListener(this);
}


/// Style and place the dialog window's components.
///
void GenericWindowValidationContent::applySkin()
{
    // style label that displays the name of the validation file
    LabelFileSelection.setMinimumHorizontalScale(1.0f);
    LabelFileSelection.setColour(
        Label::textColourId,
        Colours::black);
    LabelFileSelection.setColour(
        Label::backgroundColourId,
        Colours::white.darker(0.15f));
    LabelFileSelection.setColour(
        Label::outlineColourId,
        Colours::grey);

    // style helper label
    LabelSampleRate.setColour(
        Label::textColourId,
        Colours::black);

    // style label for displaying the sample rate
    LabelSampleRateValue.setMinimumHorizontalScale(1.0f);
    LabelSampleRateValue.setColour(
        Label::textColourId,
        Colours::black);
    LabelSampleRateValue.setColour(
        Label::backgroundColourId,
        Colours::white.darker(0.15f));
    LabelSampleRateValue.setColour(
        Label::outlineColourId,
        Colours::grey);

    // style helper label
    LabelDumpSelectedChannel.setColour(
        Label::textColourId,
        Colours::black);

    // style label for displaying the current audio channel
    SliderDumpSelectedChannel.setColour(
        GenericChannelSlider::textBoxTextColourId,
        Colours::black);
    SliderDumpSelectedChannel.setColour(
        GenericChannelSlider::textBoxBackgroundColourId,
        Colours::white.darker(0.15f));
    SliderDumpSelectedChannel.setColour(
        GenericChannelSlider::textBoxOutlineColourId,
        Colours::grey);

    // style "validation" button
    ButtonValidation.setColour(
        TextButton::textColourOffId,
        Colours::black);
    ButtonValidation.setColour(
        TextButton::buttonColourId,
        Colours::red);
    ButtonValidation.setColour(
        TextButton::buttonOnColourId,
        Colours::grey);

    // style "cancel" button
    ButtonCancel.setColour(
        TextButton::textColourOffId,
        Colours::black);
    ButtonCancel.setColour(
        TextButton::buttonColourId,
        Colours::yellow);
    ButtonCancel.setColour(
        TextButton::buttonOnColourId,
        Colours::grey);

    // place the components
    int positionX = 4;
    int positionY = 7;

    LabelFileSelection.setBounds(positionX + 4, positionY, 120, 20);
    ButtonFileSelection.setBounds(positionX + 127, positionY, 30, 20);

    positionY += 24;
    LabelSampleRate.setBounds(positionX, positionY, 75, 20);
    LabelSampleRateValue.setBounds(positionX + 66, positionY, 82, 20);

    positionY += 24;
    LabelDumpSelectedChannel.setBounds(positionX, positionY, 75, 20);
    SliderDumpSelectedChannel.setBounds(positionX + 66, positionY, 70, 20);
}


/// Called when a button is clicked.  The "Cancel" and file selection
/// buttons are already handled here.  Override this method to handle
/// other buttons.
///
/// @param button clicked button
///
void GenericWindowValidationContent::buttonClicked(Button *button)
{
    // user wants to close the window
    if (button == &ButtonCancel)
    {
        // close dialog window
        closeButtonPressed();
    }
    // user wants to select a new audio file for validation
    else if (button == &ButtonFileSelection)
    {
        // obtain wildcard string for supported audio files
        AudioFormatManager formatManager;
        formatManager.registerBasicFormats();
        String wildcards = formatManager.getWildcardForAllFormats();

        // create file browser limited to supported audio files
        FileChooser browser("Open audio file for validation",
                            validationFile, wildcards, false);

        // open file browser
        if (browser.showDialog(FileBrowserComponent::openMode |
                               FileBrowserComponent::canSelectFiles,
                               nullptr))
        {
            // get selected file
            File selectedFile(browser.getResult());

            // update validation file
            selectValidationFile(selectedFile);
        }
    }
}


/// This method is called when the user tries to close the window.
/// This is triggered by the user clicking the close button, or using
/// some other OS-specific key shortcut or OS menu for getting rid of
/// a window.
///
void GenericWindowValidationContent::closeButtonPressed()
{
    // get parent dialog window
    DialogWindow *dialogWindow = findParentComponentOfClass<DialogWindow>();

    if (dialogWindow != nullptr)
    {
        // close dialog window (exit value 0)
        dialogWindow->exitModalState(0);
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
