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

#ifndef __PLUGIN_PARAMETER_COMBINED_H__
#define __PLUGIN_PARAMETER_COMBINED_H__

#include "JuceHeader.h"
#include "plugin_parameter.h"
#include "plugin_parameter_continuous.h"
#include "plugin_parameter_switch.h"
#include "plugin_parameter_toggle_switch.h"


//==============================================================================
/**
*/
class PluginParameterCombined : virtual public PluginParameter
{
public:
    PluginParameterCombined(float real_minimum, float real_maximum, float resolution, float log_factor, int decimal_places);
    ~PluginParameterCombined();

    virtual void setName(const String &strParameterName) override;

    bool getMode();
    bool setMode(bool use_constants);
    bool toggleMode();
    PluginParameterToggleSwitch *getModeSwitch();

    void addConstant(const float fRealValue, const String &strText);
    float getInterval();

    virtual float getDefaultFloat() override;
    virtual float getDefaultRealFloat() override;
    virtual bool getDefaultBoolean() override;
    virtual int getDefaultRealInteger() override;
    virtual bool setDefaultRealFloat(float fRealValue, bool updateValue) override;

    virtual float getFloat() override;
    virtual bool setFloat(float fValue) override;

    virtual float getRealFloat() override;
    virtual bool setRealFloat(float fRealValue) override;

    virtual bool getBoolean() override;
    virtual bool setBoolean(bool bValue) override;

    virtual int getRealInteger() override;
    virtual bool setRealInteger(int nRealValue) override;

    virtual String getText() override;
    virtual bool setText(const String &strText) override;
    void setSuffix(const String &suffix);

    float getFloatFromText(const String &strText);
    String getTextFromFloat(float fValue);

    virtual bool hasChanged() override;
    virtual void clearChangeFlag() override;

    virtual bool saveFromDeletion() override;

    virtual void loadFromXml(XmlElement *xml) override;
    virtual void storeAsXml(XmlElement *xml) override;

protected:
    virtual void setChangeFlag() override;

private:
    JUCE_LEAK_DETECTOR(PluginParameterCombined);

    bool bUseConstants;

    PluginParameterToggleSwitch paramModeSwitch;
    PluginParameterSwitch paramSwitch;
    PluginParameterContinuous paramContinuous;
};


#endif  // __PLUGIN_PARAMETER_COMBINED_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
