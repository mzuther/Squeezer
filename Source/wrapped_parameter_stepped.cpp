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

#include "wrapped_parameter_stepped.h"


WrappedParameterStepped::WrappedParameterStepped()
{
    strName = "";
    strAttribute = "";
    strDefaultValue = "";

    nCurrentIndex = 0;
    fValueInternal = 0.0f;
    fInterval = 0.0f;

    setChangeFlag();
}


WrappedParameterStepped::~WrappedParameterStepped()
{
}


String WrappedParameterStepped::getName()
{
    return strName;
}


void WrappedParameterStepped::setName(const String& strParameterName)
{
    strName = strParameterName;
    strAttribute = strName.removeCharacters(" ");
}


void WrappedParameterStepped::addValue(const float fRealValue, const String& strText)
{
    strValues.add(strText);
    fRealValues.add(fRealValue);
    fInterval = 1.0f / (strValues.size() - 1.0f);
}


float WrappedParameterStepped::getInterval()
{
    return fInterval;
}


float WrappedParameterStepped::getDefaultFloat()
{
    int nIndex = strValues.indexOf(strDefaultValue);

    if (nIndex < 0)
    {
        return 0.0f;
    }
    else
    {
        return nIndex * fInterval;
    }
}


bool WrappedParameterStepped::getDefaultBoolean()
{
    return getDefaultFloat() != 0.0f;
}


int WrappedParameterStepped::getDefaultInteger()
{
    return roundf(getDefaultFloat());
}


void WrappedParameterStepped::setDefaultString(const String& strDefault, bool updateValue)
{
    strDefaultValue = strDefault;

    if (updateValue)
    {
        setText(strDefaultValue);
    }
}


float WrappedParameterStepped::getFloat()
{
    return fValueInternal;
}


void WrappedParameterStepped::setFloat(float fValue)
{
    int nCurrentIndexOld = nCurrentIndex;

    fValueInternal = fValue;
    nCurrentIndex = roundf(fValueInternal / fInterval);

    if (nCurrentIndex != nCurrentIndexOld)
    {
        setChangeFlag();
    }
}


float WrappedParameterStepped::getRealFloat()
{
    return fRealValues[nCurrentIndex];
}


void WrappedParameterStepped::setRealFloat(float fValue)
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


bool WrappedParameterStepped::getBoolean()
{
    return getRealFloat() != 0.0f;
}


void WrappedParameterStepped::setBoolean(bool bValue)
{
    setRealFloat(bValue ? 1.0f : 0.0f);
}


int WrappedParameterStepped::getInteger()
{
    return roundf(getFloat());
}


void WrappedParameterStepped::setInteger(int nValue)
{
    setFloat(nValue);
}


int WrappedParameterStepped::getRealInteger()
{
    return roundf(getRealFloat());
}


void WrappedParameterStepped::setRealInteger(int nValue)
{
    setRealFloat(nValue);
}


String WrappedParameterStepped::getText()
{
    return strValues[nCurrentIndex];
}


void WrappedParameterStepped::setText(const String& strText)
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


float WrappedParameterStepped::getFloatFromText(const String& strText)
{
    int nIndex = strValues.indexOf(strText);
    return nIndex * fInterval;
}


String WrappedParameterStepped::getTextFromFloat(float fValue)
{
    int nIndex = roundf(fValue / fInterval);
    return strValues[nIndex];
}


int WrappedParameterStepped::getIntegerFromText(const String& strText)
{
    return roundf(getFloatFromText(strText));
}


String WrappedParameterStepped::getTextFromInteger(int nValue)
{
    return getTextFromFloat(nValue);
}


bool WrappedParameterStepped::hasChanged()
{
    return bChangedValue;
}


void WrappedParameterStepped::clearChangeFlag()
{
    bChangedValue = false;
}


void WrappedParameterStepped::setChangeFlag()
{
    bChangedValue = true;
}


void WrappedParameterStepped::loadFromXml(XmlElement* xml)
{
    setRealFloat(xml->getDoubleAttribute(strAttribute, getRealFloat()));
}


void WrappedParameterStepped::storeAsXml(XmlElement* xml)
{
    xml->setAttribute(strAttribute, getRealFloat());
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
