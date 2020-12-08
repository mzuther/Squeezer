/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of citrus

   Copyright (c) 2013-2020 Martin Zuther (http://www.mzuther.de/)

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

#include "skin.h"


bool Skin::loadSkin(
   int numberOfChannels,
   bool loadExternalResources )
{
   loadExternalResources_ = loadExternalResources;

   if ( loadExternalResources_ ) {
      Logger::outputDebugString( "" );
      Logger::outputDebugString( "********************************************************************************" );
      Logger::outputDebugString( "*                                                                              *" );
      Logger::outputDebugString( "*  Loading resources from external file.  Please turn off before committing!   *" );
      Logger::outputDebugString( "*                                                                              *" );
      Logger::outputDebugString( "********************************************************************************" );
      Logger::outputDebugString( "" );
   }

   updateSkin( numberOfChannels );

   return loadFromXml( "squeezer-skin", "1.3" );
}


void Skin::updateSkin(
   int numberOfChannels )
{
   jassert( numberOfChannels > 0 );

   if ( loadExternalResources_ && ! getSkinDirectory().isDirectory() ) {
      Logger::outputDebugString(
         String( "[Skin] directory \"" ) +
         getSkinDirectory().getFullPathName() +
         "\" not found" );

      document_ = nullptr;
   }

   currentBackgroundName_ = "image";

   if ( numberOfChannels == 1 ) {
      currentFallbackName_ = "mono";
   } else {
      currentFallbackName_ = "stereo";
   }

   currentGroupName_ = currentFallbackName_;

   if ( document_ != nullptr ) {
      skinGroup_ = document_->getChildByName( currentGroupName_ );
      skinFallback_1_ = document_->getChildByName( currentFallbackName_ );
      skinFallback_2_ = document_->getChildByName( "default" );
   } else {
      skinGroup_ = nullptr;
      skinFallback_1_ = nullptr;
      skinFallback_2_ = nullptr;
   }
}


File Skin::getSkinDirectory()
{
   jassert( loadExternalResources_ );

   auto resourceDirectory = SqueezerPluginParameters::getResourceDirectory();
   return resourceDirectory.getChildFile( "./Skins/Resources/" );
}


File Skin::getSettingsFile()
{
   auto settingsDirectory = File::getSpecialLocation( File::userApplicationDataDirectory );
   auto settingsFile = settingsDirectory.getChildFile( "Squeezer.settings" );

   return settingsFile;
}


bool Skin::resourceExists( const String& strFilename )
{
   if ( loadExternalResources_ ) {
      auto fileImage = getSkinDirectory().getChildFile( strFilename );
      return fileImage.existsAsFile();
   } else {
      return squeezer::skin::resourceExists( strFilename );
   }
}


std::unique_ptr<Drawable> Skin::loadDrawable( const String& strFilename )
{
   if ( loadExternalResources_ ) {
      auto fileImage = getSkinDirectory().getChildFile( strFilename );
      return Drawable::createFromImageFile( fileImage );
   } else {
      return squeezer::skin::getDrawable( strFilename );
   }
}


std::unique_ptr<XmlElement> Skin::loadXML( const String& strFilename )
{
   if ( loadExternalResources_ ) {
      auto skinFile = getSkinDirectory().getChildFile( strFilename );
      return juce::parseXML( skinFile );
   } else {
      auto xmlData = squeezer::skin::getStringUTF8( strFilename );
      return juce::parseXML( xmlData );
   }
}
