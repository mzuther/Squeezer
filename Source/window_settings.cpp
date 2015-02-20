/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013-2015 Martin Zuther (http://www.mzuther.de/)

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

#include "window_settings.h"


WindowSettings::WindowSettings(Component *pEditorWindow, String &strPluginSettings)
    : DocumentWindow("Plugin Settings", Colours::white, 0, true)
    // create new window child
{
    int nWidth = 440;
    int nHeight = 155;

    // set dimensions to those passed to the function ...
    setSize(nWidth, nHeight + getTitleBarHeight());

    // ... center window on editor ...
    centreAroundComponent(pEditorWindow, getWidth(), getHeight());

    // ... and keep the new window on top
    setAlwaysOnTop(true);

    // this window does not have any transparent areas (increases
    // performance on redrawing)
    setOpaque(true);

    // empty windows are boring, so let's prepare a space for some
    // window components
    setContentOwned(&contentComponent, false);

    // create a word-wrapping read-only text editor component with
    // multiple lines for displaying the plugin's settings
    TextEditorSettings.setMultiLine(true, true);
    TextEditorSettings.setReadOnly(true);

    // initialise colours of the text editor component
    TextEditorSettings.setColour(TextEditor::backgroundColourId, Colours::black.withAlpha(0.25f));
    TextEditorSettings.setColour(TextEditor::textColourId, Colours::black);
    TextEditorSettings.setColour(TextEditor::highlightColourId, Colours::black.withAlpha(0.15f));
    TextEditorSettings.setColour(TextEditor::highlightedTextColourId, Colours::black);

    // set up font for regular text
    Font fontRegular(Font::getDefaultMonospacedFontName(), 15.0f, Font::plain);

    // display plug-in description
    TextEditorSettings.setFont(fontRegular);
    TextEditorSettings.insertTextAtCaret(strPluginSettings.trim());

    // copy settings to system clipboard
    TextEditorSettings.selectAll();
    TextEditorSettings.copy();

    // in case the text has become too long to fit into the text
    // editor component, scroll to the beginning
    TextEditorSettings.setCaretPosition(0);
    TextEditorSettings.scrollEditorToPositionCaret(0, 0);

    // hide cursor
    TextEditorSettings.setCaretVisible(false);

    // finally, position and display the text editor component
    TextEditorSettings.setBounds(0, 0, nWidth, nHeight - 37);
    contentComponent.addAndMakeVisible(&TextEditorSettings);

    // create and position an "close" button
    ButtonClose.setButtonText("Close");
    ButtonClose.setBounds(nWidth / 2 - 30, nHeight - 29, 60, 20);
    ButtonClose.setColour(TextButton::buttonColourId, Colours::yellow);
    ButtonClose.setColour(TextButton::buttonOnColourId, Colours::yellow);

    // add "about" window as button listener and display the button
    ButtonClose.addListener(this);
    contentComponent.addAndMakeVisible(&ButtonClose);

    // finally, display window
    setVisible(true);
}


WindowSettings::~WindowSettings()
{
}


void WindowSettings::buttonClicked(Button *button)
{
    // find out which button has been clicked
    if (button == &ButtonClose)
    {
        exitModalState(1);
    }
}


void WindowSettings::closeButtonPressed()
{
    exitModalState(0);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
