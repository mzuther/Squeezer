/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose compressor with a touch of lemon

   Copyright (c) 2013 Martin Zuther (http://www.mzuther.de/)

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

#ifndef __WRAPPED_PARAMETER_STEPPED_H__
#define __WRAPPED_PARAMETER_STEPPED_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "wrapped_parameter.h"


//==============================================================================
/**
*/
class WrappedParameterStepped : virtual public WrappedParameter
{
public:
    WrappedParameterStepped();
    ~WrappedParameterStepped();

    String getName();
    void setName(const String& strParameterName);

    void addValue(const float fRealValue, const String& strText);
    float getInterval();

    float getDefaultFloat();
    bool getDefaultBoolean();
    int getDefaultInteger();
    void setDefaultString(const String& strDefault, bool updateValue);

    float getFloat();
    void setFloat(float fValue);

    float getRealFloat();
    void setRealFloat(float fValue);

    bool getBoolean();
    void setBoolean(bool bValue);

    int getInteger();
    void setInteger(int nValue);

    int getRealInteger();
    void setRealInteger(int nValue);

    String getText();
    void setText(const String& strText);

    float getFloatFromText(const String& strText);
    String getTextFromFloat(float fValue);

    int getIntegerFromText(const String& strText);
    String getTextFromInteger(int nValue);

    bool hasChanged();
    void clearChangeFlag();
    void setChangeFlag();

    void loadFromXml(XmlElement* xml);
    void storeAsXml(XmlElement* xml);
private:
    JUCE_LEAK_DETECTOR(WrappedParameterStepped);

    String strName;
    String strAttribute;
    String strDefaultValue;

    int nCurrentIndex;
    float fValueInternal;
    bool bChangedValue;
    float fInterval;

    Array<float> fRealValues;
    StringArray strValues;
};


#endif  // __WRAPPED_PARAMETER_STEPPED_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
