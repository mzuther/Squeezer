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
/// @param arrChapters dictionary containing chapter headlines and the
///        accompanying text
///
/// ### Exit values
///
/// | %Value | %Result                           |
/// | :----: | --------------------------------- |
/// | 0      | window has been closed "by force" |
/// | 1      | user wants to close the window    |
///
GenericWindowAboutContent::GenericWindowAboutContent(StringPairArray &arrChapters)
{
    // dialog window dimensions
    int windowWidth = 270;
    int windowHeight = 540;

    // create a word-wrapping read-only text editor component with
    // multiple lines for displaying information about the plug-in
    editorAbout.setMultiLine(true, true);
    editorAbout.setReadOnly(true);

    // initialise colours of the text editor component
    editorAbout.setColour(TextEditor::backgroundColourId, Colours::black.withAlpha(0.25f));
    editorAbout.setColour(TextEditor::textColourId, Colours::black);
    editorAbout.setColour(TextEditor::highlightColourId, Colours::black.withAlpha(0.15f));
    editorAbout.setColour(TextEditor::highlightedTextColourId, Colours::black);

    // position and display the text editor component
    editorAbout.setBounds(0, 0, windowWidth, windowHeight - 47);
    this->addAndMakeVisible(editorAbout);

    // add headlines and text to the text editor component
    addChapters(arrChapters);

    // create and position an "close" button
    buttonClose.setButtonText("Close");
    buttonClose.setBounds(windowWidth - 70, windowHeight - 34, 60, 20);
    buttonClose.setColour(TextButton::buttonColourId, Colours::yellow);
    buttonClose.setColour(TextButton::buttonOnColourId, Colours::yellow);

    // add "about" window as button listener and display the button
    buttonClose.addListener(this);
    this->addAndMakeVisible(buttonClose);

    // create and position the image button which opens the license
    // text in a web browser
    buttonGpl.setBounds(6, windowHeight - 41, 64, 32);
    buttonGpl.setImages(true, false, true,
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

    // add "about" window as button listener and display the button
    buttonGpl.addListener(this);
    this->addAndMakeVisible(buttonGpl);

    // set window dimensions
    setSize(windowWidth, windowHeight);
}


/// Destructor.
///
GenericWindowAboutContent::~GenericWindowAboutContent()
{
}


/// Static helper function to create a dialog window for showing
/// version, copyright, license and so on.
///
/// @param pEditor pointer to audio plug-in editor
///
/// @param arrChapters dictionary containing chapter headlines and the
///        accompanying text
///
/// @return created dialog window
///
DialogWindow *GenericWindowAboutContent::createDialogWindow(AudioProcessorEditor *pEditor, StringPairArray &arrChapters)
{
    // prepare dialog window
    DialogWindow::LaunchOptions windowAboutLauncher;

    windowAboutLauncher.dialogTitle = String("About ") + ProjectInfo::projectName;
    windowAboutLauncher.dialogBackgroundColour = Colours::white;
    windowAboutLauncher.content.setOwned(new GenericWindowAboutContent(arrChapters));
    windowAboutLauncher.componentToCentreAround = pEditor;

    windowAboutLauncher.escapeKeyTriggersCloseButton = true;
    windowAboutLauncher.useNativeTitleBar = false;
    windowAboutLauncher.resizable = false;
    windowAboutLauncher.useBottomRightCornerResizer = false;

    // launch dialog window
    DialogWindow *windowAbout = windowAboutLauncher.launchAsync();
    windowAbout->setAlwaysOnTop(true);

    return windowAbout;
}


/// Add headlines and text to the text editor.
///
/// @param arrChapters dictionary containing chapter headlines and the
///        accompanying text
///
void GenericWindowAboutContent::addChapters(StringPairArray &arrChapters)
{
    // set up two fonts, one for headlines and one for regular text
    Font headlineFont(16.0f, Font::bold);
    Font regularFont(15.0f, Font::plain);

    // extract (sorted!) headlines from dictionary
    StringArray arrHeadlines = arrChapters.getAllKeys();

    // loop over headlines
    for (int n = 0; n < arrHeadlines.size(); ++n)
    {
        // get current headline
        String currentHeadline = arrHeadlines[n];

        // skip empty chapters
        if (!arrChapters[currentHeadline].isEmpty())
        {
            // display headline
            editorAbout.setFont(headlineFont);
            editorAbout.insertTextAtCaret(
                currentHeadline + "\n"
            );

            // display accompanying text
            editorAbout.setFont(regularFont);
            editorAbout.insertTextAtCaret(
                arrChapters[currentHeadline]
            );

            // no newline after last chapter
            if (n < arrHeadlines.size() - 1)
            {
                editorAbout.insertTextAtCaret("\n");
            }
        }
    }

    // in case the text has become too long to fit into the text
    // editor component, scroll to the beginning
    editorAbout.setCaretPosition(0);
    editorAbout.scrollEditorToPositionCaret(0, 0);

    // hide cursor
    editorAbout.setCaretVisible(false);
}


/// Called when a button is clicked.
///
/// @param button clicked button
///
void GenericWindowAboutContent::buttonClicked(Button *button)
{
    // user wants to close the window
    if (button == &buttonClose)
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
    else if (button == &buttonGpl)
    {
        // open license text in default web browser
        URL urlGpl("http://www.gnu.org/licenses/gpl-3.0.html");
        urlGpl.launchInDefaultBrowser();
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
