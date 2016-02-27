/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013-2016 Martin Zuther (http://www.mzuther.de/)

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

#include "standalone_application.h"


/// Initialise settings of stand-alone.  This includes the directory
/// in which the current state is to be stored.
///
/// @param settings settings to be initialised
void SqueezerStandalone::initialiseSettings(
    PropertiesFile::Options &settings)

{
#ifdef SQUEEZER_MONO
    settings.applicationName     = "squeezer_mono";
#else
    settings.applicationName     = "squeezer_stereo";
#endif

    settings.filenameSuffix      = "ini";
    settings.folderName          = ".config";
    settings.osxLibrarySubFolder = "Application Support";
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
