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

#ifndef SQUEEZER_SKIN_H
#define SQUEEZER_SKIN_H

#include "FrutHeader.h"
#include "plugin_parameters.h"
#include "squeezer_skin.h"


class Skin :
   public frut::skin::Skin
{
public:
   bool loadSkin( int numberOfChannels,
                  bool loadExternalResources );

   void updateSkin( int numberOfChannels );

   virtual File getSkinDirectory() override;
   virtual File getSettingsFile() override;

protected:
   bool loadExternalResources_;

   virtual bool resourceExists( const String& strFilename ) override;

   virtual std::unique_ptr<Drawable> loadDrawable( const String& strFilename ) override;
   virtual std::unique_ptr<XmlElement> loadXML( const String& strFilename ) override;

private:
   JUCE_LEAK_DETECTOR( Skin );
};

#endif  // SQUEEZER_SKIN_H
