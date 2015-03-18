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

#ifndef __GENERIC_WINDOW_SKIN_H__
#define __GENERIC_WINDOW_SKIN_H__

#include "JuceHeader.h"


/// List box model that lists all available GUI skins.
///
/// @see GenericWindowSkin
///
class GenericSkinListBoxModel : public ListBoxModel
{
public:
    GenericSkinListBoxModel();
    ~GenericSkinListBoxModel();

    void fill(const File &fileSkinDirectory);

    int getNumRows();
    int getRow(const String &skinNameToLookFor);
    const String getSkinName(int rowNumber);
    void setDefault(int rowNumber);

    void paintListBoxItem(int rowNumber, Graphics &g, int width, int height, bool isRowSelected);

private:
    WildcardFileFilter skinWildcard;
    TimeSliceThread directoryThread;

    File defaultSkinFile;
    String defaultSkinName;
    StringArray arrSkinNames;
};


/// Dialog window for selecting a GUI skin.
///
/// @see GenericSkinListBoxModel
///
class GenericWindowSkin : public DocumentWindow, ButtonListener
{
public:
    GenericWindowSkin(Component *editorWindow, const File &currentSkinFile);
    ~GenericWindowSkin();

    void buttonClicked(Button *button);
    void closeButtonPressed();

    const String getSelectedSkinName();

private:
    JUCE_LEAK_DETECTOR(GenericWindowSkin);

    Component contentComponent;

    ListBox listBox;
    GenericSkinListBoxModel listBoxModel;

    TextButton buttonSelect;
    TextButton buttonDefault;

    String currentSkinName;
};


#endif  // __GENERIC_WINDOW_SKIN_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
