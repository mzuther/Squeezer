/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013-2018 Martin Zuther (http://www.mzuther.de/)

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
    File &skinFile,
    int numberOfChannels)

{
    updateSkin(numberOfChannels);

    return loadFromXml(skinFile, "squeezer-skin", "1.0");
}


void Skin::updateSkin(
    int numberOfChannels)

{
    jassert(numberOfChannels > 0);

    currentBackgroundName_ = "image";

    if (numberOfChannels == 1)
    {
        currentFallbackName_ = "mono";
    }
    else
    {
        currentFallbackName_ = "stereo";
    }

    currentGroupName_ = currentFallbackName_;

    if (document_ != nullptr)
    {
        skinGroup_ = document_->getChildByName(currentGroupName_);
        skinFallback_1_ = document_->getChildByName(currentFallbackName_);
        skinFallback_2_ = document_->getChildByName("default");
    }
    else
    {
        skinGroup_ = nullptr;
        skinFallback_1_ = nullptr;
        skinFallback_2_ = nullptr;
    }
}
