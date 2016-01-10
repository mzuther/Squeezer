/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013-2016 Martin Zuther (http://www.mzuther.de/)

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

#include "generic_window_settings_content.h"


/// Create content component for dialog window with validation settings.
///
/// ### Exit values
///
/// | %Value | %Result                |
/// | :----: | ---------------------- |
/// | 0      | window has been closed |
///
GenericWindowSettingsContent::GenericWindowSettingsContent()
{
}


/// Static helper function to create a dialog window for plug-in
/// settings.
///
/// @param pluginEditor audio plug-in editor
///
/// @param width width of content component
///
/// @param height height of content component
///
/// @param pluginSettings string containing the formatted plug-in
///        settings
///
/// @return created dialog window
///
DialogWindow *GenericWindowSettingsContent::createDialogWindow(
    AudioProcessorEditor *pluginEditor, int width, int height,
    const String &pluginSettings)
{
    // prepare dialog window
    DialogWindow::LaunchOptions windowSettingsLauncher;

    // create content component
    GenericWindowSettingsContent *content = new GenericWindowSettingsContent();
    content->initialize(width, height, pluginSettings);

    // initialize dialog window settings
    windowSettingsLauncher.dialogTitle = String("Plug-in Settings");
    windowSettingsLauncher.dialogBackgroundColour = Colours::white;
    windowSettingsLauncher.content.setOwned(
        content);
    windowSettingsLauncher.componentToCentreAround = pluginEditor;

    windowSettingsLauncher.escapeKeyTriggersCloseButton = true;
    windowSettingsLauncher.useNativeTitleBar = false;
    windowSettingsLauncher.resizable = false;
    windowSettingsLauncher.useBottomRightCornerResizer = false;

    // launch dialog window
    DialogWindow *windowSettings = windowSettingsLauncher.launchAsync();
    windowSettings->setAlwaysOnTop(true);

    return windowSettings;
}


/// Initialise dialog window components.
///
/// @param width width of content component
///
/// @param height height of content component
///
/// @param pluginSettings string containing the formatted plug-in
///        settings
///
void GenericWindowSettingsContent::initialize(int width, int height,
        const String &pluginSettings)
{
    // set dimensions of content component
    setSize(width, height);

    // initialize text editor component for displaying the plugin's
    // settings (read-only, can display multiple lines)
    TextEditorSettings.setReadOnly(true);
    TextEditorSettings.setMultiLine(true, true);
    addAndMakeVisible(TextEditorSettings);

    // display plug-in settings
    Font fontRegular(Font::getDefaultMonospacedFontName(),
                     15.0f, Font::plain);
    TextEditorSettings.setFont(fontRegular);
    TextEditorSettings.insertTextAtCaret(pluginSettings);

    // copy settings to system clipboard
    TextEditorSettings.selectAll();
    TextEditorSettings.copy();

    // in case the text has become too long to fit into the text
    // editor component, scroll to the beginning
    TextEditorSettings.setCaretPosition(0);
    TextEditorSettings.scrollEditorToPositionCaret(0, 0);

    // initialize "close" button
    ButtonClose.setButtonText("Close");
    addAndMakeVisible(ButtonClose);
    ButtonClose.addListener(this);

    // style and place the dialog window's components
    applySkin();
}


/// Style and place the dialog window's components.
///
void GenericWindowSettingsContent::applySkin()
{
    // style text editor
    TextEditorSettings.setColour(
        TextEditor::backgroundColourId,
        Colours::black.withAlpha(0.25f));
    TextEditorSettings.setColour(
        TextEditor::textColourId,
        Colours::black);
    TextEditorSettings.setColour(
        TextEditor::highlightColourId,
        Colours::black.withAlpha(0.15f));
    TextEditorSettings.setColour(
        TextEditor::highlightedTextColourId,
        Colours::black);

    // hide cursor
    TextEditorSettings.setCaretVisible(false);

    // style "close" button
    ButtonClose.setColour(
        TextButton::buttonColourId,
        Colours::yellow);
    ButtonClose.setColour(
        TextButton::buttonOnColourId,
        Colours::yellow);

    // place components
    int width = getWidth();
    int height = getHeight();

    TextEditorSettings.setBounds(0, 0, width, height - 37);
    ButtonClose.setBounds(width / 2 - 30, height - 29, 60, 20);
}


/// Called when a button is clicked.
///
/// @param button clicked button
///
void GenericWindowSettingsContent::buttonClicked(Button *button)
{
    // user wants to close the window
    if (button == &ButtonClose)
    {
        // close dialog window
        closeButtonPressed();
    }
}


/// This method is called when the user tries to close the window.
/// This is triggered by the user clicking the close button, or using
/// some other OS-specific key shortcut or OS menu for getting rid of
/// a window.
///
void GenericWindowSettingsContent::closeButtonPressed()
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
