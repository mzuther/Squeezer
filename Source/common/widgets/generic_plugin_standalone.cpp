/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2016 Martin Zuther (http://www.mzuther.de/)

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

#include "generic_plugin_standalone.h"

/// Called when the application starts.
///
/// @param commandLineParameters the line passed in does not include
///        the name of the executable, just the parameter list. To get
///        the parameters as an array, you can call
///        JUCEApplication::getCommandLineParameters().
///
void GenericPluginStandalone::initialise(
    const String &commandLineParameters)

{
    // load standalone settings
    PropertiesFile::Options settings;
    initialiseSettings(settings);
    PropertiesFile *propertiesFile = new PropertiesFile(settings);

    // instantiate GUI
    filterWindow_ = new StandaloneFilterWindow(
        getApplicationName(),
        Colours::black,
        propertiesFile,
        true);

    // add title bar buttons to GUI
    filterWindow_->setTitleBarButtonsRequired(
        DocumentWindow::allButtons,
        false);

    // display GUI
    filterWindow_->setVisible(true);

    // GUI cannot be resized
    filterWindow_->setResizable(false, true);
}


/// Called when the application shuts down.
///
void GenericPluginStandalone::shutdown()
{
    // save plug-in settings
    filterWindow_->pluginHolder->savePluginState();

    // kill GUI
    filterWindow_ = nullptr;
}


/// Returns the application's name.
///
/// @return the application's name
///
const String GenericPluginStandalone::getApplicationName()
{
    return JucePlugin_Name;
}


/// Returns the application's version number.
///
/// @return the application's version number
///
const String GenericPluginStandalone::getApplicationVersion()
{
    return JucePlugin_VersionString;
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
