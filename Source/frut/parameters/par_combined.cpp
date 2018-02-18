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

namespace frut
{
namespace parameters
{

// The methods of this class may be called on the audio thread, so
// they are absolutely time-critical!

/// Create a parameter that can be switched between preset values
/// (ParSwitch) and continuous values (ParContinuous).  **Real**
/// values range from **real_minimum** to **real_maximum**, are
/// quantised by **real_step_size**, and may be transformed to
/// exponential or logarithmic scalings using **scaling_factor**.
/// **Internal** values simply range from 0.0 to 1.0.
///
/// @param real_minimum **real** parameter minimum (may be less than
///        **real_maximum**)
///
/// @param real_maximum **real** parameter maximum (may be higher than
///        **real_minimum**)
///
/// @param real_step_size **real** parameter values are quantised
///        using this value.  For example, a minimum value of 0,
///        maximum value of 6 and a step size of 2 will lead to
///        (unscaled) parameter values of 0, 2, 4, and 6.
///
/// @param scaling_factor set this to positive values for exponential
///        scaling and negative values for logarithmic scaling: \f$
///        realValue = \frac{10^{internalValue * scalingFactor} -
///        1}{10^{scalingFactor} - 1} \f$.  With \f$ SCF =
///        10^{scalingFactor} - 1 \f$ this can be reduced to \f$
///        realValue = \frac{10^{internalValue * scalingFactor} -
///        1}{SCF} \f$ and \f$ internalValue =
///        \frac{log_{10}(realValue * SCF + 1)}{scalingFactor} \f$.  A
///        value of 0 evokes linear scaling.
///
/// @param decimal_places number of decimal places for formatting the
///        real value
///
ParCombined::ParCombined(float real_minimum, float real_maximum, float real_step_size, float scaling_factor, int decimal_places) :

    // initialise parameter for switching between "presets" and
    // "continuous" mode
    modeSwitch("Presets", "Continuous"),

    // initialise parameter for continuous values
    continuousValues(real_minimum, real_maximum, real_step_size, scaling_factor, decimal_places)

{
    // initialise values (invalid because the parameter itself
    // contains no values)
    value_ = -1.0f;
    realValue_ = -1.0f;

    // initialise default values (invalid because the parameter itself
    // contains no values)
    defaultValue_ = -1.0f;
    defaultRealValue_ = -1.0f;

    // initially use preset values
    usePresets = true;
    modeSwitch.setBoolean(usePresets);
}


/// Set parameter's name.
///
/// @param newParameterName new parameter name
///
void ParCombined::setName(const String &newParameterName)
{
    // call base class method
    Parameter::setName(newParameterName);

    // set name of mode switch
    modeSwitch.setName(newParameterName + " Mode");
}


/// Add a preset value and a corresponding string representation.
/// **This function must be called at least twice for the parameter to
/// work as expected.**
///
/// @param newRealValue new real value
///
/// @param newLabel string representation
///
void ParCombined::addPreset(const float newRealValue, const String &newLabel)
{
    // add value to presets
    presetValues.addPreset(newRealValue, newLabel);
}


/// Get preset mode.
///
/// @return **true** for preset values and **false** for continuous
///         values
///
bool ParCombined::getMode()
{
    return usePresets;
}


/// Set preset mode.
///
/// @param use_presets **true** for preset values and **false** for
///        continuous values
///
void ParCombined::setMode(bool use_presets)
{
    // preset mode has changed
    if (use_presets != usePresets)
    {
        // toggle preset mode
        toggleMode();
    }
}


/// Toggle preset mode while retaining the current value.
///
void ParCombined::toggleMode()
{
    // toggle mode switch
    modeSwitch.toggleState();

    // use presets?
    usePresets = modeSwitch.getBoolean();

    // new mode is "presets"
    if (usePresets)
    {
        // get current continuous value
        float newRealValue = continuousValues.getRealFloat();

        // update preset value (rounds to nearest value)
        presetValues.setRealFloat(newRealValue);
    }
    // new mode is "continuous"
    else
    {
        // get current preset value
        float newRealValue = presetValues.getRealFloat();

        // update continuous value
        continuousValues.setRealFloat(newRealValue);
    }
}


/// Get pointer to mode switch.  **This pointer must not be deleted
/// outside of this class!**
///
/// @return pointer to mode switch
///
parameters::ParBoolean *ParCombined::getModeSwitch()
{
    // this pointer must not be deleted outside of this class!
    return &modeSwitch;
}


/// Get number of discrete parameter values.
///
/// @return number of discrete parameter values
///
int ParCombined::getNumberOfSteps()
{
    if (usePresets)
    {
        return presetValues.getNumberOfSteps();
    }
    else
    {
        return continuousValues.getNumberOfSteps();
    }
}


/// Get **internal** step size of parameter values.
///
/// @return step size
///
float ParCombined::getStepSize()
{
    if (usePresets)
    {
        return presetValues.getStepSize();
    }
    else
    {
        return continuousValues.getStepSize();
    }
}


/// Get **internal** default value as float.  Values range from 0.0 to
/// 1.0.
///
/// @return internal default value (between 0.0 and 1.0)
///
float ParCombined::getDefaultFloat()
{
    if (usePresets)
    {
        return presetValues.getDefaultFloat();
    }
    else
    {
        return continuousValues.getDefaultFloat();
    }
}


/// Get **real** default value as float.  Values range from the
/// parameter's minimum value to its maximum value.
///
/// @return default value
///
float ParCombined::getDefaultRealFloat()
{
    if (usePresets)
    {
        return presetValues.getDefaultRealFloat();
    }
    else
    {
        return continuousValues.getDefaultRealFloat();
    }
}


/// Get default value as Boolean.
///
/// @return **false** if the default value is set to the parameter's
///         minimum, **true** otherwise
///
bool ParCombined::getDefaultBoolean()
{
    if (usePresets)
    {
        return presetValues.getDefaultBoolean();
    }
    else
    {
        return continuousValues.getDefaultBoolean();
    }
}


/// Get **real** default value as integer.  Values range from the
/// parameter's (rounded) minimum value to its (rounded) maximum
/// value.
///
/// @return default value
///
int ParCombined::getDefaultRealInteger()
{
    if (usePresets)
    {
        return presetValues.getDefaultRealInteger();
    }
    else
    {
        return continuousValues.getDefaultRealInteger();
    }
}


/// Set **real** default value from float.  The new value must be in
/// the defined range of the parameter's values.
///
/// @param newRealValue new default value
///
/// @param updateParameter if this is true, the parameter's value will
///        be set to the new default value
///
void ParCombined::setDefaultRealFloat(float newRealValue, bool updateParameter)
{
    if (usePresets)
    {
        // update default preset value
        presetValues.setDefaultRealFloat(newRealValue, updateParameter);

        // update default continuous value (do not update internal
        // value)
        continuousValues.setDefaultRealFloat(newRealValue, false);
    }
    else
    {
        // update default continuous value
        continuousValues.setDefaultRealFloat(newRealValue, updateParameter);

        // update default preset value (do not update internal value)
        presetValues.setDefaultRealFloat(newRealValue, false);
    }
}


/// Get **internal** parameter value as float.  Values range from 0.0
/// to 1.0.
///
/// @return current value (between 0.0 and 1.0)
///
float ParCombined::getFloat()
{
    if (usePresets)
    {
        return presetValues.getFloat();
    }
    else
    {
        return continuousValues.getFloat();
    }
}


/// Set **internal** parameter value from float.  The new value must
/// be in the range from 0.0 to 1.0.
///
/// @param newValue new value (between 0.0 and 1.0)
///
void ParCombined::setFloat(float newValue)
{
    if (usePresets)
    {
        presetValues.setFloat(newValue);
    }
    else
    {
        continuousValues.setFloat(newValue);
    }
}


/// Get **real** parameter value as float.  Values range from the
/// parameter's minimum value to its maximum value.
///
/// @return current value
///
float ParCombined::getRealFloat()
{
    if (usePresets)
    {
        return presetValues.getRealFloat();
    }
    else
    {
        return continuousValues.getRealFloat();
    }
}


/// Set **real** parameter value from float.  The new value must be in
/// the defined range of the parameter's values.
///
/// @param newRealValue new value
///
void ParCombined::setRealFloat(float newRealValue)
{
    if (usePresets)
    {
        presetValues.setRealFloat(newRealValue);
    }
    else
    {
        continuousValues.setRealFloat(newRealValue);
    }
}


/// Get **real** parameter value as integer.  Values range from the
/// (rounded) parameter's minimum value to its (rounded) maximum
/// value.
///
/// @return current value
///
int ParCombined::getRealInteger()
{
    if (usePresets)
    {
        return presetValues.getRealInteger();
    }
    else
    {
        return continuousValues.getRealInteger();
    }
}


/// Set **real** parameter value from integer.  The new value must be
/// in the defined range of the parameter's values.
///
/// @param newRealValue new value
///
void ParCombined::setRealInteger(int newRealValue)
{
    if (usePresets)
    {
        presetValues.setRealInteger(newRealValue);
    }
    else
    {
        continuousValues.setRealInteger(newRealValue);
    }
}


/// Get parameter value as Boolean.
///
/// @return **false** if current value is at its minimum, **true**
///         otherwise
///
bool ParCombined::getBoolean()
{
    if (usePresets)
    {
        return presetValues.getBoolean();
    }
    else
    {
        return continuousValues.getBoolean();
    }
}


/// Set suffix that will be appended to the formatted continuous
/// parameter.
///
/// @param newSuffix new suffix (may be an empty string)
///
void ParCombined::setSuffix(const String &newSuffix)
{
    // update suffix
    continuousValues.setSuffix(newSuffix);
}


/// Transform string to **internal** parameter value.
///
/// @param newValue correctly formatted string
///
/// @return **internal** value
///
float ParCombined::getFloatFromText(const String &newValue)
{
    if (usePresets)
    {
        return presetValues.getFloatFromText(newValue);
    }
    else
    {
        return continuousValues.getFloatFromText(newValue);
    }
}


/// Transform **internal** value to string.
///
/// @param newValue **internal** value
///
/// @return formatted string
///
const String ParCombined::getTextFromFloat(float newValue)
{
    if (usePresets)
    {
        return presetValues.getTextFromFloat(newValue);
    }
    else
    {
        return continuousValues.getTextFromFloat(newValue);
    }
}


/// Get parameter's change flag.  Determines whether the parameter has
/// changed since the last time the change flag was reset.
///
/// @return change flag
///
bool ParCombined::hasChanged()
{
    bool changedPreset = presetValues.hasChanged();
    bool changedContinuous = continuousValues.hasChanged();

    return (changedPreset || changedContinuous);
}


/// Mark parameter as unchanged.
///
void ParCombined::clearChangeFlag()
{
    presetValues.clearChangeFlag();
    continuousValues.clearChangeFlag();
}


/// Mark parameter as changed.
///
void ParCombined::setChangeFlag()
{
    // do nothing till you hear from me ...
    jassert(false);
}


/// Load parameter value from XML.
///
/// @param xmlDocument XML document to load from
///
void ParCombined::loadFromXml(XmlElement *xmlDocument)
{
    // get parameter's element from XML document
    XmlElement *xmlParameter = xmlDocument->getChildByName(getTagName());

    // parameter's element found
    if (xmlParameter)
    {
        // get stored preset mode from attribute "use_presets"
        // (defaults to "true")
        bool use_presets = xmlParameter->getBoolAttribute("use_presets", true);

        // update preset mode
        setMode(use_presets);

        // get stored value from attribute "value" (or use default
        // real value)
        double realValue = xmlParameter->getDoubleAttribute("value", getDefaultRealFloat());

        // update real value
        setRealFloat((float) realValue);
    }
}


/// Store parameter value as XML.
///
/// @param xmlDocument XML document to store in
///
void ParCombined::storeAsXml(XmlElement *xmlDocument)
{
    // create new XML element with parameter's tag name (will be
    // deleted by XML document)
    XmlElement *xmlParameter = new XmlElement(getTagName());

    // XML element was successfully created
    if (xmlParameter)
    {
        // set attribute "use_presets" to current preset mode
        xmlParameter->setAttribute("use_presets", usePresets ? "1" : "0");

        // get current real value
        float realValue = getRealFloat();

        // set attribute "value" to current value
        xmlParameter->setAttribute("value", realValue);

        // add new element to XML document
        xmlDocument->addChildElement(xmlParameter);
    }
}

}
}
