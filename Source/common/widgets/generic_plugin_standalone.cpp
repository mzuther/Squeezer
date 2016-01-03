/* ----------------------------------------------------------------------------

   MZ common JUCE
   ==============
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

/// Constructor.  If a subclass implements a constructor, it shouldn't
/// call any JUCE code in there -- put your startup code in
/// initialise() instead.
///
GenericPluginStandalone::GenericPluginStandalone()
{
}


/// Destructor.  If a subclass implements a destructor, it shouldn't
/// call any JUCE code in there -- put your shutdown code in
/// shutdown() instead.
///
GenericPluginStandalone::~GenericPluginStandalone()
{
}


/// Called when the application starts.
///
/// @param commandLineParameters the line passed in does not include
/// the name of the executable, just the parameter list. To get the
/// parameters as an array, you can call
/// JUCEApplication::getCommandLineParameters().
///
void GenericPluginStandalone::initialise(const String &commandLineParameters)
{
    // load properties that were prepared in subclass
    PropertiesFile::Options options = prepare_properties();
    PropertiesFile *pPropertiesFile = new PropertiesFile(options);

    // instantiate GUI
    filterWindow = new StandaloneFilterWindow(getApplicationName(), Colours::black, pPropertiesFile, true);

    // add title bar buttons to GUI
    filterWindow->setTitleBarButtonsRequired(DocumentWindow::allButtons, false);

    // display GUI
    filterWindow->setVisible(true);

    // GUI cannot be resized
    filterWindow->setResizable(false, true);
}


/// Called when the application shuts down.
///
void GenericPluginStandalone::shutdown()
{
    // save plug-in settings
    filterWindow->pluginHolder->savePluginState();

    // kill GUI
    filterWindow = nullptr;
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
