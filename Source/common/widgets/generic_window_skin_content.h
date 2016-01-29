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
class GenericSkinListBoxModel :
    public ListBoxModel
{
public:
    GenericSkinListBoxModel();

    virtual int getNumRows();
    virtual int getRow(const String &skinNameToLookFor);

    virtual void fill(const File &skinDirectory);

    virtual const String getSkinName(int rowNumber);
    virtual void setDefault(int rowNumber);

    virtual void paintListBoxItem(int rowNumber,
                                  Graphics &g,
                                  int rowWidth,
                                  int rowHeight,
                                  bool isRowSelected);

private:
    WildcardFileFilter skinWildcard_;
    TimeSliceThread directoryThread_;

    File defaultSkinFile_;
    String defaultSkinName_;
    StringArray skinNames_;

private:
    JUCE_LEAK_DETECTOR(GenericSkinListBoxModel);
};


/// Dialog window for selecting a GUI skin.
///
/// @see GenericSkinListBoxModel
///
class GenericWindowSkinContent :
    public Component,
    public ButtonListener
{
public:
    GenericWindowSkinContent();

    static DialogWindow *createDialogWindow(AudioProcessorEditor *pluginEditor,
                                            String *currentSkinName,
                                            const File &skinDirectory);

    virtual void buttonClicked(Button *button);

    virtual void applySkin();

    virtual void initialise(String *currentSkinName,
                            const File &skinDirectory);

protected:
    ListBox skinList_;
    GenericSkinListBoxModel listModel_;

    TextButton buttonSelect_;
    TextButton buttonDefault_;

    String *currentSkinName_;

private:
    JUCE_LEAK_DETECTOR(GenericWindowSkinContent);
};


#endif  // __GENERIC_WINDOW_SKIN_CONTENT_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
