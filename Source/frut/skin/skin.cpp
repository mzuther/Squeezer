/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2020 Martin Zuther (http://www.mzuther.de/)

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

namespace frut
{
namespace skin
{

// The following class members are initialized in the derived class:
//
// * currentBackgroundName_
// * currentFallbackName_
// * currentGroupName_
// * skinGroup_
// * skinFallback_1_
// * skinFallback_2_
//
bool Skin::loadFromXml(
   const String& rootName,
   const String& assumedVersionNumber )
{
   settingsGroup_ = nullptr;
   skinGroup_ = nullptr;
   skinFallback_1_ = nullptr;
   skinFallback_2_ = nullptr;

   backgroundWidth_ = 0;
   backgroundHeight_ = 0;

   auto skinFile = getSkinDirectory().getChildFile(
                      getDefaultSkin() + ".skin" );

   Logger::outputDebugString(
      String( "[Skin] loading file \"" ) +
      skinFile.getFileName() + "\"" );

   document_ = juce::parseXML( skinFile );

   // this should not happen!
   if ( document_ == nullptr ) {
      Logger::outputDebugString(
         String( "[Skin] file \"" ) +
         skinFile.getFullPathName() +
         "\" not found" );

      return false;
   }

   auto skinVersion = getString( document_.get(), "version" );

   if ( skinVersion.compare( assumedVersionNumber ) != 0 ) {
      Logger::outputDebugString(
         String( "[Skin] file \"" ) +
         skinFile.getFileName() +
         "\" has incompatible version number \"" +
         skinVersion +
         "\"" );

      document_ = nullptr;
      return false;
   }

   settingsGroup_ = document_->getChildByName( "settings" );
   skinFallback_2_ = document_->getChildByName( "default" );

   if ( ( ! document_->hasTagName( rootName ) ) ||
        ( skinFallback_2_ == nullptr ) ) {
      Logger::outputDebugString(
         "[Skin] XML file not valid" );

      document_ = nullptr;
      return false;
   } else {
      skinGroup_ = document_->getChildByName( currentGroupName_ );

      if ( skinGroup_ == nullptr ) {
         Logger::outputDebugString(
            String( "[Skin] XML element \"" ) +
            currentGroupName_ +
            "\" not found in settings" );
      }

      skinFallback_1_ = document_->getChildByName( currentFallbackName_ );

      auto resourcePathName = getString( document_.get(), "path" );
      resourcePath_ = File( skinFile.getSiblingFile( resourcePathName ) );

      if ( ! resourcePath_.isDirectory() ) {
         Logger::outputDebugString(
            String( "[Skin] directory \"" ) +
            resourcePath_.getFullPathName() +
            "\" not found" );

         document_ = nullptr;
         return false;
      }
   }

   auto originOfY = getString( document_.get(), "origin_of_y", "top" );
   originOfYIsBottom_ = originOfY.compare( "bottom" ) == 0;

   return true;
}


String Skin::getDefaultSkin()
{
   auto settingsFile = this->getSettingsFile();

   // make sure settings file exists
   if ( ! settingsFile.existsAsFile() ) {
      settingsFile.create();

      // use "Default" skin by default, as it comes with the plug-in
      setDefaultSkin( "Default" );
   }

   auto skinName = settingsFile.loadFileAsString();
   auto skinFile = getSkinDirectory().getChildFile(
                      skinName + ".skin" );

   // use "Default" skin if specified skin file does not exist
   if ( ! skinFile.existsAsFile() ) {
      skinName = "Default";
   }

   return skinName;
}


void Skin::setDefaultSkin(
   const String& defaultSkinName )
{
   auto settingsFile = this->getSettingsFile();

   // uses Unicode encoding
   settingsFile.replaceWithText( defaultSkinName, true, true );
}


XmlElement* Skin::getSetting(
   const String& tagName )
{
   if ( document_ == nullptr ) {
      return nullptr;
   }

   if ( settingsGroup_ == nullptr ) {
      Logger::outputDebugString(
         String( "[Skin] settings group not found" ) );

      return nullptr;
   }

   XmlElement* xmlSetting = settingsGroup_->getChildByName( tagName );

   if ( xmlSetting == nullptr ) {
      Logger::outputDebugString(
         String( "[Skin] XML element \"" ) +
         tagName +
         "\" not found in settings" );

      return nullptr;
   }

   return xmlSetting;
}


XmlElement* Skin::getComponent(
   const String& tagName )
{
   XmlElement* xmlComponent;

   // suppress unnecessary warnings and save some time
   if ( document_ == nullptr ) {
      xmlComponent = nullptr;
   } else if ( ( skinGroup_ != nullptr ) &&
               ( skinGroup_->getChildByName( tagName ) != nullptr ) ) {
      xmlComponent = skinGroup_->getChildByName( tagName );
   } else if ( ( skinFallback_1_ != nullptr ) &&
               ( skinFallback_1_->getChildByName( tagName ) != nullptr ) ) {
      xmlComponent = skinFallback_1_->getChildByName( tagName );
   } else if ( ( skinFallback_2_ != nullptr ) &&
               ( skinFallback_2_->getChildByName( tagName ) != nullptr ) ) {
      xmlComponent = skinFallback_2_->getChildByName( tagName );
   } else {
      Logger::outputDebugString(
         String( "[Skin] XML element \"" ) +
         tagName +
         "\" not found" );

      xmlComponent = nullptr;
   }

   return xmlComponent;
}


bool Skin::getBoolean(
   const XmlElement* xmlComponent,
   const String& attributeName,
   const bool defaultValue )
{
   if ( xmlComponent == nullptr ) {
      return defaultValue;
   } else {
      return xmlComponent->getBoolAttribute( attributeName, defaultValue );
   }
}


int Skin::getInteger(
   const XmlElement* xmlComponent,
   const String& attributeName,
   const int defaultValue )
{
   if ( xmlComponent == nullptr ) {
      return defaultValue;
   } else {
      return xmlComponent->getIntAttribute( attributeName, defaultValue );
   }
}


float Skin::getFloat(
   const XmlElement* xmlComponent,
   const String& attributeName,
   const float defaultValue )
{
   if ( xmlComponent == nullptr ) {
      return defaultValue;
   } else {
      double result = xmlComponent->getDoubleAttribute(
                         attributeName, defaultValue );

      return static_cast<float>( result );
   }
}


const String Skin::getString(
   const XmlElement* xmlComponent,
   const String& attributeName,
   const String& defaultValue )
{
   if ( xmlComponent == nullptr ) {
      return defaultValue;
   } else {
      return xmlComponent->getStringAttribute( attributeName, defaultValue );
   }
}


const Colour Skin::getColour(
   const XmlElement* xmlComponent,
   const Colour defaultColour,
   const String valuePrefix )
{
   if ( xmlComponent == nullptr ) {
      return Colours::white;
   }

   float hue = getFloat( xmlComponent,
                         valuePrefix + "hue",
                         defaultColour.getHue() );

   float saturation = getFloat( xmlComponent,
                                valuePrefix + "saturation",
                                defaultColour.getSaturation() );

   float brightness = getFloat( xmlComponent,
                                valuePrefix + "brightness",
                                defaultColour.getBrightness() );

   float alpha = getFloat( xmlComponent,
                           valuePrefix + "alpha",
                           defaultColour.getFloatAlpha() );

   // initialise HSBA colour
   return Colour( hue, saturation, brightness, alpha );
}


std::unique_ptr<Drawable> Skin::createBogusImage(
   const String& warningText,
   int width,
   int height )
{
   auto drawable = new DrawableComposite();
   auto boundingBox = Rectangle<float>(
                         0,
                         0,
                         static_cast<float>( width ),
                         static_cast<float>( height ) );

   auto rectangle = new DrawablePath();
   rectangle->setFill( Colours::red.withAlpha( 0.8f ) );
   drawable->addAndMakeVisible( rectangle );

   Path path;
   path.addRectangle( boundingBox );
   rectangle->setPath( path );

   auto text = new DrawableText();
   drawable->addAndMakeVisible( text );

   text->setText( warningText );
   text->setColour( Colours::white );
   text->setFontHeight( 20 );
   text->setJustification( Justification::Flags::topLeft );

   float shrinkFactor = 10.0f;
   text->setBoundingBox( boundingBox.reduced( shrinkFactor ).withPosition( shrinkFactor, shrinkFactor ) );

   return std::unique_ptr<Drawable>( drawable );
}


std::unique_ptr<Drawable> Skin::loadImage(
   const String& strFilename )
{
   File fileImage = resourcePath_.getChildFile( strFilename );
   std::unique_ptr<Drawable> component;

   if ( fileImage.existsAsFile() ) {
      component = Drawable::createFromImageFile( fileImage );

      if ( ! component ) {
         Logger::outputDebugString(
            String( "[Skin] image file \"" ) +
            fileImage.getFullPathName() +
            "\" not loaded" );

         component = createBogusImage( "Image file not loaded", 200, 200 );
      }
   } else {
      Logger::outputDebugString(
         String( "[Skin] image file \"" ) +
         fileImage.getFullPathName() +
         "\" not found" );

      component = createBogusImage( "Image file not found", 200, 200 );
   }

   return component;
}


void Skin::loadImage(
   const String& strFilename,
   Image& image )
{
   auto drawable = loadImage( strFilename );

   image = Image( Image::PixelFormat::ARGB,
                  drawable->getWidth(),
                  drawable->getHeight(),
                  true );

   Graphics g( image );
   drawable->draw( g, 1.0f );
}


void Skin::setBackground(
   DrawableComposite* background,
   AudioProcessorEditor* editor )
{
   background->deleteAllChildren(); //FIXME
   std::unique_ptr<Drawable> drawable;

   if ( skinGroup_ != nullptr ) {
      XmlElement* xmlBackground = skinGroup_->getChildByName( "background" );

      if ( xmlBackground != nullptr ) {
         String imageFilename = getString( xmlBackground,
                                           currentBackgroundName_ );
         drawable = loadImage( imageFilename );

         if ( ! background ) {
            auto message = "No background specified";

            Logger::outputDebugString( String( "[Skin] " ) + message );
            drawable = createBogusImage( message, 200, 200 );
         }
      } else {
         auto message = String( "XML element \"" ) +
                        currentGroupName_ +
                        String( "\" specifies no background image" );

         Logger::outputDebugString( String( "[Skin] " ) + message );
         drawable = createBogusImage( message, 200, 200 );
      }
   } else {
      auto message = "No skin groups found";

      Logger::outputDebugString( String( "[Skin] " ) + message );
      drawable = createBogusImage( message, 200, 200 );
   }

   backgroundWidth_ = drawable->getWidth();
   backgroundHeight_ = drawable->getHeight();

   drawable->setTopLeftPosition( 0, 0 );

   background->addAndMakeVisible( drawable.release() );
   background->setBounds( 0, 0, backgroundWidth_, backgroundHeight_ );

   if ( skinGroup_ != nullptr ) {
      forEachXmlChildElementWithTagName( *skinGroup_,
                                         xmlMeterGraduation,
                                         "meter_graduation" ) {
         String imageFilename = getString( xmlMeterGraduation,
                                           currentBackgroundName_ );
         auto drawableGraduation = loadImage( imageFilename );

         auto position = getPositionFloat(
                            xmlMeterGraduation,
                            0.0f,
                            false );

         drawableGraduation->setOriginWithOriginalSize( position );
         background->addAndMakeVisible( drawableGraduation.release() );
      }
   }

   // moves background image to the back of the editor's z-plane so
   // that it doesn't overlay (and thus block) any other components
   background->toBack();

   editor->setSize( backgroundWidth_, backgroundHeight_ );
}


Point<float> Skin::getPositionFloat(
   const XmlElement* xmlComponent,
   const float componentHeight,
   bool useRelativePosition )
{
   jassert( componentHeight >= 0.0f );

   float x = getFloat( xmlComponent, "x", 0.0f );
   float y = getFloat( xmlComponent, "y", 0.0f );

   if ( useRelativePosition && originOfYIsBottom_ ) {
      y = backgroundHeight_ - ( y + componentHeight );
   }

   return Point<float>( x, y );
}


Point<int> Skin::getPositionInteger(
   const XmlElement* xmlComponent,
   const int componentHeight,
   bool useRelativePosition )
{
   auto position = getPositionFloat( xmlComponent,
                                     static_cast<float>( componentHeight ),
                                     useRelativePosition );

   return Point<int>( math::SimpleMath::round( position.getX() ),
                      math::SimpleMath::round( position.getY() ) );
}


void Skin::printPosition(
   const String& header,
   Component* component )
{
   auto output = header;
   output << component->getX() << ", " << component->getY();

   Logger::outputDebugString( output );
}


void Skin::printSize(
   const String& header,
   Component* component )
{
   auto output = header;
   output << component->getWidth() << " x " << component->getHeight();

   Logger::outputDebugString( output );
}


void Skin::printBounds(
   const String& header,
   Component* component )
{
   auto bounds = component->getBounds();
   auto output = header;

   output <<
          bounds.getX() << ", " <<
          bounds.getY() << " (" <<
          bounds.getWidth() << " x " <<
          bounds.getHeight() << ")";

   Logger::outputDebugString( output );
}


Rectangle<float> Skin::getBoundsFloat(
   const XmlElement* xmlComponent,
   float width,
   float height,
   bool useRelativePosition )
{
   jassert( width >= 0.0f );
   jassert( height >= 0.0f );

   width = getFloat( xmlComponent, "width", width );
   height = getFloat( xmlComponent, "height", height );

   auto position = getPositionFloat( xmlComponent, height, useRelativePosition );
   auto extends = position.translated( width, height );

   return Rectangle<float>( position, extends );
}


Rectangle<int> Skin::getBoundsInteger(
   const XmlElement* xmlComponent,
   int width,
   int height,
   bool useRelativePosition )
{
   auto bounds = getBoundsFloat( xmlComponent,
                                 static_cast<float>( width ),
                                 static_cast<float>( height ),
                                 useRelativePosition );

   return Rectangle<int>( math::SimpleMath::round( bounds.getX() ),
                          math::SimpleMath::round( bounds.getY() ),
                          math::SimpleMath::round( bounds.getWidth() ),
                          math::SimpleMath::round( bounds.getHeight() ) );
}


void Skin::placeComponent(
   const XmlElement* xmlComponent,
   Component* component )
{
   jassert( component != nullptr );

   if ( document_ == nullptr ) {
      return;
   }

   Rectangle<int> bounds = getBoundsInteger( xmlComponent, 10, 10, true );
   component->setBounds( bounds );
}


void Skin::placeMeterBar(
   const String& tagName,
   widgets::MeterBar* meterBar )
{
   jassert( meterBar != nullptr );

   if ( document_ == nullptr ) {
      return;
   }

   XmlElement* xmlComponent = getComponent( tagName );

   if ( xmlComponent != nullptr ) {
      int height = meterBar->getHeight();
      Point<int> position = getPositionInteger( xmlComponent, height, true );

      meterBar->setTopLeftPosition( position );

      int segment_width = getInteger( xmlComponent, "segment_width", 0 );
      bool isVertical = getBoolean( xmlComponent, "vertical", true );

      if ( segment_width < 4 ) {
         Logger::outputDebugString(
            String( "[Skin] segment width for \"" ) +
            tagName +
            "\" not set" );

         segment_width = 8;
      }

      meterBar->setSegmentWidth( segment_width );

      // vertical bar
      if ( isVertical ) {
         meterBar->setOrientation( widgets::Orientation::vertical );
         // horizontal bar
      } else {
         meterBar->setOrientation( widgets::Orientation::horizontal );
      }
   }
}


void Skin::placeAndSkinButton(
   const String& tagName,
   DrawableButton* button )
{
   jassert( button != nullptr );

   if ( document_ == nullptr ) {
      return;
   }

   auto xmlComponent = getComponent( tagName );

   if ( xmlComponent != nullptr ) {
      auto fileNameOn = getString( xmlComponent, "image_on" );
      auto drawableOn = loadImage( fileNameOn );

      auto fileNameOff = getString( xmlComponent, "image_off" );
      auto drawableOff = loadImage( fileNameOff );

      // a missing "image_over" is handled gracefully by "setImages"
      auto fileNameOver = getString( xmlComponent, "image_over" );
      auto drawableOver = loadImage( fileNameOver );

      auto bounds = drawableOn->getDrawableBounds();
      auto boundsZero = bounds.withZeroOrigin();

      // FIXME: SVG files with implied position
      if ( bounds != boundsZero ) {
         drawableOn->setTransformToFit( boundsZero, RectanglePlacement( RectanglePlacement::xLeft | RectanglePlacement::yTop ) );
         drawableOver->setTransformToFit( boundsZero, RectanglePlacement( RectanglePlacement::xLeft | RectanglePlacement::yTop ) );
         drawableOff->setTransformToFit( boundsZero, RectanglePlacement( RectanglePlacement::xLeft | RectanglePlacement::yTop ) );

         button->setTopLeftPosition( drawableOn->getX(), drawableOn->getY() );
      }

      button->setImages( drawableOff.get(),
                         drawableOver.get(),
                         drawableOn.get(),
                         nullptr,
                         drawableOn.get(),
                         drawableOn.get(),
                         drawableOff.get(),
                         nullptr );

      button->setColour( DrawableButton::backgroundOnColourId, Colours::transparentBlack );

      int width = drawableOn->getWidth();
      int height = drawableOn->getHeight();

      // FIXME: image files with position from XML
      if ( bounds == boundsZero ) {
         Point<int> position = getPositionInteger( xmlComponent, height, true );
         button->setTopLeftPosition( position );
      }

      button->setSize( width, height );
   }
}


void Skin::placeAndSkinSlider(
   const String& tagName,
   widgets::FrutSlider* slider )
{
   if ( document_ == nullptr ) {
      return;
   }

   auto xmlComponent = getComponent( tagName );

   if ( xmlComponent == nullptr ) {
      return;
   }

   auto fileName = getString( xmlComponent, "image" );
   Colour sliderColour;
   Colour toggleSwitchColour;

   if ( fileName.isNotEmpty() ) {
      auto drawable = loadImage( fileName );
      Component* recursiveChild = drawable.get();
      DrawableShape* firstDrawableShape = dynamic_cast<DrawableShape*>( recursiveChild );

      // recursively search SVG children for a "DrawableShape"
      while ( ( recursiveChild != nullptr ) && ( firstDrawableShape == nullptr ) ) {
         recursiveChild = recursiveChild->getChildComponent( 0 );
         firstDrawableShape = dynamic_cast<DrawableShape*>( recursiveChild );
      }

      if ( firstDrawableShape ) {
         sliderColour = firstDrawableShape->getFill().colour;
         toggleSwitchColour = firstDrawableShape->getStrokeFill().colour;

         // fallback in case the drawable has no stroke fill
         if ( toggleSwitchColour.getBrightness() < 0.2f ) {
            toggleSwitchColour = Colours::red;
         }

         auto bounds = firstDrawableShape->getDrawableBounds();
         slider->setBounds( bounds.getSmallestIntegerContainer() );
      }
   } else {
      sliderColour = getColour( xmlComponent );
      toggleSwitchColour = getColour( xmlComponent, sliderColour, "toggle_" );

      placeComponent( xmlComponent, slider );
   }

   slider->setSliderColour( sliderColour );

   auto sliderCombined = dynamic_cast<frut::widgets::SliderCombined*>( slider );

   if ( sliderCombined != nullptr ) {
      sliderCombined->setToggleSwitchColour( toggleSwitchColour );
   }
}


void Skin::placeAndSkinNeedleMeter(
   const String& tagName,
   widgets::NeedleMeter* meter )
{
   jassert( meter != nullptr );

   if ( document_ == nullptr ) {
      return;
   }

   XmlElement* xmlComponent = getComponent( tagName );

   if ( xmlComponent != nullptr ) {
      Image imageBackground;
      String strImageFilenameBackground = getString( xmlComponent, "image" );

      loadImage( strImageFilenameBackground, imageBackground );

      Image imageNeedle;
      String strImageFilenameNeedle = getString( xmlComponent, "image_needle" );

      loadImage( strImageFilenameNeedle, imageNeedle );

      int spacing_left = getInteger( xmlComponent, "spacing_left", 0 );
      int spacing_top = getInteger( xmlComponent, "spacing_top", 0 );

      meter->setImages( imageBackground,
                        imageNeedle,
                        spacing_left,
                        spacing_top );

      int width = imageBackground.getWidth();
      int height = imageBackground.getHeight();

      Rectangle<int> bounds = getBoundsInteger( xmlComponent, width, height, true );
      meter->setBounds( bounds );
   }
}


void Skin::placeAndSkinLabel(
   const String& tagName,
   DrawableComposite* label )
{
   label->deleteAllChildren(); //FIXME
   XmlElement* xmlComponent = getComponent( tagName );

   if ( xmlComponent == nullptr ) {
      return;
   }

   auto fileName = getString( xmlComponent, "image" );
   auto drawable = loadImage( fileName );

   label->addAndMakeVisible( drawable.release() );
}


void Skin::placeAndSkinSignalLed(
   const String& tagName,
   widgets::SignalLed* label )
{
   jassert( label != nullptr );

   if ( document_ == nullptr ) {
      return;
   }

   XmlElement* xmlComponent = getComponent( tagName );

   if ( xmlComponent != nullptr ) {
      Image imageOff;
      String strImageFilenameOff = getString( xmlComponent, "image_off" );

      loadImage( strImageFilenameOff, imageOff );

      Image imageLow;
      String strImageFilenameLow = getString( xmlComponent, "image_low" );

      loadImage( strImageFilenameLow, imageLow );

      Image imageHigh;
      String strImageFilenameHigh = getString( xmlComponent, "image_high" );

      loadImage( strImageFilenameHigh, imageHigh );

      label->setImages( imageOff, imageLow, imageHigh );

      int width = imageOff.getWidth();

      if ( ( width != imageLow.getWidth() ) ||
           ( width != imageHigh.getWidth() ) ) {
         Logger::outputDebugString(
            String( "[Skin] width of image files for \"" ) +
            tagName +
            "\" differs" );
      }

      int height = imageOff.getHeight();

      if ( ( height != imageLow.getHeight() ) ||
           ( height != imageHigh.getHeight() ) ) {
         Logger::outputDebugString(
            String( "[Skin] height of image files for \"" ) +
            tagName +
            "\" differs" );
      }

      Rectangle<int> bounds = getBoundsInteger( xmlComponent, width, height, true );
      label->setBounds( bounds );
   }
}


void Skin::placeAndSkinStateLabel(
   const String& tagName,
   widgets::StateLabel* label )
{
   jassert( label != nullptr );

   if ( document_ == nullptr ) {
      return;
   }

   XmlElement* xmlComponent = getComponent( tagName );

   if ( xmlComponent != nullptr ) {
      Image imageOff;
      String strImageFilenameOff = getString( xmlComponent, "image_off" );

      loadImage( strImageFilenameOff, imageOff );

      Image imageOn;
      String strImageFilenameOn = getString( xmlComponent, "image_on" );

      loadImage( strImageFilenameOn, imageOn );

      Image imageActive;
      String strImageFilenameActive = getString( xmlComponent, "image_active" );

      // use "image_on" if "image_active" does not exist
      if ( ! strImageFilenameActive.isEmpty() ) {
         loadImage( strImageFilenameActive, imageActive );
      } else {
         imageActive = imageOn.createCopy();
      }

      int spacing_left = getInteger( xmlComponent, "spacing_left", 0 );
      int spacing_top = getInteger( xmlComponent, "spacing_top", 0 );
      int font_size = getInteger( xmlComponent, "font_size", 12 );

      String strColourOff = getString( xmlComponent, "colour_off", "ffffff" );
      String strColourOn = getString( xmlComponent, "colour_on", "ffffff" );
      String strColourActive = getString( xmlComponent, "colour_active", "ffffff" );

      label->setImages( imageOff,
                        imageOn,
                        imageActive,
                        strColourOff,
                        strColourOn,
                        strColourActive,
                        spacing_left,
                        spacing_top,
                        static_cast<float>( font_size ) );

      int width = imageOff.getWidth();

      if ( width != imageActive.getWidth() ) {
         Logger::outputDebugString(
            String( "[Skin] width of image files for \"" ) +
            tagName +
            "\" differs" );
      }

      int height = imageOff.getHeight();

      if ( height != imageActive.getHeight() ) {
         Logger::outputDebugString(
            String( "[Skin] height of image files for \"" ) +
            tagName +
            "\" differs" );
      }

      Rectangle<int> bounds = getBoundsInteger( xmlComponent, width, height, true );
      label->setBounds( bounds );
   }
}

}
}
