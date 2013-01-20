/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

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

    nCurrentIndex = -1;
    fDefaultRealValue = -1.0f;
    fValueInternal = fDefaultRealValue;
    fInterval = -1.0f;

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


void WrappedParameterSwitch::addConstant(const float fRealValue, const String& strText)
{
    fRealValues.add(fRealValue);
    strValues.add(strText);
    fInterval = 1.0f / (strValues.size() - 1.0f);

    if (fRealValues.size() == 1)
    {
        setDefaultRealFloat(fRealValue, true);
    }
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
        DBG("[Squeezer] default value for \"" + strName + "\" not found.");
        return -1.0f;
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


bool WrappedParameterSwitch::getDefaultRealBoolean()
{
    return getDefaultRealFloat() != 0.0f;
}


int WrappedParameterSwitch::getDefaultRealInteger()
{
    return round_mz(getDefaultRealFloat());
}


bool WrappedParameterSwitch::setDefaultRealFloat(float fRealValue, bool updateValue)
{
    int nIndex = fRealValues.indexOf(fRealValue);

    if (nIndex < 0)
    {
        DBG("[Squeezer] new default value \"" + String(fRealValue) + "\" not found in \"" + strName + "\".");
        return false;
    }
    else
    {
        fDefaultRealValue = fRealValue;

        if (updateValue)
        {
            setRealFloat(fDefaultRealValue);
        }

        return true;
    }
}


float WrappedParameterSwitch::getFloat()
{
    return fValueInternal;
}


bool WrappedParameterSwitch::setFloat(float fValue)
{
    if ((fValue < 0.0f) || (fValue > 1.0f))
    {
        DBG("[Squeezer] value \"" + String(fValue) + "\" not found in \"" + strName + "\".");
        return false;
    }
    else
    {
        int nCurrentIndexOld = nCurrentIndex;

        fValueInternal = fValue;
        nCurrentIndex = round_mz(fValueInternal / fInterval);

        if (nCurrentIndex != nCurrentIndexOld)
        {
            setChangeFlag();
        }

        return true;
    }
}


float WrappedParameterSwitch::getRealFloat()
{
    return fRealValues[nCurrentIndex];
}


bool WrappedParameterSwitch::setRealFloat(float fRealValue)
{
    int nCurrentIndexOld = nCurrentIndex;
    int nIndex = fRealValues.indexOf(fRealValue);

    if (nIndex < 0)
    {
        DBG("[Squeezer] value \"" + String(fRealValue) + "\" not found in \"" + strName + "\".");
        return false;
    }
    else
    {
        nCurrentIndex = nIndex;
        fValueInternal = nCurrentIndex * fInterval;

        if (nCurrentIndex != nCurrentIndexOld)
        {
            setChangeFlag();
        }

        return true;
    }
}


bool WrappedParameterSwitch::setNearestRealFloat(float fRealValue)
{
    int nIndexSelected = 0;
    float fDifference = fabs(fRealValue - fRealValues[nIndexSelected]);

    for (int nIndex = 1; nIndex < fRealValues.size(); nIndex++)
    {
        float fDifferenceNew = fabs(fRealValue - fRealValues[nIndex]);

        if (fDifferenceNew < fDifference)
        {
            nIndexSelected = nIndex;
            fDifference = fDifferenceNew;
        }
    }

    return setRealFloat(fRealValues[nIndexSelected]);
}


bool WrappedParameterSwitch::getBoolean()
{
    return getRealFloat() != 0.0f;
}


bool WrappedParameterSwitch::setBoolean(bool bValue)
{
    return setRealFloat(bValue ? 1.0f : 0.0f);
}


int WrappedParameterSwitch::getRealInteger()
{
    return round_mz(getRealFloat());
}


bool WrappedParameterSwitch::setRealInteger(int nRealValue)
{
    return setRealFloat((float) nRealValue);
}


String WrappedParameterSwitch::getText()
{
    return strValues[nCurrentIndex];
}


bool WrappedParameterSwitch::setText(const String& strText)
{
    int nCurrentIndexOld = nCurrentIndex;
    int nIndex = strValues.indexOf(strText);

    if (nIndex < 0)
    {
        DBG("[Squeezer] text value \"" + strText + "\" not found in \"" + strName + "\".");
        return false;
    }
    else
    {
        nCurrentIndex = nIndex;
        fValueInternal = nCurrentIndex * fInterval;

        if (nCurrentIndex != nCurrentIndexOld)
        {
            setChangeFlag();
        }

        return true;
    }
}


float WrappedParameterSwitch::getFloatFromText(const String& strText)
{
    int nIndex = strValues.indexOf(strText);

    if (nIndex < 0)
    {
        DBG("[Squeezer] text value \"" + strText + "\" not found in \"" + strName + "\".");
        return -1.0f;
    }
    else
    {
        return nIndex * fInterval;
    }
}


String WrappedParameterSwitch::getTextFromFloat(float fValue)
{
    if ((fValue < 0.0f) || (fValue > 1.0f))
    {
        DBG("[Squeezer] value \"" + String(fValue) + "\" not found in \"" + strName + "\".");
        return "not found";
    }
    else
    {
        int nIndex = round_mz(fValue / fInterval);
        return strValues[nIndex];
    }
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
        float fRealValue = (float) xml_element->getDoubleAttribute("value", getDefaultRealFloat());
        setRealFloat(fRealValue);
    }
}


void WrappedParameterSwitch::storeAsXml(XmlElement* xml)
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
