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

#ifndef FRUT_WIDGETS_SLIDER_COMBINED_H
#define FRUT_WIDGETS_SLIDER_COMBINED_H

namespace frut
{
namespace widgets
{

class SliderCombined :
    virtual public FrutSlider
{
public:
    SliderCombined(parameters::Juggler *pParameters, int nParameterIndex, int nParameterIndexSwitch);

    void visibilityChanged();
    void resized();
    virtual void setSliderColour(const Colour &colour);

    void addButtonListener(Button::Listener *newListener);
    void removeListener(Button::Listener *listener);

    void updateMode();

    float getRealFloat();
    bool getBoolean();
    int getRealInteger();

    double getValueFromText(const String &strText);
    String getTextFromValue(double dValue);
private:
    JUCE_LEAK_DETECTOR(SliderCombined);

    ScopedPointer<DrawableButton> toggleButton;
    Colour colourRotary;

    parameters::ParCombined *pCombined;
    parameters::ParBoolean *pModeSwitch;
};

}
}

#endif  // FRUT_WIDGETS_SLIDER_COMBINED_H
