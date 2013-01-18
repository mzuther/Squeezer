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

#include "wrapped_parameter_continuous.h"


WrappedParameterContinuous::WrappedParameterContinuous(float fRealMinimumNew, float fRealMaximumNew)
{
    strName = "";
    strAttribute = "";

    fRealMinimum = fRealMinimumNew;
    fRealMaximum = fRealMaximumNew;

    fRealRange = fRealMaximum - fRealMinimum;
    fInterval = 1.0f / fRealRange;

    setRealFloat(fRealMinimumNew);
    setChangeFlag();
}


WrappedParameterContinuous::~WrappedParameterContinuous()
{
}


String WrappedParameterContinuous::getName()
{
    return strName;
}


void WrappedParameterContinuous::setName(const String& strParameterName)
{
    strName = strParameterName;
    strAttribute = strName.removeCharacters(" ");
}


float WrappedParameterContinuous::getInterval()
{
    return fInterval;
}


float WrappedParameterContinuous::getDefaultFloat()
{
    return (fDefaultRealValue - fRealMinimum) / fRealRange;
}


float WrappedParameterContinuous::getDefaultRealFloat()
{
    return fDefaultRealValue;
}


bool WrappedParameterContinuous::getDefaultRealBoolean()
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
        DBG("[Squeezer] default value for \"" + strName + "\" set to minimum.");
        fDefaultRealValue = fRealMinimum;
        bReturn = false;
    }
    else if (fRealValue > fRealMaximum)
    {
        DBG("[Squeezer] default value for \"" + strName + "\" set to maximum.");
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
        DBG("[Squeezer] value for \"" + strName + "\" set to minimum.");
        fValueInternal = 0.0f;
        bReturn = false;
    }
    else if (fValue > 1.0f)
    {
        DBG("[Squeezer] value for \"" + strName + "\" set to maximum.");
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
    return fValueInternal * fRealRange + fRealMinimum;
}


bool WrappedParameterContinuous::setRealFloat(float fRealValue)
{
    float fValue = (fRealValue - fRealMinimum) / fRealRange;
    return setFloat(fValue);
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


bool WrappedParameterContinuous::setText(const String& strText)
{
    return setRealFloat(getFloatFromText(strText));
}


float WrappedParameterContinuous::getFloatFromText(const String& strText)
{
    return (strText.getFloatValue() - fRealMinimum) / fRealRange;
}


String WrappedParameterContinuous::getTextFromFloat(float fValue)
{
    return String(round_mz(fValue * fRealRange + fRealMinimum));
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


void WrappedParameterContinuous::loadFromXml(XmlElement* xml)
{
    XmlElement* xml_element = xml->getChildByName(strAttribute);

    if (xml_element)
    {
        float fRealValue = (float) xml_element->getDoubleAttribute("value", getDefaultRealFloat());
        setRealFloat(fRealValue);
    }
}


void WrappedParameterContinuous::storeAsXml(XmlElement* xml)
{
    XmlElement* xml_element = new XmlElement(strAttribute);

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
