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

#include "plugin_parameter_switch.h"


// The methods of this class may be called on the audio thread, so
// they are absolutely time-critical!

/// Create a stepped parameter that can be filled with arbitrary
/// preset values.  **Internal** values simply range from 0.0 to 1.0.
///
/// @param save_from_deletion should parameter be spared from deletion
///        in destructor of ParameterJuggler?
///
PluginParameterSwitch::PluginParameterSwitch(bool save_from_deletion)
{
    // initialise values (invalid because parameter contains no
    // values)
    value = -1.0f;
    realValue = -1.0f;

    // initialise default values (invalid because parameter contains
    // no values)
    defaultValue = -1.0f;
    defaultRealValue = -1.0f;

    // internal spacing between values (invalid because parameter
    // contains no values)
    stepSize = -1.0f;

    // may parameter be deleted?
    doNotDelete = save_from_deletion;

    // mark parameter as changed
    setChangeFlag();
}


/// Add a preset value and a corresponding string representation.
/// **This function must be called at least twice for the parameter to
/// work as expected.**
///
/// @param newRealValue new real value
///
/// @param newLabel string representation
///
void PluginParameterSwitch::addPreset(const float newRealValue, const String &newLabel)
{
    // store real value in array
    arrRealValues.add(newRealValue);

    // store label in array
    arrLabels.add(newLabel);

    // this is the first preset value we have added
    if (arrRealValues.size() == 1)
    {
        // update default value and value
        setDefaultRealFloat(newRealValue, true);

        // mark parameter as changed
        setChangeFlag();
    }
    // prevent division by zero
    else
    {
        // update step size
        stepSize = 1.0f / (arrLabels.size() - 1.0f);
    }
}


/// Get number of discrete parameter values.
///
/// @return number of discrete parameter values
///
int PluginParameterSwitch::getNumberOfSteps()
{
    return arrLabels.size();
}


/// Get **internal** step size of parameter values.
///
/// @return step size
///
float PluginParameterSwitch::getStepSize()
{
    return 1.0f / float(getNumberOfSteps() - 1);
}


/// Convert **internal** parameter value to **real** value.
///
/// @param newValue **internal** parameter value
///
/// @return **real** parameter value
///
float PluginParameterSwitch::toRealFloat(float newValue)
{
    // confine new value to internal parameter range
    if (newValue < 0.0f)
    {
        newValue = 0.0f;
    }
    else if (newValue > 1.0f)
    {
        newValue = 1.0f;
    }

    // find matching index in array with real values
    int selectedIndex = MZ_Juce_Common::round(newValue / stepSize);

    // return real value
    return arrRealValues[selectedIndex];
}


/// Convert **real** parameter value to **internal** value.
///
/// @param newRealValue **real** parameter value
///
/// @return **internal** parameter value
///
float PluginParameterSwitch::toInternalFloat(float newRealValue)
{
    // try to find new value in array
    int selectedIndex = arrRealValues.indexOf(newRealValue);

    // new value not found, find nearest stored value
    if (selectedIndex < 0)
    {
        // reset index
        selectedIndex = 0;

        // get difference between value and first stored value
        float oldDifference = fabs(newRealValue - arrRealValues[selectedIndex]);

        // loop over stored values
        for (int currentIndex = 1; currentIndex < arrRealValues.size(); ++currentIndex)
        {
            // get difference between value and current stored value
            float newDifference = fabs(newRealValue - arrRealValues[currentIndex]);

            // found smaller difference
            if (newDifference < oldDifference)
            {
                // store new index and difference
                selectedIndex = currentIndex;
                oldDifference = newDifference;
            }
        }
    }

    // convert index to internal value
    float newValue = selectedIndex * stepSize;

    // return internal value
    return newValue;
}


/// Set **real** default value from float.  The new value must be in
/// the defined range of the parameter's values.
///
/// @param newRealValue new default value
///
/// @param updateParameter if this is true, the parameter's value will
///        be set to the new default value
///
void PluginParameterSwitch::setDefaultRealFloat(float newRealValue, bool updateParameter)
{
    // update internal default value
    defaultValue = toInternalFloat(newRealValue);

    // update real default value
    defaultRealValue = toRealFloat(defaultValue);

    // optionally, update current parameter value
    if (updateParameter)
    {
        setFloat(defaultValue);
    }
}


/// Set **internal** parameter value from float.  The new value must
/// be in the range from 0.0 to 1.0.
///
/// @param newValue new value (between 0.0 and 1.0)
///
void PluginParameterSwitch::setFloat(float newValue)
{
    // calculate internal value
    float newRealValue = toRealFloat(newValue);

    // value has changed
    if (newRealValue != realValue)
    {
        // update real parameter value
        realValue = newRealValue;

        // update internal parameter value
        value = toInternalFloat(realValue);

        // mark parameter as changed
        setChangeFlag();
    }
}


/// Set **real** parameter value from float.  The new value must be in
/// the defined range of the parameter's values.
///
/// @param newRealValue new value
///
void PluginParameterSwitch::setRealFloat(float newRealValue)
{
    // transform real value to internal value
    float internalValue = toInternalFloat(newRealValue);

    // update internal value
    setFloat(internalValue);
}


/// Transform string to **internal** parameter value.
///
/// @param newValue correctly formatted string
///
/// @return **internal** value
///
float PluginParameterSwitch::getFloatFromText(const String &newValue)
{
    // try to find value in array
    int selectedIndex = arrLabels.indexOf(newValue);

    // new value not found
    if (selectedIndex < 0)
    {
        DBG("[Juggler] text value \"" + newValue + "\" not found in \"" + getName() + "\".");

        // return invalid value
        return -1.0f;
    }
    else
    {
        // return internal value
        return selectedIndex * stepSize;
    }
}


/// Transform **internal** value to string.
///
/// @param newValue **internal** value
///
/// @return formatted string
///
const String PluginParameterSwitch::getTextFromFloat(float newValue)
{
    // confine new value to internal parameter range
    if (newValue < 0.0f)
    {
        newValue = 0.0f;
    }
    else if (newValue > 1.0f)
    {
        newValue = 1.0f;
    }

    // find matching index in array with real values
    int selectedIndex = MZ_Juce_Common::round(newValue / stepSize);

    // return label
    return arrLabels[selectedIndex];
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
