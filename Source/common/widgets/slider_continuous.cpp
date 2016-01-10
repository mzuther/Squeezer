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

#include "slider_continuous.h"


SliderContinuous::SliderContinuous(const String &componentName, ParameterJuggler *pParameters, int nParameterIndex) : Slider(componentName)
{
    pContinuous = dynamic_cast<PluginParameterContinuous *>(pParameters->getPluginParameter(nParameterIndex));
    jassert(pContinuous != nullptr);

    setRange(0.0f, 1.0f, pContinuous->getStepSize());
    setSliderStyle(Slider::RotaryVerticalDrag);
    colourRotary = Colours::white;

    setDoubleClickReturnValue(true, pContinuous->getDefaultFloat());
}


void SliderContinuous::visibilityChanged()
{
    Slider::visibilityChanged();

    setColour(Slider::rotarySliderFillColourId, colourRotary);
    setColour(Slider::textBoxTextColourId, Colours::white);
    setColour(Slider::textBoxBackgroundColourId, Colours::darkgrey.darker(0.7f));
    setColour(Slider::textBoxOutlineColourId, Colours::darkgrey.darker(0.4f));
}


void SliderContinuous::resized()
{
    Slider::resized();

    int nWidth = getBounds().getWidth();
    setTextBoxStyle(Slider::TextBoxBelow, true, nWidth, 18);
}


void SliderContinuous::setSliderColour(const Colour &colour)
{
    colourRotary = colour;
    setColour(Slider::rotarySliderFillColourId, colourRotary);
}


float SliderContinuous::getRealFloat()
{
    return pContinuous->getRealFloat();
}


bool SliderContinuous::getBoolean()
{
    return pContinuous->getBoolean();
}


int SliderContinuous::getRealInteger()
{
    return pContinuous->getRealInteger();
}


double SliderContinuous::getValueFromText(const String &strText)
{
    return pContinuous->getFloatFromText(strText);
}


String SliderContinuous::getTextFromValue(double dValue)
{
    return pContinuous->getTextFromFloat((float) dValue);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
