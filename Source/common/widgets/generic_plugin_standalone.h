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

#ifndef __JUCE_WIDGET_PLUGIN_STANDALONE_H__
#define __JUCE_WIDGET_PLUGIN_STANDALONE_H__


/// Stand-alone application for audio plug-ins.  When an instance of
/// this class is created, the plug-in is instantiated and its GUI
/// opened.
///
class GenericPluginStandalone :
    public JUCEApplication
{
public:
    virtual void initialise(const String &commandLineParameters);
    virtual void shutdown();

    virtual const String getApplicationName();
    virtual const String getApplicationVersion();

protected:
    /// Initialise settings of stand-alone.  This includes the
    /// directory in which the current state is to be stored.
    ///
    /// @param settings settings to be initialised
    virtual void initialiseSettings(PropertiesFile::Options &settings) = 0;

    ScopedPointer<StandaloneFilterWindow> filterWindow_;

private:
    JUCE_LEAK_DETECTOR(GenericPluginStandalone);
};

#endif   // __JUCE_WIDGET_PLUGIN_STANDALONE_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
