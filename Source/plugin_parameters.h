/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of citrus

   Copyright (c) 2013-2020 Martin Zuther (http://www.mzuther.de/)

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

#ifndef SQUEEZER_PLUGIN_PARAMETERS_H
#define SQUEEZER_PLUGIN_PARAMETERS_H

#include "FrutHeader.h"
#include "compressor.h"


class SqueezerPluginParameters :
    public frut::parameters::Juggler
{
public:
    SqueezerPluginParameters();

    String toString();

    static const File getResourceDirectory();

    enum Parameters  // public namespace!
    {
        selBypass = 0,
        selRmsWindowSize,
        selDesign,
        selCurveType,
        selGainStage,

        selThresholdSwitch,
        selThreshold,
        selRatioSwitch,
        selRatio,
        selKneeWidth,

        selAttackRateSwitch,
        selAttackRate,
        selReleaseRateSwitch,
        selReleaseRate,

        selAutoMakeupGain,
        selMakeupGainSwitch,
        selMakeupGain,
        selWetMixSwitch,
        selWetMix,

        selSidechainInput,
        selSidechainHPFCutoffSwitch,
        selSidechainHPFCutoff,
        selSidechainLPFCutoffSwitch,
        selSidechainLPFCutoff,
        selSidechainListen,

        selInputTrimSwitch,
        selInputTrim,
        selStereoLinkSwitch,
        selStereoLink,

        numberOfParametersRevealed,

        numberOfParametersComplete = numberOfParametersRevealed,
    };

    virtual XmlElement *handleUpgrades(XmlElement *xmlDocument,
                                       int oldMajorVersion,
                                       int oldMinorVersion) override;

private:
    JUCE_LEAK_DETECTOR(SqueezerPluginParameters);
};

#endif  // SQUEEZER_PLUGIN_PARAMETERS_H
