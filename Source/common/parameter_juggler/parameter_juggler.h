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
#include "plugin_parameter_combined.h"
#include "plugin_parameter_string.h"
#include "plugin_parameter_switch.h"
#include "plugin_parameter_toggle_switch.h"


//============================================================================
class ParameterJuggler
{
public:
    //==========================================================================

    ParameterJuggler();
    ~ParameterJuggler();

    PluginParameter *getPluginParameter(int nIndex);
    String toString();

    void add(PluginParameter *parameter, int nIndex, bool mayModify = true);
    void addCombined(PluginParameterCombined *parameter, int nIndex, int nIndexSwitch, bool mayModify = true);
    int getNumParameters(bool bIncludeHiddenParameters);

    String getName(int nIndex);
    void setName(int nIndex, const String &strParameterName);

    float getInterval(int nIndex);

    float getDefaultFloat(int nIndex);
    float getDefaultRealFloat(int nIndex);
    bool getDefaultBoolean(int nIndex);
    int getDefaultRealInteger(int nIndex);
    bool setDefaultRealFloat(int nIndex, float fRealValue, bool updateValue);

    float getFloat(int nIndex);
    bool setFloat(int nIndex, float fValue);

    bool getBoolean(int nIndex);
    bool setBoolean(int nIndex, bool bValue);

    float getRealFloat(int nIndex);
    bool setRealFloat(int nIndex, float fRealValue);

    int getRealInteger(int nIndex);
    bool setRealInteger(int nIndex, int nRealValue);

    String getText(int nIndex);
    bool setText(int nIndex, const String &strText);

    bool hasChanged(int nIndex);
    void clearChangeFlag(int nIndex);
    void setChangeFlag(int nIndex);

    float getFloatFromText(int nIndex, const String &strText);
    String getTextFromFloat(int nIndex, float fValue);

    void loadFromXml(XmlElement *xml);
    XmlElement storeAsXml();

    enum Parameters  // public namespace!
    {
        nNumParametersRevealed = 0,
        nNumParametersComplete = nNumParametersRevealed
    };

protected:
    int nNumParameters;
    Array<PluginParameter *> arrParameters;
    Array<bool> arrMayModify;

    String strSettingsID;

private:
    JUCE_LEAK_DETECTOR(ParameterJuggler);
};

#endif  // __PARAMETER_JUGGLER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
