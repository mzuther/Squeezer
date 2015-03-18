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

#include "plugin_parameter_combined.h"


PluginParameterCombined::PluginParameterCombined(float real_minimum, float real_maximum, float resolution, float log_factor, int decimal_places) :
    paramModeSwitch("Discrete", "Continuous", true),
    paramSwitch(true),
    paramContinuous(real_minimum, real_maximum, resolution, log_factor, decimal_places, true)
{
    bUseConstants = true;
    paramModeSwitch.setBoolean(bUseConstants);
}


PluginParameterCombined::~PluginParameterCombined()
{
}


void PluginParameterCombined::setName(const String &strParameterName)
{
    // call base class method
    PluginParameter::setName(strParameterName);

    paramModeSwitch.setName(strParameterName + " Mode");
}


void PluginParameterCombined::addConstant(const float fRealValue, const String &strText)
{
    paramSwitch.addConstant(fRealValue, strText);
}


bool PluginParameterCombined::getMode()
{
    return bUseConstants;
}


bool PluginParameterCombined::setMode(bool use_constants)
{
    if (bUseConstants != use_constants)
    {
        return toggleMode();
    }
    else
    {
        return true;
    }
}


bool PluginParameterCombined::toggleMode()
{
    paramModeSwitch.toggleState();
    bUseConstants = paramModeSwitch.getBoolean();

    if (bUseConstants)
    {
        float fRealValue = paramContinuous.getRealFloat();
        return paramSwitch.setNearestRealFloat(fRealValue);
    }
    else
    {
        float fRealValue = paramSwitch.getRealFloat();
        return paramContinuous.setNearestRealFloat(fRealValue);
    }
}


PluginParameterToggleSwitch *PluginParameterCombined::getModeSwitch()
{
    // this pointer must not be deleted outside of this class!
    return &paramModeSwitch;
}


float PluginParameterCombined::getInterval()
{
    if (bUseConstants)
    {
        return paramSwitch.getInterval();
    }
    else
    {
        return paramContinuous.getInterval();
    }
}


float PluginParameterCombined::getDefaultFloat()
{
    if (bUseConstants)
    {
        return paramSwitch.getDefaultFloat();
    }
    else
    {
        return paramContinuous.getDefaultFloat();
    }
}


float PluginParameterCombined::getDefaultRealFloat()
{
    if (bUseConstants)
    {
        return paramSwitch.getDefaultRealFloat();
    }
    else
    {
        return paramContinuous.getDefaultRealFloat();
    }
}


bool PluginParameterCombined::getDefaultBoolean()
{
    if (bUseConstants)
    {
        return paramSwitch.getDefaultBoolean();
    }
    else
    {
        return paramContinuous.getDefaultBoolean();
    }
}


int PluginParameterCombined::getDefaultRealInteger()
{
    if (bUseConstants)
    {
        return paramSwitch.getDefaultRealInteger();
    }
    else
    {
        return paramContinuous.getDefaultRealInteger();
    }
}


bool PluginParameterCombined::setDefaultRealFloat(float fRealValue, bool updateValue)
{
    if (bUseConstants)
    {
        bool bSwitchFound = paramSwitch.setDefaultRealFloat(fRealValue, updateValue);
        bool bContinuousFound = paramContinuous.setDefaultRealFloat(fRealValue, false);

        return (bSwitchFound && bContinuousFound);
    }
    else
    {
        bool bContinuousFound = paramContinuous.setDefaultRealFloat(fRealValue, updateValue);
        bool bSwitchFound = paramSwitch.setDefaultRealFloat(fRealValue, true);

        return (bContinuousFound && bSwitchFound);
    }
}


float PluginParameterCombined::getFloat()
{
    if (bUseConstants)
    {
        return paramSwitch.getFloat();
    }
    else
    {
        return paramContinuous.getFloat();
    }
}


bool PluginParameterCombined::setFloat(float fValue)
{
    if (bUseConstants)
    {
        return paramSwitch.setFloat(fValue);
    }
    else
    {
        return paramContinuous.setFloat(fValue);
    }
}


