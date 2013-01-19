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


WrappedParameterCombined::WrappedParameterCombined(float real_minimum, float real_maximum, float log_factor)
{
    strName = "";
    strAttribute = "";

    pSwitch = new WrappedParameterSwitch();
    pContinuous = new WrappedParameterContinuous(real_minimum, real_maximum, log_factor);

    bUseConstants = true;
    setChangeFlag();
}


WrappedParameterCombined::~WrappedParameterCombined()
{
    delete pSwitch;
    pSwitch = NULL;

    delete pContinuous;
    pContinuous = NULL;
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
    if (bUseConstants)
    {
        return pSwitch->getInterval();
    }
    else
    {
        return pContinuous->getInterval();
    }
}


float WrappedParameterCombined::getDefaultFloat()
{
    if (bUseConstants)
    {
        return pSwitch->getDefaultFloat();
    }
    else
    {
        return pContinuous->getDefaultFloat();
    }
}


float WrappedParameterCombined::getDefaultRealFloat()
{
    if (bUseConstants)
    {
        return pSwitch->getDefaultRealFloat();
    }
    else
    {
        return pContinuous->getDefaultRealFloat();
    }
}


bool WrappedParameterCombined::getDefaultRealBoolean()
{
    if (bUseConstants)
    {
        return pSwitch->getDefaultRealBoolean();
    }
    else
    {
        return pContinuous->getDefaultRealBoolean();
    }
}


int WrappedParameterCombined::getDefaultRealInteger()
{
    if (bUseConstants)
    {
        return pSwitch->getDefaultRealInteger();
    }
    else
    {
        return pContinuous->getDefaultRealInteger();
    }
}


bool WrappedParameterCombined::setDefaultRealFloat(float fRealValue, bool updateValue)
{
    if (bUseConstants)
    {
        return pSwitch->setDefaultRealFloat(fRealValue, updateValue);
    }
    else
    {
        return pContinuous->setDefaultRealFloat(fRealValue, updateValue);
    }
}


float WrappedParameterCombined::getFloat()
{
    if (bUseConstants)
    {
        return pSwitch->getFloat();
    }
    else
    {
        return pContinuous->getFloat();
    }
}


bool WrappedParameterCombined::setFloat(float fValue)
{
    if (bUseConstants)
    {
        return pSwitch->setFloat(fValue);
    }
    else
    {
        return pContinuous->setFloat(fValue);
    }
}


float WrappedParameterCombined::getRealFloat()
{
    if (bUseConstants)
    {
        return pSwitch->getRealFloat();
    }
    else
    {
        return pContinuous->getRealFloat();
    }
}


bool WrappedParameterCombined::setRealFloat(float fRealValue)
{
    if (bUseConstants)
    {
        return pSwitch->setRealFloat(fRealValue);
    }
    else
    {
        return pContinuous->setRealFloat(fRealValue);
    }
}


bool WrappedParameterCombined::getBoolean()
{
    if (bUseConstants)
    {
        return pSwitch->getBoolean();
    }
    else
    {
        return pContinuous->getBoolean();
    }
}


bool WrappedParameterCombined::setBoolean(bool bValue)
{
    if (bUseConstants)
    {
        return pSwitch->setBoolean(bValue);
    }
    else
    {
        return pContinuous->setBoolean(bValue);
    }
}


int WrappedParameterCombined::getRealInteger()
{
    if (bUseConstants)
    {
        return pSwitch->getRealInteger();
    }
    else
    {
        return pContinuous->getRealInteger();
    }
}


bool WrappedParameterCombined::setRealInteger(int nRealValue)
{
    if (bUseConstants)
    {
        return pSwitch->setRealInteger(nRealValue);
    }
    else
    {
        return pContinuous->setRealInteger(nRealValue);
    }
}


String WrappedParameterCombined::getText()
{
    if (bUseConstants)
    {
        return pSwitch->getText();
    }
    else
    {
        return pContinuous->getText();
    }
}


bool WrappedParameterCombined::setText(const String& strText)
{
    if (bUseConstants)
    {
        return pSwitch->setText(strText);
    }
    else
    {
        return pContinuous->setText(strText);
    }
}


float WrappedParameterCombined::getFloatFromText(const String& strText)
{
    if (bUseConstants)
    {
        return pSwitch->getFloatFromText(strText);
    }
    else
    {
        return pContinuous->getFloatFromText(strText);
    }
}


String WrappedParameterCombined::getTextFromFloat(float fValue)
{
    if (bUseConstants)
    {
        return pSwitch->getTextFromFloat(fValue);
    }
    else
    {
        return pContinuous->getTextFromFloat(fValue);
    }
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
