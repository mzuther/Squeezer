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

#include "plugin_parameters.h"


// The methods of this class may be called on the audio thread, so
// they are absolutely time-critical!

SqueezerPluginParameters::SqueezerPluginParameters() :
    Juggler("SQUEEZER_SETTINGS", numberOfParametersComplete,
            numberOfParametersRevealed)
{
    // parameters created here will be deleted in
    // "frut::parameter::Juggler"!

    frut::parameter::ParBoolean *ParameterBypass =
        new frut::parameter::ParBoolean("Bypassed", "Active");
    ParameterBypass->setName("Bypass");
    ParameterBypass->setDefaultBoolean(false, true);
    add(ParameterBypass, selBypass);


    frut::parameter::ParSwitch *ParameterDetectorRmsFilter =
        new frut::parameter::ParSwitch();
    ParameterDetectorRmsFilter->setName("RMS Filter");

    ParameterDetectorRmsFilter->addPreset(0.0f,  "Peak");
    ParameterDetectorRmsFilter->addPreset(30.0f, "RMS (30 ms)");

    ParameterDetectorRmsFilter->setDefaultRealFloat(30.0f, true);
    add(ParameterDetectorRmsFilter, selDetectorRmsFilter);


    frut::parameter::ParSwitch *ParameterDesign =
        new frut::parameter::ParSwitch();
    ParameterDesign->setName("Design");

    ParameterDesign->addPreset(Compressor::DesignFeedForward, "Feed-Forward");
    ParameterDesign->addPreset(Compressor::DesignFeedBack,    "Feed-Back");

    ParameterDesign->setDefaultRealFloat(Compressor::DesignFeedBack, true);
    add(ParameterDesign, selDesign);


    frut::parameter::ParSwitch *ParameterDetector =
        new frut::parameter::ParSwitch();
    ParameterDetector->setName("Detector");

    ParameterDetector->addPreset(Compressor::DetectorLinear,          "Linear");
    ParameterDetector->addPreset(Compressor::DetectorSmoothDecoupled, "S-Curve");
    ParameterDetector->addPreset(Compressor::DetectorSmoothBranching, "Logarithmic");

    ParameterDetector->setDefaultRealFloat(Compressor::DetectorSmoothBranching, true);
    add(ParameterDetector, selDetector);


    frut::parameter::ParSwitch *ParameterGainStage =
        new frut::parameter::ParSwitch();
    ParameterGainStage->setName("Gain Stage");

    ParameterGainStage->addPreset(Compressor::GainStageFET,     "FET");
    ParameterGainStage->addPreset(Compressor::GainStageOptical, "Optical");

    ParameterGainStage->setDefaultRealFloat(Compressor::GainStageFET, true);
    add(ParameterGainStage, selGainStage);


    float realMinimum = -60.0f;
    float realMaximum = +18.0f;
    float stepSize = 1.0f;
    float scalingFactor = 0.0f;
    int decimalPlaces = 0;

    frut::parameter::ParCombined *ParameterThreshold =
        new frut::parameter::ParCombined(realMinimum, realMaximum, stepSize, scalingFactor, decimalPlaces);
    ParameterThreshold->setName("Threshold");

    ParameterThreshold->addPreset(-48.0f, "-48 dB");
    ParameterThreshold->addPreset(-46.0f, "-46 dB");
    ParameterThreshold->addPreset(-44.0f, "-44 dB");
    ParameterThreshold->addPreset(-42.0f, "-42 dB");
    ParameterThreshold->addPreset(-40.0f, "-40 dB");
    ParameterThreshold->addPreset(-38.0f, "-38 dB");
    ParameterThreshold->addPreset(-36.0f, "-36 dB");
    ParameterThreshold->addPreset(-34.0f, "-34 dB");
    ParameterThreshold->addPreset(-32.0f, "-32 dB");
    ParameterThreshold->addPreset(-30.0f, "-30 dB");
    ParameterThreshold->addPreset(-28.0f, "-28 dB");
    ParameterThreshold->addPreset(-26.0f, "-26 dB");
    ParameterThreshold->addPreset(-24.0f, "-24 dB");
    ParameterThreshold->addPreset(-22.0f, "-22 dB");
    ParameterThreshold->addPreset(-20.0f, "-20 dB");
    ParameterThreshold->addPreset(-18.0f, "-18 dB");
    ParameterThreshold->addPreset(-16.0f, "-16 dB");
    ParameterThreshold->addPreset(-14.0f, "-14 dB");
    ParameterThreshold->addPreset(-12.0f, "-12 dB");
    ParameterThreshold->addPreset(-10.0f, "-10 dB");
    ParameterThreshold->addPreset(-8.0f,   "-8 dB");
    ParameterThreshold->addPreset(-6.0f,   "-6 dB");
    ParameterThreshold->addPreset(-4.0f,   "-4 dB");
    ParameterThreshold->addPreset(-2.0f,   "-2 dB");
    ParameterThreshold->addPreset(0.0f,     "0 dB");
    ParameterThreshold->addPreset(+2.0f,   "+2 dB");
    ParameterThreshold->addPreset(+4.0f,   "+4 dB");
    ParameterThreshold->addPreset(+6.0f,   "+6 dB");
    ParameterThreshold->addPreset(+8.0f,   "+8 dB");
    ParameterThreshold->addPreset(+10.0f, "+10 dB");
    ParameterThreshold->addPreset(+12.0f, "+12 dB");
    ParameterThreshold->addPreset(+14.0f, "+14 dB");
    ParameterThreshold->addPreset(+16.0f, "+16 dB");
    ParameterThreshold->addPreset(+18.0f, "+18 dB");

    ParameterThreshold->setSuffix(" dB");
    ParameterThreshold->setDefaultRealFloat(-12.0f, true);
    addCombined(ParameterThreshold, selThresholdSwitch, selThreshold);


    realMinimum = 0.1f;
    realMaximum = 10.0f;
    stepSize = 0.05f;
    scalingFactor = 1.0f;
    decimalPlaces = 2;

    frut::parameter::ParCombined *ParameterRatio =
        new frut::parameter::ParCombined(realMinimum, realMaximum, stepSize, scalingFactor, decimalPlaces);
    ParameterRatio->setName("Ratio");

    ParameterRatio->addPreset(0.50f, "0.50:1");
    ParameterRatio->addPreset(0.67f, "0.67:1");
    ParameterRatio->addPreset(0.83f, "0.83:1");
    ParameterRatio->addPreset(0.91f, "0.91:1");
    ParameterRatio->addPreset(1.00f, "Bypass");
    ParameterRatio->addPreset(1.10f,  "1.1:1");
    ParameterRatio->addPreset(1.20f,  "1.2:1");
    ParameterRatio->addPreset(1.50f,  "1.5:1");
    ParameterRatio->addPreset(2.00f,    "2:1");
    ParameterRatio->addPreset(2.50f,  "2.5:1");
    ParameterRatio->addPreset(3.00f,    "3:1");
    ParameterRatio->addPreset(4.00f,    "4:1");
    ParameterRatio->addPreset(6.00f,    "6:1");
    ParameterRatio->addPreset(8.00f,    "8:1");
    ParameterRatio->addPreset(10.00f,  "10:1");

    ParameterRatio->setSuffix(":1");
    ParameterRatio->setDefaultRealFloat(2.0f, true);
    addCombined(ParameterRatio, selRatioSwitch, selRatio);


    realMinimum = 0.0f;
    realMaximum = 48.0f;
    stepSize = 6.0f;
    scalingFactor = 0.0f;
    decimalPlaces = 0;

    frut::parameter::ParCombined *ParameterKneeWidth =
        new frut::parameter::ParCombined(realMinimum, realMaximum, stepSize, scalingFactor, decimalPlaces);
    ParameterKneeWidth->setName("Knee Width");

    ParameterKneeWidth->addPreset(0.0f,  "Hard");
    ParameterKneeWidth->addPreset(12.0f, "Medium");
    ParameterKneeWidth->addPreset(48.0f, "Soft");

    ParameterKneeWidth->setSuffix(" dB");
    ParameterKneeWidth->setDefaultRealFloat(0.0f, true);
    addCombined(ParameterKneeWidth, selKneeWidthSwitch, selKneeWidth);


    realMinimum = 0.0f;
    realMaximum = 500.0f;
    stepSize = 1.0f;
    scalingFactor = 2.0f;
    decimalPlaces = 0;

    frut::parameter::ParCombined *ParameterAttackRate =
        new frut::parameter::ParCombined(realMinimum, realMaximum, stepSize, scalingFactor, decimalPlaces);
    ParameterAttackRate->setName("Attack Rate");

    ParameterAttackRate->addPreset(1.0f,     "1 ms");
    ParameterAttackRate->addPreset(2.0f,     "2 ms");
    ParameterAttackRate->addPreset(5.0f,     "5 ms");
    ParameterAttackRate->addPreset(10.0f,   "10 ms");
    ParameterAttackRate->addPreset(20.0f,   "20 ms");
    ParameterAttackRate->addPreset(50.0f,   "50 ms");
    ParameterAttackRate->addPreset(100.0f, "100 ms");
    ParameterAttackRate->addPreset(200.0f, "200 ms");
    ParameterAttackRate->addPreset(500.0f, "500 ms");

    ParameterAttackRate->setSuffix(" ms");
    ParameterAttackRate->setDefaultRealFloat(20.0f, true);
    addCombined(ParameterAttackRate, selAttackRateSwitch, selAttackRate);


    realMinimum = 0.0f;
    realMaximum = 8000.0f;
    stepSize = 1.0f;
    scalingFactor = 3.0f;
    decimalPlaces = 0;

    frut::parameter::ParCombined *ParameterReleaseRate =
        new frut::parameter::ParCombined(realMinimum, realMaximum, stepSize, scalingFactor, decimalPlaces);
    ParameterReleaseRate->setName("Release Rate");

    ParameterReleaseRate->addPreset(50.0f,    "50 ms");
    ParameterReleaseRate->addPreset(75.0f,    "75 ms");
    ParameterReleaseRate->addPreset(100.0f,  "100 ms");
    ParameterReleaseRate->addPreset(125.0f,  "125 ms");
    ParameterReleaseRate->addPreset(150.0f,  "150 ms");
    ParameterReleaseRate->addPreset(175.0f,  "175 ms");
    ParameterReleaseRate->addPreset(200.0f,  "200 ms");
    ParameterReleaseRate->addPreset(250.0f,  "250 ms");
    ParameterReleaseRate->addPreset(375.0f,  "375 ms");
    ParameterReleaseRate->addPreset(500.0f,  "500 ms");
    ParameterReleaseRate->addPreset(750.0f,  "750 ms");
    ParameterReleaseRate->addPreset(1000.0f, "1.0 s");
    ParameterReleaseRate->addPreset(1500.0f, "1.5 s");
    ParameterReleaseRate->addPreset(2000.0f, "2.0 s");
    ParameterReleaseRate->addPreset(4000.0f, "4.0 s");
    ParameterReleaseRate->addPreset(8000.0f, "8.0 s");

    ParameterReleaseRate->setSuffix(" ms");
    ParameterReleaseRate->setDefaultRealFloat(150.0f, true);
    addCombined(ParameterReleaseRate, selReleaseRateSwitch, selReleaseRate);


    realMinimum = 0.0f;
    realMaximum = 100.0f;
    stepSize = 1.0f;
    scalingFactor = 0.0f;
    decimalPlaces = 0;

#ifdef SQUEEZER_STEREO
    frut::parameter::ParCombined *ParameterStereoLink =
        new frut::parameter::ParCombined(realMinimum, realMaximum, stepSize, scalingFactor, decimalPlaces);
    ParameterStereoLink->setName("Stereo Link");

    ParameterStereoLink->addPreset(0.0f,     "Off");
    ParameterStereoLink->addPreset(50.0f,   "50 %");
    ParameterStereoLink->addPreset(75.0f,   "75 %");
    ParameterStereoLink->addPreset(90.0f,   "90 %");
    ParameterStereoLink->addPreset(100.0f, "100 %");

    ParameterStereoLink->setSuffix(" %");
    ParameterStereoLink->setDefaultRealFloat(100.0f, true);
    addCombined(ParameterStereoLink, selStereoLinkSwitch, selStereoLink);
#endif


    frut::parameter::ParBoolean *ParameterAutoMakeupGain =
        new frut::parameter::ParBoolean("Auto", "Manual");
    ParameterAutoMakeupGain->setName("Auto Make-Up Gain");
    ParameterAutoMakeupGain->setDefaultBoolean(true, true);
    add(ParameterAutoMakeupGain, selAutoMakeupGain);


    realMinimum = -36.0f;
    realMaximum = +36.0f;
    stepSize = 0.5f;
    scalingFactor = 0.0f;
    decimalPlaces = 1;

    frut::parameter::ParCombined *ParameterMakeupGain =
        new frut::parameter::ParCombined(realMinimum, realMaximum, stepSize, scalingFactor, decimalPlaces);
    ParameterMakeupGain->setName("Make-Up Gain");

    ParameterMakeupGain->addPreset(-18.0f, "-18 dB");
    ParameterMakeupGain->addPreset(-17.0f, "-17 dB");
    ParameterMakeupGain->addPreset(-16.0f, "-16 dB");
    ParameterMakeupGain->addPreset(-15.0f, "-15 dB");
    ParameterMakeupGain->addPreset(-14.0f, "-14 dB");
    ParameterMakeupGain->addPreset(-13.0f, "-13 dB");
    ParameterMakeupGain->addPreset(-12.0f, "-12 dB");
    ParameterMakeupGain->addPreset(-11.0f, "-11 dB");
    ParameterMakeupGain->addPreset(-10.0f, "-10 dB");
    ParameterMakeupGain->addPreset(-9.0f,   "-9 dB");
    ParameterMakeupGain->addPreset(-8.0f,   "-8 dB");
    ParameterMakeupGain->addPreset(-7.0f,   "-7 dB");
    ParameterMakeupGain->addPreset(-6.0f,   "-6 dB");
    ParameterMakeupGain->addPreset(-5.0f,   "-5 dB");
    ParameterMakeupGain->addPreset(-4.0f,   "-4 dB");
    ParameterMakeupGain->addPreset(-3.0f,   "-3 dB");
    ParameterMakeupGain->addPreset(-2.0f,   "-2 dB");
    ParameterMakeupGain->addPreset(-1.0f,   "-1 dB");
    ParameterMakeupGain->addPreset(0.0f,     "0 dB");
    ParameterMakeupGain->addPreset(+1.0f,   "+1 dB");
    ParameterMakeupGain->addPreset(+2.0f,   "+2 dB");
    ParameterMakeupGain->addPreset(+3.0f,   "+3 dB");
    ParameterMakeupGain->addPreset(+4.0f,   "+4 dB");
    ParameterMakeupGain->addPreset(+5.0f,   "+5 dB");
    ParameterMakeupGain->addPreset(+6.0f,   "+6 dB");
    ParameterMakeupGain->addPreset(+7.0f,   "+7 dB");
    ParameterMakeupGain->addPreset(+8.0f,   "+8 dB");
    ParameterMakeupGain->addPreset(+9.0f,   "+9 dB");
    ParameterMakeupGain->addPreset(+10.0f, "+10 dB");
    ParameterMakeupGain->addPreset(+11.0f, "+11 dB");
    ParameterMakeupGain->addPreset(+12.0f, "+12 dB");
    ParameterMakeupGain->addPreset(+13.0f, "+13 dB");
    ParameterMakeupGain->addPreset(+14.0f, "+14 dB");
    ParameterMakeupGain->addPreset(+15.0f, "+15 dB");
    ParameterMakeupGain->addPreset(+16.0f, "+16 dB");
    ParameterMakeupGain->addPreset(+17.0f, "+17 dB");
    ParameterMakeupGain->addPreset(+18.0f, "+18 dB");

    ParameterMakeupGain->setSuffix(" dB");
    ParameterMakeupGain->setDefaultRealFloat(0.0f, true);
    addCombined(ParameterMakeupGain, selMakeupGainSwitch, selMakeupGain);


    realMinimum = 0.0f;
    realMaximum = 100.0f;
    stepSize = 1.0f;
    scalingFactor = 1.0f;
    decimalPlaces = 0;

    frut::parameter::ParCombined *ParameterWetMix =
        new frut::parameter::ParCombined(realMinimum, realMaximum, stepSize, scalingFactor, decimalPlaces);
    ParameterWetMix->setName("Wet Mix");

    ParameterWetMix->addPreset(0.0f,   "Bypass");
    ParameterWetMix->addPreset(5.0f,      "5 %");
    ParameterWetMix->addPreset(10.0f,    "10 %");
    ParameterWetMix->addPreset(15.0f,    "15 %");
    ParameterWetMix->addPreset(20.0f,    "20 %");
    ParameterWetMix->addPreset(25.0f,    "25 %");
    ParameterWetMix->addPreset(30.0f,    "30 %");
    ParameterWetMix->addPreset(40.0f,    "40 %");
    ParameterWetMix->addPreset(50.0f,    "50 %");
    ParameterWetMix->addPreset(75.0f,    "75 %");
    ParameterWetMix->addPreset(100.0f,  "100 %");

    ParameterWetMix->setSuffix(" %");
    ParameterWetMix->setDefaultRealFloat(100.0f, true);
    addCombined(ParameterWetMix, selWetMixSwitch, selWetMix);


    frut::parameter::ParBoolean *ParameterSidechainFilterState =
        new frut::parameter::ParBoolean("Enabled", "Disabled");
    ParameterSidechainFilterState->setName("SC Filter State");
    ParameterSidechainFilterState->setDefaultBoolean(false, true);
    add(ParameterSidechainFilterState, selSidechainFilterState);


    realMinimum = 60.0f;
    realMaximum = 12000.0f;
    stepSize = 10.0f;
    scalingFactor = 1.0f;
    decimalPlaces = 0;

    frut::parameter::ParCombined *ParameterSidechainFilterCutoff =
        new frut::parameter::ParCombined(realMinimum, realMaximum, stepSize, scalingFactor, decimalPlaces);
    ParameterSidechainFilterCutoff->setName("SC Filter Cutoff Frequency");

    // high-pass filter
    ParameterSidechainFilterCutoff->addPreset(100.0f,    "100 Hz");
    ParameterSidechainFilterCutoff->addPreset(250.0f,    "250 Hz");
    ParameterSidechainFilterCutoff->addPreset(500.0f,    "500 Hz");
    ParameterSidechainFilterCutoff->addPreset(1000.0f,  "1.0 kHz");
    ParameterSidechainFilterCutoff->addPreset(1500.0f,  "1.5 kHz");
    ParameterSidechainFilterCutoff->addPreset(2500.0f,  "2.5 kHz");

    // low-pass filter
    ParameterSidechainFilterCutoff->addPreset(3000.0f,  "3.0 kHz");
    ParameterSidechainFilterCutoff->addPreset(4000.0f,  "4.0 kHz");
    ParameterSidechainFilterCutoff->addPreset(5000.0f,  "5.0 kHz");
    ParameterSidechainFilterCutoff->addPreset(6500.0f,  "6.5 kHz");
    ParameterSidechainFilterCutoff->addPreset(9000.0f,  "9.0 kHz");
    ParameterSidechainFilterCutoff->addPreset(12000.0f,  "12 kHz");

    ParameterSidechainFilterCutoff->setSuffix(" Hz");
    ParameterSidechainFilterCutoff->setDefaultRealFloat(100.0f, true);
    addCombined(ParameterSidechainFilterCutoff, selSidechainFilterCutoffSwitch, selSidechainFilterCutoff);


    frut::parameter::ParSwitch *ParameterSidechainFilterGain =
        new frut::parameter::ParSwitch();
    ParameterSidechainFilterGain->setName("SC Filter Gain");

    ParameterSidechainFilterGain->addPreset(-12.0f, "-12 dB");
    ParameterSidechainFilterGain->addPreset(-11.0f, "-11 dB");
    ParameterSidechainFilterGain->addPreset(-10.0f, "-10 dB");
    ParameterSidechainFilterGain->addPreset(-9.0f,   "-9 dB");
    ParameterSidechainFilterGain->addPreset(-8.0f,   "-8 dB");
    ParameterSidechainFilterGain->addPreset(-7.0f,   "-7 dB");
    ParameterSidechainFilterGain->addPreset(-6.0f,   "-6 dB");
    ParameterSidechainFilterGain->addPreset(-5.0f,   "-5 dB");
    ParameterSidechainFilterGain->addPreset(-4.0f,   "-4 dB");
    ParameterSidechainFilterGain->addPreset(-3.0f,   "-3 dB");
    ParameterSidechainFilterGain->addPreset(-2.0f,   "-2 dB");
    ParameterSidechainFilterGain->addPreset(-1.0f,   "-1 dB");
    ParameterSidechainFilterGain->addPreset(0.0f,     "0 dB");
    ParameterSidechainFilterGain->addPreset(+1.0f,   "+1 dB");
    ParameterSidechainFilterGain->addPreset(+2.0f,   "+2 dB");
    ParameterSidechainFilterGain->addPreset(+3.0f,   "+3 dB");
    ParameterSidechainFilterGain->addPreset(+4.0f,   "+4 dB");
    ParameterSidechainFilterGain->addPreset(+5.0f,   "+5 dB");
    ParameterSidechainFilterGain->addPreset(+6.0f,   "+6 dB");
    ParameterSidechainFilterGain->addPreset(+7.0f,   "+7 dB");
    ParameterSidechainFilterGain->addPreset(+8.0f,   "+8 dB");
    ParameterSidechainFilterGain->addPreset(+9.0f,   "+9 dB");
    ParameterSidechainFilterGain->addPreset(+10.0f, "+10 dB");
    ParameterSidechainFilterGain->addPreset(+11.0f, "+11 dB");
    ParameterSidechainFilterGain->addPreset(+12.0f, "+12 dB");

    ParameterSidechainFilterGain->setDefaultRealFloat(0.0f, true);
    add(ParameterSidechainFilterGain, selSidechainFilterGain);


    frut::parameter::ParBoolean *ParameterSidechainListen =
        new frut::parameter::ParBoolean("Side-Chain", "Compressor");
    ParameterSidechainListen->setName("SC Listen");
    ParameterSidechainListen->setDefaultBoolean(false, true);
    add(ParameterSidechainListen, selSidechainListen);
}


