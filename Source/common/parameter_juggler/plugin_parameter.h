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


/// Common base class for plug-in parameters.
///
class PluginParameter
{
public:
    /// Empty constructor.  Only defined for the sake of
    /// completeness...
    ///
    PluginParameter()
    {
        parameterName = String::empty;
        xmlTagName = String::empty;
    }

    /// Empty destructor.
    ///
    virtual ~PluginParameter() {}

    /// Get parameter's name.
    ///
    /// @return parameter name
    ///
    String getName()
    {
        return parameterName;
    }

    /// Set parameter's name.
    ///
    /// @param newParameterName new parameter name
    ///
    void setName(const String &newParameterName)
    {
        // set new parameter name
        parameterName = newParameterName;

        // trim leading and trailing white space
        parameterName = parameterName.trim();

        // set xml tag name from parameter name
        xmlTagName = parameterName;

        // convert tag name to lower case
        xmlTagName = xmlTagName.toLowerCase();

        // strip all characters except a-z, space and underscore
        xmlTagName = xmlTagName.retainCharacters(
                         "abcdefghijklmnopqrstuvwxyz _");

        // change all spaces to underscores
        xmlTagName = xmlTagName.replace(" ", "_");
    }

    /// Get parameter's XML tag name.
    ///
    /// @return XML tag name
    ///
    String getTagName()
    {
        return xmlTagName;
    }

    /// Get **normalised** default value as float.  Values range from
    /// 0.0 to 1.0.  Subclasses must override this method.
    ///
    /// @return normalised default value (between 0.0 and 1.0)
    ///
    virtual float getDefaultFloat() = 0;

    /// Get **real** default value as float.  Values range from the
    /// parameter's minimum value to its maximum value.  Subclasses
    /// must override this method.
    ///
    /// @return default value
    ///
    virtual float getDefaultRealFloat() = 0;

    /// Get default value as Boolean.  Subclasses must override this
    /// method.
    ///
    /// @return default value
    ///
    virtual bool getDefaultBoolean() = 0;

    /// Get **real** default value as integer.  Values range from the
    /// parameter's minimum value to its maximum value.  Subclasses
    /// must override this method.
    ///
    /// @return default value
    ///
    virtual int getDefaultRealInteger() = 0;

    /// Set **real** default value from float.  The new value must be
    /// in the defined range of the parameter's values.  Subclasses
    /// must override this method.
    ///
    /// @param newRealValue new default value
    ///
    /// @param updateParameter if this is true, the parameter's value
    ///        will be set to the new default value
    ///
    /// @return **true** if update was successful, **false** otherwise
    ///
    virtual bool setDefaultRealFloat(float newRealValue, bool updateValue) = 0;

    /// Get **normalised** parameter value as float.  Values range from 0.0
    /// to 1.0.  Subclasses must override this method.
    ///
    /// @return current value (between 0.0 and 1.0)
    ///
    virtual float getFloat() = 0;

    /// Set **normalised** parameter value from float.  The new value
    /// must be in the range from 0.0 to 1.0.  Subclasses must
    /// override this method.
    ///
    /// @param newValue new value (between 0.0 and 1.0)
    ///
    /// @return **true** if update was successful, **false** otherwise
    ///
    virtual bool setFloat(float newValue) = 0;

    /// Get **real** parameter value as float.  Values range from the
    /// parameter's minimum value to its maximum value.  Subclasses
    /// must override this method.
    ///
    /// @return current value
    ///
    virtual float getRealFloat() = 0;

    /// Set **real** parameter value from float.  The new value must
    /// be in the defined range of the parameter's values.  Subclasses
    /// must override this method.
    ///
    /// @param newRealValue new value
    ///
    /// @return **true** if update was successful, **false** otherwise
    ///
    virtual bool setRealFloat(float newRealValue) = 0;

    /// Get parameter value as Boolean.  Subclasses must override this
    /// method.
    ///
    /// @return current value
    ///
    virtual bool getBoolean() = 0;

    /// Set parameter value from Boolean.  Subclasses must override
    /// this method.
    ///
    /// @param newValue new value
    ///
    /// @return **true** if update was successful, **false** otherwise
    ///
    virtual bool setBoolean(bool newValue) = 0;

    /// Get **real** parameter value as integer.  Values range from
    /// the parameter's minimum value to its maximum value.
    /// Subclasses must override this method.
    ///
    /// @return current value
    ///
    virtual int getRealInteger() = 0;

    /// Set **real** parameter value from integer.  The new value must
    /// be in the defined range of the parameter's values.  Subclasses
    /// must override this method.
    ///
    /// @param newRealValue new value
    ///
    /// @return **true** if update was successful, **false** otherwise
    ///
    virtual bool setRealInteger(int newRealValue) = 0;

    /// Get parameter value as formatted string.  Subclasses must
    /// override this method.
    ///
    /// @return current value
    ///
    virtual String getText() = 0;

    /// Set parameter value from (correctly) formatted string.
    /// Subclasses must override this method.
    ///
    /// @param newValue new value as formatted string
    ///
    /// @return **true** if update was successful, **false** otherwise
    ///
    virtual bool setText(const String &newValue) = 0;

    /// Get parameter's change flag.  Determines whether the parameter
    /// has changed since the last time the change flag was reset.
    /// Subclasses must override this method.
    ///
    /// @return change flag
    ///
    virtual bool hasChanged() = 0;

    /// Mark parameter as unchanged.  Subclasses must override this
    /// method.
    ///
    virtual void clearChangeFlag() = 0;

    /// Should parameter be spared from deletion in destructor of
    /// ParameterJuggler?
    ///
    /// Sorry, I know this sort of method is *really* ugly, but I
    /// currently see no other way to implement
    /// PluginParameterCombined.  Subclasses must override this
    /// method.
    ///
    /// @return true if parameter should be spare, false otherwise
    ///
    virtual bool saveFromDeletion() = 0;

    /// Load parameter value from XML.  Subclasses must override this
    /// method.
    ///
    /// @param xml XML element to load from
    ///
    virtual void loadFromXml(XmlElement *xml) = 0;

    /// Store parameter value as XML.  Subclasses must override this
    /// method.
    ///
    /// @param xml XML element to store in
    ///
    virtual void storeAsXml(XmlElement *xml) = 0;

    /// Round given value to nearest integer.  Will always round
    /// **away** from zero!
    ///
    /// @param x value to be rounded
    ///
    /// @return rounded value
    ///
    inline static int round_mz(float x)
    {
        // value is greater than (or exactly) zero
        if (x >= 0.0f)
        {
            // round away from zero by adding 0.5
            x += 0.5f;
        }
        // value is below zero
        else
        {
            // round away from zero by substracting 0.5
            x -= 0.5f;
        }

        // remove remainder and return it
        return (int) x;
    }

private:
    String parameterName;
    String xmlTagName;
};


#endif  // __PLUGIN_PARAMETER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
