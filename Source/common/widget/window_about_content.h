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

#ifndef __FRUT_WIDGET_WINDOW_ABOUT_CONTENT_H__
#define __FRUT_WIDGET_WINDOW_ABOUT_CONTENT_H__


/// Dialog window for displaying version, copyright, license and so
/// on.
///
class WindowAboutContent :
    public Component,
    public ButtonListener
{
public:
    WindowAboutContent();

    static DialogWindow *createDialogWindow(AudioProcessorEditor *pluginEditor,
                                            int componentWidth,
                                            int componentHeight,
                                            const StringPairArray &chapters);

    virtual void buttonClicked(Button *button);

    virtual void applySkin();

    virtual void initialise(int componentWidth,
                            int componentHeight,
                            const StringPairArray &chapters);

protected:
    virtual void addChapters(const StringPairArray &chapters);

    TextEditor textEditor_;
    TextButton buttonClose_;
    ImageButton buttonLicense_;

private:
    JUCE_LEAK_DETECTOR(WindowAboutContent);
};


#endif  // __FRUT_WIDGET_WINDOW_ABOUT_CONTENT_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