String SqueezerPluginParameters::toString()
{
    String parameterValues;

    parameterValues = String(ProjectInfo::projectName) + " " + String(JucePlugin_VersionString);
#ifdef SQUEEZER_STEREO
    parameterValues += " (Stereo)";
#else
    parameterValues += " (Mono)";
#endif

    int stringLength = parameterValues.length();
    parameterValues += "\n";

    for (int n = 0; n < stringLength; ++n)
    {
        parameterValues += "=";
    }

    parameterValues += "\nRMS Filter: ";
    parameterValues += getText(selDetectorRmsFilter);

    parameterValues += ", Design: ";
    parameterValues += getText(selDesign);

    parameterValues += "\nThresh: ";
    parameterValues += getText(selThreshold);

    parameterValues += ", Ratio: ";
    parameterValues += getText(selRatio);

    parameterValues += ", Knee: ";
    parameterValues += getText(selKneeWidth);

    parameterValues += "\nAttack: ";
    parameterValues += getText(selAttackRate);

    parameterValues += ", Release: ";
    parameterValues += getText(selReleaseRate);

    parameterValues += " (";
    parameterValues += getText(selDetector);

    parameterValues += ", ";
    parameterValues += getText(selGainStage);

    parameterValues += ")\nSide-Chain: ";

    if (!getBoolean(selSidechainFilterState))
    {
        parameterValues += "Filter Bypassed";
    }
    else
    {
        if (getRealInteger(selSidechainFilterCutoff) < 2900)
        {
            parameterValues += "HPF, ";
        }
        else
        {
            parameterValues += "LPF, ";
        }

        parameterValues += getText(selSidechainFilterCutoff);

        parameterValues += ", ";

        parameterValues += getText(selSidechainFilterGain);
    }

    parameterValues += "\nMake-Up: ";
    parameterValues += getText(selMakeupGain);

    parameterValues += " (";
    parameterValues += getText(selAutoMakeupGain);

    parameterValues += "), Wet: ";
    parameterValues += getText(selWetMix);

#ifdef SQUEEZER_STEREO
    parameterValues += ", Link: ";
    parameterValues += getText(selStereoLink);
#endif

    return parameterValues + "\n";
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
