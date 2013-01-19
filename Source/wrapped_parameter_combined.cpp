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

#include "wrapped_parameter_combined.h"


WrappedParameterCombined::WrappedParameterCombined()
{
    strName = "";
    strAttribute = "";

    pSwitch = new WrappedParameterSwitch();

    setChangeFlag();
}


WrappedParameterCombined::~WrappedParameterCombined()
{
    delete pSwitch;
    pSwitch = NULL;
}


String WrappedParameterCombined::getName()
{
    return strName;
}


void WrappedParameterCombined::setName(const String& strParameterName)
{
    strName = strParameterName;
    strAttribute = strName.removeCharacters(" ");
}


void WrappedParameterCombined::addConstant(const float fRealValue, const String& strText)
{
    pSwitch->addConstant(fRealValue, strText);
}


float WrappedParameterCombined::getInterval()
{
    return pSwitch->getInterval();
}


float WrappedParameterCombined::getDefaultFloat()
{
    return pSwitch->getDefaultFloat();
}


float WrappedParameterCombined::getDefaultRealFloat()
{
    return pSwitch->getDefaultRealFloat();
}


bool WrappedParameterCombined::getDefaultRealBoolean()
{
    return pSwitch->getDefaultRealBoolean();
}


int WrappedParameterCombined::getDefaultRealInteger()
{
    return pSwitch->getDefaultRealInteger();
}


bool WrappedParameterCombined::setDefaultRealFloat(float fRealValue, bool updateValue)
{
    return pSwitch->setDefaultRealFloat(fRealValue, updateValue);
}


float WrappedParameterCombined::getFloat()
{
    return pSwitch->getFloat();
}


bool WrappedParameterCombined::setFloat(float fValue)
{
    return pSwitch->setFloat(fValue);
}


float WrappedParameterCombined::getRealFloat()
{
    return pSwitch->getRealFloat();
}


bool WrappedParameterCombined::setRealFloat(float fRealValue)
{
    return pSwitch->setRealFloat(fRealValue);
}


bool WrappedParameterCombined::getBoolean()
{
    return pSwitch->getBoolean();
}


bool WrappedParameterCombined::setBoolean(bool bValue)
{
    return pSwitch->setBoolean(bValue);
}


int WrappedParameterCombined::getRealInteger()
{
    return pSwitch->getRealInteger();
}


bool WrappedParameterCombined::setRealInteger(int nRealValue)
{
    return pSwitch->setRealInteger(nRealValue);
}


String WrappedParameterCombined::getText()
{
    return pSwitch->getText();
}


bool WrappedParameterCombined::setText(const String& strText)
{
    return pSwitch->setText(strText);
}


float WrappedParameterCombined::getFloatFromText(const String& strText)
{
    return pSwitch->getFloatFromText(strText);
}


String WrappedParameterCombined::getTextFromFloat(float fValue)
{
    return pSwitch->getTextFromFloat(fValue);
}


bool WrappedParameterCombined::hasChanged()
{
    return bChangedValue;
}


void WrappedParameterCombined::clearChangeFlag()
{
    bChangedValue = false;
}


void WrappedParameterCombined::setChangeFlag()
{
    bChangedValue = true;
}


void WrappedParameterCombined::loadFromXml(XmlElement* xml)
{
    XmlElement* xml_element = xml->getChildByName(strAttribute);

    if (xml_element)
    {
        float fRealValue = (float) xml_element->getDoubleAttribute("value", getDefaultRealFloat());
        setRealFloat(fRealValue);
    }
}


void WrappedParameterCombined::storeAsXml(XmlElement* xml)
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
