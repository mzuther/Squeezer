/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2018 Martin Zuther (http://www.mzuther.de/)

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

#pragma once


// external includes
#include "JuceHeader.h"


namespace frut
{

/// Simple helper class.
///
class Frut
{
public:
    /// Get version number of Frut common classes.
    ///
    /// @return version number
    ///
    static const juce::String getVersion()
    {
        return "1.8.7";
    }


    static void printVersionNumbers()
    {
        String cpu = "CPU  " + juce::SystemStats::getCpuModel();

        if (juce::SystemStats::hasSSE())
        {
            cpu += " SSE";
        }

        if (juce::SystemStats::hasSSE2())
        {
            cpu += " SSE2";
        }

        if (juce::SystemStats::hasAVX())
        {
            cpu += " AVX";
        }

        if (juce::SystemStats::hasAVX2())
        {
            cpu += " AVX2";
        }

#ifndef DEBUG
        Logger::outputDebugString(SystemStats::getJUCEVersion());
#endif

        Logger::outputDebugString(cpu);
        Logger::outputDebugString(String("FRUT v") + frut::Frut::getVersion());
        Logger::outputDebugString(String("App  v") + JucePlugin_VersionString);

        Logger::outputDebugString("");
    }
};

}


// normal includes
#include "amalgamated/include_frut_audio.h"
#include "amalgamated/include_frut_dsp.h"
#include "amalgamated/include_frut_math.h"
#include "amalgamated/include_frut_parameter.h"
#include "amalgamated/include_frut_widget.h"

// post includes
#include "amalgamated/include_frut_skin.h"