float PluginParameterCombined::getRealFloat()
{
    if (bUseConstants)
    {
        return paramSwitch.getRealFloat();
    }
    else
    {
        return paramContinuous.getRealFloat();
    }
}


bool PluginParameterCombined::setRealFloat(float fRealValue)
{
    if (bUseConstants)
    {
        return paramSwitch.setRealFloat(fRealValue);
    }
    else
    {
        return paramContinuous.setRealFloat(fRealValue);
    }
}


bool PluginParameterCombined::getBoolean()
{
    if (bUseConstants)
    {
        return paramSwitch.getBoolean();
    }
    else
    {
        return paramContinuous.getBoolean();
    }
}


bool PluginParameterCombined::setBoolean(bool bValue)
{
    if (bUseConstants)
    {
        return paramSwitch.setBoolean(bValue);
    }
    else
    {
        return paramContinuous.setBoolean(bValue);
    }
}


int PluginParameterCombined::getRealInteger()
{
    if (bUseConstants)
    {
        return paramSwitch.getRealInteger();
    }
    else
    {
        return paramContinuous.getRealInteger();
    }
}


bool PluginParameterCombined::setRealInteger(int nRealValue)
{
    if (bUseConstants)
    {
        return paramSwitch.setRealInteger(nRealValue);
    }
    else
    {
        return paramContinuous.setRealInteger(nRealValue);
    }
}


String PluginParameterCombined::getText()
{
    if (bUseConstants)
    {
        return paramSwitch.getText();
    }
    else
    {
        return paramContinuous.getText();
    }
}


bool PluginParameterCombined::setText(const String &strText)
{
    if (bUseConstants)
    {
        return paramSwitch.setText(strText);
    }
    else
    {
        return paramContinuous.setText(strText);
    }
}


void PluginParameterCombined::setSuffix(const String &suffix)
{
    paramContinuous.setSuffix(suffix);
}


float PluginParameterCombined::getFloatFromText(const String &strText)
{
    if (bUseConstants)
    {
        return paramSwitch.getFloatFromText(strText);
    }
    else
    {
        return paramContinuous.getFloatFromText(strText);
    }
}


String PluginParameterCombined::getTextFromFloat(float fValue)
{
    if (bUseConstants)
    {
        return paramSwitch.getTextFromFloat(fValue);
    }
    else
    {
        return paramContinuous.getTextFromFloat(fValue);
    }
}


bool PluginParameterCombined::hasChanged()
{
    bool bChangedSwitch = paramSwitch.hasChanged();
    bool bChangedContinuous = paramContinuous.hasChanged();

    return (bChangedSwitch || bChangedContinuous);
}


void PluginParameterCombined::clearChangeFlag()
{
    paramSwitch.clearChangeFlag();
    paramContinuous.clearChangeFlag();
}


bool PluginParameterCombined::saveFromDeletion()
{
    return false;
}


void PluginParameterCombined::loadFromXml(XmlElement *xml)
{
    XmlElement *xml_element = xml->getChildByName(getTagName());

    if (xml_element)
    {
        bool useConstants = xml_element->getBoolAttribute("use_constants", true);
        float fRealValue;

        if (xml_element->hasAttribute("value"))
        {
            fRealValue = (float) xml_element->getDoubleAttribute("value", getDefaultRealFloat());
        }
        else
        {
            String strRealValue = xml_element->getAllSubText().trim();

            if (strRealValue.isEmpty())
            {
                fRealValue = getDefaultRealFloat();
            }
            else
            {
                fRealValue = strRealValue.getFloatValue();
            }
        }

        setMode(useConstants);
        setRealFloat(fRealValue);
    }
}


void PluginParameterCombined::storeAsXml(XmlElement *xml)
{
    XmlElement *xml_element = new XmlElement(getTagName());

    if (xml_element)
    {
        float fRealValue = getRealFloat();

        xml_element->setAttribute("use_constants", bUseConstants ? "true" : "false");
        xml_element->setAttribute("value", fRealValue);
        xml->addChildElement(xml_element);
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
