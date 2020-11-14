/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2020 Martin Zuther (http://www.mzuther.de/)

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

namespace frut
{
namespace widgets
{

/// Create content component for dialog window with validation settings.
///
/// ### Exit values
///
/// | %Value | %Result                                        |
/// | :----: | ---------------------------------------------- |
/// | 0      | window has been closed "by force" or by using the "Cancel" button |
/// | 1      | window has been closed to start validation     |
///
WindowValidationContent::WindowValidationContent()
{
}


/// Initialise dialog window components.
///
/// @param componentWidth width of content component
///
/// @param componentHeight height of content component
///
/// @param numberOfInputChannels current number of audio input
///        channels
///
/// @param sampleRate current audio sample rate
///
/// @param selectedChannel current audio channel used for validation
///        (starting at 0; -1 designates all channels)
///
/// @param validationFile current audio file used for validation
///
void WindowValidationContent::initialise(
   int componentWidth,
   int componentHeight,
   int numberOfInputChannels,
   int sampleRate,
   int selectedChannel,
   const File& validationFile )

{
   // set content component dimensions
   setSize( componentWidth, componentHeight );

   // set audio file used for validation
   validationFile_ = validationFile;

   // initialise label that displays the name of the validation file
   labelFileSelection_.setText( validationFile_.getFileName(),
                                dontSendNotification );
   addAndMakeVisible( labelFileSelection_ );

   // initialise button for validation file selection
   buttonFileSelection_.setButtonText( "..." );
   addAndMakeVisible( buttonFileSelection_ );
   buttonFileSelection_.addListener( this );

   // initialise helper label
   labelSampleRate_.setText( "Host SR: ",
                             dontSendNotification );
   addAndMakeVisible( labelSampleRate_ );

   // format sample rate
   String formattedSampleRate( sampleRate );
   formattedSampleRate = formattedSampleRate.dropLastCharacters( 3 ) + " " +
                         formattedSampleRate.getLastCharacters( 3 ) + " Hz";

   // initialise label for displaying the sample rate
   labelSampleRateValue_.setText( formattedSampleRate,
                                  dontSendNotification );
   addAndMakeVisible( labelSampleRateValue_ );

   // initialise helper label
   labelSelectedChannel.setText( "Channel: ",
                                 dontSendNotification );
   addAndMakeVisible( labelSelectedChannel );

   // initialise label for displaying the current audio channel
   sliderSelectChannel_.setNumberOfChannels( numberOfInputChannels );
   sliderSelectChannel_.setValue( selectedChannel,
                                  dontSendNotification );
   addAndMakeVisible( sliderSelectChannel_ );

   // initialise "validation" button
   buttonValidation_.setButtonText( "Validate" );
   addAndMakeVisible( buttonValidation_ );
   buttonValidation_.addListener( this );

   // initialise "cancel" button
   buttonCancel_.setButtonText( "Cancel" );
   addAndMakeVisible( buttonCancel_ );
   buttonCancel_.addListener( this );
}


void WindowValidationContent::styleButton(
   ToggleButton& button )
{
   button.setColour(
      ToggleButton::textColourId,
      Colours::white );

   button.setColour(
      ToggleButton::tickColourId,
      Colours::white );

   button.setColour(
      ToggleButton::tickDisabledColourId,
      Colours::grey );
}


/// Style and place the dialog window's components.
///
void WindowValidationContent::applySkin()
{
   // style label that displays the name of the validation file
   labelFileSelection_.setMinimumHorizontalScale( 1.0f );

   labelFileSelection_.setColour(
      Label::textColourId,
      Colours::white );

   labelFileSelection_.setColour(
      Label::backgroundColourId,
      Colours::black.brighter( 0.3f ) );

   labelFileSelection_.setColour(
      Label::outlineColourId,
      Colours::grey );


   // style helper label
   labelSampleRate_.setColour(
      Label::textColourId,
      Colours::white );


   // style label for displaying the sample rate
   labelSampleRateValue_.setMinimumHorizontalScale( 1.0f );

   labelSampleRateValue_.setColour(
      Label::textColourId,
      Colours::white );

   labelSampleRateValue_.setColour(
      Label::backgroundColourId,
      Colours::black.brighter( 0.3f ) );

   labelSampleRateValue_.setColour(
      Label::outlineColourId,
      Colours::grey );


   // style helper label
   labelSelectedChannel.setColour(
      Label::textColourId,
      Colours::white );


   // style label for displaying the current audio channel
   sliderSelectChannel_.setColour(
      Slider::textBoxTextColourId,
      Colours::white );

   sliderSelectChannel_.setColour(
      Slider::textBoxBackgroundColourId,
      Colours::black.brighter( 0.3f ) );

   sliderSelectChannel_.setColour(
      Slider::textBoxOutlineColourId,
      Colours::grey );


   // style "validation" button
   buttonValidation_.setColour(
      TextButton::buttonColourId,
      Colours::yellow );

   buttonValidation_.setColour(
      TextButton::textColourOffId,
      Colours::black );


   // place the components
   int positionX = 4;
   int positionY = 7;

   labelFileSelection_.setBounds( positionX + 4, positionY, 120, 20 );
   buttonFileSelection_.setBounds( positionX + 127, positionY, 30, 20 );

   positionY += 24;
   labelSampleRate_.setBounds( positionX, positionY, 75, 20 );
   labelSampleRateValue_.setBounds( positionX + 66, positionY, 82, 20 );

   positionY += 24;
   labelSelectedChannel.setBounds( positionX, positionY, 75, 20 );
   sliderSelectChannel_.setBounds( positionX + 66, positionY, 70, 20 );
}


/// Called when a button is clicked.  The "Cancel" and file selection
/// buttons are already handled here.  Override this method to handle
/// other buttons.
///
/// @param button clicked button
///
void WindowValidationContent::buttonClicked(
   Button* button )

{
   // user wants to close the window
   if ( button == &buttonCancel_ ) {
      // close dialog window
      closeButtonPressed();
      // user wants to select a new audio file for validation
   } else if ( button == &buttonFileSelection_ ) {
      // obtain wildcard string for supported audio files
      AudioFormatManager formatManager;
      formatManager.registerBasicFormats();
      String wildcards = formatManager.getWildcardForAllFormats();

      // create file browser limited to supported audio files
      FileChooser browser( "Open audio file for validation",
                           validationFile_,
                           wildcards,
                           false );

      // open file browser
      if ( browser.showDialog( FileBrowserComponent::openMode |
                               FileBrowserComponent::canSelectFiles,
                               nullptr ) ) {
         // get selected file
         File selectedFile( browser.getResult() );

         // update validation file
         selectValidationFile( selectedFile );
      }
   }
}


/// This method is called when the user tries to close the window.
/// This is triggered by the user clicking the close button, or using
/// some other OS-specific key shortcut or OS menu for getting rid of
/// a window.
///
void WindowValidationContent::closeButtonPressed()
{
   // get parent dialog window
   DialogWindow* dialogWindow = findParentComponentOfClass<DialogWindow>();

   if ( dialogWindow != nullptr ) {
      // close dialog window (exit value 0)
      dialogWindow->exitModalState( 0 );
   }
}

}
}
