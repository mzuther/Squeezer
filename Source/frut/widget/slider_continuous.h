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

#pragma once


class SliderContinuous :
    virtual public FrutSlider
{
public:
    SliderContinuous(parameter::Juggler *pParameters, int parameterIndex);

    virtual void visibilityChanged();
    virtual void resized();
    virtual void setSliderColour(const Colour &colour);

    float getRealFloat();
    bool getBoolean();
    int getRealInteger();

    virtual double getValueFromText(const String &strText);
    virtual String getTextFromValue(double dValue);

protected:
    Colour colourRotary_;
    parameter::ParContinuous *parameter_;

private:
    JUCE_LEAK_DETECTOR(SliderContinuous);
};
