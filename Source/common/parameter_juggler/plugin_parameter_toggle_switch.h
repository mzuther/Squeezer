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

#ifndef __PLUGIN_PARAMETER_TOGGLE_SWITCH_H__
#define __PLUGIN_PARAMETER_TOGGLE_SWITCH_H__

#include "JuceHeader.h"
#include "plugin_parameter.h"


//==============================================================================
/**
*/
class PluginParameterToggleSwitch : virtual public PluginParameter
{
public:
    PluginParameterToggleSwitch(const String &state_on, const String &state_off, bool save_from_deletion = false);
    ~PluginParameterToggleSwitch();

    void toggleState();

    virtual float getDefaultFloat() override;
    virtual float getDefaultRealFloat() override;
    virtual bool getDefaultBoolean() override;
    virtual int getDefaultRealInteger() override;
    bool setDefaultBoolean(bool bValue, bool updateValue);
    virtual bool setDefaultRealFloat(float fRealValue, bool updateValue) override;

    virtual float getFloat() override;
    virtual bool setFloat(float fValue) override;

    virtual float getRealFloat() override;
    virtual bool setRealFloat(float fRealValue) override;

    virtual String getText() override;
    virtual bool setText(const String &strText) override;

    virtual bool saveFromDeletion() override;

private:
    JUCE_LEAK_DETECTOR(PluginParameterToggleSwitch);

    String strStateOn;
    String strStateOff;

    bool bState;
    bool bDefaultState;
    bool bChangedValue;
    bool bSaveFromDeletion;
};


#endif  // __PLUGIN_PARAMETER_TOGGLE_SWITCH_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
