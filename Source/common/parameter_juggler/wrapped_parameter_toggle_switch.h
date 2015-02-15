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

#ifndef __WRAPPED_PARAMETER_TOGGLE_SWITCH_H__
#define __WRAPPED_PARAMETER_TOGGLE_SWITCH_H__

#include "JuceHeader.h"
#include "wrapped_parameter.h"


//==============================================================================
/**
*/
class WrappedParameterToggleSwitch : virtual public WrappedParameter
{
public:
    WrappedParameterToggleSwitch(const String &state_on, const String &state_off, bool save_from_deletion = false);
    ~WrappedParameterToggleSwitch();

    String getName();
    void setName(const String &strParameterName);

    float getInterval();
    void toggleState();

    float getDefaultFloat();
    float getDefaultRealFloat();
    bool getDefaultBoolean();
    int getDefaultRealInteger();
    bool setDefaultBoolean(bool bValue, bool updateValue);
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

    float getFloatFromText(const String &strText);
    String getTextFromFloat(float fValue);

    bool hasChanged();
    void clearChangeFlag();
    void setChangeFlag();

    bool saveFromDeletion();

    void loadFromXml(XmlElement *xml);
    void storeAsXml(XmlElement *xml);
private:
    JUCE_LEAK_DETECTOR(WrappedParameterToggleSwitch);

    String strName;
    String strAttribute;
    String strStateOn;
    String strStateOff;

    bool bState;
    bool bDefaultState;
    bool bChangedValue;
    bool bSaveFromDeletion;
};


#endif  // __WRAPPED_PARAMETER_TOGGLE_SWITCH_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
