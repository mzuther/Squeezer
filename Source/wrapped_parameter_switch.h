/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

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

#ifndef __WRAPPED_PARAMETER_SWITCH_H__
#define __WRAPPED_PARAMETER_SWITCH_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "wrapped_parameter.h"


//==============================================================================
/**
*/
class WrappedParameterSwitch : virtual public WrappedParameter
{
public:
    WrappedParameterSwitch();
    ~WrappedParameterSwitch();

    String getName();
    void setName(const String& strParameterName);

    void addConstant(const float fRealValue, const String& strText);
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
    bool setNearestRealFloat(float fRealValue);

    bool getBoolean();
    bool setBoolean(bool bValue);

    int getRealInteger();
    bool setRealInteger(int nRealValue);

    String getText();
    bool setText(const String& strText);

    float getFloatFromText(const String& strText);
    String getTextFromFloat(float fValue);

    bool hasChanged();
    void clearChangeFlag();
    void setChangeFlag();

    void loadFromXml(XmlElement* xml);
    void storeAsXml(XmlElement* xml);
private:
    JUCE_LEAK_DETECTOR(WrappedParameterSwitch);

    String strName;
    String strAttribute;

    float fDefaultRealValue;
    int nCurrentIndex;
    float fValueInternal;
    bool bChangedValue;
    float fInterval;

    Array<float> fRealValues;
    StringArray strValues;
};


#endif  // __WRAPPED_PARAMETER_SWITCH_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
