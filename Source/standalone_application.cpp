/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013 Martin Zuther (http://www.mzuther.de/)

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

StandaloneApplication::StandaloneApplication()
{
}

StandaloneApplication::~StandaloneApplication()
{
}

void StandaloneApplication::initialise(const String& commandLineParameters)
{
    PropertiesFile::Options options;
    options.applicationName     = "squeezer_stereo";
    options.folderName          = ".config";
    options.filenameSuffix      = "ini";
    options.osxLibrarySubFolder = "Application Support";

    PropertiesFile* pPropertiesFile = new PropertiesFile(options);
    String strApplicationName = getApplicationName();

    filterWindow = new StandaloneFilterWindow(strApplicationName, Colours::black, pPropertiesFile);

    filterWindow->setTitleBarButtonsRequired(DocumentWindow::allButtons, false);
    filterWindow->setVisible(true);
    filterWindow->setResizable(false, true);
}

void StandaloneApplication::shutdown()
{
    deleteAndZero(filterWindow);
}

const String StandaloneApplication::getApplicationName()
{
    return JucePlugin_Name;
}

const String StandaloneApplication::getApplicationVersion()
{
    return JucePlugin_VersionString;
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
