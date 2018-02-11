/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2018 Martin Zuther (http://www.mzuther.de/)

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
WindowSkinContent::WindowSkinContent()
{
}


/// Static helper function to create a dialog window for selecting a
/// new GUI skin.
///
/// @param pluginEditor audio plug-in editor
///
/// @param currentSkinName name of the currently used skin (will
/// always be changed to reflect the current skin name)
///
/// @param skinDirectory directory containing the skins
///
/// @return created dialog window
///
DialogWindow *WindowSkinContent::createDialogWindow(
    AudioProcessorEditor *pluginEditor,
    String *currentSkinName,
    const File &skinDirectory)

{
    // prepare dialog window
    DialogWindow::LaunchOptions windowSkinLauncher;

    // create content component
    WindowSkinContent *contentComponent =
        new WindowSkinContent();

    contentComponent->initialise(currentSkinName,
                                 skinDirectory);

    // initialise dialog window settings
    windowSkinLauncher.dialogTitle = "Select skin";
    windowSkinLauncher.dialogBackgroundColour = Colours::white;
    windowSkinLauncher.content.setOwned(contentComponent);
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
/// @param currentSkinName name of the currently used skin (will
///        always be changed to reflect the current skin name)
///
/// @param skinDirectory directory containing the skins
///
void WindowSkinContent::initialise(
    String *currentSkinName,
    const File &skinDirectory)

{
    // store name of the currenty used skin
    currentSkinName_ = currentSkinName;

    // fill list box model with skin names
    listModel_.fill(skinDirectory);

    // initialise list box
    skinList_.setModel(&listModel_);
    addAndMakeVisible(skinList_);

    // disable multiple selections
    skinList_.setMultipleSelectionEnabled(false);

    // select current skin in list box
    int row = listModel_.getRow(*currentSkinName_);
    skinList_.selectRow(row);

    // initialise "select" button for selecting a new skin
    buttonSelect_.setButtonText("Select");
    addAndMakeVisible(buttonSelect_);
    buttonSelect_.addListener(this);

    // initialise "default" button for selecting a new default skin
    buttonDefault_.setButtonText("Default");
    addAndMakeVisible(buttonDefault_);
    buttonDefault_.addListener(this);

    // style and place the dialog window's components
    applySkin();
}


/// Style and place the dialog window's components.
///
void WindowSkinContent::applySkin()
{
    // style list box
    skinList_.setOutlineThickness(1);

    skinList_.setColour(
        ListBox::backgroundColourId,
        Colours::white);

    skinList_.setColour(
        ListBox::outlineColourId,
        Colours::grey);


    // style "select" button
    buttonSelect_.setColour(
        TextButton::buttonColourId,
        Colours::yellow);

    buttonSelect_.setColour(
        TextButton::textColourOffId,
        Colours::black);


    // initialise content component dimensions
    int width = 150;
    int height = 50;

    // calculate optimal height of list box
    int listBoxHeight = listModel_.getNumRows() * skinList_.getRowHeight() +
                        2;

    // add height of list box to height of content component
    height += listBoxHeight;

    // set component window dimensions
    setSize(width, height);


    // place components
    skinList_.setBounds(10, 10, width - 20, listBoxHeight);
    buttonSelect_.setBounds(10, height - 30, 60, 20);
    buttonDefault_.setBounds(width - 70, height - 30, 60, 20);
}


/// Called when a button is clicked.
///
/// @param button clicked button
///
void WindowSkinContent::buttonClicked(
    Button *button)

{
    // user has selected a skin
    if (button == &buttonSelect_)
    {
        // get first selected row index from list box
        int selectedRow = skinList_.getSelectedRow(0);

        // get selected skin name and update editor
        String newSkinName = listModel_.getSkinName(selectedRow);

        // get parent dialog window
        DialogWindow *dialogWindow = findParentComponentOfClass<DialogWindow>();

        if (dialogWindow != nullptr)
        {
            // old skin re-selected
            if (newSkinName == *currentSkinName_)
            {
                // close dialog window (exit code 1)
                dialogWindow->exitModalState(1);
            }
            // new skin selected
            else
            {
                // update skin name
                *currentSkinName_ = newSkinName;

                // close dialog window (exit code 2)
                dialogWindow->exitModalState(2);
            }
        }
    }
    // user has selected a default skin
    else if (button == &buttonDefault_)
    {
        // get first selected row index from list box
        int selectedRow = skinList_.getSelectedRow(0);

        // update default skin
        listModel_.setDefault(selectedRow);

        // redraw list box
        skinList_.repaint();
    }
}


/// Create a list box model for a WindowSkinContent.
///
SkinListBoxModel::SkinListBoxModel()
    : skinWildcard_("*.skin", String::empty, "Skin files"),
      directoryThread_("Skin directory scanner")
{
}


/// Fill list box model with all skins in a given directory.
///
/// @param skinDirectory directory containing the skins
///
void SkinListBoxModel::fill(
    const File &skinDirectory)

{
    // search skin files in directory
    DirectoryContentsList skinFiles(&skinWildcard_, directoryThread_);
    skinFiles.setDirectory(skinDirectory, false, true);
    directoryThread_.startThread();

    // locate file containing the default skin's name
    defaultSkinFile_ = skinDirectory.getChildFile("default_skin.ini");

    // make sure the file exists
    if (!defaultSkinFile_.existsAsFile())
    {
        // create file
        DBG("[Skin] creating new \"default_skin.ini\" file");
        defaultSkinFile_.create();

        // set "Default" as default skin as it comes with the plug-in
        // (uses Unicode encoding)
        defaultSkinFile_.replaceWithText("Default", true, true);
    }

    // load name of default skin from file
    defaultSkinName_ = defaultSkinFile_.loadFileAsString();

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

        // get file name without extension
        String skinName = skinFile.getFileNameWithoutExtension();

        // add skin name to internal array
        skinNames_.add(skinName);
    }
}


/// Get the number of skin names in the list box.
///
/// @return number of skin names
///
int SkinListBoxModel::getNumRows()
{
    // return size of internal array
    return skinNames_.size();
}


/// Get the index of the first row that matches the given skin name.
///
/// @param skinNameToLookFor skin name to look for
///
/// @return index of the first matching element
///
int SkinListBoxModel::getRow(
    const String &skinNameToLookFor)

{
    // search internal array for skin name
    return skinNames_.indexOf(skinNameToLookFor);
}


/// Return one of the skin names in the list box.
///
/// @param rowNumber zero-based row number to retrieve
///
/// @return skin name
///
const String SkinListBoxModel::getSkinName(
    int rowNumber)

{
    // check for valid index
    if (rowNumber < skinNames_.size())
    {
        // return matching skin name from internal array
        return skinNames_[rowNumber];
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
void SkinListBoxModel::setDefault(
    int rowNumber)

{
    // skip invalid index
    if (rowNumber >= skinNames_.size())
    {
        return;
    }

    // store name of default skin
    defaultSkinName_ = skinNames_[rowNumber];

    // store default skin name in file "default_skin.ini" (uses
    // Unicode encoding)
    defaultSkinFile_.replaceWithText(defaultSkinName_, true, true);
}


/// Draws a row of the list.
///
/// @param rowNumber index of row to draw
///
/// @param g graphics context
///
/// @param rowWidth row width
///
/// @param rowHeight row height
///
/// @param isRowSelected indicates whether row is currently selected
///
void SkinListBoxModel::paintListBoxItem(
    int rowNumber,
    Graphics &g,
    int rowWidth,
    int rowHeight,
    bool isRowSelected)

{
    // draw selected rows in light blue
    if (isRowSelected)
    {
        g.fillAll(Colours::lightblue);
    }

    // get skin name for current row
    String skinName = getSkinName(rowNumber);

    // draw default skin name in bold font
    if (skinName == defaultSkinName_)
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
    g.drawText(skinName,
               2, 0, rowWidth - 4, rowHeight,
               Justification::centredLeft,
               true);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
