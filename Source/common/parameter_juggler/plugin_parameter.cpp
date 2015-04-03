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


/// Constructor.
///
PluginParameter::PluginParameter()
{
    // initialise current value
    value = 0.0f;
    realValue = 0.0f;
    textValue = String::empty;

    // initialise default value
    defaultValue = 0.0f;
    defaultRealValue = 0.0f;

    // initialise parameter name
    parameterName = String::empty;

    // initialise XML tag name
    xmlTagName = String::empty;

    // parameter may be deleted
    doNotDelete = false;
}


/// Destructor.
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

    // strip all characters except a-z, underscore and space
    xmlTagName = xmlTagName.retainCharacters(
                     "abcdefghijklmnopqrstuvwxyz_ ");

    // change all spaces to underscores
    xmlTagName = xmlTagName.replace(" ", "_");

    // remove double underscores
    xmlTagName = xmlTagName.replace("__", "_");
}


/// Get parameter's XML tag name.
///
/// @return XML tag name
///
String PluginParameter::getTagName()
{
    return xmlTagName;
}


/// Get **internal** default value as float.  Values range from 0.0 to
/// 1.0.
///
/// @return internal default value (between 0.0 and 1.0)
///
float PluginParameter::getDefaultFloat()
{
    return defaultValue;
}


/// Get **real** default value as float.  Values range from the
/// parameter's minimum value to its maximum value.
///
/// @return default value
///
float PluginParameter::getDefaultRealFloat()
{
    return defaultRealValue;
}


/// Get default value as Boolean.
///
/// @return **false** if the default value is set to the parameter's
///         minimum, **true** otherwise
///
bool PluginParameter::getDefaultBoolean()
{
    return getDefaultFloat() != 0.0f;
}


/// Get **real** default value as integer.  Values range from the
/// parameter's (rounded) minimum value to its (rounded) maximum
/// value.
///
/// @return default value
///
int PluginParameter::getDefaultRealInteger()
{
    return round_mz(getDefaultRealFloat());
}


/// Get **internal** parameter value as float.  Values range from 0.0
/// to 1.0.
///
/// @return current value (between 0.0 and 1.0)
///
float PluginParameter::getFloat()
{
    return value;
}


/// Get **real** parameter value as float.  Values range from the
/// parameter's minimum value to its maximum value.
///
/// @return current value
///
float PluginParameter::getRealFloat()
{
    return realValue;
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


/// Set **real** parameter value from integer.  The new value must be
/// in the defined range of the parameter's values.
///
/// @param newRealValue new value
///
void PluginParameter::setRealInteger(int newRealValue)
{
    setRealFloat((float) newRealValue);
}


/// Get parameter value as Boolean.
///
/// @return **false** if current value is at its minimum, **true**
///         otherwise
///
bool PluginParameter::getBoolean()
{
    return getFloat() != 0.0f;
}


/// Get parameter value as formatted string.
///
/// @return current value
///
String PluginParameter::getText()
{
    return textValue;
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
/// @param xmlDocument XML document to load from
///
void PluginParameter::loadFromXml(XmlElement *xmlDocument)
{
    float realValue;

    // get parameter's element from XML document
    XmlElement *xmlParameter = xmlDocument->getChildByName(getTagName());

    // parameter's element found
    if (xmlParameter)
    {
        // get stored value from attribute "value" (or use default
        // real value)
        realValue = (float) xmlParameter->getDoubleAttribute("value", getDefaultRealFloat());
    }
    // otherwise
    else
    {
        // use default real value
        realValue = getDefaultRealFloat();
    }

    // update real value
    setRealFloat(realValue);
}


/// Store parameter value as XML.
///
/// @param xmlDocument XML document to store in
///
void PluginParameter::storeAsXml(XmlElement *xmlDocument)
{
    // create new XML element with parameter's tag name (will be
    // deleted by XML document)
    XmlElement *xmlParameter = new XmlElement(getTagName());

    // XML element was successfully created
    if (xmlParameter)
    {
        // get current real value
        float realValue = getRealFloat();

        // set attribute "value" to current value
        xmlParameter->setAttribute("value", realValue);

        // add new element to XML document
        xmlDocument->addChildElement(xmlParameter);
    }
}


/// Should parameter be spared from deletion in destructor of
/// ParameterJuggler?
///
/// Sorry, I know this sort of method is *really* ugly, but I
/// currently see no other way to implement PluginParameterCombined.
///
/// @return **true** if parameter should be spared, **false**
///         otherwise
///
bool PluginParameter::saveFromDeletion()
{
    return doNotDelete;
}


/// Round given value to nearest integer.  Will always round **away**
/// from zero!
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
