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

#ifndef FRUT_WIDGETS_WINDOW_VALIDATION_CONTENT_H
#define FRUT_WIDGETS_WINDOW_VALIDATION_CONTENT_H

namespace frut
{
namespace widgets
{

/// Dialog window for validation settings.
///
class WindowValidationContent :
   public Component,
   public Button::Listener
{
public:
   WindowValidationContent();

   virtual void buttonClicked( Button* button ) override;
   virtual void closeButtonPressed();

   virtual void styleButton( ToggleButton& button );
   virtual void applySkin();

   virtual void initialise( int componentWidth,
                            int componentHeight,
                            int numberOfInputChannels,
                            int sampleRate,
                            int selectedChannel,
                            const File& validationFile );

   /// Select a new audio file for validation.
   ///
   /// @param validationFile audio file for validation
   ///
   virtual void selectValidationFile( const File& validationFile ) = 0;

protected:
   File validationFile_;

   Label labelFileSelection_;
   Label labelSampleRate_;
   Label labelSampleRateValue_;

   TextButton buttonFileSelection_;
   TextButton buttonValidation_;
   TextButton buttonCancel_;

   Label labelSelectedChannel;
   widgets::ChannelSlider sliderSelectChannel_;

private:
   JUCE_LEAK_DETECTOR( WindowValidationContent );
};

}
}

#endif  // FRUT_WIDGETS_WINDOW_VALIDATION_CONTENT_H
