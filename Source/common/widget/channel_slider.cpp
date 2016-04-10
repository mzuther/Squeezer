/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
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


/// Create a new channel slider.
///
ChannelSlider::ChannelSlider()
{
    // initialise number of channels
    setNumberOfChannels(0);

    // format slider
    setSliderStyle(Slider::IncDecButtons);
    setTextBoxStyle(Slider::TextBoxLeft, true, 30, 20);
    setIncDecButtonsMode(Slider::incDecButtonsNotDraggable);
}


/// Get number of availabe audio channels.
///
/// @return number of audio channels
///
int ChannelSlider::getNumberOfChannels()
{
    return numberOfChannels_;
}


/// Set number of availabe audio channels.
///
/// @param numberOfChannels number of audio channels
///
void ChannelSlider::setNumberOfChannels(
    int numberOfChannels)

{
    numberOfChannels_ = numberOfChannels;

    // channels start with index 0, -1 designates "all channels"
    setRange(-1.0, numberOfChannels_ - 1, 1.0);
    setValue(-1.0, sendNotificationAsync);
}


/// Get selected channel as double.  Channel 1 is 0.0, channel 2 is
/// 1.0 and so one.  A value of -1.0 designates "all channels".
///
/// @return index of selected audio channel
///
double ChannelSlider::getDouble()
{
    double sliderRange = getMaximum() - getMinimum();
    double channelIndex = (getValue() - getMinimum()) / sliderRange;

    return channelIndex;
}


/// Get selected channel as float.  Channel 1 is 0.0, channel 2 is 1.0
/// and so one.  A value of -1.0 designates "all channels".
///
/// @return index of selected audio channel
///
float ChannelSlider::getFloat()
{
    return static_cast<float>(getDouble());
}


/// When the user enters something into the text-entry box, this
/// method is called to convert it to a value.  Channel "1" is 0.0,
/// channel "2" is 1.0 and so one.  "All" channels are converted to
/// -1.0.
///
/// @param inputString string to be converted
///
/// @return "name" of selected audio channel
///
double ChannelSlider::getValueFromText(
    const String &inputString)

{
    // -1 designates "all channels"
    if (inputString == "All")
    {
        return -1.0;
    }
    // convert string to float
    else
    {
        return inputString.getDoubleValue() - 1.0;
    }
}


/// Turns the slider's current value into a text string.  If a suffix
/// string has been set using setTextValueSuffix(), this will be
/// appended to the text.  Channel "1" is 0.0, channel "2" is 1.0 and
/// so one.  Values below zero are converted to "All" channels.
///
/// @param inputValue value to be converted
///
/// @return "name" of selected audio channel
///
String ChannelSlider::getTextFromValue(
    double inputValue)

{
    // -1 designates "all channels"
    if (inputValue < 0)
    {
        return String("All");
    }
    // convert double to string
    else
    {
        int channelIndex = math::SimpleMath::round(static_cast<float>(inputValue)) + 1;

        return String(channelIndex);
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
