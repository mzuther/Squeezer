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

#include "generic_window_about_content.h"


/// Create content component for dialog window showing version,
/// copyright, license and so on.
///
/// ### Exit values
///
/// | %Value | %Result                           |
/// | :----: | --------------------------------- |
/// | 0      | window has been closed "by force" |
/// | 1      | user wants to close the window    |
///
GenericWindowAboutContent::GenericWindowAboutContent()
{
}


/// Static helper function to create a dialog window for showing
/// version, copyright, license and so on.
///
/// @param pluginEditor audio plug-in editor
///
/// @param width width of content component
///
/// @param height height of content component
///
/// @param chapters dictionary containing chapter headlines and the
///        accompanying text
///
/// @return created dialog window
///
DialogWindow *GenericWindowAboutContent::createDialogWindow(
    AudioProcessorEditor *pluginEditor, int width, int height,
    const StringPairArray &chapters)
{
    // prepare dialog window
    DialogWindow::LaunchOptions windowAboutLauncher;

    // create content component
    GenericWindowAboutContent *content = new GenericWindowAboutContent();
    content->initialize(width, height, chapters);

    // initialize dialog window settings
    windowAboutLauncher.dialogTitle = String("About ") + ProjectInfo::projectName;
    windowAboutLauncher.dialogBackgroundColour = Colours::white;
    windowAboutLauncher.content.setOwned(content);
    windowAboutLauncher.componentToCentreAround = pluginEditor;

    windowAboutLauncher.escapeKeyTriggersCloseButton = true;
    windowAboutLauncher.useNativeTitleBar = false;
    windowAboutLauncher.resizable = false;
    windowAboutLauncher.useBottomRightCornerResizer = false;

    // launch dialog window
    DialogWindow *windowAbout = windowAboutLauncher.launchAsync();
    windowAbout->setAlwaysOnTop(true);

    return windowAbout;
}


/// Initialise dialog window components.
///
/// @param width width of content component
///
/// @param height height of content component
///
/// @param chapters dictionary containing chapter headlines and the
///        accompanying text
///
void GenericWindowAboutContent::initialize(int width, int height,
        const StringPairArray &chapters)
{
    // set dimensions of content component
    setSize(width, height);

    // initialize text editor component for displaying the plugin's
    // settings (read-only, can display multiple lines)
    EditorAbout.setMultiLine(true, true);
    EditorAbout.setReadOnly(true);
    addAndMakeVisible(EditorAbout);

    // display plug-in description
    addChapters(chapters);

    // initialize "license" button which opens the license text in a
    // web browser
    addAndMakeVisible(ButtonGpl);
    ButtonGpl.addListener(this);

    // set license button images
    ButtonGpl.setImages(true, false, true,
                        ImageCache::getFromMemory(
                            resources::button_gpl_normal_png,
                            resources::button_gpl_normal_pngSize),
                        1.0f, Colour(),
                        ImageCache::getFromMemory(
                            resources::button_gpl_down_png,
                            resources::button_gpl_down_pngSize),
                        1.0f, Colour(),
                        ImageCache::getFromMemory(
                            resources::button_gpl_down_png,
                            resources::button_gpl_down_pngSize),
                        1.0f, Colour());

    // initialize "close" button
    ButtonClose.setButtonText("Close");
    addAndMakeVisible(ButtonClose);
    ButtonClose.addListener(this);

    // style and place the dialog window's components
    applySkin();
}


/// Style and place the dialog window's components.
///
void GenericWindowAboutContent::applySkin()
{
    // style text editor component
    EditorAbout.setColour(
        TextEditor::backgroundColourId,
        Colours::black.withAlpha(0.25f));
    EditorAbout.setColour(
        TextEditor::textColourId,
        Colours::black);
    EditorAbout.setColour(
        TextEditor::highlightColourId,
        Colours::black.withAlpha(0.15f));
    EditorAbout.setColour(
        TextEditor::highlightedTextColourId,
        Colours::black);

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

    EditorAbout.setBounds(0, 0, width, height - 47);
    ButtonGpl.setBounds(6, height - 41, 64, 32);
    ButtonClose.setBounds(width - 70, height - 34, 60, 20);
}


/// Add headlines and text to the text editor.
///
/// @param chapters dictionary containing chapter headlines and the
///        accompanying text
///
void GenericWindowAboutContent::addChapters(const StringPairArray &chapters)
{
    // set up two fonts, one for headlines and one for regular text
    Font headlineFont(16.0f, Font::bold);
    Font regularFont(15.0f, Font::plain);

    // extract (sorted!) headlines from dictionary
    StringArray headlines = chapters.getAllKeys();

    // loop over headlines
    for (int n = 0; n < headlines.size(); ++n)
    {
        // get current headline
        String currentHeadline = headlines[n];

        // skip empty chapters
        if (!chapters[currentHeadline].isEmpty())
        {
            // display headline
            EditorAbout.setFont(headlineFont);
            EditorAbout.insertTextAtCaret(
                currentHeadline + "\n"
            );

            // display accompanying text
            EditorAbout.setFont(regularFont);
            EditorAbout.insertTextAtCaret(
                chapters[currentHeadline]
            );

            // no newline after last chapter
            if (n < headlines.size() - 1)
            {
                EditorAbout.insertTextAtCaret("\n");
            }
        }
    }

    // in case the text has become too long to fit into the text
    // editor component, scroll to the beginning
    EditorAbout.setCaretPosition(0);
    EditorAbout.scrollEditorToPositionCaret(0, 0);
}


/// Called when a button is clicked.
///
/// @param button clicked button
///
void GenericWindowAboutContent::buttonClicked(Button *button)
{
    // user wants to close the window
    if (button == &ButtonClose)
    {
        // get parent dialog window
        DialogWindow *dialogWindow = findParentComponentOfClass<DialogWindow>();

        if (dialogWindow != nullptr)
        {
            // close dialog window (exit code 1)
            dialogWindow->exitModalState(1);
        }
    }
    // user wants to read the GPL
    else if (button == &ButtonGpl)
    {
        // open license text in default web browser
        URL urlGpl("http://www.gnu.org/licenses/gpl-3.0.html");
        urlGpl.launchInDefaultBrowser();
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
