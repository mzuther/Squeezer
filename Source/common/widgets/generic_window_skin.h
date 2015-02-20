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

class GenericSkinListBoxModel;


class GenericWindowSkin : public DocumentWindow, ButtonListener
{
public:
    GenericWindowSkin(Component *pEditorWindow, const File &fileSkin);
    ~GenericWindowSkin();

    void buttonClicked(Button *button);
    void closeButtonPressed();

    const String &getSelectedString();

private:
    JUCE_LEAK_DETECTOR(GenericWindowSkin);

    ScopedPointer<Component> contentComponent;

    ScopedPointer<ListBox> listBox;
    ScopedPointer<GenericSkinListBoxModel> listBoxModel;

    ScopedPointer<TextButton> ButtonSelect;
    ScopedPointer<TextButton> ButtonDefault;

    String strSkinName;
};


class GenericSkinListBoxModel : public ListBoxModel
{
public:
    GenericSkinListBoxModel(const File &fileSkinDirectory);
    ~GenericSkinListBoxModel();

    int getNumRows();
    int getRow(const String &strQuery);
    const String &getValue(int nRow);
    void setDefault(int nRow);

    void paintListBoxItem(int rowNumber, Graphics &g, int width, int height, bool rowIsSelected);

private:
    WildcardFileFilter skinWildcard;
    TimeSliceThread directoryThread;

    File fileDefaultSkin;
    String strDefaultSkin;
    StringArray strValues;
};


#endif  // __GENERIC_WINDOW_SKIN_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
