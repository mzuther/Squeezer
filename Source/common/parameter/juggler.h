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

#ifndef __FRUT_PARAMETER_JUGGLER_H__
#define __FRUT_PARAMETER_JUGGLER_H__


/// Container for all parameters of a plug-in.
///
/// The methods of this class may be called on the audio thread, so
/// they are absolutely time-critical!
///
class Juggler
{
public:
    Juggler(const String &settingsID,
            int completeParameters,
            int revealedParameters);

    Parameter *getPluginParameter(int index);
    String toString();

    void add(Parameter *parameter,
             int index);

    void addProtected(Parameter *parameter,
                      int index);

    void addCombined(parameter::ParCombined *parameter,
                     int switchIndex,
                     int parameterIndex);

    int getNumParameters(bool includeHiddenParameters);

    String getName(int index);
    void setName(int index,
                 const String &newParameterName);

    float getDefaultFloat(int index);
    float getDefaultRealFloat(int index);
    bool getDefaultBoolean(int index);
    int getDefaultRealInteger(int index);
    void setDefaultRealFloat(int index,
                             float newRealValue,
                             bool updateParameter);

    float getFloat(int index);
    void setFloat(int index,
                  float newValue);

    float getRealFloat(int index);
    void setRealFloat(int index,
                      float newRealValue);

    int getRealInteger(int index);
    void setRealInteger(int index,
                        int newRealValue);

    bool getBoolean(int index);

    String getText(int index);
    void setText(int index, const String &newValue);

    bool hasChanged(int index);
    void clearChangeFlag(int index);

    void loadFromXml(XmlElement *xmlDocument);
    XmlElement storeAsXml();

protected:
#ifdef DEBUG
    void assertParameter(int index,
                         bool wantModification);
#endif

    int numberOfParameters_;
    int numberOfRevealedParameters_;

    String jugglerId_;

    // This array deletes the contained parameters when the class is
    // destructed.  It holds the *real* parameter that is passed to
    // the juggler.  *Real* and *virtual* parameters only differ for
    // the class *CombinedParameter* which creates *two* virtual
    // parameters (switch and continuous) instead of one.
    OwnedArray<Parameter> garbageCollector_;

    // This array holds the *virtual* parameters.  See the
    // documentation of garbageCollector_ for more info.
    Array<Parameter *> virtualParameters_;

    // This array a Boolean for every *virtual* parameter.  If the
    // Boolean is *true*, the parameter may be modified, otherwise it
    // will be handled as a constant.
    Array<bool> mayModify_;

private:
    JUCE_LEAK_DETECTOR(Juggler);
};

#endif  // __FRUT_PARAMETER_JUGGLER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
