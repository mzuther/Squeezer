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


// The methods of this class may be called on the audio thread, so
// they are absolutely time-critical!

/// Create a Boolean parameter.
///
/// @param state_true string that is returned when the parameter is
///        **true**
///
/// @param state_false string that is returned when the parameter is
///        **false**
///
ParBoolean::ParBoolean(const String &state_true, const String &state_false)
{
    // intialise string that is returned when the parameter is "true"
    labelTrue = state_true;

    // intialise string that is returned when the parameter is "false"
    labelFalse = state_false;

    // force update
    value_ = -1.0f;

    // initialise internal values to "false" (marks parameter as
    // changed)
    setFloat(0.0f);
}


/// Toggle parameter state from **true** to **false** and vice versa.
///
void ParBoolean::toggleState()
{
    // value is "false"
    if (value_ == 0.0f)
    {
        // set to "true"
        setFloat(1.0f);
    }
    // value is "true"
    else
    {
        // set to "false"
        setFloat(0.0f);
    }
}


/// Set **real** default value from float.  The new value will be
/// clipped to either 0.0 or 1.0.
///
/// @param newRealValue new default value
///
/// @param updateParameter if this is true, the parameter's value will
///        be set to the new default value
///
void ParBoolean::setDefaultRealFloat(float newRealValue, bool updateParameter)
{
    // confine to allowed values
    if (newRealValue != 0.0f)
    {
        newRealValue = 1.0f;
    }

    // update default values
    defaultValue_ = newRealValue;
    defaultRealValue_ = newRealValue;

    // optionally, update current parameter value
    if (updateParameter)
    {
        setFloat(defaultValue_);
    }
}


/// Set default value from Boolean.
///
/// @param newValue new default value
///
/// @param updateParameter if this is true, the parameter's value will
///        be set to the new default value
///
void ParBoolean::setDefaultBoolean(bool newValue, bool updateParameter)
{
    setDefaultRealFloat(newValue ? 1.0f : 0.0f, updateParameter);
}


/// Set **internal** parameter value from float.  The new value must
/// be in the range from 0.0 to 1.0.
///
/// @param newValue new value
///
void ParBoolean::setFloat(float newValue)
{
    // confine to allowed values
    if (newValue != 0.0f)
    {
        newValue = 1.0f;
    }

    // value has changed
    if (newValue != value_)
    {
        // update value
        value_ = newValue;
        realValue_ = newValue;

        // mark parameter as changed
        setChangeFlag();
    }
}


/// Set **real** parameter value from float.  The new value must be in
/// the range from 0.0 to 1.0.
///
/// @param newRealValue new value
///
void ParBoolean::setRealFloat(float newRealValue)
{
    setFloat(newRealValue);
}


/// Set value from Boolean.
///
/// @param newValue new value
///
void ParBoolean::setBoolean(bool newValue)
{
    setFloat(newValue ? 1.0f : 0.0f);
}


/// Transform string to **internal** parameter value.
///
/// @param newValue correctly formatted string
///
/// @return **internal** value
///
float ParBoolean::getFloatFromText(const String &newValue)
{
    // return "true" (1.0) if string matches "labelTrue"
    if (newValue.compare(labelTrue) == 0)
    {
        return 1.0f;
    }
    // otherwise, return "false" (0.0)
    else
    {
        return 0.0f;
    }
}


/// Transform **internal** value to string.
///
/// @param newValue **internal** value
///
/// @return formatted string
///
const String ParBoolean::getTextFromFloat(float newValue)
{
    if (newValue == 0.0f)
    {
        return labelFalse;
    }
    else
    {
        return labelTrue;
    }
}
