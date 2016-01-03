/* ----------------------------------------------------------------------------

   MZ common JUCE
   ==============
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

#ifndef __PLUGIN_PARAMETER_H__
#define __PLUGIN_PARAMETER_H__

#include "JuceHeader.h"
#include "../mz_juce_common.h"


/// Common base class for plug-in parameters.
///
/// The methods of this class may be called on the audio thread, so
/// they are absolutely time-critical!
///
class PluginParameter
{
public:
    PluginParameter();
    virtual ~PluginParameter();

    virtual String getName();
    virtual void setName(const String &newParameterName);
    virtual String getTagName();

    virtual float getDefaultFloat();
    virtual float getDefaultRealFloat();
    virtual bool getDefaultBoolean();
    virtual int getDefaultRealInteger();

    /// Set **real** default value from float.  The new value must be
    /// in the defined range of the parameter's values.  Subclasses
    /// must override this method.
    ///
    /// @param newRealValue new default value
    ///
    /// @param updateParameter if this is true, the parameter's value
    ///        will be set to the new default value
    ///
    virtual void setDefaultRealFloat(float newRealValue, bool updateParameter) = 0;

    virtual float getFloat();
    virtual float getRealFloat();

    /// Set **internal** parameter value from float.  The new value
    /// must be in the range from 0.0 to 1.0.  Subclasses must
    /// override this method.
    ///
    /// @param newValue new value (between 0.0 and 1.0)
    ///
    virtual void setFloat(float newValue) = 0;

    /// Set **real** parameter value from float.  The new value must
    /// be in the defined range of the parameter's values.  Subclasses
    /// must override this method.
    ///
    /// @param newRealValue new value
    ///
    virtual void setRealFloat(float newRealValue) = 0;

    virtual int getRealInteger();
    virtual void setRealInteger(int newRealValue);

    virtual bool getBoolean();

    virtual const String getText();
    virtual void setText(const String &newValue);

    /// Transform string to **internal** parameter value.  Subclasses
    /// must override this method.
    ///
    /// @param newValue correctly formatted string
    ///
    /// @return **internal** value
    ///
    virtual float getFloatFromText(const String &newValue) = 0;

    /// Transform **internal** value to string.  Subclasses must
    /// override this method.
    ///
    /// @param newValue **internal** value
    ///
    /// @return formatted string
    ///
    virtual const String getTextFromFloat(float newValue) = 0;

    virtual bool hasChanged();
    virtual void clearChangeFlag();

    virtual bool saveFromDeletion();

    virtual void loadFromXml(XmlElement *xmlDocument);
    virtual void storeAsXml(XmlElement *xmlDocument);

protected:
    virtual void setChangeFlag();

    float value;
    float realValue;

    float defaultValue;
    float defaultRealValue;

    bool doNotDelete;

private:
    String parameterName;
    String xmlTagName;

    bool valueHasChanged;
};


#endif  // __PLUGIN_PARAMETER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
