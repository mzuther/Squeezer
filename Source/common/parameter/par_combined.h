/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2018 Martin Zuther (http://www.mzuther.de/)

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

#ifndef __FRUT_PARAMETER_COMBINED_H__
#define __FRUT_PARAMETER_COMBINED_H__


/// Plug-in parameter for storing a floating-point value (can be
/// switched between preset and continuous values).
///
/// The methods of this class may be called on the audio thread, so
/// they are absolutely time-critical!
///
class ParCombined : virtual public Parameter
{
public:
    ParCombined(float real_minimum, float real_maximum, float real_step_size, float scaling_factor, int decimal_places);

    virtual void setName(const String &newParameterName) override;

    bool getMode();
    void setMode(bool use_presets);
    void toggleMode();
    parameter::ParBoolean *getModeSwitch();

    void addPreset(const float newRealValue, const String &newLabel);
    int getNumberOfSteps();
    float getStepSize();

    virtual float getDefaultFloat() override;
    virtual float getDefaultRealFloat() override;
    virtual bool getDefaultBoolean() override;
    virtual int getDefaultRealInteger() override;
    virtual void setDefaultRealFloat(float newRealValue, bool updateParameter) override;

    virtual float getFloat() override;
    virtual void setFloat(float newValue) override;

    virtual float getRealFloat() override;
    virtual void setRealFloat(float newRealValue) override;

    virtual int getRealInteger() override;
    virtual void setRealInteger(int nRealValue) override;

    virtual bool getBoolean() override;

    void setSuffix(const String &newSuffix);

    virtual float getFloatFromText(const String &newValue) override;
    virtual const String getTextFromFloat(float newValue) override;

    virtual bool hasChanged() override;
    virtual void clearChangeFlag() override;

    virtual void loadFromXml(XmlElement *xmlDocument) override;
    virtual void storeAsXml(XmlElement *xmlDocument) override;

protected:
    virtual void setChangeFlag() override;

private:
    JUCE_LEAK_DETECTOR(ParCombined);

    bool usePresets;

    parameter::ParBoolean modeSwitch;
    parameter::ParSwitch presetValues;
    parameter::ParContinuous continuousValues;
};


#endif  // __FRUT_PARAMETER_COMBINED_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
