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

#ifndef __PLUGIN_PARAMETERS_SQUEEZER_H__
#define __PLUGIN_PARAMETERS_SQUEEZER_H__

class SqueezerPluginParameters;

#include "../JuceLibraryCode/JuceHeader.h"
#include "plugin_parameters.h"
#include "wrapped_parameter_combined.h"
#include "wrapped_parameter_switch.h"


//============================================================================
class SqueezerPluginParameters  : public PluginParameters
{
public:
    //==========================================================================

    SqueezerPluginParameters();
    ~SqueezerPluginParameters();

    int getNumParameters(bool bIncludeHiddenParameters);

    enum Parameters  // public namespace!
    {
        selBypass = 0,
        selDesign,

        selThreshold,
        selRatio,

        selAttackRate,
        selAttackMode,
        selReleaseRate,
        selReleaseMode,

        selStereoLink,
        selInputGain,
        selOutputGain,
        selWetMix,

        nNumParametersRevealed,

        nNumParametersComplete = nNumParametersRevealed,

        selDesignModern = 0,
        selDesignVintage,

        selAttackModeLinear = 0,
        selAttackModeLogarithmic,

        selReleaseModeLinear = 0,
        selReleaseModeLogarithmic,
    };

private:
    JUCE_LEAK_DETECTOR(SqueezerPluginParameters);
};

#endif  // __PLUGIN_PARAMETERS_SQUEEZER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
