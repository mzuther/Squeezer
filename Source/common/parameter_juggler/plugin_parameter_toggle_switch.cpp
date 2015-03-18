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

#include "plugin_parameter_toggle_switch.h"


PluginParameterToggleSwitch::PluginParameterToggleSwitch(const String &state_on, const String &state_off, bool save_from_deletion)
{
    strStateOn = state_on;
    strStateOff = state_off;

    bDefaultState = false;
    bState = bDefaultState;

    bSaveFromDeletion = save_from_deletion;

    setChangeFlag();
}


PluginParameterToggleSwitch::~PluginParameterToggleSwitch()
{
}


void PluginParameterToggleSwitch::toggleState()
{
    bState = !bState;
    setChangeFlag();
}


float PluginParameterToggleSwitch::getDefaultFloat()
{
    return bDefaultState ? 1.0f : 0.0f;
}


float PluginParameterToggleSwitch::getDefaultRealFloat()
{
    return bDefaultState ? 1.0f : 0.0f;
}


bool PluginParameterToggleSwitch::getDefaultBoolean()
{
    return bDefaultState;
}


int PluginParameterToggleSwitch::getDefaultRealInteger()
{
    return bDefaultState ? 1 : 0;
}


bool PluginParameterToggleSwitch::setDefaultBoolean(bool bValue, bool updateValue)
{
    bool bDefaultState = bValue;

    if (updateValue)
    {
        setBoolean(bDefaultState);
    }

    return true;
}


bool PluginParameterToggleSwitch::setDefaultRealFloat(float fRealValue, bool updateValue)
{
    return setDefaultBoolean((fRealValue != 0.0f), updateValue);
}


float PluginParameterToggleSwitch::getFloat()
{
    return bState ? 1.0f : 0.0f;
}


bool PluginParameterToggleSwitch::setFloat(float fValue)
{
    return setBoolean(fValue != 0.0f);
}


float PluginParameterToggleSwitch::getRealFloat()
{
    return bState ? 1.0f : 0.0f;
}


bool PluginParameterToggleSwitch::setRealFloat(float fRealValue)
{
    return setBoolean(fRealValue != 0.0f);
}


bool PluginParameterToggleSwitch::getBoolean()
{
    return bState;
}


bool PluginParameterToggleSwitch::setBoolean(bool bValue)
{
    if (bValue != bState)
    {
        toggleState();
    }

    return true;
}


int PluginParameterToggleSwitch::getRealInteger()
{
    return bState ? 1 : 0;
}


bool PluginParameterToggleSwitch::setRealInteger(int nRealValue)
{
    return setBoolean(nRealValue != 0);
}


String PluginParameterToggleSwitch::getText()
{
    return bState ? strStateOn : strStateOff;
}


bool PluginParameterToggleSwitch::setText(const String &strText)
{
    return setBoolean(strText.compare(strStateOn) == 0);
}


bool PluginParameterToggleSwitch::hasChanged()
{
    return bChangedValue;
}


void PluginParameterToggleSwitch::clearChangeFlag()
{
    bChangedValue = false;
}


void PluginParameterToggleSwitch::setChangeFlag()
{
    bChangedValue = true;
}


bool PluginParameterToggleSwitch::saveFromDeletion()
{
    return bSaveFromDeletion;
}


void PluginParameterToggleSwitch::loadFromXml(XmlElement *xml)
{
    XmlElement *xml_element = xml->getChildByName(getTagName());

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


void PluginParameterToggleSwitch::storeAsXml(XmlElement *xml)
{
    XmlElement *xml_element = new XmlElement(getTagName());

    if (xml_element)
    {
        xml_element->setAttribute("value", bState ? "true" : "false");
        xml->addChildElement(xml_element);
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
