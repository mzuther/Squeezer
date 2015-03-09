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
    strName = String::empty;
    strAttribute = String::empty;
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


String PluginParameterContinuous::getName()
{
    return strName;
}


void PluginParameterContinuous::setName(const String &strParameterName)
{
    strName = strParameterName;
    strAttribute = strName.removeCharacters(" ");
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


bool PluginParameterContinuous::getDefaultBoolean()
{
    return getDefaultRealFloat() != 0.0f;
}


int PluginParameterContinuous::getDefaultRealInteger()
{
    return round_mz(getDefaultRealFloat());
}


bool PluginParameterContinuous::setDefaultRealFloat(float fRealValue, bool updateValue)
{
    bool bReturn;

    if (fRealValue < fRealMinimum)
    {
        DBG("[Juggler] default value for \"" + strName + "\" set to minimum.");
        fDefaultRealValue = fRealMinimum;
        bReturn = false;
    }
    else if (fRealValue > fRealMaximum)
    {
        DBG("[Juggler] default value for \"" + strName + "\" set to maximum.");
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
        DBG("[Juggler] value for \"" + strName + "\" set to minimum.");
        fValueInternal = 0.0f;
        bReturn = false;
    }
    else if (fValue > 1.0f)
    {
        DBG("[Juggler] value for \"" + strName + "\" set to maximum.");
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


bool PluginParameterContinuous::getBoolean()
{
    return getRealFloat() != 0.0f;
}


bool PluginParameterContinuous::setBoolean(bool bValue)
{
    return setFloat(bValue ? 1.0f : 0.0f);
}


int PluginParameterContinuous::getRealInteger()
{
    return round_mz(getRealFloat());
}


bool PluginParameterContinuous::setRealInteger(int nRealValue)
{
    return setRealFloat((float) nRealValue);
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


bool PluginParameterContinuous::hasChanged()
{
    return bChangedValue;
}


void PluginParameterContinuous::clearChangeFlag()
{
    bChangedValue = false;
}


void PluginParameterContinuous::setChangeFlag()
{
    bChangedValue = true;
}


bool PluginParameterContinuous::saveFromDeletion()
{
    return bSaveFromDeletion;
}


void PluginParameterContinuous::loadFromXml(XmlElement *xml)
{
    XmlElement *xml_element = xml->getChildByName(strAttribute);

    if (xml_element)
    {
        float fRealValue;

        if (xml_element->hasAttribute("value"))
        {
            fRealValue = (float) xml_element->getDoubleAttribute("value", getDefaultRealFloat());
        }
        else
        {
            String strRealValue = xml_element->getAllSubText().trim();

            if (strRealValue.isEmpty())
            {
                fRealValue = getDefaultRealFloat();
            }
            else
            {
                fRealValue = strRealValue.getFloatValue();
            }
        }

        setRealFloat(fRealValue);
    }
}


void PluginParameterContinuous::storeAsXml(XmlElement *xml)
{
    XmlElement *xml_element = new XmlElement(strAttribute);

    if (xml_element)
    {
        float fRealValue = getRealFloat();
        xml_element->setAttribute("value", fRealValue);
        xml->addChildElement(xml_element);
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
