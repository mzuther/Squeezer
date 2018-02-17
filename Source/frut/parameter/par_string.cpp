/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2018 Martin Zuther (http://www.mzuther.de/)

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


// The methods of this class may be called on the audio thread, so
// they are absolutely time-critical!

/// Create a parameter for storing a string.  **Note: several
/// functions must not be used -- see below!**
///
/// @param newValue intial value
///
ParString::ParString(const String &newValue)
{
    // initialise value
    setText(newValue);

    // mark parameter as changed
    setChangeFlag();
}


/// Set **real** default value from float.  **This function must not
/// be used!**
///
/// @param newRealValue do not use
///
/// @param updateParameter do not use
///
void ParString::setDefaultRealFloat(float newRealValue, bool updateParameter)
{
    ignoreUnused(newRealValue, updateParameter);

    // do nothing till you hear from me ...
    jassert(false);
}


/// Set **internal** parameter value from float.  **This function must not
/// be used!**
///
/// @param newValue do not use
///
void ParString::setFloat(float newValue)
{
    ignoreUnused(newValue);

    // do nothing till you hear from me ...
    jassert(false);
}


/// Set **real** parameter value from float.  **This function must not
/// be used!**
///
/// @param newRealValue do not use
///
void ParString::setRealFloat(float newRealValue)
{
    ignoreUnused(newRealValue);

    // do nothing till you hear from me ...
    jassert(false);
}


/// Get parameter value as formatted string.
///
/// @return current value
///
const String ParString::getText()
{
    return textValue;
}


/// Set parameter value from string.
///
/// @param newValue new parameter value
///
void ParString::setText(const String &newValue)
{
    // mark parameter as changed if necessary
    if (textValue.compare(newValue) != 0)
    {
        setChangeFlag();
    }

    // update value
    textValue = newValue;
}


/// Transform string to **internal** parameter value.  **This function
/// must not be used!**
///
/// @param newValue correctly formatted string
///
/// @return **internal** value
///
float ParString::getFloatFromText(const String &newValue)
{
    ignoreUnused(newValue);

    // do nothing till you hear from me ...
    jassert(false);

    return 0.0f;
}


/// Transform **internal** value to string.  **This function must not
/// be used!**
///
/// @param newValue **internal** value
///
/// @return formatted string
///
const String ParString::getTextFromFloat(float newValue)
{
    ignoreUnused(newValue);

    // do nothing till you hear from me ...
    jassert(false);

    return String::empty;
}


/// Load parameter value from XML.
///
/// @param xmlDocument XML document to load from
///
void ParString::loadFromXml(XmlElement *xmlDocument)
{
    // get parameter's element from XML document
    XmlElement *xmlParameter = xmlDocument->getChildByName(getTagName());

    // parameter's element found
    if (xmlParameter)
    {
        // get stored value from attribute "value" (or use empty string)
        String newValue = xmlParameter->getStringAttribute("value", String::empty);

        // update value
        setText(newValue);
    }
}


/// Store parameter value as XML.
///
/// @param xmlDocument XML document to store in
///
void ParString::storeAsXml(XmlElement *xmlDocument)
{
    // create new XML element with parameter's tag name (will be
    // deleted by XML document)
    XmlElement *xmlParameter = new XmlElement(getTagName());

    // XML element was successfully created
    if (xmlParameter)
    {
        // set attribute "value" to current value
        xmlParameter->setAttribute("value", getText());

        // add new element to XML document
        xmlDocument->addChildElement(xmlParameter);
    }
}
