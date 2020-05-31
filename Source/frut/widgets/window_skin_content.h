/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2020 Martin Zuther (http://www.mzuther.de/)

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

#ifndef FRUT_WIDGETS_WINDOW_SKIN_CONTENT_H
#define FRUT_WIDGETS_WINDOW_SKIN_CONTENT_H

namespace frut
{

namespace skin
{
class Skin;
}


namespace widgets
{

/// List box model that lists all available GUI skins.
///
/// @see WindowSkinContent
///
class SkinListBoxModel :
    public ListBoxModel
{
public:
    SkinListBoxModel();

    virtual int getNumRows();
    virtual int getRow(const String &skinNameToLookFor);

    virtual void fill(frut::skin::Skin *skin);

    virtual const String getSkinName(int rowNumber);

    virtual void paintListBoxItem(int rowNumber,
                                  Graphics &g,
                                  int rowWidth,
                                  int rowHeight,
                                  bool isRowSelected);

private:
    frut::skin::Skin *skin_;

    WildcardFileFilter skinWildcard_;
    TimeSliceThread directoryThread_;

    StringArray skinNames_;

private:
    JUCE_LEAK_DETECTOR(SkinListBoxModel);
};


/// Dialog window for selecting a GUI skin.
///
/// @see SkinListBoxModel
///
class WindowSkinContent :
    public Component,
    public Button::Listener
{
public:
    WindowSkinContent();

    static DialogWindow *createDialogWindow(AudioProcessorEditor *pluginEditor,
                                            frut::skin::Skin *skin);

    virtual void buttonClicked(Button *button);

    virtual void applySkin();

    virtual void initialise(frut::skin::Skin *skin);

protected:
    frut::skin::Skin *skin_;

    ListBox skinList_;
    SkinListBoxModel listModel_;

    TextButton buttonSelect_;

private:
    JUCE_LEAK_DETECTOR(WindowSkinContent);
};

}
}

#endif  // FRUT_WIDGETS_WINDOW_SKIN_CONTENT_H
