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

#include "plugin_parameter.h"


/// Empty constructor.  Only defined for the sake of
/// completeness...
///
PluginParameter::PluginParameter()
{
    parameterName = String::empty;
    xmlTagName = String::empty;
}


/// Empty destructor.
///
PluginParameter::~PluginParameter()
{
}


/// Get parameter's name.
///
/// @return parameter name
///
String PluginParameter::getName()
{
    return parameterName;
}


/// Set parameter's name.
///
/// @param newParameterName new parameter name
///
void PluginParameter::setName(const String &newParameterName)
{
    // set new parameter name
    parameterName = newParameterName;

    // trim leading and trailing white space
    parameterName = parameterName.trim();

    // set xml tag name from parameter name
    xmlTagName = parameterName;

    // convert tag name to lower case
    xmlTagName = xmlTagName.toLowerCase();

    // strip all characters except a-z, space and underscore
    xmlTagName = xmlTagName.retainCharacters(
                     "abcdefghijklmnopqrstuvwxyz _");

    // change all spaces to underscores
    xmlTagName = xmlTagName.replace(" ", "_");
}


/// Get parameter's XML tag name.
///
/// @return XML tag name
///
String PluginParameter::getTagName()
{
    return xmlTagName;
}


/// Get parameter value as Boolean.
///
/// @return current value
///
bool PluginParameter::getBoolean()
{
    return getRealFloat() != 0.0f;
}


/// Set parameter value from Boolean.
///
/// @param newValue new value
///
/// @return **true** if update was successful, **false** otherwise
///
bool PluginParameter::setBoolean(bool newValue)
{
    return setRealFloat(newValue ? 1.0f : 0.0f);
}


/// Get **real** parameter value as integer.  Values range from the
/// (rounded) parameter's minimum value to its (rounded) maximum
/// value.
///
/// @return current value
///
int PluginParameter::getRealInteger()
{
    return round_mz(getRealFloat());
}


/// Set **real** parameter value from integer.  The new value must
/// be in the defined range of the parameter's values.
///
/// @param newRealValue new value
///
/// @return **true** if update was successful, **false** otherwise
///
bool PluginParameter::setRealInteger(int newRealValue)
{
    return setRealFloat((float) newRealValue);
}


/// Get parameter's change flag.  Determines whether the parameter has
/// changed since the last time the change flag was reset.
///
/// @return change flag
///
bool PluginParameter::hasChanged()
{
    return valueHasChanged;
}


/// Mark parameter as unchanged.
///
void PluginParameter::clearChangeFlag()
{
    valueHasChanged = false;
}


/// Mark parameter as changed.
///
void PluginParameter::setChangeFlag()
{
    valueHasChanged = true;
}


/// Load parameter value from XML.
///
/// @param xml XML element to load from
///
void PluginParameter::loadFromXml(XmlElement *xml)
{
    XmlElement *xml_element = xml->getChildByName(getTagName());

    if (xml_element)
    {
        double realValue = xml_element->getDoubleAttribute("value", getDefaultRealFloat());
        setRealFloat((float) realValue);
    }
}


/// Store parameter value as XML.
///
/// @param xml XML element to store in
///
void PluginParameter::storeAsXml(XmlElement *xml)
{
    XmlElement *xml_element = new XmlElement(getTagName());

    if (xml_element)
    {
        float realValue = getRealFloat();
        xml_element->setAttribute("value", realValue);
        xml->addChildElement(xml_element);
    }
}


/// Round given value to nearest integer.  Will always round
/// **away** from zero!
///
/// @param x value to be rounded
///
/// @return rounded value
///
int PluginParameter::round_mz(float x)
{
    // value is greater than (or exactly) zero
    if (x >= 0.0f)
    {
        // round away from zero by adding 0.5
        x += 0.5f;
    }
    // value is below zero
    else
    {
        // round away from zero by substracting 0.5
        x -= 0.5f;
    }

    // remove remainder and return it
    return (int) x;
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
