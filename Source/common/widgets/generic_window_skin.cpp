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


GenericWindowSkin::GenericWindowSkin(Component *pEditorWindow, const File &fileSkin)
    : DocumentWindow("Select skin", Colours::white, 0, true)
    // create new window child
{
    int nWidth = 150;
    int nHeight = 0;

    // empty windows are boring, so let's prepare a space for some
    // window components
    contentComponent = new Component("Window Area");
    setContentOwned(contentComponent, false);

    // prepare a list box and its model
    pListBox = new ListBox("ListBox Skins", nullptr);
    pListBoxModel = new GenericSkinListBoxModel(fileSkin.getParentDirectory());
    pListBox->setModel(pListBoxModel);

    // calculate and set necessary height for list box
    int nHeightListBox = pListBoxModel->getNumRows() * pListBox->getRowHeight() + 2;
    pListBox->setBounds(10, 10, nWidth - 20, nHeightListBox);

    // set look of list box
    pListBox->setColour(ListBox::outlineColourId, Colours::grey);
    pListBox->setOutlineThickness(1);

    // disable multiple selections
    pListBox->setMultipleSelectionEnabled(false);

    // select current skin in list box
    strSkinName = fileSkin.getFileNameWithoutExtension();
    pListBox->selectRow(pListBoxModel->getRow(strSkinName));

    // display list box
    contentComponent->addAndMakeVisible(pListBox);

    // calculate window height from height of list box
    nHeight = nHeightListBox + 50;

    // create and position an "select" button
    ButtonSelect = new TextButton("Select");
    ButtonSelect->setBounds(10, nHeight - 30, 60, 20);
    ButtonSelect->setColour(TextButton::buttonColourId, Colours::red);
    ButtonSelect->setColour(TextButton::buttonOnColourId, Colours::red);

    // add "skin" window as button listener and display the button
    ButtonSelect->addListener(this);
    contentComponent->addAndMakeVisible(ButtonSelect);

    // create and position an "default" button
    ButtonDefault = new TextButton("Default");
    ButtonDefault->setBounds(nWidth - 70, nHeight - 30, 60, 20);
    ButtonDefault->setColour(TextButton::buttonColourId, Colours::yellow);
    ButtonDefault->setColour(TextButton::buttonOnColourId, Colours::yellow);

    // add "skin" window as button listener and display the button
    ButtonDefault->addListener(this);
    contentComponent->addAndMakeVisible(ButtonDefault);

    // set window dimensions to those passed to the function ...
    setSize(nWidth, nHeight + getTitleBarHeight());

    // ... center window on editor ...
    centreAroundComponent(pEditorWindow, getWidth(), getHeight());

    // ... and keep the new window on top
    setAlwaysOnTop(true);

    // this window does not have any transparent areas (increases
    // performance on redrawing)
    setOpaque(true);

    // finally, display window
    setVisible(true);
}


GenericWindowSkin::~GenericWindowSkin()
{
    delete pListBoxModel;
    pListBoxModel = nullptr;

    // delete all children of the window; "contentComponent" will be
    // deleted by the base class, so please leave it alone!
    contentComponent->deleteAllChildren();
}


void GenericWindowSkin::buttonClicked(Button *button)
{
    // find out which button has been clicked
    if (button == ButtonSelect)
    {
        // close window by making it invisible
        setVisible(false);
    }
    else if (button == ButtonDefault)
    {
        // get selected row from list box
        int nSelectedRow = pListBox->getSelectedRow(0);

        // update default skin
        pListBoxModel->setDefault(nSelectedRow);

        // redraw list box
        pListBox->repaint();
    }
}


const String &GenericWindowSkin::getSelectedString()
{
    // get selected row from list box
    int nSelectedRow = pListBox->getSelectedRow(0);

    // get and return selected value
    return pListBoxModel->getValue(nSelectedRow);
}


GenericSkinListBoxModel::GenericSkinListBoxModel(const File &fileSkinDirectory)
    : skinWildcard("*.skin", String::empty, "skin files"),
      directoryThread("skin directory scanner")
{
    // set up scanner for skin directory
    DirectoryContentsList skinFiles(&skinWildcard, directoryThread);
    skinFiles.setDirectory(fileSkinDirectory, false, true);
    directoryThread.startThread();

    // load name of default skin
    fileDefaultSkin = fileSkinDirectory.getChildFile("default_skin.ini");
    strDefaultSkin = fileDefaultSkin.loadFileAsString();

    // wait for scan of directory
    while (skinFiles.isStillLoading())
    {
        // keep Visual C++ from optimising (and thus breaking) the loop
        Thread::sleep(10);
    }

    // add found skins to list box
    for (int n = 0; n < skinFiles.getNumFiles(); n++)
    {
        File fileSkin = skinFiles.getFile(n);
        strValues.add(fileSkin.getFileNameWithoutExtension());
    }
}


GenericSkinListBoxModel::~GenericSkinListBoxModel()
{
}


int GenericSkinListBoxModel::getNumRows()
{
    return strValues.size();
}


int GenericSkinListBoxModel::getRow(const String &strQuery)
{
    return strValues.indexOf(strQuery);
}


const String &GenericSkinListBoxModel::getValue(int nRow)
{
    return strValues[nRow];
}


void GenericSkinListBoxModel::setDefault(int nRow)
{
    // update internal note to default skin
    strDefaultSkin = strValues[nRow];

    // update file to default skin
    fileDefaultSkin.replaceWithText(strDefaultSkin, true, true);
}


void GenericSkinListBoxModel::paintListBoxItem(int rowNumber, Graphics &g, int width, int height, bool rowIsSelected)
{
    // draw selected rows in light blue
    if (rowIsSelected)
    {
        g.fillAll(Colours::lightblue);
    }

    // get value of list item
    String strListItem = getValue(rowNumber);

    // draw default skin in bold font
    if (strListItem == strDefaultSkin)
    {
        g.setFont(Font(14.0f, Font::bold));
    }
    else
    {
        g.setFont(Font(14.0f, Font::plain));
    }

    // render row text
    g.setColour(Colours::black);
    g.drawText(strListItem, 2, 0, width - 4, height, Justification::centredLeft, true);
}

// Local Variables:
// ispell-local-dictionary: "british"
// End:
