/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
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


// The methods of this class may be called on the audio thread, so
// they are absolutely time-critical!

/// Create a stepped parameter with continuous floating-point values.
/// **Real** values range from **real_minimum** to **real_maximum**,
/// are quantised by **real_step_size**, and may be transformed to
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
/// @param save_from_deletion should parameter be spared from deletion
///        in destructor of ParameterJuggler?
///
PluginParameterContinuous::PluginParameterContinuous(float real_minimum, float real_maximum, float real_step_size, float scaling_factor, int decimal_places, bool save_from_deletion)
{
    // minimum and maximum real parameter value
    realMinimum = real_minimum;
    realMaximum = real_maximum;

    // range of real parameter values
    realRange = realMaximum - realMinimum;

    // real parameter values are quantised using the number of steps
    // (which depends on step size).  For example, 4 steps with a
    // minimum value of 0 and a maximum value of 6 will lead to
    // (unscaled) parameter values of 0, 2, 4, and 6.
    //
    // So far, this is only reflected in the function
    // getNumberOfSteps().  Intermediate parameter values are accepted
    // and left alone.
    numberOfSteps = int(realRange / real_step_size) + 1;

    // number of decimal places (for formatting of value only)
    decimalPlaces = decimal_places;

    // initialise value suffix (for formatting of value only)
    valueSuffix = String::empty;

    // initialise scaling factor (for exponential or logarithmic
    // parameters)
    scalingFactor = scaling_factor;

    // parameter is linear
    if (scalingFactor == 0.0f)
    {
        isNonlinear = false;
        scalingConstantFactor = 0.0f;
    }
    // parameter is non-linear
    else
    {
        isNonlinear = true;

        // initialise constant scaling factor to increase processing
        // speed (see formula in description)
        scalingConstantFactor = powf(10.0f, scalingFactor) - 1.0f;
    }

    // may parameter be deleted?
    doNotDelete = save_from_deletion;

    // force update
    value = -1.0f;

    // set parameter to minimum value (also marks parameter as
    // changed)
    setFloat(0.0f);
}


/// Convert **internal** parameter value to **real** value.
///
/// @param newValue **internal** parameter value
///
/// @return **real** parameter value
///
float PluginParameterContinuous::toRealFloat(float newValue)
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

    // transform exponential and logarithmic parameters
    if (isNonlinear)
    {
        newValue = (powf(10.0f, newValue * scalingFactor) - 1.0f) / scalingConstantFactor;
    }

    // transform to real parameter range
    float newRealValue = (newValue * realRange) + realMinimum;

    // TODO: quantise to step size here!

    return newRealValue;
}


/// Convert **real** parameter value to **internal** value.
///
/// @param newRealValue **real** parameter value
///
/// @return **internal** parameter value
///
float PluginParameterContinuous::toInternalFloat(float newRealValue)
{
    // confine new value to real parameter range
    if (newRealValue < realMinimum)
    {
        newRealValue = realMinimum;
    }
    else if (newRealValue > realMaximum)
    {
        newRealValue = realMaximum;
    }

    // transform to internal parameter range
    float newValue = (newRealValue - realMinimum) / realRange;

    // TODO: quantise to step size here!

    // transform exponential and logarithmic parameters
    if (isNonlinear)
    {
        newValue = log10f(newValue * scalingConstantFactor + 1.0f) / scalingFactor;
    }

    return newValue;
}


/// Get number of discrete parameter values.
///
/// @return number of discrete parameter values
///
int PluginParameterContinuous::getNumberOfSteps()
{
    return numberOfSteps;
}


/// Get **internal** step size of parameter values.
///
/// @return step size
///
float PluginParameterContinuous::getStepSize()
{
    return 1.0f / float(numberOfSteps - 1);
}


/// Set **real** default value from float.  The new value must be in
/// the defined range of the parameter's values.
///
/// @param newRealValue new default value
///
/// @param updateParameter if this is true, the parameter's value will
///        be set to the new default value
///
void PluginParameterContinuous::setDefaultRealFloat(float newRealValue, bool updateParameter)
{
    // confine new default value to real parameter range
    if (newRealValue < realMinimum)
    {
        newRealValue = realMinimum;
    }
    else if (newRealValue > realMaximum)
    {
        newRealValue = realMaximum;
    }

    // update real default value
    defaultRealValue = newRealValue;

    // update internal default value
    defaultValue = toInternalFloat(defaultRealValue);

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
void PluginParameterContinuous::setFloat(float newValue)
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

    // value has changed
    if (newValue != value)
    {
        // update internal parameter value
        value = newValue;

        // update real parameter value
        realValue = toRealFloat(value);

        // mark parameter as changed
        setChangeFlag();
    }
}


/// Set **real** parameter value from float.  The new value must be in
/// the defined range of the parameter's values.
///
/// @param newRealValue new value
///
void PluginParameterContinuous::setRealFloat(float newRealValue)
{
    // transform real value to internal value
    float internalValue = toInternalFloat(newRealValue);

    // update internal value
    setFloat(internalValue);
}


/// Set suffix that will be appended to the formatted parameter.
///
/// @param newSuffix new suffix (may be an empty string)
///
void PluginParameterContinuous::setSuffix(const String &newSuffix)
{
    // set new suffix for text value
    valueSuffix = newSuffix;
}


/// Transform string to **internal** parameter value.
///
/// @param newValue correctly formatted string
///
/// @return **internal** value
///
float PluginParameterContinuous::getFloatFromText(const String &newValue)
{
    // removed suffix from string
    String cleanedTextValue = newValue.upToLastOccurrenceOf(valueSuffix, false, false);

    // convert string to float
    float realValueNew = cleanedTextValue.getFloatValue();

    // transform real value to internal value
    return toInternalFloat(realValueNew);
}


/// Transform **internal** value to string.
///
/// @param newValue **internal** value
///
/// @return formatted string
///
const String PluginParameterContinuous::getTextFromFloat(float newValue)
{
    // transform internal value to real value
    float newRealValue = toRealFloat(newValue);

    String textValueNew;

    // use decimal places
    if (decimalPlaces > 0)
    {
        // format parameter value using given number of decimal places
        textValueNew = String(newRealValue, decimalPlaces);
    }
    else
    {
        // round real parameter value
        newRealValue = (float) math::Simple::round(newRealValue);

        // format parameter value
        textValueNew = String(newRealValue);
    }

    // add parameter suffix
    textValueNew += valueSuffix;

    // return formatted string
    return textValueNew;
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
