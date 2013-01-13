/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose compressor with a touch of lemon

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

#ifndef __SLIDER_SWITCH_H__
#define __SLIDER_SWITCH_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "plugin_parameters_squeezer.h"
#include "wrapped_parameter_switch.h"


//==============================================================================
/**
*/
class SliderSwitch : public Slider
{
public:
    SliderSwitch(const String& componentName, int nWidth, SqueezerPluginParameters* parameters, int parameter_index);
    ~SliderSwitch();

    void setSliderColour(const Colour& colour);

    double getValueFromText(const String& strText);
    String getTextFromValue(double dValue);
private:
    JUCE_LEAK_DETECTOR(SliderSwitch);


    SqueezerPluginParameters* pPluginParameters;
    int nParameterIndex;
};


#endif  // __SLIDER_SWITCH_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
