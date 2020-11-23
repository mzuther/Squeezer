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

namespace frut::widgets
{

/// Create content component for dialog window with validation settings.
///
/// ### Exit values
///
/// | %Value | %Result                |
/// | :----: | ---------------------- |
/// | 0      | window has been closed |
///
WindowSettingsContent::WindowSettingsContent()
{
}


/// Static helper function to create a dialog window for plug-in
/// settings.
///
/// @param pluginEditor audio plug-in editor
///
/// @param componentWidth width of content component
///
/// @param componentHeight height of content component
///
/// @param pluginSettings string containing the formatted plug-in
///        settings
///
/// @return created dialog window
///
DialogWindow* WindowSettingsContent::createDialogWindow(
   AudioProcessorEditor* pluginEditor,
   int componentWidth,
   int componentHeight,
   const String& pluginSettings )

{
   // prepare dialog window
   DialogWindow::LaunchOptions windowSettingsLauncher;

   // create content component
   WindowSettingsContent* contentComponent =
      new WindowSettingsContent();

   contentComponent->initialise( componentWidth,
                                 componentHeight,
                                 pluginSettings );

   // initialise dialog window settings
   windowSettingsLauncher.dialogTitle = String( "Plug-in Settings" );
   windowSettingsLauncher.dialogBackgroundColour = Colours::black.brighter( 0.2f );
   windowSettingsLauncher.content.setOwned( contentComponent );
   windowSettingsLauncher.componentToCentreAround = pluginEditor;

   windowSettingsLauncher.escapeKeyTriggersCloseButton = true;
   windowSettingsLauncher.useNativeTitleBar = false;
   windowSettingsLauncher.resizable = false;
   windowSettingsLauncher.useBottomRightCornerResizer = false;

   // launch dialog window
   DialogWindow* windowSettings = windowSettingsLauncher.launchAsync();
   windowSettings->setAlwaysOnTop( true );

   return windowSettings;
}


/// Initialise dialog window components.
///
/// @param componentWidth width of content component
///
/// @param componentHeight height of content component
///
/// @param pluginSettings string containing the formatted plug-in
///        settings
///
void WindowSettingsContent::initialise(
   int componentWidth,
   int componentHeight,
   const String& pluginSettings )

{
   // set dimensions of content component
   setSize( componentWidth, componentHeight );

   // initialise text editor component for displaying the plugin's
   // settings (read-only, can display multiple lines)
   textEditor_.setReadOnly( true );
   textEditor_.setMultiLine( true, true );
   addAndMakeVisible( textEditor_ );

   // display plug-in settings
   Font fontRegular( Font::getDefaultMonospacedFontName(), 15.0f, Font::plain );

   textEditor_.setFont( fontRegular );
   textEditor_.insertTextAtCaret( pluginSettings );

   // copy settings to system clipboard
   textEditor_.selectAll();
   textEditor_.copy();

   // in case the text has become too long to fit into the text
   // editor component, scroll to the beginning
   textEditor_.setCaretPosition( 0 );
   textEditor_.scrollEditorToPositionCaret( 0, 0 );

   // initialise "close" button
   buttonClose_.setButtonText( "Close" );
   addAndMakeVisible( buttonClose_ );
   buttonClose_.addListener( this );

   // style and place the dialog window's components
   applySkin();
}


/// Style and place the dialog window's components.
///
void WindowSettingsContent::applySkin()
{
   // style text editor component
   textEditor_.setColour(
      TextEditor::backgroundColourId,
      Colours::darkgrey );

   textEditor_.setColour(
      TextEditor::outlineColourId,
      Colours::black );

   textEditor_.applyColourToAllText(
      Colours::white );

   textEditor_.setColour(
      TextEditor::highlightColourId,
      Colours::yellow.withAlpha( 0.85f ) );

   textEditor_.setColour(
      TextEditor::highlightedTextColourId,
      Colours::black );

   // hide cursor
   textEditor_.setCaretVisible( false );


   // style "close" button
   buttonClose_.setColour(
      TextButton::buttonColourId,
      Colours::yellow );

   buttonClose_.setColour(
      TextButton::textColourOffId,
      Colours::black );


   // place components
   int width = getWidth();
   int height = getHeight();

   textEditor_.setBounds( 4, 6, width - 8, height - 43 );
   buttonClose_.setBounds( width / 2 - 30, height - 29, 60, 20 );
}


/// Called when a button is clicked.
///
/// @param button clicked button
///
void WindowSettingsContent::buttonClicked(
   Button* button )

{
   // user wants to close the window
   if ( button == &buttonClose_ ) {
      // close dialog window
      closeButtonPressed();
   }
}


/// This method is called when the user tries to close the window.
/// This is triggered by the user clicking the close button, or using
/// some other OS-specific key shortcut or OS menu for getting rid of
/// a window.
///
void WindowSettingsContent::closeButtonPressed()
{
   // get parent dialog window
   DialogWindow* dialogWindow = findParentComponentOfClass<DialogWindow>();

   if ( dialogWindow != nullptr ) {
      // close dialog window (exit value 0)
      dialogWindow->exitModalState( 0 );
   }
}

} // namespace
