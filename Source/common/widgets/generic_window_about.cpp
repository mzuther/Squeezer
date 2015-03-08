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

#include "generic_window_about.h"


/// Create dialog window showing version, copyright, license and so
/// on.
///
/// @param editorWindow pointer to the plug-in GUI
///
/// ### Exit values
///
/// | %Value | %Result                           |
/// | :----: | --------------------------------- |
/// | 0      | window has been closed "by force" |
/// | 1      | user wants to close the window    |
///
GenericWindowAbout::GenericWindowAbout(Component *editorWindow)
    : DocumentWindow(String("About ") + ProjectInfo::projectName,
                     Colours::white, 0, true)
{
    // dialog window dimentsions
    int windowWidth = 270;
    int windowHeight = 540;

    // empty windows are boring, so let's prepare a space for some
    // window components
    setContentOwned(&contentComponent, false);

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
    contentComponent.addAndMakeVisible(editorAbout);

    // create and position an "close" button
    buttonClose.setButtonText("Close");
    buttonClose.setBounds(windowWidth - 70, windowHeight - 34, 60, 20);
    buttonClose.setColour(TextButton::buttonColourId, Colours::yellow);
    buttonClose.setColour(TextButton::buttonOnColourId, Colours::yellow);

    // add "about" window as button listener and display the button
    buttonClose.addListener(this);
    contentComponent.addAndMakeVisible(buttonClose);

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
    contentComponent.addAndMakeVisible(buttonGpl);

    // set window dimensions
    setSize(windowWidth, windowHeight + getTitleBarHeight());

    // keep dialog window on top
    setAlwaysOnTop(true);

    // center window on editor
    centreAroundComponent(editorWindow, getWidth(), getHeight());

    // this window does not have any transparent areas (increases
    // performance on redrawing)
    setOpaque(true);

    // finally, display window
    setVisible(true);
}


/// Destructor.
///
GenericWindowAbout::~GenericWindowAbout()
{
}


/// Add headlines and text to the text editor.
///
/// @param arrChapters dictionary containing chapter headlines and the
///        accompanying text
///
void GenericWindowAbout::addChapters(StringPairArray &arrChapters)
{
    // set up two fonts, one for headlines and one for regular text
    Font headlineFont(16.0f, Font::bold);
    Font regularFont(15.0f, Font::plain);

    // extract (sorted!) headlines from dictionary
    StringArray arrHeadlines = arrChapters.getAllKeys();

    // loop over headlines
    for (int n = 0; n < arrHeadlines.size(); n++)
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
void GenericWindowAbout::buttonClicked(Button *button)
{
    // user wants to close the window
    if (button == &buttonClose)
    {
        // exit code 1: user wants to close the window
        int exitValue = 1;

        // close window and return exit value
        exitModalState(exitValue);
    }
    // user wants to read the GPL
    else if (button == &buttonGpl)
    {
        // open license text in default web browser
        URL urlGpl("http://www.gnu.org/licenses/gpl-3.0.html");
        urlGpl.launchInDefaultBrowser();
    }
}


/// This method is called when the user tries to close the window "by
/// force".  For example, this may be achieved by pressing the close
/// button on the title bar.
///
void GenericWindowAbout::closeButtonPressed()
{
    // exit code 0: window closed by force
    int exitValue = 0;

    // close window and return exit value
    exitModalState(exitValue);
}

// Local Variables:
// ispell-local-dictionary: "british"
// End:
