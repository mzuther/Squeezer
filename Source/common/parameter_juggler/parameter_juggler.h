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

#ifndef __PARAMETER_JUGGLER_H__
#define __PARAMETER_JUGGLER_H__

class ParameterJuggler;

#include "JuceHeader.h"
#include "../mz_juce_common.h"
#include "plugin_parameter.h"
#include "plugin_parameter_boolean.h"
#include "plugin_parameter_combined.h"
#include "plugin_parameter_string.h"
#include "plugin_parameter_switch.h"


class ParameterJuggler
{
public:
    ParameterJuggler();
    ~ParameterJuggler();

    PluginParameter *getPluginParameter(int index);
    String toString();

    void add(PluginParameter *parameter, int index, bool mayModify = true);
    void addCombined(PluginParameterCombined *parameter, int parameterIndex, int switchIndex, bool mayModify = true);
    int getNumParameters(bool includeHiddenParameters);

    String getName(int index);
    void setName(int index, const String &strParameterName);

    float getDefaultFloat(int index);
    float getDefaultRealFloat(int index);
    bool getDefaultBoolean(int index);
    int getDefaultRealInteger(int index);
    void setDefaultRealFloat(int index, float newRealValue, bool updateParameter);

    float getFloat(int index);
    void setFloat(int index, float newValue);

    float getRealFloat(int index);
    void setRealFloat(int index, float newRealValue);

    int getRealInteger(int index);
    void setRealInteger(int index, int newRealValue);

    bool getBoolean(int index);

    String getText(int index);
    void setText(int index, const String &newValue);

    bool hasChanged(int index);
    void clearChangeFlag(int index);

    void loadFromXml(XmlElement *xmlDocument);
    XmlElement storeAsXml();

    enum Parameters  // public namespace!
    {
        numberOfParametersRevealed = 0,
        numberOfParametersComplete = 0,
    };

protected:
    int numberOfParameters;

    String jugglerID;

    Array<PluginParameter *> arrParameters;
    Array<bool> arrMayModify;

private:
    JUCE_LEAK_DETECTOR(ParameterJuggler);
};

#endif  // __PARAMETER_JUGGLER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
