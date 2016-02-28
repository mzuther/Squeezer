/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2016 Martin Zuther (http://www.mzuther.de/)

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

#include "plugin_parameter_combined.h"


/// Container for all parameters of a plug-in.
///
/// The methods of this class may be called on the audio thread, so
/// they are absolutely time-critical!
///
class ParameterJuggler
{
public:
    ParameterJuggler(const String &settingsID, int completeParameters, int revealedParameters);
    ~ParameterJuggler();

    PluginParameter *getPluginParameter(int index);
    String toString();

    void add(PluginParameter *parameter, int index);
    void addProtected(PluginParameter *parameter, int index);
    void addCombined(PluginParameterCombined *parameter, int switchIndex, int parameterIndex);
    int getNumParameters(bool includeHiddenParameters);

    String getName(int index);
    void setName(int index, const String &newParameterName);

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

protected:
    void assertParameter(int index, bool wantModification);

    int numberOfParameters;
    int numberOfRevealedParameters;

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
