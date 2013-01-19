/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013 Martin Zuther (http://www.mzuther.de/)

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

#include "slider_continuous.h"


SliderContinuous::SliderContinuous(const String& componentName, int nWidth, SqueezerPluginParameters* parameters, int parameter_index) : Slider(componentName)
{
    pPluginParameters = parameters;
    nParameterIndex = parameter_index;

    setRange(0.0f, 1.0f, pPluginParameters->getInterval(nParameterIndex));
    setSliderStyle(Slider::RotaryVerticalDrag);
    setTextBoxStyle(Slider::TextBoxBelow, true, nWidth, 18);

    setColour(Slider::rotarySliderFillColourId, Colours::white);
    setColour(Slider::textBoxTextColourId, Colours::white);
    setColour(Slider::textBoxBackgroundColourId, Colours::darkgrey.darker(0.7f));
    setColour(Slider::textBoxOutlineColourId, Colours::darkgrey.darker(0.4f));

    setDoubleClickReturnValue(true, pPluginParameters->getDefaultFloat(nParameterIndex));
}


SliderContinuous::~SliderContinuous()
{
}


void SliderContinuous::setSliderColour(const Colour& colour)
{
    setColour(Slider::rotarySliderFillColourId, colour);
}


double SliderContinuous::getValueFromText(const String& strText)
{
    return pPluginParameters->getFloatFromText(nParameterIndex, strText);
}


String SliderContinuous::getTextFromValue(double dValue)
{
    return pPluginParameters->getTextFromFloat(nParameterIndex, (float) dValue);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
