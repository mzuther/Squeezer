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

#include "wrapped_parameter_string.h"


WrappedParameterString::WrappedParameterString(const String &strText)
{
    strName = String::empty;
    strAttribute = String::empty;

    strValue = strText;
    setChangeFlag();
}


WrappedParameterString::~WrappedParameterString()
{
}


String WrappedParameterString::getName()
{
    return strName;
}


void WrappedParameterString::setName(const String &strParameterName)
{
    strName = strParameterName;
    strAttribute = strName.removeCharacters(" ");
}


float WrappedParameterString::getInterval()
{
    return 0.0f;
}


float WrappedParameterString::getDefaultFloat()
{
    return 0.0f;
}


float WrappedParameterString::getDefaultRealFloat()
{
    return 0.0f;
}


bool WrappedParameterString::getDefaultBoolean()
{
    return false;
}


int WrappedParameterString::getDefaultRealInteger()
{
    return 0;
}


bool WrappedParameterString::setDefaultRealFloat(float fRealValue, bool updateValue)
{
    return false;
}


float WrappedParameterString::getFloat()
{
    return 0.0f;
}


bool WrappedParameterString::setFloat(float fValue)
{
    return false;
}


float WrappedParameterString::getRealFloat()
{
    return 0.0f;
}


bool WrappedParameterString::setRealFloat(float fRealValue)
{
    return false;
}


bool WrappedParameterString::getBoolean()
{
    return false;
}


bool WrappedParameterString::setBoolean(bool bValue)
{
    return false;
}


int WrappedParameterString::getRealInteger()
{
    return 0;
}


bool WrappedParameterString::setRealInteger(int nRealValue)
{
    return false;
}


String WrappedParameterString::getText()
{
    return strValue;
}


bool WrappedParameterString::setText(const String &strText)
{
    strValue = strText;
    setChangeFlag();

    return true;
}


float WrappedParameterString::getFloatFromText(const String &strText)
{
    return 0.0f;
}


String WrappedParameterString::getTextFromFloat(float fValue)
{
    return strValue;
}


bool WrappedParameterString::hasChanged()
{
    return bChangedValue;
}


void WrappedParameterString::clearChangeFlag()
{
    bChangedValue = false;
}


void WrappedParameterString::setChangeFlag()
{
    bChangedValue = true;
}


bool WrappedParameterString::saveFromDeletion()
{
    return false;
}


void WrappedParameterString::loadFromXml(XmlElement *xml)
{
    XmlElement *xml_element = xml->getChildByName(strAttribute);

    if (xml_element)
    {
        if (xml_element->hasAttribute("value"))
        {
            setText(xml_element->getStringAttribute("value", getText()));
        }
    }
}


void WrappedParameterString::storeAsXml(XmlElement *xml)
{
    XmlElement *xml_element = new XmlElement(strAttribute);

    if (xml_element)
    {
        xml_element->setAttribute("value", getText());
        xml->addChildElement(xml_element);
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
