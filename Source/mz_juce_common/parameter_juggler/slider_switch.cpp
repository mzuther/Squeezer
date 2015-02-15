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

#include "slider_switch.h"


SliderSwitch::SliderSwitch(const String &componentName, ParameterJuggler *pParameters, int nParameterIndex) : Slider(componentName)
{
    pSwitch = dynamic_cast<WrappedParameterSwitch *>(pParameters->getWrappedParameter(nParameterIndex));
    jassert(pSwitch != nullptr);

    setRange(0.0f, 1.0f, pSwitch->getInterval());
    setSliderStyle(Slider::RotaryVerticalDrag);
    colourRotary = Colours::white;

    setDoubleClickReturnValue(true, pSwitch->getDefaultFloat());
}


SliderSwitch::~SliderSwitch()
{
}


void SliderSwitch::visibilityChanged()
{
    Slider::visibilityChanged();

    setColour(Slider::rotarySliderFillColourId, colourRotary);
    setColour(Slider::textBoxTextColourId, Colours::white);
    setColour(Slider::textBoxBackgroundColourId, Colours::darkgrey.darker(0.7f));
    setColour(Slider::textBoxOutlineColourId, Colours::darkgrey.darker(0.4f));
}


void SliderSwitch::resized()
{
    Slider::resized();

    int nWidth = getBounds().getWidth();
    setTextBoxStyle(Slider::TextBoxBelow, true, nWidth, 18);
}


void SliderSwitch::setSliderColour(const Colour &colour)
{
    colourRotary = colour;
    setColour(Slider::rotarySliderFillColourId, colourRotary);
}

float SliderSwitch::getRealFloat()
{
    return pSwitch->getRealFloat();
}


bool SliderSwitch::getBoolean()
{
    return pSwitch->getBoolean();
}


int SliderSwitch::getRealInteger()
{
    return pSwitch->getRealInteger();
}


double SliderSwitch::getValueFromText(const String &strText)
{
    return pSwitch->getFloatFromText(strText);
}


String SliderSwitch::getTextFromValue(double dValue)
{
    return pSwitch->getTextFromFloat((float) dValue);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
