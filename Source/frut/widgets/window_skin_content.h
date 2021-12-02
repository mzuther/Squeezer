/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2021 Martin Zuther (http://www.mzuther.de/)

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

namespace frut::widgets
{

/// Dialog window for selecting a GUI skin.
///
class WindowSkinContent :
   public Component,
   public Button::Listener
{
public:
   WindowSkinContent();

   static DialogWindow* createDialogWindow(
      AudioProcessorEditor* pluginEditor,
      int scale );

   virtual void initialise( int scale );
   virtual void applySkin();
   virtual void buttonClicked( Button* button ) override;

protected:
   juce::Grid grid_;

   TextButton button100_;
   TextButton button125_;
   TextButton button150_;
   TextButton button200_;
   TextButton button250_;
   TextButton button300_;

private:
   JUCE_LEAK_DETECTOR( WindowSkinContent );
};

} // namespace

#endif  // FRUT_WIDGETS_WINDOW_SKIN_CONTENT_H
