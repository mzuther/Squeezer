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

#ifndef __WRAPPED_PARAMETER_COMBINED_H__
#define __WRAPPED_PARAMETER_COMBINED_H__

#include "JuceHeader.h"
#include "wrapped_parameter.h"
#include "wrapped_parameter_continuous.h"
#include "wrapped_parameter_switch.h"
#include "wrapped_parameter_toggle_switch.h"


//==============================================================================
/**
*/
class WrappedParameterCombined : virtual public WrappedParameter
{
public:
    WrappedParameterCombined(float real_minimum, float real_maximum, float resolution, float log_factor, int decimal_places);
    ~WrappedParameterCombined();

    String getName();
    void setName(const String &strParameterName);

    bool getMode();
    bool setMode(bool use_constants);
    bool toggleMode();
    WrappedParameterToggleSwitch *getModeSwitch();

    void addConstant(const float fRealValue, const String &strText);
    float getInterval();

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
    void setSuffix(const String &suffix);

    float getFloatFromText(const String &strText);
    String getTextFromFloat(float fValue);

    bool hasChanged();
    void clearChangeFlag();
    void setChangeFlag();

    bool hasChangedMode();
    void clearChangeFlagMode();
    void setChangeFlagMode();

    bool saveFromDeletion();

    void loadFromXml(XmlElement *xml);
    void storeAsXml(XmlElement *xml);
private:
    JUCE_LEAK_DETECTOR(WrappedParameterCombined);

    String strName;
    String strAttribute;

    bool bUseConstants;
    bool bChangedMode;

    WrappedParameterToggleSwitch paramModeSwitch;
    WrappedParameterSwitch paramSwitch;
    WrappedParameterContinuous paramContinuous;
};


#endif  // __WRAPPED_PARAMETER_COMBINED_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
