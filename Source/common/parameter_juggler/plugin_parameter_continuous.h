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

#ifndef __PLUGIN_PARAMETER_CONTINUOUS_H__
#define __PLUGIN_PARAMETER_CONTINUOUS_H__

#include "JuceHeader.h"
#include "plugin_parameter.h"


class PluginParameterContinuous : virtual public PluginParameter
{
public:
    PluginParameterContinuous(float real_minimum, float real_maximum, float resolution, float scaling_factor, int decimal_places, bool save_from_deletion = false);
    ~PluginParameterContinuous();

    int getNumberOfSteps();

    virtual void setDefaultRealFloat(float newRealValue, bool updateParameter) override;

    virtual void setFloat(float newValue) override;
    virtual void setRealFloat(float newRealValue) override;

    virtual void setText(const String &newValue) override;
    void setSuffix(const String &newSuffix);

    float getFloatFromText(const String &newValue);
    String getTextFromFloat(float newValue);

private:
    JUCE_LEAK_DETECTOR(PluginParameterContinuous);

    float toRealFloat(float newValue);
    float toInternalFloat(float newRealValue);

    float realMinimum;
    float realMaximum;
    float realRange;

    float stepSize;
    int decimalPlaces;
    String valueSuffix;

    bool isNonlinear;
    float scalingFactor;
    float scalingConstantFactor;
};


#endif  // __PLUGIN_PARAMETER_CONTINUOUS_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
