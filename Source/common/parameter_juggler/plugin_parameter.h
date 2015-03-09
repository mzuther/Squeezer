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

#ifndef __PLUGIN_PARAMETER_H__
#define __PLUGIN_PARAMETER_H__

#include "JuceHeader.h"


//==============================================================================
/**
*/
class PluginParameter
{
public:
    virtual ~PluginParameter() {}

    virtual String getName() = 0;
    virtual void setName(const String &strParameterName) = 0;

    virtual float getInterval() = 0;

    virtual float getDefaultFloat() = 0;
    virtual float getDefaultRealFloat() = 0;
    virtual bool getDefaultBoolean() = 0;
    virtual int getDefaultRealInteger() = 0;
    virtual bool setDefaultRealFloat(float fRealValue, bool updateValue) = 0;

    virtual float getFloat() = 0;
    virtual bool setFloat(float fValue) = 0;

    virtual float getRealFloat() = 0;
    virtual bool setRealFloat(float fRealValue) = 0;

    virtual bool getBoolean() = 0;
    virtual bool setBoolean(bool bValue) = 0;

    virtual int getRealInteger() = 0;
    virtual bool setRealInteger(int nRealValue) = 0;

    virtual String getText() = 0;
    virtual bool setText(const String &strText) = 0;

    virtual bool hasChanged() = 0;
    virtual void clearChangeFlag() = 0;
    virtual void setChangeFlag() = 0;

    virtual float getFloatFromText(const String &strText) = 0;
    virtual String getTextFromFloat(float fValue) = 0;

    virtual bool saveFromDeletion() = 0;

    virtual void loadFromXml(XmlElement *xml) = 0;
    virtual void storeAsXml(XmlElement *xml) = 0;

    inline static int round_mz(float x)
    {
        x += (x >= 0.0f) ? 0.5f : -0.5f;
        return (int) x;
    }
protected:
    PluginParameter() {}
};


#endif  // __PLUGIN_PARAMETER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
