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

#include "wrapped_parameter_combined.h"


WrappedParameterCombined::WrappedParameterCombined(float real_minimum, float real_maximum, float resolution, float log_factor, int decimal_places) :
    paramModeSwitch("Discrete", "Continuous", true),
    paramSwitch(true),
    paramContinuous(real_minimum, real_maximum, resolution, log_factor, decimal_places, true)

{
    strName = String::empty;
    strAttribute = String::empty;

    bUseConstants = true;
    paramModeSwitch.setBoolean(bUseConstants);

    setChangeFlag();
    setChangeFlagMode();
}


WrappedParameterCombined::~WrappedParameterCombined()
{
}


String WrappedParameterCombined::getName()
{
    return strName;
}


void WrappedParameterCombined::setName(const String &strParameterName)
{
    strName = strParameterName;
    strAttribute = strName.removeCharacters(" ");

    paramModeSwitch.setName(strName + " Mode");
}


void WrappedParameterCombined::addConstant(const float fRealValue, const String &strText)
{
    paramSwitch.addConstant(fRealValue, strText);
}


bool WrappedParameterCombined::getMode()
{
    return bUseConstants;
}


bool WrappedParameterCombined::setMode(bool use_constants)
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


bool WrappedParameterCombined::toggleMode()
{
    paramModeSwitch.toggleState();
    bUseConstants = paramModeSwitch.getBoolean();

    setChangeFlag();
    setChangeFlagMode();

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


WrappedParameterToggleSwitch *WrappedParameterCombined::getModeSwitch()
{
    // this pointer must not be deleted outside of this class!
    return &paramModeSwitch;
}


float WrappedParameterCombined::getInterval()
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


float WrappedParameterCombined::getDefaultFloat()
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


float WrappedParameterCombined::getDefaultRealFloat()
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


bool WrappedParameterCombined::getDefaultBoolean()
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


int WrappedParameterCombined::getDefaultRealInteger()
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


bool WrappedParameterCombined::setDefaultRealFloat(float fRealValue, bool updateValue)
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


float WrappedParameterCombined::getFloat()
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


bool WrappedParameterCombined::setFloat(float fValue)
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


float WrappedParameterCombined::getRealFloat()
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


bool WrappedParameterCombined::setRealFloat(float fRealValue)
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


bool WrappedParameterCombined::getBoolean()
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


bool WrappedParameterCombined::setBoolean(bool bValue)
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


int WrappedParameterCombined::getRealInteger()
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


bool WrappedParameterCombined::setRealInteger(int nRealValue)
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


String WrappedParameterCombined::getText()
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


bool WrappedParameterCombined::setText(const String &strText)
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


void WrappedParameterCombined::setSuffix(const String &suffix)
{
    paramContinuous.setSuffix(suffix);
}


float WrappedParameterCombined::getFloatFromText(const String &strText)
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


String WrappedParameterCombined::getTextFromFloat(float fValue)
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


bool WrappedParameterCombined::hasChanged()
{
    bool bChangedSwitch = paramSwitch.hasChanged();
    bool bChangedContinuous = paramContinuous.hasChanged();

    return (bChangedSwitch || bChangedContinuous);
}


void WrappedParameterCombined::clearChangeFlag()
{
    paramSwitch.clearChangeFlag();
    paramContinuous.clearChangeFlag();
}


void WrappedParameterCombined::setChangeFlag()
{
    paramSwitch.setChangeFlag();
    paramContinuous.setChangeFlag();
}


bool WrappedParameterCombined::hasChangedMode()
{
    return bChangedMode;
}


void WrappedParameterCombined::clearChangeFlagMode()
{
    bChangedMode = false;
}


void WrappedParameterCombined::setChangeFlagMode()
{
    bChangedMode = true;
}


bool WrappedParameterCombined::saveFromDeletion()
{
    return false;
}


void WrappedParameterCombined::loadFromXml(XmlElement *xml)
{
    XmlElement *xml_element = xml->getChildByName(strAttribute);

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


void WrappedParameterCombined::storeAsXml(XmlElement *xml)
{
    XmlElement *xml_element = new XmlElement(strAttribute);

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
