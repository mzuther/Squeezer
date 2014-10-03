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
    arrParameters.clear();
    arrMayModify.clear();
}


WrappedParameter *PluginParameters::getWrappedParameter(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex];
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


void PluginParameters::add(WrappedParameter *parameter, int nIndex, bool mayModify)
{
    arrParameters.add(parameter);
    arrMayModify.add(mayModify);
    nNumParameters = arrParameters.size();

    jassert(nNumParameters == nIndex + 1);
}


void PluginParameters::addCombined(WrappedParameterCombined *parameter, int nIndex, int nIndexSwitch, bool mayModify)
{
    jassert(nIndex == (nIndexSwitch + 1));

    add(parameter->getModeSwitch(), nIndexSwitch, false);
    add(parameter, nIndex, mayModify);
}


String PluginParameters::getName(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getName();
}


void PluginParameters::setName(int nIndex, const String &strParameterName)
{
    jassert(arrMayModify[nIndex]);
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


float PluginParameters::getDefaultRealFloat(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getDefaultRealFloat();
}


bool PluginParameters::getDefaultBoolean(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getDefaultBoolean();
}


int PluginParameters::getDefaultRealInteger(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getDefaultRealInteger();
}


bool PluginParameters::setDefaultRealFloat(int nIndex, float fRealValue, bool updateValue)
{
    jassert(arrMayModify[nIndex]);
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->setDefaultRealFloat(fRealValue, updateValue);
}


float PluginParameters::getFloat(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getFloat();
}


bool PluginParameters::setFloat(int nIndex, float fValue)
{
    jassert(arrMayModify[nIndex]);
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->setFloat(fValue);
}


float PluginParameters::getRealFloat(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getRealFloat();
}


bool PluginParameters::setRealFloat(int nIndex, float fRealValue)
{
    jassert(arrMayModify[nIndex]);
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->setRealFloat(fRealValue);
}


bool PluginParameters::getBoolean(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getBoolean();
}


bool PluginParameters::setBoolean(int nIndex, bool bValue)
{
    jassert(arrMayModify[nIndex]);
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->setBoolean(bValue);
}


int PluginParameters::getRealInteger(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getRealInteger();
}


bool PluginParameters::setRealInteger(int nIndex, int nRealValue)
{
    jassert(arrMayModify[nIndex]);
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->setRealInteger(nRealValue);
}


String PluginParameters::getText(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getText();
}


bool PluginParameters::setText(int nIndex, const String &strText)
{
    jassert(arrMayModify[nIndex]);
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->setText(strText);
}


float PluginParameters::getFloatFromText(int nIndex, const String &strText)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getFloatFromText(strText);
}


String PluginParameters::getTextFromFloat(int nIndex, float fValue)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getTextFromFloat(fValue);
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
    jassert(arrMayModify[nIndex]);
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->setChangeFlag();
}


void PluginParameters::loadFromXml(XmlElement *xml)
{
    if (xml && xml->hasTagName(strSettingsID))
    {
        for (int nIndex = 0; nIndex < nNumParameters; nIndex++)
        {
            if (arrMayModify[nIndex])
            {
                arrParameters[nIndex]->loadFromXml(xml);
            }
        }
    }
}


XmlElement PluginParameters::storeAsXml()
{
    XmlElement xml(strSettingsID);
    xml.setAttribute("version", JucePlugin_VersionString);

    for (int nIndex = 0; nIndex < nNumParameters; nIndex++)
    {
        if (arrMayModify[nIndex])
        {
            arrParameters[nIndex]->storeAsXml(&xml);
        }
    }

    return xml;
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
