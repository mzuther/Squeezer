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

#include "wrapped_parameter_continuous.h"


WrappedParameterContinuous::WrappedParameterContinuous(float real_minimum, float real_maximum, float resolution, float log_factor, int decimal_places, bool save_from_deletion)
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


WrappedParameterContinuous::~WrappedParameterContinuous()
{
}


String WrappedParameterContinuous::getName()
{
    return strName;
}


void WrappedParameterContinuous::setName(const String &strParameterName)
{
    strName = strParameterName;
    strAttribute = strName.removeCharacters(" ");
}


float WrappedParameterContinuous::getInterval()
{
    return fInterval;
}


float WrappedParameterContinuous::toRealFloat(float fValue)
{
    if (bLogarithmic)
    {
        fValue = (powf(10.0f, fValue * fLogFactor) - 1.0f) / fLogPowerFactor;
    }

    return (fValue * fRealRange) + fRealMinimum;
}


float WrappedParameterContinuous::toInternalFloat(float fRealValue)
{
    fRealValue = (fRealValue - fRealMinimum) / fRealRange;

    if (bLogarithmic)
    {
        fRealValue = log10f(fRealValue * fLogPowerFactor + 1.0f) / fLogFactor;
    }

    return fRealValue;
}


float WrappedParameterContinuous::getDefaultFloat()
{
    return toInternalFloat(fDefaultRealValue);
}


float WrappedParameterContinuous::getDefaultRealFloat()
{
    return fDefaultRealValue;
}


bool WrappedParameterContinuous::getDefaultBoolean()
{
    return getDefaultRealFloat() != 0.0f;
}


int WrappedParameterContinuous::getDefaultRealInteger()
{
    return round_mz(getDefaultRealFloat());
}


bool WrappedParameterContinuous::setDefaultRealFloat(float fRealValue, bool updateValue)
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


float WrappedParameterContinuous::getFloat()
{
    return fValueInternal;
}


bool WrappedParameterContinuous::setFloat(float fValue)
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


float WrappedParameterContinuous::getRealFloat()
{
    return toRealFloat(fValueInternal);
}


bool WrappedParameterContinuous::setRealFloat(float fRealValue)
{
    float fValue = toInternalFloat(fRealValue);
    return setFloat(fValue);
}


bool WrappedParameterContinuous::setNearestRealFloat(float fRealValue)
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


bool WrappedParameterContinuous::getBoolean()
{
    return getRealFloat() != 0.0f;
}


bool WrappedParameterContinuous::setBoolean(bool bValue)
{
    return setFloat(bValue ? 1.0f : 0.0f);
}


int WrappedParameterContinuous::getRealInteger()
{
    return round_mz(getRealFloat());
}


bool WrappedParameterContinuous::setRealInteger(int nRealValue)
{
    return setRealFloat((float) nRealValue);
}


String WrappedParameterContinuous::getText()
{
    return getTextFromFloat(getFloat());
}


bool WrappedParameterContinuous::setText(const String &strText)
{
    return setRealFloat(getFloatFromText(strText));
}


void WrappedParameterContinuous::setSuffix(const String &suffix)
{
    strSuffix = suffix;
}


float WrappedParameterContinuous::getFloatFromText(const String &strText)
{
    String strWithoutSuffix = strText.upToLastOccurrenceOf(strSuffix, false, false);
    float fRealValue = strWithoutSuffix.getFloatValue();

    return toInternalFloat(fRealValue);
}


String WrappedParameterContinuous::getTextFromFloat(float fValue)
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


bool WrappedParameterContinuous::hasChanged()
{
    return bChangedValue;
}


void WrappedParameterContinuous::clearChangeFlag()
{
    bChangedValue = false;
}


void WrappedParameterContinuous::setChangeFlag()
{
    bChangedValue = true;
}


bool WrappedParameterContinuous::saveFromDeletion()
{
    return bSaveFromDeletion;
}


void WrappedParameterContinuous::loadFromXml(XmlElement *xml)
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


void WrappedParameterContinuous::storeAsXml(XmlElement *xml)
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
