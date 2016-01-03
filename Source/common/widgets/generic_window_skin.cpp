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

#include "generic_window_skin.h"


/// Create dialog window for selecting a new GUI skin.
///
/// @param pSkinName pointer to name of the currently used skin
///
/// @param fileSkinDirectory directory containing the skins
///
/// ### Dialog window exit values
///
/// | %Value | %Result                           |
/// | :----: | --------------------------------- |
/// | 0      | window has been closed "by force" |
/// | 1      | user has selected old skin        |
/// | 2      | user has selected a new skin      |
///
GenericWindowSkin::GenericWindowSkin(String *pSkinName, const File &fileSkinDirectory)
{
    // store pointer to name of the currenty used skin
    currentSkinName = pSkinName;

    // dialog window dimensions
    int windowWidth = 150;
    int windowHeight = 0;

    // fill list box model
    listBoxModel.fill(fileSkinDirectory);

    // set model for list box
    listBox.setModel(&listBoxModel);

    // calculate and set list box dimensions
    int listBoxWidth = windowWidth - 20;
    int listBoxHeight = listBoxModel.getNumRows() * listBox.getRowHeight() + 2;
    listBox.setBounds(10, 10, listBoxWidth, listBoxHeight);

    // set look of list box
    listBox.setColour(ListBox::outlineColourId, Colours::grey);
    listBox.setOutlineThickness(1);

    // disable multiple selections
    listBox.setMultipleSelectionEnabled(false);

    // select current skin in list box
    int rowNumber = listBoxModel.getRow(*currentSkinName);
    listBox.selectRow(rowNumber);

    // display list box
    this->addAndMakeVisible(listBox);

    // calculate dialog window height from height of list box
    windowHeight = listBoxHeight + 50;

    // create and position an "select" button
    buttonSelect.setButtonText("Select");
    buttonSelect.setBounds(10, windowHeight - 30, 60, 20);
    buttonSelect.setColour(TextButton::buttonColourId, Colours::red);
    buttonSelect.setColour(TextButton::buttonOnColourId, Colours::red);

    // add "skin" window as button listener and display the button
    buttonSelect.addListener(this);
    this->addAndMakeVisible(buttonSelect);

    // create and position an "default" button
    buttonDefault.setButtonText("Default");
    buttonDefault.setBounds(windowWidth - 70, windowHeight - 30, 60, 20);
    buttonDefault.setColour(TextButton::buttonColourId, Colours::yellow);
    buttonDefault.setColour(TextButton::buttonOnColourId, Colours::yellow);

    // add "skin" window as button listener and display the button
    buttonDefault.addListener(this);
    this->addAndMakeVisible(buttonDefault);

    // set component window dimensions
    setSize(windowWidth, windowHeight);
}


/// Destructor.
///
GenericWindowSkin::~GenericWindowSkin()
{
}


/// Static helper function to create a dialog window for selecting a
/// new GUI skin.
///
/// @param pEditor pointer to audio plug-in editor
///
/// @param pSkinName pointer to name of the currently used skin
///
/// @param fileSkinDirectory directory containing the skins
///
/// @return created dialog window
///
DialogWindow *GenericWindowSkin::createWindowSkin(AudioProcessorEditor *pEditor, String *pSkinName, const File &fileSkinDirectory)
{
    // prepare dialog window
    DialogWindow::LaunchOptions windowSkinLauncher;

    windowSkinLauncher.dialogTitle = "Select skin";
    windowSkinLauncher.dialogBackgroundColour = Colours::white;
    windowSkinLauncher.content.setOwned(new GenericWindowSkin(pSkinName, fileSkinDirectory));
    windowSkinLauncher.componentToCentreAround = pEditor;

    windowSkinLauncher.escapeKeyTriggersCloseButton = true;
    windowSkinLauncher.useNativeTitleBar = false;
    windowSkinLauncher.resizable = false;
    windowSkinLauncher.useBottomRightCornerResizer = false;

    // launch dialog window
    DialogWindow *windowSkin = windowSkinLauncher.launchAsync();
    windowSkin->setAlwaysOnTop(true);

    return windowSkin;
}


/// Called when a button is clicked.
///
/// @param button clicked button
///
void GenericWindowSkin::buttonClicked(Button *button)
{
    // user has selected a skin
    if (button == &buttonSelect)
    {
        // get parent dialog window
        DialogWindow *dialogWindow = findParentComponentOfClass<DialogWindow>();

        if (dialogWindow != nullptr)
        {
            // get first selected row index from list box
            int selectedRow = listBox.getSelectedRow(0);

            // get selected skin name and update editor
            String newSkinName = listBoxModel.getSkinName(selectedRow);

            if (newSkinName == *currentSkinName)
            {
                // close dialog window (exit code 1)
                dialogWindow->exitModalState(1);
            }
            else
            {
                // update skin name
                *currentSkinName = newSkinName;

                // close dialog window (exit code 2)
                dialogWindow->exitModalState(2);
            }
        }
    }
    // user has selected a default skin
    else if (button == &buttonDefault)
    {
        // get first selected row index from list box
        int selectedRow = listBox.getSelectedRow(0);

        // update default skin
        listBoxModel.setDefault(selectedRow);

        // redraw list box
        listBox.repaint();
    }
}


/// Create a list box model for a GenericWindowSkin.
///
GenericSkinListBoxModel::GenericSkinListBoxModel()
    : skinWildcard("*.skin", String::empty, "Skin files"),
      directoryThread("Skin directory scanner")
{
}


/// Destructor.
///
GenericSkinListBoxModel::~GenericSkinListBoxModel()
{
}


/// Fill list box model with all skins in a given directory.
///
/// @param fileSkinDirectory directory containing the skins
///
void GenericSkinListBoxModel::fill(const File &fileSkinDirectory)
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
