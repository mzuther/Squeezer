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

#include "parameter_juggler.h"


// The methods of this class may be called on the audio thread, so
// they are absolutely time-critical!

// The methods of this class may be called on the audio thread, so
// they are absolutely time-critical!

ParameterJuggler::ParameterJuggler()
{
    strSettingsID = "SETTINGS";
    nNumParameters = 0;
}


ParameterJuggler::~ParameterJuggler()
{
    while (arrParameters.size())
    {
        WrappedParameter *parameter = arrParameters.remove(0);

        // do not delete elements of combined parameters!
        if (!parameter->saveFromDeletion())
        {
            delete parameter;
            parameter = nullptr;
        }
    }

    arrParameters.clear();
    arrMayModify.clear();
}


WrappedParameter *ParameterJuggler::getWrappedParameter(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex];
}


String ParameterJuggler::toString()
{
    String strParameters;

    for (int nIndex = 0; nIndex < nNumParameters; nIndex++)
    {
        strParameters += arrParameters[nIndex]->getName() + ":  ";
        strParameters += arrParameters[nIndex]->getText() + "\n";
    }

    return strParameters;
}


int ParameterJuggler::getNumParameters(bool bIncludeHiddenParameters)
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


void ParameterJuggler::add(WrappedParameter *parameter, int nIndex, bool mayModify)
{
    arrParameters.add(parameter);
    arrMayModify.add(mayModify);
    nNumParameters = arrParameters.size();

    jassert(nNumParameters == nIndex + 1);
}


void ParameterJuggler::addCombined(WrappedParameterCombined *parameter, int nIndex, int nIndexSwitch, bool mayModify)
{
    jassert(nIndex == (nIndexSwitch + 1));

    add(parameter->getModeSwitch(), nIndexSwitch, false);
    add(parameter, nIndex, mayModify);
}


String ParameterJuggler::getName(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getName();
}


void ParameterJuggler::setName(int nIndex, const String &strParameterName)
{
    jassert(arrMayModify[nIndex]);
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    arrParameters[nIndex]->setName(strParameterName);
}


float ParameterJuggler::getInterval(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getInterval();
}


float ParameterJuggler::getDefaultFloat(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getDefaultFloat();
}


float ParameterJuggler::getDefaultRealFloat(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getDefaultRealFloat();
}


bool ParameterJuggler::getDefaultBoolean(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getDefaultBoolean();
}


int ParameterJuggler::getDefaultRealInteger(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getDefaultRealInteger();
}


bool ParameterJuggler::setDefaultRealFloat(int nIndex, float fRealValue, bool updateValue)
{
    jassert(arrMayModify[nIndex]);
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->setDefaultRealFloat(fRealValue, updateValue);
}


float ParameterJuggler::getFloat(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getFloat();
}


bool ParameterJuggler::setFloat(int nIndex, float fValue)
{
    jassert(arrMayModify[nIndex]);
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->setFloat(fValue);
}


float ParameterJuggler::getRealFloat(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getRealFloat();
}


bool ParameterJuggler::setRealFloat(int nIndex, float fRealValue)
{
    jassert(arrMayModify[nIndex]);
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->setRealFloat(fRealValue);
}


bool ParameterJuggler::getBoolean(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getBoolean();
}


bool ParameterJuggler::setBoolean(int nIndex, bool bValue)
{
    jassert(arrMayModify[nIndex]);
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->setBoolean(bValue);
}


int ParameterJuggler::getRealInteger(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getRealInteger();
}


bool ParameterJuggler::setRealInteger(int nIndex, int nRealValue)
{
    jassert(arrMayModify[nIndex]);
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->setRealInteger(nRealValue);
}


String ParameterJuggler::getText(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getText();
}


bool ParameterJuggler::setText(int nIndex, const String &strText)
{
    jassert(arrMayModify[nIndex]);
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->setText(strText);
}


float ParameterJuggler::getFloatFromText(int nIndex, const String &strText)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getFloatFromText(strText);
}


String ParameterJuggler::getTextFromFloat(int nIndex, float fValue)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->getTextFromFloat(fValue);
}


bool ParameterJuggler::hasChanged(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->hasChanged();
}


void ParameterJuggler::clearChangeFlag(int nIndex)
{
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->clearChangeFlag();
}


void ParameterJuggler::setChangeFlag(int nIndex)
{
    jassert(arrMayModify[nIndex]);
    jassert((nIndex >= 0) && (nIndex < nNumParameters));
    return arrParameters[nIndex]->setChangeFlag();
}


void ParameterJuggler::loadFromXml(XmlElement *xml)
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


XmlElement ParameterJuggler::storeAsXml()
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
