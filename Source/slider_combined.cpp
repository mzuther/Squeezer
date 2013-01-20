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

#include "slider_combined.h"


SliderCombined::SliderCombined(const String& componentName, SqueezerPluginParameters* pParameters, int nParameterIndex) : Slider(componentName)
{
    pCombined = dynamic_cast<WrappedParameterCombined*>(pParameters->getWrappedParameter(nParameterIndex));
    jassert(pCombined != NULL);

    setRange(0.0f, 1.0f, pCombined->getInterval());
    setSliderStyle(Slider::RotaryVerticalDrag);

    setColour(Slider::rotarySliderFillColourId, Colours::white);
    setColour(Slider::textBoxTextColourId, Colours::white);
    setColour(Slider::textBoxBackgroundColourId, Colours::darkgrey.darker(0.7f));
    setColour(Slider::textBoxOutlineColourId, Colours::darkgrey.darker(0.4f));

    setDoubleClickReturnValue(true, pCombined->getDefaultFloat());
}


SliderCombined::~SliderCombined()
{
}


void SliderCombined::resized()
{
    Slider::resized();

    int nWidth = getBounds().getWidth();
    int nSwitchWidth = 8;
    int nSwitchX = nWidth - nSwitchWidth - 1;
    rectSwitchPosition.setBounds(nSwitchX, 1, nSwitchWidth, nSwitchWidth);

    setTextBoxStyle(Slider::TextBoxBelow, true, nWidth, 18);
}


void SliderCombined::paintOverChildren(Graphics& g)
{
    Slider::paintOverChildren(g);

    if (pCombined->getMode())
    {
        g.setColour(Colours::red.darker(0.5f));
    }
    else
    {
        g.setColour(Colours::red.darker(5.0f));
    }

    g.fillEllipse(rectSwitchPosition.getX(), rectSwitchPosition.getY(), rectSwitchPosition.getWidth(), rectSwitchPosition.getHeight());

    g.setColour(Colours::black);
    g.drawEllipse(rectSwitchPosition.getX(), rectSwitchPosition.getY(), rectSwitchPosition.getWidth(), rectSwitchPosition.getHeight(), 1.0f);
}


void SliderCombined::setSliderColour(const Colour& colour)
{
    setColour(Slider::rotarySliderFillColourId, colour);
}


void SliderCombined::mouseDown(const MouseEvent& e)
{
    if (rectSwitchPosition.contains(e.getMouseDownPosition()))
    {
        if (isEnabled())
        {
            toggleMode();
        }
    }
    else
    {
        Slider::mouseDown(e);
    }
}


void SliderCombined::toggleMode()
{
    pCombined->toggleMode();

    setRange(0.0f, 1.0f, pCombined->getInterval());
    setValue(pCombined->getFloat());

    repaint();
}


double SliderCombined::getValueFromText(const String& strText)
{
    return pCombined->getFloatFromText(strText);
}


String SliderCombined::getTextFromValue(double dValue)
{
    return pCombined->getTextFromFloat((float) dValue);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
