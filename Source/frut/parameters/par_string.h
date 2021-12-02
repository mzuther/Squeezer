/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2021 Martin Zuther (http://www.mzuther.de/)

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

#ifndef FRUT_PARAMETER_PARSTRING_H
#define FRUT_PARAMETER_PARSTRING_H

namespace frut::parameters
{

/// Plug-in parameter for storing a String value.
///
/// The methods of this class may be called on the audio thread, so
/// they are absolutely time-critical!
///
class ParString : virtual public Parameter
{
public:
   ParString();
   void init( const String& newValue );

   virtual void setDefaultRealFloat( float newRealValue, bool updateParameter ) override;
   virtual void setFloat( float newValue ) override;
   virtual void setRealFloat( float newRealValue ) override;

   virtual const String getText() override;
   virtual void setText( const String& newValue ) override;

   virtual float getFloatFromText( const String& newValue ) override;
   virtual const String getTextFromFloat( float newValue ) override;

   virtual void loadFromXml( XmlElement* xmlDocument ) override;
   virtual void storeAsXml( XmlElement* xmlDocument ) override;

private:
   JUCE_LEAK_DETECTOR( ParString );

   String textValue;
};

} // namespace

#endif  // FRUT_PARAMETER_PARSTRING_H
