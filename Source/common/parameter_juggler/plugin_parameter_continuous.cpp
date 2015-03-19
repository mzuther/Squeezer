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

#include "plugin_parameter_continuous.h"


PluginParameterContinuous::PluginParameterContinuous(float real_minimum, float real_maximum, float resolution, float log_factor, int decimal_places, bool save_from_deletion)
{
    strSuffix = String::empty;

    fRealMinimum = real_minimum;
    fRealMaximum = real_maximum;

    fRealRange = fRealMaximum - fRealMinimum;
    fResolution = resolution;
    fInterval = fResolution / fRealRange;
    nDecimalPlaces = decimal_places;

    if (log_factor > 0.0f)
    {
        bLogarithmic = true;
        fLogFactor = log_factor;
        fLogPowerFactor = powf(10.0f, fLogFactor) - 1.0f;
    }
    else
    {
        bLogarithmic = false;
        fLogFactor = -1.0f;
        fLogPowerFactor = -1.0f;
    }

    bSaveFromDeletion = save_from_deletion;

    setRealFloat(fRealMinimum);
    setChangeFlag();
}


PluginParameterContinuous::~PluginParameterContinuous()
{
}


float PluginParameterContinuous::getInterval()
{
    return fInterval;
}


float PluginParameterContinuous::toRealFloat(float fValue)
{
    if (bLogarithmic)
    {
        fValue = (powf(10.0f, fValue * fLogFactor) - 1.0f) / fLogPowerFactor;
    }

    return (fValue * fRealRange) + fRealMinimum;
}


float PluginParameterContinuous::toInternalFloat(float fRealValue)
{
    fRealValue = (fRealValue - fRealMinimum) / fRealRange;

    if (bLogarithmic)
    {
        fRealValue = log10f(fRealValue * fLogPowerFactor + 1.0f) / fLogFactor;
    }

    return fRealValue;
}


float PluginParameterContinuous::getDefaultFloat()
{
    return toInternalFloat(fDefaultRealValue);
}


float PluginParameterContinuous::getDefaultRealFloat()
{
    return fDefaultRealValue;
}


bool PluginParameterContinuous::setDefaultRealFloat(float fRealValue, bool updateValue)
{
    bool bReturn;

    if (fRealValue < fRealMinimum)
    {
        DBG("[Juggler] default value for \"" + getName() + "\" set to minimum.");
        fDefaultRealValue = fRealMinimum;
        bReturn = false;
    }
    else if (fRealValue > fRealMaximum)
    {
        DBG("[Juggler] default value for \"" + getName() + "\" set to maximum.");
        fDefaultRealValue = fRealMaximum;
        bReturn = false;
    }
    else
    {
        fDefaultRealValue = fRealValue;
        bReturn = true;
    }

    if (updateValue)
    {
        setRealFloat(fDefaultRealValue);
    }

    return bReturn;
}


float PluginParameterContinuous::getFloat()
{
    return fValueInternal;
}


bool PluginParameterContinuous::setFloat(float fValue)
{
    float fValueInternalOld = fValueInternal;
    bool bReturn;

    if (fValue < 0.0f)
    {
        DBG("[Juggler] value for \"" + getName() + "\" set to minimum.");
        fValueInternal = 0.0f;
        bReturn = false;
    }
    else if (fValue > 1.0f)
    {
        DBG("[Juggler] value for \"" + getName() + "\" set to maximum.");
        fValueInternal = 1.0f;
        bReturn = false;
    }
    else
    {
        fValueInternal = fValue;
        bReturn = true;
    }

    if (fValueInternal != fValueInternalOld)
    {
        setChangeFlag();
    }

    return bReturn;
}


float PluginParameterContinuous::getRealFloat()
{
    return toRealFloat(fValueInternal);
}


bool PluginParameterContinuous::setRealFloat(float fRealValue)
{
    float fValue = toInternalFloat(fRealValue);
    return setFloat(fValue);
}


bool PluginParameterContinuous::setNearestRealFloat(float fRealValue)
{
    if (fRealValue < fRealMinimum)
    {
        setRealFloat(fRealMinimum);
        return false;
    }
    else if (fRealValue > fRealMaximum)
    {
        setRealFloat(fRealMaximum);
        return false;
    }
    else
    {
        return setRealFloat(fRealValue);
    }
}


String PluginParameterContinuous::getText()
{
    return getTextFromFloat(getFloat());
}


bool PluginParameterContinuous::setText(const String &strText)
{
    return setRealFloat(getFloatFromText(strText));
}


void PluginParameterContinuous::setSuffix(const String &suffix)
{
    strSuffix = suffix;
}


float PluginParameterContinuous::getFloatFromText(const String &strText)
{
    String strWithoutSuffix = strText.upToLastOccurrenceOf(strSuffix, false, false);
    float fRealValue = strWithoutSuffix.getFloatValue();

    return toInternalFloat(fRealValue);
}


String PluginParameterContinuous::getTextFromFloat(float fValue)
{
    float fRealValue = toRealFloat(fValue);

    if (nDecimalPlaces > 0)
    {
        return String(fRealValue, nDecimalPlaces) + strSuffix;
    }
    else
    {
        return String(round_mz(fRealValue)) + strSuffix;
    }
}


bool PluginParameterContinuous::saveFromDeletion()
{
    return bSaveFromDeletion;
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
