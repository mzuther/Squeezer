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

#ifndef FRUT_SKIN_SKIN_H
#define FRUT_SKIN_SKIN_H

namespace frut::skin
{

class Skin
{
public:
   bool loadFromXml( const String& rootName,
                     const String& assumedVersionNumber );

   int getUiScale();
   void setUiScale( int scale );

   String getQualifiedTag( XmlElement* xml );
   XmlElement* getSetting( const String& tagName );
   XmlElement* getComponent( const String& tagName );

   bool getBoolean( const XmlElement* xmlComponent,
                    const String& attributeName,
                    const bool defaultValue = false );

   int getInteger( const XmlElement* xmlComponent,
                   const String& attributeName,
                   const int defaultValue = 0 );

   float getFloat( const XmlElement* xmlComponent,
                   const String& attributeName,
                   const float defaultValue = 0.0f );

   const String getString( const XmlElement* xmlComponent,
                           const String& attributeName,
                           const String& defaultValue = "" );

   const Colour getColour( const XmlElement* xmlComponent,
                           const Colour defaultColour = Colours::red,
                           const String valuePrefix = "" );

   bool getAttributesFromSvgFile( const String& tagName,
                                  const String& attributeName,
                                  Colour& strokeColour,
                                  Colour& fillColour,
                                  Rectangle<int>& bounds );

   std::unique_ptr<Drawable> loadImageAsDrawable( const String& strFilename );
   std::unique_ptr<Drawable> loadImageAsDrawable( const String& tagName,
                                                  const String& attributeName,
                                                  Drawable* alternativeDrawable = nullptr );

   Image loadImage( const String& strFilename );
   Image loadImage( const String& tagName,
                    const String& attributeName );

   Image imageFromDrawable( std::unique_ptr<Drawable>& drawable );

   void setBackground( DrawableComposite* background,
                       AudioProcessorEditor* editor );

   void printPosition( const String& header,
                       Component* component );

   void printSize( const String& header,
                   Component* component );

   void printBounds( const String& header,
                     Component* component );

   Point<float> getPositionFloat( const XmlElement* xmlComponent,
                                  const float componentHeight,
                                  bool useRelativePosition );

   Point<int> getPositionInteger( const XmlElement* xmlComponent,
                                  const int componentHeight,
                                  bool useRelativePosition );

   Rectangle<float> getBoundsFloat( const XmlElement* xmlComponent,
                                    float width,
                                    float height,
                                    bool useRelativePosition );

   Rectangle<int> getBoundsInteger( const XmlElement* xmlComponent,
                                    int width,
                                    int height,
                                    bool useRelativePosition );

   void placeComponent( const XmlElement* xmlComponent,
                        Component* component );

   void placeMeterBar( const String& tagName,
                       widgets::MeterBar* meterBar );

   void placeAndSkinButton( const String& tagName,
                            DrawableButton* button );

   void placeAndSkinSlider( const String& tagName,
                            widgets::FrutSlider* slider );

   void placeAndSkinNeedleMeter( const String& tagName,
                                 widgets::NeedleMeter* meter );

   void placeAndSkinLabel( const String& tagName,
                           DrawableComposite* label );

   void placeAndSkinSignalLed( const String& tagName,
                               widgets::SignalLed* label );

   void placeAndSkinStateLabel( const String& tagName,
                                widgets::StateLabel* label );

   virtual File getSkinDirectory() = 0;
   virtual File getSettingsFile() = 0;


protected:
   virtual bool resourceExists( const String& strFilename ) = 0;
   virtual std::unique_ptr<Drawable> loadDrawable( const String& strFilename ) = 0;
   virtual std::unique_ptr<XmlElement> loadXML( const String& strFilename ) = 0;

   std::unique_ptr<XmlElement> document_;

   std::unique_ptr<Drawable> createBogusImage( const String& warningText,
                                               int width,
                                               int height );

   XmlElement* settingsGroup_;
   XmlElement* skinGroup_;
   XmlElement* skinFallback_1_;
   XmlElement* skinFallback_2_;

   String currentBackgroundName_;
   String currentGroupName_;
   String currentFallbackName_;

   bool originOfYIsBottom_;

   int backgroundWidth_;
   int backgroundHeight_;

private:
   JUCE_LEAK_DETECTOR( Skin );
};

} // namespace

#endif  // FRUT_SKIN_SKIN_H
