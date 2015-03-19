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

#include "plugin_parameter_string.h"


PluginParameterString::PluginParameterString(const String &strText)
{
    strValue = strText;
    setChangeFlag();
}


PluginParameterString::~PluginParameterString()
{
}


float PluginParameterString::getDefaultFloat()
{
    return 0.0f;
}


float PluginParameterString::getDefaultRealFloat()
{
    return 0.0f;
}


bool PluginParameterString::getDefaultBoolean()
{
    return false;
}


int PluginParameterString::getDefaultRealInteger()
{
    return 0;
}


bool PluginParameterString::setDefaultRealFloat(float fRealValue, bool updateValue)
{
    return false;
}


float PluginParameterString::getFloat()
{
    return 0.0f;
}


bool PluginParameterString::setFloat(float fValue)
{
    return false;
}


float PluginParameterString::getRealFloat()
{
    return getFloat();
}


bool PluginParameterString::setRealFloat(float fRealValue)
{
    return setFloat(fRealValue);
}


String PluginParameterString::getText()
{
    return strValue;
}


bool PluginParameterString::setText(const String &strText)
{
    strValue = strText;
    setChangeFlag();

    return true;
}


bool PluginParameterString::saveFromDeletion()
{
    return false;
}


void PluginParameterString::loadFromXml(XmlElement *xml)
{
    XmlElement *xml_element = xml->getChildByName(getTagName());

    if (xml_element)
    {
        setText(xml_element->getStringAttribute("value", getText()));
    }
}


void PluginParameterString::storeAsXml(XmlElement *xml)
{
    XmlElement *xml_element = new XmlElement(getTagName());

    if (xml_element)
    {
        xml_element->setAttribute("value", getText());
        xml->addChildElement(xml_element);
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
