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

#include "generic_window_skin_content.h"


/// Create content component for dialog window that allows selecting a
/// new GUI skin.
///
/// ### Dialog window exit values
///
/// | %Value | %Result                           |
/// | :----: | --------------------------------- |
/// | 0      | window has been closed "by force" |
/// | 1      | user has selected old skin        |
/// | 2      | user has selected a new skin      |
///
GenericWindowSkinContent::GenericWindowSkinContent()
{
}


/// Static helper function to create a dialog window for selecting a
/// new GUI skin.
///
/// @param pluginEditor audio plug-in editor
///
/// @param skinName name of the currently used skin (will always be
///        changed to reflect the current skin name)
///
/// @param skinDirectory directory containing the skins
///
/// @return created dialog window
///
DialogWindow *GenericWindowSkinContent::createDialogWindow(
    AudioProcessorEditor *pluginEditor, String *skinName,
    const File &skinDirectory)
{
    // prepare dialog window
    DialogWindow::LaunchOptions windowSkinLauncher;

    // create content component
    GenericWindowSkinContent *content = new GenericWindowSkinContent();
    content->initialize(skinName, skinDirectory);

    // initialize dialog window settings
    windowSkinLauncher.dialogTitle = "Select skin";
    windowSkinLauncher.dialogBackgroundColour = Colours::white;
    windowSkinLauncher.content.setOwned(content);
    windowSkinLauncher.componentToCentreAround = pluginEditor;

    windowSkinLauncher.escapeKeyTriggersCloseButton = true;
    windowSkinLauncher.useNativeTitleBar = false;
    windowSkinLauncher.resizable = false;
    windowSkinLauncher.useBottomRightCornerResizer = false;

    // launch dialog window
    DialogWindow *windowSkin = windowSkinLauncher.launchAsync();
    windowSkin->setAlwaysOnTop(true);

    return windowSkin;
}


/// Initialise dialog window components.
///
/// @param skinName name of the currently used skin (will always be
///        changed to reflect the current skin name)
///
/// @param skinDirectory directory containing the skins
///
void GenericWindowSkinContent::initialize(String *skinName,
        const File &skinDirectory)
{
    // store name of the currenty used skin
    currentSkinName = skinName;

    // initialize list box
    ListBoxModel.fill(skinDirectory);
    SkinList.setModel(&ListBoxModel);
    addAndMakeVisible(SkinList);

    // disable multiple selections
    SkinList.setMultipleSelectionEnabled(false);

    // select current skin in list box
    int row = ListBoxModel.getRow(*currentSkinName);
    SkinList.selectRow(row);

    // initialize "select" button for selecting a new skin
    ButtonSelect.setButtonText("Select");
    addAndMakeVisible(ButtonSelect);
    ButtonSelect.addListener(this);

    // initialize "default" button for selecting a new default skin
    ButtonDefault.setButtonText("Default");
    addAndMakeVisible(ButtonDefault);
    ButtonDefault.addListener(this);

    // style and place the dialog window's components
    applySkin();
}


/// Style and place the dialog window's components.
///
void GenericWindowSkinContent::applySkin()
{
    // style list box
    SkinList.setOutlineThickness(1);
    SkinList.setColour(
        ListBox::outlineColourId,
        Colours::grey);

    // style "style" button
    ButtonSelect.setColour(
        TextButton::buttonColourId,
        Colours::red);
    ButtonSelect.setColour(
        TextButton::buttonOnColourId,
        Colours::red);

    // style "default" button
    ButtonDefault.setColour(
        TextButton::buttonColourId,
        Colours::yellow);
    ButtonDefault.setColour(
        TextButton::buttonOnColourId,
        Colours::yellow);

    // initialize content component dimensions
    int width = 150;
    int height = 50;

    // add height of list box to height of content component
    int listBoxHeight = ListBoxModel.getNumRows() * SkinList.getRowHeight() + 2;
    height += listBoxHeight;

    // set component window dimensions
    setSize(width, height);

    // place components
    SkinList.setBounds(10, 10, width - 20, listBoxHeight);
    ButtonSelect.setBounds(10, height - 30, 60, 20);
    ButtonDefault.setBounds(width - 70, height - 30, 60, 20);
}


/// Called when a button is clicked.
///
/// @param button clicked button
///
void GenericWindowSkinContent::buttonClicked(Button *button)
{
    // user has selected a skin
    if (button == &ButtonSelect)
    {
        // get first selected row index from list box
        int selectedRow = SkinList.getSelectedRow(0);

        // get selected skin name and update editor
        String newSkinName = ListBoxModel.getSkinName(selectedRow);

        // get parent dialog window
        DialogWindow *dialogWindow = findParentComponentOfClass<DialogWindow>();

        if (dialogWindow != nullptr)
        {
            // old skin re-selected
            if (newSkinName == *currentSkinName)
            {
                // close dialog window (exit code 1)
                dialogWindow->exitModalState(1);
            }
            // new skin selected
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
    else if (button == &ButtonDefault)
    {
        // get first selected row index from list box
        int selectedRow = SkinList.getSelectedRow(0);

        // update default skin
        ListBoxModel.setDefault(selectedRow);

        // redraw list box
        SkinList.repaint();
    }
}


/// Create a list box model for a GenericWindowSkinContent.
///
GenericSkinListBoxModel::GenericSkinListBoxModel()
    : skinWildcard("*.skin", String::empty, "Skin files"),
      directoryThread("Skin directory scanner")
{
}


/// Fill list box model with all skins in a given directory.
///
/// @param skinDirectory directory containing the skins
///
void GenericSkinListBoxModel::fill(const File &skinDirectory)
{
    // search skin files in directory
    DirectoryContentsList skinFiles(&skinWildcard, directoryThread);
    skinFiles.setDirectory(skinDirectory, false, true);
    directoryThread.startThread();

    // locate file containing the default skin's name
    defaultSkinFile = skinDirectory.getChildFile("default_skin.ini");

    // make sure the file exists
    if (!defaultSkinFile.existsAsFile())
    {
        // create file
        DBG("[Skin] creating new \"default_skin.ini\" file");
        defaultSkinFile.create();

        // set "Default" as default skin as it comes with the plug-in
        // (uses Unicode encoding)
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
    for (int n = 0; n < skinFiles.getNumFiles(); ++n)
    {
        // get skin file
        File skinFile = skinFiles.getFile(n);

        // remove file extension
        String skinName = skinFile.getFileNameWithoutExtension();

        // add skin name to internal array
        skinNames.add(skinName);
    }
}


/// Get the number of skin names in the list box.
///
/// @return number of skin names
///
int GenericSkinListBoxModel::getNumRows()
{
    // return size of internal array
    return skinNames.size();
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
    return skinNames.indexOf(skinNameToLookFor);
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
    if (rowNumber < skinNames.size())
    {
        // return matching skin name from internal array
        return skinNames[rowNumber];
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
    if (rowNumber >= skinNames.size())
    {
        return;
    }

    // store name of default skin
    defaultSkinName = skinNames[rowNumber];

    // store default skin name in file "default_skin.ini" (uses
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
    g.drawText(skinName, 2, 0, width - 4, height,
               Justification::centredLeft, true);
}

// Local Variables:
// ispell-local-dictionary: "british"
// End:
