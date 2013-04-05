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

#include "wrapped_parameter_toggle_switch.h"


WrappedParameterToggleSwitch::WrappedParameterToggleSwitch(const String& state_on, const String& state_off)
{
    strName = "";
    strAttribute = "";

    strStateOn = state_on;
    strStateOff = state_off;

    bDefaultState = false;
    bState = bDefaultState;

    setChangeFlag();
}


WrappedParameterToggleSwitch::~WrappedParameterToggleSwitch()
{
}


String WrappedParameterToggleSwitch::getName()
{
    return strName;
}


void WrappedParameterToggleSwitch::setName(const String& strParameterName)
{
    strName = strParameterName;
    strAttribute = strName.removeCharacters(" ");
}


float WrappedParameterToggleSwitch::getInterval()
{
    return 1.0f;
}


void WrappedParameterToggleSwitch::toggleState()
{
    bState = !bState;
    setChangeFlag();
}


float WrappedParameterToggleSwitch::getDefaultFloat()
{
    return bDefaultState ? 1.0f : 0.0f;
}


float WrappedParameterToggleSwitch::getDefaultRealFloat()
{
    return bDefaultState ? 1.0f : 0.0f;
}


bool WrappedParameterToggleSwitch::getDefaultBoolean()
{
    return bDefaultState;
}


int WrappedParameterToggleSwitch::getDefaultRealInteger()
{
    return bDefaultState ? 1 : 0;
}


bool WrappedParameterToggleSwitch::setDefaultBoolean(bool bValue, bool updateValue)
{
    bool bDefaultState = bValue;

    if (updateValue)
    {
        setBoolean(bDefaultState);
    }

    return true;
}


bool WrappedParameterToggleSwitch::setDefaultRealFloat(float fRealValue, bool updateValue)
{
    return setDefaultBoolean((fRealValue != 0.0f), updateValue);
}


float WrappedParameterToggleSwitch::getFloat()
{
    return bState ? 1.0f : 0.0f;
}


bool WrappedParameterToggleSwitch::setFloat(float fValue)
{
    return setBoolean(fValue != 0.0f);
}


float WrappedParameterToggleSwitch::getRealFloat()
{
    return bState ? 1.0f : 0.0f;
}


bool WrappedParameterToggleSwitch::setRealFloat(float fRealValue)
{
    return setBoolean(fRealValue != 0.0f);
}


bool WrappedParameterToggleSwitch::getBoolean()
{
    return bState;
}


bool WrappedParameterToggleSwitch::setBoolean(bool bValue)
{
    if (bValue != bState)
    {
        toggleState();
    }

    return true;
}


int WrappedParameterToggleSwitch::getRealInteger()
{
    return bState ? 1 : 0;
}


bool WrappedParameterToggleSwitch::setRealInteger(int nRealValue)
{
    return setBoolean(nRealValue != 0);
}


String WrappedParameterToggleSwitch::getText()
{
    return bState ? strStateOn : strStateOff;
}


bool WrappedParameterToggleSwitch::setText(const String& strText)
{
    return setBoolean(strText.compare(strStateOn) == 0);
}


float WrappedParameterToggleSwitch::getFloatFromText(const String& strText)
{
    return (strText.compare(strStateOn) == 0) ? 1.0f : 0.0f;
}


String WrappedParameterToggleSwitch::getTextFromFloat(float fValue)
{
    return (fValue == 0.0f) ? strStateOff : strStateOn;
}


bool WrappedParameterToggleSwitch::hasChanged()
{
    return bChangedValue;
}


void WrappedParameterToggleSwitch::clearChangeFlag()
{
    bChangedValue = false;
}


void WrappedParameterToggleSwitch::setChangeFlag()
{
    bChangedValue = true;
}


void WrappedParameterToggleSwitch::loadFromXml(XmlElement* xml)
{
    XmlElement* xml_element = xml->getChildByName(strAttribute);

    if (xml_element)
    {
        bool bStateNew;

        if (xml_element->hasAttribute("value"))
        {
            bStateNew = xml_element->getBoolAttribute("value", getDefaultBoolean());
        }
        else
        {
            String strStateInteger = xml_element->getAllSubText().trim();

            if (strStateInteger.isEmpty())
            {
                bStateNew = getDefaultBoolean();
            }
            else
            {
                bStateNew = (strStateInteger.getIntValue() != 0);
            }
        }

        setBoolean(bStateNew);
    }
}


void WrappedParameterToggleSwitch::storeAsXml(XmlElement* xml)
{
    XmlElement* xml_element = new XmlElement(strAttribute);

    if (xml_element)
    {
        xml_element->setAttribute("value", bState ? "true" : "false");
        xml->addChildElement(xml_element);
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
