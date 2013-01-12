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

#include "wrapped_parameter_switch.h"


WrappedParameterSwitch::WrappedParameterSwitch()
{
    strName = "";
    strAttribute = "";
    fDefaultRealValue = 0.0f;

    nCurrentIndex = 0;
    fValueInternal = 0.0f;
    fInterval = 0.0f;

    setChangeFlag();
}


WrappedParameterSwitch::~WrappedParameterSwitch()
{
}


String WrappedParameterSwitch::getName()
{
    return strName;
}


void WrappedParameterSwitch::setName(const String& strParameterName)
{
    strName = strParameterName;
    strAttribute = strName.removeCharacters(" ");
}


void WrappedParameterSwitch::addValue(const float fRealValue, const String& strText)
{
    strValues.add(strText);
    fRealValues.add(fRealValue);
    fInterval = 1.0f / (strValues.size() - 1.0f);
}


float WrappedParameterSwitch::getInterval()
{
    return fInterval;
}


float WrappedParameterSwitch::getDefaultFloat()
{
    int nIndex = fRealValues.indexOf(fDefaultRealValue);

    if (nIndex < 0)
    {
        return 0.0f;
    }
    else
    {
        return nIndex * fInterval;
    }
}


float WrappedParameterSwitch::getDefaultRealFloat()
{
    return fDefaultRealValue;
}


bool WrappedParameterSwitch::getDefaultBoolean()
{
    return getDefaultFloat() != 0.0f;
}


int WrappedParameterSwitch::getDefaultInteger()
{
    return roundf(getDefaultFloat());
}


void WrappedParameterSwitch::setDefaultRealFloat(float fRealValue, bool updateValue)
{
    fDefaultRealValue = fRealValue;

    if (updateValue)
    {
        setRealFloat(fDefaultRealValue);
    }
}


float WrappedParameterSwitch::getFloat()
{
    return fValueInternal;
}


void WrappedParameterSwitch::setFloat(float fValue)
{
    int nCurrentIndexOld = nCurrentIndex;

    fValueInternal = fValue;
    nCurrentIndex = roundf(fValueInternal / fInterval);

    if (nCurrentIndex != nCurrentIndexOld)
    {
        setChangeFlag();
    }
}


float WrappedParameterSwitch::getRealFloat()
{
    return fRealValues[nCurrentIndex];
}


void WrappedParameterSwitch::setRealFloat(float fValue)
{
    int nCurrentIndexOld = nCurrentIndex;
    int nIndex = fRealValues.indexOf(fValue);

    if (nIndex > -1)
    {
        nCurrentIndex = nIndex;
        fValueInternal = nCurrentIndex * fInterval;
    }

    if (nCurrentIndex != nCurrentIndexOld)
    {
        setChangeFlag();
    }
}


bool WrappedParameterSwitch::getBoolean()
{
    return getRealFloat() != 0.0f;
}


void WrappedParameterSwitch::setBoolean(bool bValue)
{
    setRealFloat(bValue ? 1.0f : 0.0f);
}


int WrappedParameterSwitch::getInteger()
{
    return roundf(getFloat());
}


void WrappedParameterSwitch::setInteger(int nValue)
{
    setFloat(nValue);
}


int WrappedParameterSwitch::getRealInteger()
{
    return roundf(getRealFloat());
}


void WrappedParameterSwitch::setRealInteger(int nValue)
{
    setRealFloat(nValue);
}


String WrappedParameterSwitch::getText()
{
    return strValues[nCurrentIndex];
}


void WrappedParameterSwitch::setText(const String& strText)
{
    int nCurrentIndexOld = nCurrentIndex;
    int nIndex = strValues.indexOf(strText);

    if (nIndex > -1)
    {
        nCurrentIndex = nIndex;
        fValueInternal = nCurrentIndex * fInterval;
    }

    if (nCurrentIndex != nCurrentIndexOld)
    {
        setChangeFlag();
    }
}


float WrappedParameterSwitch::getFloatFromText(const String& strText)
{
    int nIndex = strValues.indexOf(strText);
    return nIndex * fInterval;
}


String WrappedParameterSwitch::getTextFromFloat(float fValue)
{
    int nIndex = roundf(fValue / fInterval);
    return strValues[nIndex];
}


int WrappedParameterSwitch::getIntegerFromText(const String& strText)
{
    return roundf(getFloatFromText(strText));
}


String WrappedParameterSwitch::getTextFromInteger(int nValue)
{
    return getTextFromFloat(nValue);
}


bool WrappedParameterSwitch::hasChanged()
{
    return bChangedValue;
}


void WrappedParameterSwitch::clearChangeFlag()
{
    bChangedValue = false;
}


void WrappedParameterSwitch::setChangeFlag()
{
    bChangedValue = true;
}


void WrappedParameterSwitch::loadFromXml(XmlElement* xml)
{
    XmlElement* xml_element = xml->getChildByName(strAttribute);

    if (xml_element)
    {
        float fValue = xml_element->getDoubleAttribute("value", getRealFloat());
        setRealFloat(fValue);
    }
}


void WrappedParameterSwitch::storeAsXml(XmlElement* xml)
{
    XmlElement* xml_element = new XmlElement(strAttribute);

    if (xml_element)
    {
        float fValue = getRealFloat();
        xml_element->setAttribute("value", fValue);
        xml->addChildElement(xml_element);
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
