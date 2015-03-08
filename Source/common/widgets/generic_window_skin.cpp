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

#include "generic_window_skin.h"


/// Create dialog window for selecting a new GUI skin.
///
/// @param editorWindow pointer to the plug-in GUI
/// @param currentSkinFile file pointing to the currently used skin
///
/// ### Exit values
///
/// | %Value | %Result                            |
/// | :----: | ---------------------------------- |
/// | 0      | skin selection has been cancelled  |
/// | 1      | user has selected a skin           |
///
GenericWindowSkin::GenericWindowSkin(Component *editorWindow, const File &currentSkinFile)
    : DocumentWindow("Select skin", Colours::white, 0, true)
{
    // dialog window dimentsions
    int windowWidth = 150;
    int windowHeight = 0;

    // empty windows are boring, so let's prepare a space for some
    // window components
    contentComponent = new Component("Window Area");
    setContentOwned(contentComponent, false);

    // locate skin directory and prepare list box model
    File fileSkinDirectory = currentSkinFile.getParentDirectory();
    listBoxModel = new GenericSkinListBoxModel(fileSkinDirectory);

    // prepare a list box
    listBox = new ListBox("ListBox Skins", nullptr);
    listBox->setModel(listBoxModel);

    // calculate and set list box dimensions
    int listBoxWidth = windowWidth - 20;
    int listBoxHeight = listBoxModel->getNumRows() * listBox->getRowHeight() + 2;
    listBox->setBounds(10, 10, listBoxWidth, listBoxHeight);

    // set look of list box
    listBox->setColour(ListBox::outlineColourId, Colours::grey);
    listBox->setOutlineThickness(1);

    // disable multiple selections
    listBox->setMultipleSelectionEnabled(false);

    // select current skin in list box
    currentSkinName = currentSkinFile.getFileNameWithoutExtension();
    int rowNumber = listBoxModel->getRow(currentSkinName);
    listBox->selectRow(rowNumber);

    // display list box
    contentComponent->addAndMakeVisible(listBox);

    // calculate dialog window height from height of list box
    windowHeight = listBoxHeight + 50;

    // create and position an "select" button
    buttonSelect = new TextButton("Select");
    buttonSelect->setBounds(10, windowHeight - 30, 60, 20);
    buttonSelect->setColour(TextButton::buttonColourId, Colours::red);
    buttonSelect->setColour(TextButton::buttonOnColourId, Colours::red);

    // add "skin" window as button listener and display the button
    buttonSelect->addListener(this);
    contentComponent->addAndMakeVisible(buttonSelect);

    // create and position an "default" button
    buttonDefault = new TextButton("Default");
    buttonDefault->setBounds(windowWidth - 70, windowHeight - 30, 60, 20);
    buttonDefault->setColour(TextButton::buttonColourId, Colours::yellow);
    buttonDefault->setColour(TextButton::buttonOnColourId, Colours::yellow);

    // add "skin" window as button listener and display the button
    buttonDefault->addListener(this);
    contentComponent->addAndMakeVisible(buttonDefault);

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
GenericWindowSkin::~GenericWindowSkin()
{
}


/// Called when a button is clicked.
///
/// @param button clicked button
///
void GenericWindowSkin::buttonClicked(Button *button)
{
    // user has selected a skin
    if (button == buttonSelect)
    {
        // exit code 1: user has selected a skin
        int exitValue = 1;

        // close window and return exit value -- the selected skin's
        // name may be obtained from the plug-in's GUI window
        exitModalState(exitValue);
    }
    // user has selected a default skin
    else if (button == buttonDefault)
    {
        // get first selected row index from list box
        int selectedRow = listBox->getSelectedRow(0);

        // update default skin
        listBoxModel->setDefault(selectedRow);

        // redraw list box
        listBox->repaint();
    }
}


/// This method is called when the user tries to close the window.
///
void GenericWindowSkin::closeButtonPressed()
{
    // exit code 0: skin selection has been cancelled
    int exitValue = 0;

    // close window and return exit value
    exitModalState(exitValue);
}


/// Get the currently selected string.  This method may be called
/// after closing the window, as long as the window class instance
/// still exists.
///
/// @return base name of the selected skin (without file ending or
///         directory)
///
const String GenericWindowSkin::getSelectedSkinName()
{
    // get first selected row index from list box
    int selectedRow = listBox->getSelectedRow(0);

    // get and return selected skin name
    return listBoxModel->getSkinName(selectedRow);
}


GenericSkinListBoxModel::GenericSkinListBoxModel(const File &fileSkinDirectory)
    : skinWildcard("*.skin", String::empty, "Skin files"),
      directoryThread("Skin directory scanner")
{
    // set up scanner for skin directory
    DirectoryContentsList skinFiles(&skinWildcard, directoryThread);
    skinFiles.setDirectory(fileSkinDirectory, false, true);

    // search skin files in directory
    directoryThread.startThread();

    // locate file containing the default skin's name
    defaultSkinFile = fileSkinDirectory.getChildFile("default_skin.ini");

    // make sure the file exists
    if (!defaultSkinFile.existsAsFile())
    {
        // create file
        defaultSkinFile.create();

        // set "Default" as default skin (using Unicode encoding)
        defaultSkinFile.replaceWithText("Default", true, true);
    }

    // load name of default skin from file
    defaultSkinName = defaultSkinFile.loadFileAsString();

    // wait for directory scan
    while (skinFiles.isStillLoading())
    {
        // keep Visual C++ from optimising (and thus breaking) the loop
        Thread::sleep(20);
    }

    // add found skins to list box
    for (int n = 0; n < skinFiles.getNumFiles(); n++)
    {
        // get skin file
        File skinFile = skinFiles.getFile(n);

        // remove file extension
        String skinName = skinFile.getFileNameWithoutExtension();

        // add skin name to internal array
        arrSkinNames.add(skinName);
    }
}


/// Destructor.
///
GenericSkinListBoxModel::~GenericSkinListBoxModel()
{
}


/// Get the number of skin names in the list box.
///
/// @return number of skin names
///
int GenericSkinListBoxModel::getNumRows()
{
    // return size of internal array
    return arrSkinNames.size();
}


/// Get the index of the first row that matches the given skin name.
///
/// @param skinNameToLookFor skin name to look for
///
/// @return index of the first matching element
///
int GenericSkinListBoxModel::getRow(const String &skinNameToLookFor)
{
    // search internal array for skin name
    return arrSkinNames.indexOf(skinNameToLookFor);
}


/// Return one of the skin names in the list box.
///
/// @param rowNumber zero-based row number to retrieve
///
/// @return skin name
///
const String GenericSkinListBoxModel::getSkinName(int rowNumber)
{
    // check for valid index
    if (rowNumber < arrSkinNames.size())
    {
        // return matching skin name from internal array
        return arrSkinNames[rowNumber];
    }
    // invalid index
    else
    {
        // return empty string
        return String::empty;
    }
}


/// Set default skin name.  This skin name will be painted in bold
/// font.
///
/// @param rowNumber zero-based row number containing default skin
///        name
///
void GenericSkinListBoxModel::setDefault(int rowNumber)
{
    // skip invalid index
    if (rowNumber >= arrSkinNames.size())
    {
        return;
    }

    // store name of default skin
    defaultSkinName = arrSkinNames[rowNumber];

    // store default skin name in file "default_skin.ini" (using
    // Unicode encoding)
    defaultSkinFile.replaceWithText(defaultSkinName, true, true);
}


/// Draws a row of the list.
///
/// @param rowNumber index of row to draw
///
/// @param g graphics context
///
/// @param width row width
///
/// @param height row height
///
/// @param isRowSelected indicates whether row is currently selected
///
void GenericSkinListBoxModel::paintListBoxItem(int rowNumber, Graphics &g, int width, int height, bool isRowSelected)
{
    // draw selected rows in light blue
    if (isRowSelected)
    {
        g.fillAll(Colours::lightblue);
    }

    // get skin name for current row
    String skinName = getSkinName(rowNumber);

    // draw default skin name in bold font
    if (skinName == defaultSkinName)
    {
        g.setFont(Font(14.0f, Font::bold));
    }
    // draw all other skin names in plain font
    else
    {
        g.setFont(Font(14.0f, Font::plain));
    }

    // render row text in black
    g.setColour(Colours::black);
    g.drawText(skinName, 2, 0, width - 4, height, Justification::centredLeft, true);
}

// Local Variables:
// ispell-local-dictionary: "british"
// End:
