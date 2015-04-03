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

ParameterJuggler::ParameterJuggler()
{
    jugglerID = "SETTINGS";
    numberOfParameters = 0;
}


ParameterJuggler::~ParameterJuggler()
{
    while (arrParameters.size())
    {
        PluginParameter *parameter = arrParameters.remove(0);

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


PluginParameter *ParameterJuggler::getPluginParameter(int index)
{
    jassert(index >= 0);
    jassert(index < numberOfParameters);

    return arrParameters[index];
}


String ParameterJuggler::toString()
{
    String strParameters;

    for (int n = 0; n < numberOfParameters; n++)
    {
        strParameters += arrParameters[n]->getName() + ":  ";
        strParameters += arrParameters[n]->getText() + "\n";
    }

    return strParameters;
}


int ParameterJuggler::getNumParameters(bool includeHiddenParameters)
{
    if (includeHiddenParameters)
    {
        return numberOfParametersComplete;
    }
    else
    {
        return numberOfParametersRevealed;
    }
}


void ParameterJuggler::add(PluginParameter *parameter, int index, bool mayModify)
{
    arrParameters.add(parameter);
    arrMayModify.add(mayModify);
    numberOfParameters = arrParameters.size();

    jassert(numberOfParameters == index + 1);
}


void ParameterJuggler::addCombined(PluginParameterCombined *parameter, int parameterIndex, int switchIndex, bool mayModify)
{
    jassert(parameterIndex == (switchIndex + 1));

    add(parameter->getModeSwitch(), switchIndex, false);
    add(parameter, parameterIndex, mayModify);
}


String ParameterJuggler::getName(int index)
{
    jassert(index >= 0);
    jassert(index < numberOfParameters);

    return arrParameters[index]->getName();
}


void ParameterJuggler::setName(int index, const String &strParameterName)
{
    jassert(index >= 0);
    jassert(index < numberOfParameters);
    jassert(arrMayModify[index]);

    arrParameters[index]->setName(strParameterName);
}


float ParameterJuggler::getDefaultFloat(int index)
{
    jassert(index >= 0);
    jassert(index < numberOfParameters);

    return arrParameters[index]->getDefaultFloat();
}


float ParameterJuggler::getDefaultRealFloat(int index)
{
    jassert(index >= 0);
    jassert(index < numberOfParameters);

    return arrParameters[index]->getDefaultRealFloat();
}


bool ParameterJuggler::getDefaultBoolean(int index)
{
    jassert(index >= 0);
    jassert(index < numberOfParameters);

    return arrParameters[index]->getDefaultBoolean();
}


int ParameterJuggler::getDefaultRealInteger(int index)
{
    jassert(index >= 0);
    jassert(index < numberOfParameters);

    return arrParameters[index]->getDefaultRealInteger();
}


void ParameterJuggler::setDefaultRealFloat(int index, float newRealValue, bool updateParameter)
{
    jassert(index >= 0);
    jassert(index < numberOfParameters);
    jassert(arrMayModify[index]);

    arrParameters[index]->setDefaultRealFloat(newRealValue, updateParameter);
}


float ParameterJuggler::getFloat(int index)
{
    jassert(index >= 0);
    jassert(index < numberOfParameters);

    return arrParameters[index]->getFloat();
}


void ParameterJuggler::setFloat(int index, float newValue)
{
    jassert(index >= 0);
    jassert(index < numberOfParameters);
    jassert(arrMayModify[index]);

    arrParameters[index]->setFloat(newValue);
}


float ParameterJuggler::getRealFloat(int index)
{
    jassert(index >= 0);
    jassert(index < numberOfParameters);

    return arrParameters[index]->getRealFloat();
}


void ParameterJuggler::setRealFloat(int index, float newRealValue)
{
    jassert(index >= 0);
    jassert(index < numberOfParameters);
    jassert(arrMayModify[index]);

    arrParameters[index]->setRealFloat(newRealValue);
}


int ParameterJuggler::getRealInteger(int index)
{
    jassert(index >= 0);
    jassert(index < numberOfParameters);

    return arrParameters[index]->getRealInteger();
}


void ParameterJuggler::setRealInteger(int index, int newRealValue)
{
    jassert(index >= 0);
    jassert(index < numberOfParameters);
    jassert(arrMayModify[index]);

    arrParameters[index]->setRealInteger(newRealValue);
}


bool ParameterJuggler::getBoolean(int index)
{
    jassert(index >= 0);
    jassert(index < numberOfParameters);

    return arrParameters[index]->getBoolean();
}


String ParameterJuggler::getText(int index)
{
    jassert(index >= 0);
    jassert(index < numberOfParameters);

    return arrParameters[index]->getText();
}


void ParameterJuggler::setText(int index, const String &newValue)
{
    jassert(index >= 0);
    jassert(index < numberOfParameters);
    jassert(arrMayModify[index]);

    arrParameters[index]->setText(newValue);
}


bool ParameterJuggler::hasChanged(int index)
{
    jassert(index >= 0);
    jassert(index < numberOfParameters);

    return arrParameters[index]->hasChanged();
}


void ParameterJuggler::clearChangeFlag(int index)
{
    jassert(index >= 0);
    jassert(index < numberOfParameters);

    return arrParameters[index]->clearChangeFlag();
}


void ParameterJuggler::loadFromXml(XmlElement *xmlDocument)
{
    if (xmlDocument && xmlDocument->hasTagName(jugglerID))
    {
        for (int n = 0; n < numberOfParameters; n++)
        {
            if (arrMayModify[n])
            {
                arrParameters[n]->loadFromXml(xmlDocument);
            }
        }
    }
}


XmlElement ParameterJuggler::storeAsXml()
{
    XmlElement xmlDocument(jugglerID);
    xmlDocument.setAttribute("version", JucePlugin_VersionString);

    for (int n = 0; n < numberOfParameters; n++)
    {
        if (arrMayModify[n])
        {
            arrParameters[n]->storeAsXml(&xmlDocument);
        }
    }

    return xmlDocument;
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
