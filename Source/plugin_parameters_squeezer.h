/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013-2015 Martin Zuther (http://www.mzuther.de/)

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
#include "compressor.h"
#include "plugin_parameters.h"


//============================================================================
class SqueezerPluginParameters  : public PluginParameters
{
public:
    //==========================================================================

    SqueezerPluginParameters();
    ~SqueezerPluginParameters();

    String toString();

    int getNumParameters(bool bIncludeHiddenParameters);

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

        selStereoLinkSwitch,
        selStereoLink,
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

        nNumParametersRevealed,

        nNumParametersComplete = nNumParametersRevealed,
    };

private:
    JUCE_LEAK_DETECTOR(SqueezerPluginParameters);

    WrappedParameterToggleSwitch *ParameterBypass;
    WrappedParameterSwitch *ParameterDetectorRmsFilter;
    WrappedParameterSwitch *ParameterDesign;
    WrappedParameterSwitch *ParameterDetector;
    WrappedParameterSwitch *ParameterGainStage;

    WrappedParameterCombined *ParameterThreshold;
    WrappedParameterCombined *ParameterRatio;
    WrappedParameterCombined *ParameterKneeWidth;

    WrappedParameterCombined *ParameterAttackRate;
    WrappedParameterCombined *ParameterReleaseRate;

    WrappedParameterCombined *ParameterStereoLink;
    WrappedParameterToggleSwitch *ParameterAutoMakeupGain;
    WrappedParameterCombined *ParameterMakeupGain;
    WrappedParameterCombined *ParameterWetMix;

    WrappedParameterToggleSwitch *ParameterSidechainFilterState;
    WrappedParameterCombined *ParameterSidechainFilterCutoff;
    WrappedParameterSwitch *ParameterSidechainFilterGain;
    WrappedParameterToggleSwitch *ParameterSidechainListen;
};

#endif  // __PLUGIN_PARAMETERS_SQUEEZER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
