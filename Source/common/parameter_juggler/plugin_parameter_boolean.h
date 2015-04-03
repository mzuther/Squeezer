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

#ifndef __PLUGIN_PARAMETER_BOOLEAN_H__
#define __PLUGIN_PARAMETER_BOOLEAN_H__

#include "JuceHeader.h"
#include "plugin_parameter.h"


class PluginParameterBoolean : virtual public PluginParameter
{
public:
    PluginParameterBoolean(const String &state_true, const String &state_false, bool save_from_deletion = false);
    ~PluginParameterBoolean();

    void toggleState();

    virtual void setDefaultRealFloat(float newRealValue, bool updateParameter) override;
    void setDefaultBoolean(bool newValue, bool updateParameter);

    virtual void setFloat(float newValue) override;
    virtual void setRealFloat(float newRealValue) override;
    void setBoolean(bool newValue);

    virtual void setText(const String &newValue) override;

private:
    JUCE_LEAK_DETECTOR(PluginParameterBoolean);

    String labelTrue;
    String labelFalse;
};


#endif  // __PLUGIN_PARAMETER_BOOLEAN_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
