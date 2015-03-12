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

#ifndef __PLUGIN_PARAMETER_STRING_H__
#define __PLUGIN_PARAMETER_STRING_H__

#include "JuceHeader.h"
#include "plugin_parameter.h"


//==============================================================================
/**
*/
class PluginParameterString : virtual public PluginParameter
{
public:
    PluginParameterString(const String &strText);
    ~PluginParameterString();

    String getName();
    void setName(const String &strParameterName);

    float getDefaultFloat();
    float getDefaultRealFloat();
    bool getDefaultBoolean();
    int getDefaultRealInteger();
    bool setDefaultRealFloat(float fRealValue, bool updateValue);

    float getFloat();
    bool setFloat(float fValue);

    float getRealFloat();
    bool setRealFloat(float fRealValue);

    bool getBoolean();
    bool setBoolean(bool bValue);

    int getRealInteger();
    bool setRealInteger(int nRealValue);

    String getText();
    bool setText(const String &strText);

    bool hasChanged();
    void clearChangeFlag();

    bool saveFromDeletion();

    void loadFromXml(XmlElement *xml);
    void storeAsXml(XmlElement *xml);
private:
    JUCE_LEAK_DETECTOR(PluginParameterString);

    void setChangeFlag();

    String strName;
    String strAttribute;

    String strValue;
    bool bChangedValue;
};


#endif  // __PLUGIN_PARAMETER_STRING_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
