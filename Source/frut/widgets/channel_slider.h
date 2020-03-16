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

#ifndef FRUT_WIDGETS_CHANNEL_SLIDER_H
#define FRUT_WIDGETS_CHANNEL_SLIDER_H

namespace frut
{
namespace widgets
{

/// Slider component for selecting an audio channel.  This component
/// can either select a distinct channel or all channels at once.
///
class ChannelSlider :
    public Slider
{
public:
    ChannelSlider();

    int getNumberOfChannels();
    void setNumberOfChannels(int numberOfChannels);

    double getDouble();
    float getFloat();

    virtual double getValueFromText(const String &inputString);
    virtual String getTextFromValue(double inputValue);

protected:
    int numberOfChannels_;

private:
    JUCE_LEAK_DETECTOR(ChannelSlider);
};

}
}

#endif  // FRUT_WIDGETS_CHANNEL_SLIDER_H
