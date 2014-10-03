/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013 Martin Zuther (http://www.mzuther.de/)

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


WindowSettings::WindowSettings(int nWidth, int nHeight, String &strPluginSettings)
    : ResizableWindow("Plugin Settings", false)
    // create new window child of width "nWidth" and height "nHeight"
{
    // set dimensions to those passed to the function ...
    setBounds(0, 0, nWidth, nHeight);

    // ... and keep the new window on top
    setAlwaysOnTop(true);

    // this window does not have any transparent areas (increases
    // performance on redrawing)
    setOpaque(true);

    // prohibit movement of the new window
    pConstrainer = new ProhibitingBoundsConstrainer();
    setConstrainer(pConstrainer);

    // empty windows are boring, so let's prepare a space for some
    // window components
    contentComponent = new Component("Window Area");
    setContentOwned(contentComponent, false);

    // create a word-wrapping read-only text editor component with
    // multiple lines for displaying the plugin's settings
    TextEditorSettings = new TextEditor("Settings Text");
    TextEditorSettings->setMultiLine(true, true);
    TextEditorSettings->setReadOnly(true);

    // initialise colours of the text editor component
    TextEditorSettings->setColour(TextEditor::backgroundColourId, Colours::black.withAlpha(0.25f));
    TextEditorSettings->setColour(TextEditor::textColourId, Colours::white);
    TextEditorSettings->setColour(TextEditor::highlightColourId, Colours::white.withAlpha(0.25f));
    TextEditorSettings->setColour(TextEditor::highlightedTextColourId, Colours::white);

    // set up font for regular text
    Font fontRegular(Font::getDefaultMonospacedFontName(), 15.0f, Font::plain);

    // display plug-in description
    TextEditorSettings->setFont(fontRegular);
    TextEditorSettings->insertTextAtCaret(strPluginSettings);

    // copy settings to system clipboard
    TextEditorSettings->selectAll();
    TextEditorSettings->copy();

    // in case the text has become too long to fit into the text
    // editor component, scroll to the beginning
    TextEditorSettings->setCaretPosition(0);
    TextEditorSettings->scrollEditorToPositionCaret(0, 0);

    // finally, position and display the text editor component
    TextEditorSettings->setBounds(0, 0, nWidth, nHeight - 47);
    contentComponent->addAndMakeVisible(TextEditorSettings);

    // create and position a "settings" button that appears as if it
    // were pressed down and which closes the window when clicked
    ButtonSettings = new TextButton("Settings");
    ButtonSettings->setBounds(nWidth - 59, nHeight - 43, 52, 20);
    ButtonSettings->setColour(TextButton::buttonColourId, Colours::yellow);
    ButtonSettings->setColour(TextButton::buttonOnColourId, Colours::grey);

    // add "settings" window as button listener and display the button
    ButtonSettings->addListener(this);
    contentComponent->addAndMakeVisible(ButtonSettings);
}


WindowSettings::~WindowSettings()
{
    delete pConstrainer;
    pConstrainer = nullptr;

    // delete all children of the window; "contentComponent" will be
    // deleted by the base class, so please leave it alone!
    contentComponent->deleteAllChildren();
}


void WindowSettings::paint(Graphics &g)
{
    // fill window background with grey colour gradient
    g.setGradientFill(ColourGradient(Colours::darkgrey.darker(0.4f), 0, 0, Colours::darkgrey.darker(1.0f), 0, (float) getHeight(), false));
    g.fillAll();
}


void WindowSettings::buttonClicked(Button *button)
{
    // find out which button has been clicked
    if (button == ButtonSettings)
    {
        // close window by making it invisible
        setVisible(false);
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
