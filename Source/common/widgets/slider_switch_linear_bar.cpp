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

#include "slider_switch_linear_bar.h"


SliderSwitchLinearBar::SliderSwitchLinearBar(const String &componentName, ParameterJuggler *pParameters, int nParameterIndex) : Slider(componentName)
{
    pSwitch = dynamic_cast<PluginParameterSwitch *>(pParameters->getPluginParameter(nParameterIndex));
    jassert(pSwitch != nullptr);

    setRange(0.0f, 1.0f, 1.0f / pSwitch->getNumberOfSteps());
    setSliderStyle(Slider::LinearBar);
    colourThumb = Colours::white;

    setTextBoxIsEditable(false);
    setDoubleClickReturnValue(true, pSwitch->getDefaultFloat());
}


SliderSwitchLinearBar::~SliderSwitchLinearBar()
{
}


void SliderSwitchLinearBar::visibilityChanged()
{
    Slider::visibilityChanged();

    setColour(Slider::thumbColourId, colourThumb);
    setColour(Slider::textBoxTextColourId, Colours::white);
    setColour(Slider::backgroundColourId, Colours::darkgrey.darker(0.7f));
    setColour(Slider::textBoxOutlineColourId, Colours::darkgrey.darker(0.4f));
}


void SliderSwitchLinearBar::setSliderColour(const Colour &colour)
{
    colourThumb = colour;
    setColour(Slider::thumbColourId, colourThumb);
}

float SliderSwitchLinearBar::getRealFloat()
{
    return pSwitch->getRealFloat();
}


bool SliderSwitchLinearBar::getBoolean()
{
    return pSwitch->getBoolean();
}


int SliderSwitchLinearBar::getRealInteger()
{
    return pSwitch->getRealInteger();
}


double SliderSwitchLinearBar::getValueFromText(const String &strText)
{
    return pSwitch->getFloatFromText(strText);
}


String SliderSwitchLinearBar::getTextFromValue(double dValue)
{
    return pSwitch->getTextFromFloat((float) dValue);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
