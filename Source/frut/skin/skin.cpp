/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2024 Martin Zuther (http://www.mzuther.de/)

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

namespace frut::skin
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
bool Skin::loadFromXml( const String& rootName,
                        const String& assumedVersionNumber )
{
   settingsGroup_ = nullptr;
   skinGroup_ = nullptr;
   skinFallback_1_ = nullptr;
   skinFallback_2_ = nullptr;

   backgroundWidth_ = 0;
   backgroundHeight_ = 0;

   String xmlFilename = "Skin.xml";

   Logger::outputDebugString(
      String( "[Skin] loading file \"" ) +
      xmlFilename +
      "\"" );

   document_ = loadXML( xmlFilename );

   // this should not happen!
   if ( document_ == nullptr ) {
      Logger::outputDebugString(
         String( "[Skin] file \"" ) +
         xmlFilename +
         "\" not found" );

      return false;
   }

   auto skinVersion = getString( document_.get(), "version" );

   if ( skinVersion.compare( assumedVersionNumber ) != 0 ) {
      Logger::outputDebugString(
         String( "[Skin] file \"" ) +
         xmlFilename +
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
   }

   auto originOfY = getString( document_.get(), "origin_of_y", "top" );
   originOfYIsBottom_ = originOfY.compare( "bottom" ) == 0;

   return true;
}


int Skin::getUiScale()
{
   FileInputStream in ( this->getSettingsFile() );

   if ( in.openedOk() ) {
      auto json = JSON::parse( in );
      return json.getProperty( "ui_scale", 100 );
   }

   return 100;
}


void Skin::setUiScale( int scale )
{
   FileOutputStream out ( this->getSettingsFile() );

   if ( out.openedOk() ) {
      // overwrite old settings
      out.setPosition ( 0 );
      out.truncate();

      auto settings = std::make_unique<DynamicObject>();
      settings->setProperty( "ui_scale", var( scale ) );

      auto json = var( settings.release() );
      JSON::writeToStream( out, json );
   }
}


String Skin::getQualifiedTag( XmlElement* xml )
{
   auto output = document_->getTagName();

   while ( xml != nullptr ) {
      output = xml->getTagName() + "/" + output;
      xml = document_->findParentElementOf( xml );
   }

   return output;
}


XmlElement* Skin::getSetting( const String& tagName )
{
   if ( document_ == nullptr ) {
      return nullptr;
   }

   if ( settingsGroup_ == nullptr ) {
      Logger::outputDebugString(
         String( "[Skin] settings group not found" ) );

      return nullptr;
   }

   auto xmlSetting = settingsGroup_->getChildByName( tagName );

   if ( xmlSetting == nullptr ) {
      Logger::outputDebugString(
         String( "[Skin] XML element \"" ) +
         tagName +
         "\" not found in settings" );

      return nullptr;
   }

   return xmlSetting;
}


XmlElement* Skin::getComponent( const String& tagName )
{
   if ( document_ == nullptr ) {
      return nullptr;
   }

   XmlElement* xmlComponent;

   if ( ( skinGroup_ != nullptr ) &&
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


bool Skin::getBoolean( const XmlElement* xmlComponent,
                       const String& attributeName,
                       const bool defaultValue )
{
   if ( xmlComponent == nullptr ) {
      return defaultValue;
   } else {
      return xmlComponent->getBoolAttribute( attributeName, defaultValue );
   }
}


int Skin::getInteger( const XmlElement* xmlComponent,
                      const String& attributeName,
                      const int defaultValue )
{
   if ( xmlComponent == nullptr ) {
      return defaultValue;
   } else {
      return xmlComponent->getIntAttribute( attributeName, defaultValue );
   }
}


float Skin::getFloat( const XmlElement* xmlComponent,
                      const String& attributeName,
                      const float defaultValue )
{
   if ( xmlComponent == nullptr ) {
      return defaultValue;
   } else {
      auto result = xmlComponent->getDoubleAttribute( attributeName, defaultValue );

      return static_cast<float>( result );
   }
}


const String Skin::getString( const XmlElement* xmlComponent,
                              const String& attributeName,
                              const String& defaultValue )
{
   if ( xmlComponent == nullptr ) {
      return defaultValue;
   } else {
      return xmlComponent->getStringAttribute( attributeName, defaultValue );
   }
}


const Colour Skin::getColour( const XmlElement* xmlComponent,
                              const Colour defaultColour,
                              const String valuePrefix )
{
   if ( xmlComponent == nullptr ) {
      return Colours::white;
   }

   auto hue = getFloat( xmlComponent,
                        valuePrefix + "hue",
                        defaultColour.getHue() );

   auto saturation = getFloat( xmlComponent,
                               valuePrefix + "saturation",
                               defaultColour.getSaturation() );

   auto brightness = getFloat( xmlComponent,
                               valuePrefix + "brightness",
                               defaultColour.getBrightness() );

   auto alpha = getFloat( xmlComponent,
                          valuePrefix + "alpha",
                          defaultColour.getFloatAlpha() );

   // initialise HSBA colour
   return Colour( hue, saturation, brightness, alpha );
}


// recursively search SVG for drawable and get its colours and bounds;
// return "true" when successful and "false" when not
bool Skin::getAttributesFromSvgFile( const String& tagName,
                                     const String& attributeName,
                                     Colour& strokeColour,
                                     Colour& fillColour,
                                     Rectangle<int>& bounds )
{
   auto drawable = loadImageAsDrawable( tagName, attributeName );
   Component* recursiveChild = drawable.get();
   auto firstDrawableShape = dynamic_cast<DrawableShape*>( recursiveChild );

   // recursively search SVG children for a "DrawableShape"
   while ( ( recursiveChild != nullptr ) && ( firstDrawableShape == nullptr ) ) {
      recursiveChild = recursiveChild->getChildComponent( 0 );
      firstDrawableShape = dynamic_cast<DrawableShape*>( recursiveChild );
   }

   if ( firstDrawableShape == nullptr ) {
      return false;
   }

   strokeColour = firstDrawableShape->getStrokeFill().colour;
   fillColour = firstDrawableShape->getFill().colour;

   auto boundsFloat = firstDrawableShape->getDrawableBounds();
   bounds = boundsFloat.getSmallestIntegerContainer();

   return true;
}


std::unique_ptr<Drawable> Skin::createBogusImage( const String& warningText,
                                                  int width,
                                                  int height )
{
   auto drawable = std::make_unique<DrawableComposite>();
   auto boundingBox = Rectangle<float>(
                         0,
                         0,
                         static_cast<float>( width ),
                         static_cast<float>( height ) );

   auto rectangle = std::make_unique<DrawablePath>();
   rectangle->setFill( Colours::red.withAlpha( 0.8f ) );

   Path path;
   path.addRectangle( boundingBox );
   rectangle->setPath( path );

   auto text = std::make_unique<DrawableText>();

   text->setText( warningText );
   text->setColour( Colours::white );
   text->setFontHeight( 20 );
   text->setJustification( Justification::Flags::topLeft );

   float shrinkFactor = 10.0f;
   text->setBoundingBox( boundingBox.reduced( shrinkFactor ).withPosition( shrinkFactor, shrinkFactor ) );

   drawable->addAndMakeVisible( rectangle.release() );
   drawable->addAndMakeVisible( text.release() );

   return std::unique_ptr<Drawable>( drawable.release() );
}


std::unique_ptr<Drawable> Skin::loadImageAsDrawable( const String& strFilename )
{
   std::unique_ptr<Drawable> component;

   if ( resourceExists( strFilename ) ) {
      component = loadDrawable( strFilename );

      if ( ! component ) {
         Logger::outputDebugString(
            String( "[Skin] image file \"" ) +
            strFilename +
            "\" not loaded" );

         component = createBogusImage( "Image file not loaded", 200, 200 );
      }
   } else {
      Logger::outputDebugString(
         String( "[Skin] image file \"" ) +
         strFilename +
         "\" not found" );

      component = createBogusImage( "Image file not found", 200, 200 );
   }

   return component;
}


std::unique_ptr<Drawable> Skin::loadImageAsDrawable( const String& tagName,
                                                     const String& attributeName,
                                                     Drawable* alternativeDrawable )
{
   if ( document_ == nullptr ) {
      Logger::outputDebugString( "[Skin] invalid skin" );
      return createBogusImage( "Invalid skin", 200, 200 );
   }

   auto xmlComponent = getComponent( tagName );

   // use alternative if attribute does not exist
   if ( xmlComponent == nullptr ) {
      if ( alternativeDrawable ) {
         return alternativeDrawable->createCopy();
      }

      Logger::outputDebugString(
         String( "[Skin] tag \"" ) +
         tagName +
         "\" not found" );

      return createBogusImage( "Tag not found", 200, 200 );
   }

   auto imageFilename = getString( xmlComponent, attributeName );

   if ( imageFilename.isEmpty() ) {
      if ( alternativeDrawable ) {
         return alternativeDrawable->createCopy();
      }

      Logger::outputDebugString(
         String( "[Skin] attribute \"" ) +
         attributeName + "\" not found in tag \"" +
         getQualifiedTag( xmlComponent ) + "\""
      );

      return createBogusImage( "Attribute not found", 200, 200 );
   }

   return loadImageAsDrawable( imageFilename );
}


Image Skin::loadImage( const String& strFilename )
{
   auto drawable = loadImageAsDrawable( strFilename );

   Image image( Image::PixelFormat::ARGB,
                drawable->getWidth(),
                drawable->getHeight(),
                true );

   Graphics g( image );
   drawable->draw( g, 1.0f );

   return image;
}


Image Skin::loadImage( const String& tagName,
                       const String& attributeName )
{
   auto drawable = loadImageAsDrawable( tagName, attributeName );

   Image image( Image::PixelFormat::ARGB,
                drawable->getWidth(),
                drawable->getHeight(),
                true );

   Graphics g( image );
   drawable->draw( g, 1.0f );

   return image;
}


void Skin::setBackground( DrawableComposite* background,
                          AudioProcessorEditor* editor )
{
   std::unique_ptr<Drawable> drawable;

   if ( skinGroup_ != nullptr ) {
      auto xmlBackground = skinGroup_->getChildByName( "background" );

      if ( xmlBackground != nullptr ) {
         auto imageFilename = getString( xmlBackground,
                                         currentBackgroundName_ );
         drawable = loadImageAsDrawable( imageFilename );

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
      for ( auto* xmlMeterGraduation : skinGroup_->getChildWithTagNameIterator(
               "meter_graduation" ) ) {
         auto imageFilename = getString( xmlMeterGraduation,
                                         currentBackgroundName_ );
         auto drawableGraduation = loadImageAsDrawable( imageFilename );

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


Image Skin::imageFromDrawable( std::unique_ptr<Drawable>& drawable )
{
   auto bounds = drawable->getDrawableBounds();
   auto boundsZero = bounds.withZeroOrigin();

   if ( bounds != boundsZero ) {
      drawable->setTransformToFit( boundsZero, RectanglePlacement(
                                      RectanglePlacement::xLeft | RectanglePlacement::yTop ) );
   }

   auto image = Image( Image::PixelFormat::ARGB,
                       drawable->getWidth(),
                       drawable->getHeight(),
                       true );

   Graphics g( image );
   drawable->draw( g, 1.0f );

   return image;
}


Point<float> Skin::getPositionFloat( const XmlElement* xmlComponent,
                                     const float componentHeight,
                                     bool useRelativePosition )
{
   jassert( componentHeight >= 0.0f );

   auto x = getFloat( xmlComponent, "x", 0.0f );
   auto y = getFloat( xmlComponent, "y", 0.0f );

   if ( useRelativePosition && originOfYIsBottom_ ) {
      y = backgroundHeight_ - ( y + componentHeight );
   }

   return Point<float>( x, y );
}


Point<int> Skin::getPositionInteger( const XmlElement* xmlComponent,
                                     const int componentHeight,
                                     bool useRelativePosition )
{
   auto position = getPositionFloat( xmlComponent,
                                     static_cast<float>( componentHeight ),
                                     useRelativePosition );

   return Point<int>( math::SimpleMath::round( position.getX() ),
                      math::SimpleMath::round( position.getY() ) );
}


void Skin::printPosition( const String& header,
                          Component* component )
{
   auto output = header;
   output << component->getX() << ", " << component->getY();

   Logger::outputDebugString( output );
}


void Skin::printSize( const String& header,
                      Component* component )
{
   auto output = header;
   output << component->getWidth() << " x " << component->getHeight();

   Logger::outputDebugString( output );
}


void Skin::printBounds( const String& header,
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


Rectangle<float> Skin::getBoundsFloat( const XmlElement* xmlComponent,
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


Rectangle<int> Skin::getBoundsInteger( const XmlElement* xmlComponent,
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


void Skin::placeComponent( const XmlElement* xmlComponent,
                           Component* component )
{
   jassert( component != nullptr );

   if ( xmlComponent == nullptr ) {
      return;
   }

   auto bounds = getBoundsInteger( xmlComponent, 10, 10, true );
   component->setBounds( bounds );
}


void Skin::placeMeterBar( const String& tagName,
                          widgets::MeterBar* meterBar )
{
   jassert( meterBar != nullptr );

   auto drawable = loadImageAsDrawable( tagName, "image" );

   auto bounds = drawable->getDrawableBounds();
   auto boundsInteger = bounds.getSmallestIntegerContainer();
   auto segmentWidth = boundsInteger.getWidth();

   meterBar->setBounds( boundsInteger );

   if ( segmentWidth < 4 ) {
      Logger::outputDebugString(
         String( "[Skin] segment width for \"" ) +
         tagName +
         "\" too small" );

      segmentWidth = 8;
   }

   meterBar->setSegmentWidth( segmentWidth );
}


void Skin::placeAndSkinButton( const String& tagName,
                               DrawableButton* button )
{
   jassert( button != nullptr );

   auto drawableOn = loadImageAsDrawable( tagName, "image_on" );
   auto drawableOff = loadImageAsDrawable( tagName, "image_off" );
   auto drawableOver = loadImageAsDrawable( tagName, "image_over", drawableOn.get() );

   auto bounds = drawableOn->getDrawableBounds();
   auto boundsZero = bounds.withZeroOrigin();

   if ( bounds != boundsZero ) {
      drawableOn->setTransformToFit( boundsZero, RectanglePlacement( RectanglePlacement::xLeft | RectanglePlacement::yTop ) );
      drawableOff->setTransformToFit( boundsZero, RectanglePlacement( RectanglePlacement::xLeft | RectanglePlacement::yTop ) );
      drawableOver->setTransformToFit( boundsZero, RectanglePlacement( RectanglePlacement::xLeft | RectanglePlacement::yTop ) );

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

   button->setColour( DrawableButton::backgroundOnColourId,
                      Colours::transparentBlack );

   button->setSize( drawableOn->getWidth(),
                    drawableOn->getHeight() );
}


void Skin::placeAndSkinSlider( const String& tagName,
                               widgets::FrutSlider* slider )
{
   auto xmlComponent = getComponent( tagName );

   if ( xmlComponent == nullptr ) {
      return;
   }

   Colour sliderColour;
   Colour toggleSwitchColour;
   Rectangle<int> bounds;

   auto success = getAttributesFromSvgFile( tagName, "image",
                                            toggleSwitchColour, sliderColour, bounds );

   if ( success ) {
      // fallback in case the drawable has no stroke fill
      if ( toggleSwitchColour.getBrightness() < 0.2f ) {
         toggleSwitchColour = Colours::red;
      }

      slider->setBounds( bounds );
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


void Skin::placeAndSkinNeedleMeter( const String& tagName,
                                    widgets::NeedleMeter* meter )
{
   jassert( meter != nullptr );

   auto xmlComponent = getComponent( tagName );

   if ( xmlComponent == nullptr ) {
      return;
   }

   auto drawableBackground = loadImageAsDrawable( tagName, "image" );
   auto drawableNeedle = loadImageAsDrawable( tagName, "image_needle" );

   auto bounds = drawableBackground->getDrawableBounds();
   auto boundsZero = bounds.withZeroOrigin();

   if ( bounds != boundsZero ) {
      drawableBackground->setTransformToFit( boundsZero, RectanglePlacement( RectanglePlacement::xLeft | RectanglePlacement::yTop ) );
      meter->setTopLeftPosition( drawableBackground->getX(), drawableBackground->getY() );
   }

   auto spacing_left = getInteger( xmlComponent, "spacing_left", 0 );
   auto spacing_top = getInteger( xmlComponent, "spacing_top", 0 );

   meter->setImages( imageFromDrawable( drawableBackground ),
                     imageFromDrawable( drawableNeedle ),
                     spacing_left,
                     spacing_top );

   meter->setSize( drawableBackground->getWidth(),
                   drawableBackground->getHeight() );
}


void Skin::placeAndSkinLabel( const String& tagName,
                              DrawableComposite* label )
{
   auto drawable = loadImageAsDrawable( tagName, "image" );
   label->addAndMakeVisible( drawable.release() );
}


void Skin::placeAndSkinSignalLed( const String& tagName,
                                  widgets::SignalLed* label )
{
   jassert( label != nullptr );

   auto drawableOff = loadImageAsDrawable( tagName, "image_off" );
   auto drawableLow = loadImageAsDrawable( tagName, "image_low" );
   auto drawableHigh = loadImageAsDrawable( tagName, "image_high" );

   auto bounds = drawableOff->getDrawableBounds();
   auto boundsZero = bounds.withZeroOrigin();

   if ( bounds != boundsZero ) {
      drawableOff->setTransformToFit( boundsZero, RectanglePlacement( RectanglePlacement::xLeft | RectanglePlacement::yTop ) );
      drawableLow->setTransformToFit( boundsZero, RectanglePlacement( RectanglePlacement::xLeft | RectanglePlacement::yTop ) );
      drawableHigh->setTransformToFit( boundsZero, RectanglePlacement( RectanglePlacement::xLeft | RectanglePlacement::yTop ) );

      label->setTopLeftPosition( drawableOff->getX(), drawableOff->getY() );
   }

   label->setImages( imageFromDrawable( drawableOff ),
                     imageFromDrawable( drawableLow ),
                     imageFromDrawable( drawableHigh ) );

   label->setSize( drawableOff->getWidth(),
                   drawableOff->getHeight() );
}


void Skin::placeAndSkinStateLabel( const String& tagName,
                                   widgets::StateLabel* label )
{
   jassert( label != nullptr );

   auto xmlComponent = getComponent( tagName );

   if ( xmlComponent == nullptr ) {
      return;
   }

   auto drawableOff = loadImageAsDrawable( tagName, "image_off" );
   auto drawableOn = loadImageAsDrawable( tagName, "image_on" );
   auto drawableActive = loadImageAsDrawable( tagName, "image_active", drawableOn.get() );

   auto bounds = drawableOn->getDrawableBounds();
   auto boundsZero = bounds.withZeroOrigin();

   if ( bounds != boundsZero ) {
      drawableOff->setTransformToFit( boundsZero, RectanglePlacement( RectanglePlacement::xLeft | RectanglePlacement::yTop ) );
      drawableOn->setTransformToFit( boundsZero, RectanglePlacement( RectanglePlacement::xLeft | RectanglePlacement::yTop ) );
      drawableActive->setTransformToFit( boundsZero, RectanglePlacement( RectanglePlacement::xLeft | RectanglePlacement::yTop ) );

      label->setTopLeftPosition( drawableOn->getX(), drawableOn->getY() );
   }

   auto spacing_left = getInteger( xmlComponent, "spacing_left", 0 );
   auto spacing_top = getInteger( xmlComponent, "spacing_top", 0 );
   auto font_size = getInteger( xmlComponent, "font_size", 12 );

   auto textColourOff = getString( xmlComponent, "text_colour_off", "ffffff" );
   auto textColourOn = getString( xmlComponent, "text_colour_on", "ffffff" );
   auto textColourActive = getString( xmlComponent, "text_colour_active", "ffffff" );

   label->setImages( imageFromDrawable( drawableOff ),
                     imageFromDrawable ( drawableOn ),
                     imageFromDrawable( drawableActive ),
                     textColourOff,
                     textColourOn,
                     textColourActive,
                     spacing_left,
                     spacing_top,
                     static_cast<float>( font_size ) );

   label->setSize( drawableOff->getWidth(),
                   drawableOff->getHeight() );
}

} // namespace
