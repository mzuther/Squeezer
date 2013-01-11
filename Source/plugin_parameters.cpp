/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose compressor with a touch of lemon

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

#include "plugin_parameters.h"


// The methods of this class may be called on the audio thread, so
// they are absolutely time-critical!

// The methods of this class may be called on the audio thread, so
// they are absolutely time-critical!

PluginParameters::PluginParameters()
{
    strSettingsID = "SETTINGS";
    nNumParameters = 0;
}


PluginParameters::~PluginParameters()
{
    for (int nIndex = 0; nIndex < nNumParameters; nIndex++)
    {
        delete arrParameters[nIndex];
    }

    arrParameters.clear();

    removeAllActionListeners();
}


String PluginParameters::toString()
{
    String strParameters;

    for (int nIndex = 0; nIndex < nNumParameters; nIndex++)
    {
        strParameters += arrParameters[nIndex]->getName() + ":  ";
        strParameters += arrParameters[nIndex]->getText() + "\n";
    }

    return strParameters;
}


int PluginParameters::getNumParameters(bool bIncludeHiddenParameters)
{
    if (bIncludeHiddenParameters)
    {
        return nNumParametersComplete;
    }
    else
    {
        return nNumParametersRevealed;
    }
}


void PluginParameters::add(WrappedParameter* parameter)
{
    arrParameters.add(parameter);
    nNumParameters = arrParameters.size();
}


String PluginParameters::getName(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getName();
}


void PluginParameters::setName(int nIndex, const String& strParameterName)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    arrParameters[nIndex]->setName(strParameterName);
}


float PluginParameters::getInterval(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getInterval();
}


float PluginParameters::getDefaultFloat(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getDefaultFloat();
}


bool PluginParameters::getDefaultBoolean(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getDefaultBoolean();
}


int PluginParameters::getDefaultInteger(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getDefaultInteger();
}


void PluginParameters::setDefaultString(int nIndex, const String& strDefault, bool updateValue)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    arrParameters[nIndex]->setDefaultString(strDefault, updateValue);
}


float PluginParameters::getFloat(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getFloat();
}


void PluginParameters::setFloat(int nIndex, float fValue)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    arrParameters[nIndex]->setFloat(fValue);

    if (arrParameters[nIndex]->hasChanged())
    {
        // "PC" --> parameter changed, followed by a hash and the
        // parameter's ID
        sendActionMessage("PC#" + String(nIndex));
    }
}


float PluginParameters::getRealFloat(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getRealFloat();
}


void PluginParameters::setRealFloat(int nIndex, float fValue)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    arrParameters[nIndex]->setRealFloat(fValue);
}


bool PluginParameters::getBoolean(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getBoolean();
}


void PluginParameters::setBoolean(int nIndex, bool bValue)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    arrParameters[nIndex]->setBoolean(bValue);
}


int PluginParameters::getInteger(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getInteger();
}


void PluginParameters::setInteger(int nIndex, int nValue)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    arrParameters[nIndex]->setInteger(nValue);

    if (arrParameters[nIndex]->hasChanged())
    {
        // "PC" --> parameter changed, followed by a hash and the
        // parameter's ID
        sendActionMessage("PC#" + String(nIndex));
    }
}


int PluginParameters::getRealInteger(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getRealInteger();
}


void PluginParameters::setRealInteger(int nIndex, int nValue)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    arrParameters[nIndex]->setRealInteger(nValue);
}


String PluginParameters::getText(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getText();
}


void PluginParameters::setText(int nIndex, const String& strText)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    arrParameters[nIndex]->setText(strText);
}


float PluginParameters::getFloatFromText(int nIndex, const String& strText)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getFloatFromText(strText);
}


String PluginParameters::getTextFromFloat(int nIndex, float fValue)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getTextFromFloat(fValue);
}


int PluginParameters::getIntegerFromText(int nIndex, const String& strText)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getIntegerFromText(strText);
}


String PluginParameters::getTextFromInteger(int nIndex, int nValue)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getTextFromInteger(nValue);
}


bool PluginParameters::hasChanged(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->hasChanged();
}


void PluginParameters::clearChangeFlag(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->clearChangeFlag();
}


void PluginParameters::setChangeFlag(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->setChangeFlag();
}


void PluginParameters::loadFromXml(XmlElement* xml)
{
    if (xml && xml->hasTagName(strSettingsID))
    {
        for (int nIndex = 0; nIndex < nNumParameters; nIndex++)
        {
            arrParameters[nIndex]->loadFromXml(xml);
        }
    }
}


XmlElement PluginParameters::storeAsXml()
{
    XmlElement xml(strSettingsID);

    for (int nIndex = 0; nIndex < nNumParameters; nIndex++)
    {
        arrParameters[nIndex]->storeAsXml(&xml);
    }

    return xml;
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
