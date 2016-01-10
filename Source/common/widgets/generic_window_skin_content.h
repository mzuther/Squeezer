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

#ifndef __GENERIC_WINDOW_SKIN_CONTENT_H__
#define __GENERIC_WINDOW_SKIN_CONTENT_H__

#include "JuceHeader.h"


/// List box model that lists all available GUI skins.
///
/// @see GenericWindowSkinContent
///
class GenericSkinListBoxModel : public ListBoxModel
{
public:
    GenericSkinListBoxModel();

    void fill(const File &skinDirectory);

    int getNumRows();
    int getRow(const String &skinNameToLookFor);
    const String getSkinName(int rowNumber);
    void setDefault(int rowNumber);

    void paintListBoxItem(int rowNumber, Graphics &g, int width, int height,
                          bool isRowSelected);

private:
    WildcardFileFilter skinWildcard;
    TimeSliceThread directoryThread;

    File defaultSkinFile;
    String defaultSkinName;
    StringArray skinNames;
};


/// Dialog window for selecting a GUI skin.
///
/// @see GenericSkinListBoxModel
///
class GenericWindowSkinContent : public Component, public ButtonListener
{
public:
    GenericWindowSkinContent();

    static DialogWindow *createDialogWindow(
        AudioProcessorEditor *pluginEditor, String *skinName,
        const File &skinDirectory);

    virtual void buttonClicked(Button *button);

    virtual void applySkin();
    virtual void initialize(String *skinName, const File &skinDirectory);

private:
    JUCE_LEAK_DETECTOR(GenericWindowSkinContent);

    ListBox SkinList;
    GenericSkinListBoxModel ListBoxModel;

    TextButton ButtonSelect;
    TextButton ButtonDefault;

    String *currentSkinName;
};


#endif  // __GENERIC_WINDOW_SKIN_CONTENT_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
