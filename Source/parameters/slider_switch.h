/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013-2015 Martin Zuther (http://www.mzuther.de/)

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

#ifndef __SLIDER_SWITCH_H__
#define __SLIDER_SWITCH_H__

#include "JuceHeader.h"
#include "plugin_parameters.h"
#include "wrapped_parameter_switch.h"


//==============================================================================
/**
*/
class SliderSwitch : public Slider
{
public:
    SliderSwitch(const String &componentName, PluginParameters *pParameters, int nParameterIndex);
    ~SliderSwitch();

    void visibilityChanged();
    void resized();
    void setSliderColour(const Colour &colour);

    float getRealFloat();
    bool getBoolean();
    int getRealInteger();

    double getValueFromText(const String &strText);
    String getTextFromValue(double dValue);
private:
    JUCE_LEAK_DETECTOR(SliderSwitch);

    Colour colourRotary;
    WrappedParameterSwitch *pSwitch;
};


#endif  // __SLIDER_SWITCH_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
