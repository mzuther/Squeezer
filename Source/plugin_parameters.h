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

#ifndef __PLUGIN_PARAMETERS_SQUEEZER_H__
#define __PLUGIN_PARAMETERS_SQUEEZER_H__

#include "FrutHeader.h"
#include "compressor.h"


//============================================================================
class SqueezerPluginParameters :
    public frut::ParameterJuggler
{
public:
    //==========================================================================

    SqueezerPluginParameters();

    String toString();

    enum Parameters  // public namespace!
    {
        selBypass = 0,
        selDetectorRmsFilter,
        selDesign,
        selDetector,
        selGainStage,

        selThresholdSwitch,
        selThreshold,
        selRatioSwitch,
        selRatio,
        selKneeWidthSwitch,
        selKneeWidth,

        selAttackRateSwitch,
        selAttackRate,
        selReleaseRateSwitch,
        selReleaseRate,

#ifdef SQUEEZER_STEREO
        selStereoLinkSwitch,
        selStereoLink,
#endif
        selAutoMakeupGain,
        selMakeupGainSwitch,
        selMakeupGain,
        selWetMixSwitch,
        selWetMix,

        selSidechainFilterState,
        selSidechainFilterCutoffSwitch,
        selSidechainFilterCutoff,
        selSidechainFilterGain,
        selSidechainListen,

        numberOfParametersRevealed,

        numberOfParametersComplete = numberOfParametersRevealed,
    };

private:
    JUCE_LEAK_DETECTOR(SqueezerPluginParameters);
};

#endif  // __PLUGIN_PARAMETERS_SQUEEZER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
