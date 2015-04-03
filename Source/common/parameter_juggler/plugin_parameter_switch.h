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

#ifndef __PLUGIN_PARAMETER_SWITCH_H__
#define __PLUGIN_PARAMETER_SWITCH_H__

#include "JuceHeader.h"
#include "plugin_parameter.h"


//==============================================================================
/**
*/
class PluginParameterSwitch : virtual public PluginParameter
{
public:
    PluginParameterSwitch(bool save_from_deletion = false);
    ~PluginParameterSwitch();

    void addPreset(const float newRealValue, const String &newLabel);
    int getNumberOfSteps();

    float toRealFloat(float newValue);
    float toInternalFloat(float newRealValue);

    virtual void setDefaultRealFloat(float newRealValue, bool updateParameter) override;

    virtual void setFloat(float newValue) override;
    virtual void setRealFloat(float newRealValue) override;
    virtual void setText(const String &newValue) override;

    float getFloatFromText(const String &newValue);
    String getTextFromFloat(float newValue);

private:
    JUCE_LEAK_DETECTOR(PluginParameterSwitch);

    Array<float> arrRealValues;
    StringArray arrLabels;

    float stepSize;
};


#endif  // __PLUGIN_PARAMETER_SWITCH_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
