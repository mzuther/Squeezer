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

#include "plugin_parameters.h"


// The methods of this class may be called on the audio thread, so
// they are absolutely time-critical!

SqueezerPluginParameters::SqueezerPluginParameters() :
    Juggler("SQUEEZER_SETTINGS", numberOfParametersComplete,
            numberOfParametersRevealed)
{
    // parameters created here will be deleted in
    // "frut::parameters::Juggler"!

    frut::parameters::ParBoolean *ParameterBypass =
        new frut::parameters::ParBoolean("Bypassed", "Active");
    ParameterBypass->setName("Bypass");
    ParameterBypass->setDefaultBoolean(false, true);
    add(ParameterBypass, selBypass);


    frut::parameters::ParSwitch *ParameterDetectorRmsFilter =
        new frut::parameters::ParSwitch();
    ParameterDetectorRmsFilter->setName("RMS Filter");

    ParameterDetectorRmsFilter->addPreset(0.0f,  "Peak");
    ParameterDetectorRmsFilter->addPreset(30.0f, "RMS (30 ms)");

    ParameterDetectorRmsFilter->setDefaultRealFloat(0.0f, true);
    add(ParameterDetectorRmsFilter, selDetectorRmsFilter);


    frut::parameters::ParSwitch *ParameterDesign =
        new frut::parameters::ParSwitch();
    ParameterDesign->setName("Design");

    ParameterDesign->addPreset(Compressor::DesignFeedForward, "Feed-Forward");
    ParameterDesign->addPreset(Compressor::DesignFeedBack,    "Feed-Back");

    ParameterDesign->setDefaultRealFloat(Compressor::DesignFeedBack, true);
    add(ParameterDesign, selDesign);


    frut::parameters::ParSwitch *ParameterCurveType =
        new frut::parameters::ParSwitch();
    ParameterCurveType->setName("Detector");  // keep old name for backward compatibility!

    ParameterCurveType->addPreset(SideChain::CurveLinear,          "Linear");
    ParameterCurveType->addPreset(SideChain::CurveSmoothDecoupled, "Smooth");
    ParameterCurveType->addPreset(SideChain::CurveSmoothBranching, "Logarithmic");

    ParameterCurveType->setDefaultRealFloat(SideChain::CurveSmoothBranching, true);
    add(ParameterCurveType, selCurveType);


    frut::parameters::ParSwitch *ParameterGainStage =
        new frut::parameters::ParSwitch();
    ParameterGainStage->setName("Gain Stage");

    ParameterGainStage->addPreset(GainStage::FET,     "FET");
    ParameterGainStage->addPreset(GainStage::Optical, "Optical");

    ParameterGainStage->setDefaultRealFloat(GainStage::FET, true);
    add(ParameterGainStage, selGainStage);


    float realMinimum = -48.0f;
    float realMaximum = +19.0f;
    float stepSize = 1.0f;
    float scalingFactor = 0.0f;
    int decimalPlaces = 0;

    frut::parameters::ParCombined *ParameterThreshold =
        new frut::parameters::ParCombined(realMinimum, realMaximum, stepSize, scalingFactor, decimalPlaces);
    ParameterThreshold->setName("Threshold");

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
    realMaximum = 20.0f;
    stepSize = 0.05f;
    scalingFactor = 2.0f;
    decimalPlaces = -2;

    frut::parameters::ParCombined *ParameterRatio =
        new frut::parameters::ParCombined(realMinimum, realMaximum, stepSize, scalingFactor, decimalPlaces);
    ParameterRatio->setName("Ratio");

    ParameterRatio->addPreset(0.50f, "0.50:1");
    ParameterRatio->addPreset(0.67f, "0.67:1");
    ParameterRatio->addPreset(0.83f, "0.83:1");
    ParameterRatio->addPreset(0.91f, "0.91:1");
    ParameterRatio->addPreset(1.00f, "Bypass");
    ParameterRatio->addPreset(1.10f,  "1.1:1");
    ParameterRatio->addPreset(1.20f,  "1.2:1");
    ParameterRatio->addPreset(1.50f,  "1.5:1");
    ParameterRatio->addPreset(2.00f,  "2.0:1");
    ParameterRatio->addPreset(2.50f,  "2.5:1");
    ParameterRatio->addPreset(3.00f,    "3:1");
    ParameterRatio->addPreset(4.00f,    "4:1");
    ParameterRatio->addPreset(6.00f,    "6:1");
    ParameterRatio->addPreset(8.00f,    "8:1");
    ParameterRatio->addPreset(10.00f,  "10:1");
    ParameterRatio->addPreset(14.00f,  "14:1");
    ParameterRatio->addPreset(20.00f,  "20:1");

    ParameterRatio->setSuffix(":1");
    ParameterRatio->setDefaultRealFloat(3.0f, true);
    addCombined(ParameterRatio, selRatioSwitch, selRatio);


    frut::parameters::ParSwitch *ParameterKneeWidth =
        new frut::parameters::ParSwitch();
    ParameterKneeWidth->setName("Knee Width");

    ParameterKneeWidth->addPreset(0.0f,  "Hard (0 dB)");
    ParameterKneeWidth->addPreset(24.0f, "Medium (24 dB)");
    ParameterKneeWidth->addPreset(48.0f, "Soft (48 dB)");

    ParameterKneeWidth->setDefaultRealFloat(0.0f, true);
    add(ParameterKneeWidth, selKneeWidth);


    realMinimum = 0.02f;
    realMaximum = 500.0f;
    stepSize = 0.01f;
    scalingFactor = 5.0f;
    decimalPlaces = -2;

    frut::parameters::ParCombined *ParameterAttackRate =
        new frut::parameters::ParCombined(realMinimum, realMaximum, stepSize, scalingFactor, decimalPlaces);
    ParameterAttackRate->setName("Attack Rate");

    // Equivalent of 20 microseconds
    // =============================
    //  44100 Hz:  0.9 samples
    //  48000 Hz:  1.0 samples
    //  96000 Hz:  1.9 samples
    // 192000 Hz:  3.8 samples
    ParameterAttackRate->addPreset(0.02f,    "20 us");
    ParameterAttackRate->addPreset(0.05f,    "50 us");
    ParameterAttackRate->addPreset(0.10f,   "100 us");
    ParameterAttackRate->addPreset(0.20f,   "200 us");
    ParameterAttackRate->addPreset(0.50f,   "500 us");
    ParameterAttackRate->addPreset(1.0f,      "1 ms");
    ParameterAttackRate->addPreset(2.0f,      "2 ms");
    ParameterAttackRate->addPreset(5.0f,      "5 ms");
    ParameterAttackRate->addPreset(10.0f,    "10 ms");
    ParameterAttackRate->addPreset(20.0f,    "20 ms");
    ParameterAttackRate->addPreset(50.0f,    "50 ms");
    ParameterAttackRate->addPreset(100.0f,  "100 ms");
    ParameterAttackRate->addPreset(200.0f,  "200 ms");
    ParameterAttackRate->addPreset(500.0f,  "500 ms");

    ParameterAttackRate->setSuffix(" ms");
    ParameterAttackRate->setDefaultRealFloat(10.0f, true);
    addCombined(ParameterAttackRate, selAttackRateSwitch, selAttackRate);


    realMinimum = 0.0f;
    realMaximum = 8000.0f;
    stepSize = 1.0f;
    scalingFactor = 4.0f;
    decimalPlaces = 0;

    frut::parameters::ParCombined *ParameterReleaseRate =
        new frut::parameters::ParCombined(realMinimum, realMaximum, stepSize, scalingFactor, decimalPlaces);
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

    ParameterReleaseRate->setSuffix(" ms");
    ParameterReleaseRate->setDefaultRealFloat(175.0f, true);
    addCombined(ParameterReleaseRate, selReleaseRateSwitch, selReleaseRate);


    frut::parameters::ParBoolean *ParameterAutoMakeupGain =
        new frut::parameters::ParBoolean("Auto", "Manual");
    ParameterAutoMakeupGain->setName("Auto Make-Up Gain");
    ParameterAutoMakeupGain->setDefaultBoolean(false, true);
    add(ParameterAutoMakeupGain, selAutoMakeupGain);


    realMinimum = -36.0f;
    realMaximum = +36.0f;
    stepSize = 0.5f;
    scalingFactor = 0.0f;
    decimalPlaces = 1;

    frut::parameters::ParCombined *ParameterMakeupGain =
        new frut::parameters::ParCombined(realMinimum, realMaximum, stepSize, scalingFactor, decimalPlaces);
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

    frut::parameters::ParCombined *ParameterWetMix =
        new frut::parameters::ParCombined(realMinimum, realMaximum, stepSize, scalingFactor, decimalPlaces);
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


    frut::parameters::ParBoolean *ParameterSidechainInput =
        new frut::parameters::ParBoolean("External", "Internal");
    ParameterSidechainInput->setName("SC Input");
    ParameterSidechainInput->setDefaultBoolean(false, true);
    add(ParameterSidechainInput, selSidechainInput);


    realMinimum = 20.0f;
    realMaximum = 15000.0f;
    stepSize = 10.0f;
    scalingFactor = 1.8f;
    decimalPlaces = 0;

    frut::parameters::ParCombined *ParameterSidechainHPFCutoff =
        new frut::parameters::ParCombined(realMinimum, realMaximum, stepSize, scalingFactor, decimalPlaces);
    ParameterSidechainHPFCutoff->setName("SC HPF Cutoff Frequency");

    // E6 series: 1.0, 1.5, 2.2, 3.3, 4.7, 6.8
    ParameterSidechainHPFCutoff->addPreset(20.0f,     "Bypass");
    ParameterSidechainHPFCutoff->addPreset(47.0f,      "47 Hz");
    ParameterSidechainHPFCutoff->addPreset(68.0f,      "68 Hz");
    ParameterSidechainHPFCutoff->addPreset(100.0f,    "100 Hz");
    ParameterSidechainHPFCutoff->addPreset(150.0f,    "150 Hz");
    ParameterSidechainHPFCutoff->addPreset(220.0f,    "220 Hz");
    ParameterSidechainHPFCutoff->addPreset(330.0f,    "330 Hz");
    ParameterSidechainHPFCutoff->addPreset(470.0f,    "470 Hz");
    ParameterSidechainHPFCutoff->addPreset(680.0f,    "680 Hz");
    ParameterSidechainHPFCutoff->addPreset(1000.0f,  "1.0 kHz");
    ParameterSidechainHPFCutoff->addPreset(1500.0f,  "1.5 kHz");
    ParameterSidechainHPFCutoff->addPreset(2200.0f,  "2.2 kHz");
    ParameterSidechainHPFCutoff->addPreset(3300.0f,  "3.3 kHz");
    ParameterSidechainHPFCutoff->addPreset(4700.0f,  "4.7 kHz");
    ParameterSidechainHPFCutoff->addPreset(6800.0f,  "6.8 kHz");
    ParameterSidechainHPFCutoff->addPreset(10000.0f,  "10 kHz");

    ParameterSidechainHPFCutoff->setSuffix(" Hz");
    ParameterSidechainHPFCutoff->setDefaultRealFloat(20.0f, true);
    addCombined(ParameterSidechainHPFCutoff, selSidechainHPFCutoffSwitch, selSidechainHPFCutoff);


    realMinimum = 20.0f;
    realMaximum = 15000.0f;
    stepSize = 10.0f;
    scalingFactor = 1.8f;
    decimalPlaces = 0;

    frut::parameters::ParCombined *ParameterSidechainLPFCutoff =
        new frut::parameters::ParCombined(realMinimum, realMaximum, stepSize, scalingFactor, decimalPlaces);
    ParameterSidechainLPFCutoff->setName("SC LPF Cutoff Frequency");

    // E6 series: 1.0, 1.5, 2.2, 3.3, 4.7, 6.8
    ParameterSidechainLPFCutoff->addPreset(47.0f,      "47 Hz");
    ParameterSidechainLPFCutoff->addPreset(68.0f,      "68 Hz");
    ParameterSidechainLPFCutoff->addPreset(100.0f,    "100 Hz");
    ParameterSidechainLPFCutoff->addPreset(150.0f,    "150 Hz");
    ParameterSidechainLPFCutoff->addPreset(220.0f,    "220 Hz");
    ParameterSidechainLPFCutoff->addPreset(330.0f,    "330 Hz");
    ParameterSidechainLPFCutoff->addPreset(470.0f,    "470 Hz");
    ParameterSidechainLPFCutoff->addPreset(680.0f,    "680 Hz");
    ParameterSidechainLPFCutoff->addPreset(1000.0f,  "1.0 kHz");
    ParameterSidechainLPFCutoff->addPreset(1500.0f,  "1.5 kHz");
    ParameterSidechainLPFCutoff->addPreset(2200.0f,  "2.2 kHz");
    ParameterSidechainLPFCutoff->addPreset(3300.0f,  "3.3 kHz");
    ParameterSidechainLPFCutoff->addPreset(4700.0f,  "4.7 kHz");
    ParameterSidechainLPFCutoff->addPreset(6800.0f,  "6.8 kHz");
    ParameterSidechainLPFCutoff->addPreset(10000.0f,  "10 kHz");
    ParameterSidechainLPFCutoff->addPreset(15000.0f,  "Bypass");

    ParameterSidechainLPFCutoff->setSuffix(" Hz");
    ParameterSidechainLPFCutoff->setDefaultRealFloat(15000.0f, true);
    addCombined(ParameterSidechainLPFCutoff, selSidechainLPFCutoffSwitch, selSidechainLPFCutoff);


    frut::parameters::ParBoolean *ParameterSidechainListen =
        new frut::parameters::ParBoolean("Side-Chain", "Compressor");
    ParameterSidechainListen->setName("SC Listen");
    ParameterSidechainListen->setDefaultBoolean(false, true);
    add(ParameterSidechainListen, selSidechainListen);


    realMinimum = -36.0f;
    realMaximum = +36.0f;
    stepSize = 0.5f;
    scalingFactor = 0.0f;
    decimalPlaces = 1;

    frut::parameters::ParCombined *ParameterInputTrim =
        new frut::parameters::ParCombined(realMinimum, realMaximum, stepSize, scalingFactor, decimalPlaces);
    ParameterInputTrim->setName("Input Trim");

    ParameterInputTrim->addPreset(-18.0f, "-18 dB");
    ParameterInputTrim->addPreset(-17.0f, "-17 dB");
    ParameterInputTrim->addPreset(-16.0f, "-16 dB");
    ParameterInputTrim->addPreset(-15.0f, "-15 dB");
    ParameterInputTrim->addPreset(-14.0f, "-14 dB");
    ParameterInputTrim->addPreset(-13.0f, "-13 dB");
    ParameterInputTrim->addPreset(-12.0f, "-12 dB");
    ParameterInputTrim->addPreset(-11.0f, "-11 dB");
    ParameterInputTrim->addPreset(-10.0f, "-10 dB");
    ParameterInputTrim->addPreset(-9.0f, "-9 dB");
    ParameterInputTrim->addPreset(-8.0f, "-8 dB");
    ParameterInputTrim->addPreset(-7.0f, "-7 dB");
    ParameterInputTrim->addPreset(-6.0f, "-6 dB");
    ParameterInputTrim->addPreset(-5.0f, "-5 dB");
    ParameterInputTrim->addPreset(-4.0f, "-4 dB");
    ParameterInputTrim->addPreset(-3.0f, "-3 dB");
    ParameterInputTrim->addPreset(-2.0f, "-2 dB");
    ParameterInputTrim->addPreset(-1.0f, "-1 dB");
    ParameterInputTrim->addPreset(0.0f, "0 dB");
    ParameterInputTrim->addPreset(+1.0f, "+1 dB");
    ParameterInputTrim->addPreset(+2.0f, "+2 dB");
    ParameterInputTrim->addPreset(+3.0f, "+3 dB");
    ParameterInputTrim->addPreset(+4.0f, "+4 dB");
    ParameterInputTrim->addPreset(+5.0f, "+5 dB");
    ParameterInputTrim->addPreset(+6.0f, "+6 dB");
    ParameterInputTrim->addPreset(+7.0f, "+7 dB");
    ParameterInputTrim->addPreset(+8.0f, "+8 dB");
    ParameterInputTrim->addPreset(+9.0f, "+9 dB");
    ParameterInputTrim->addPreset(+10.0f, "+10 dB");
    ParameterInputTrim->addPreset(+11.0f, "+11 dB");
    ParameterInputTrim->addPreset(+12.0f, "+12 dB");
    ParameterInputTrim->addPreset(+13.0f, "+13 dB");
    ParameterInputTrim->addPreset(+14.0f, "+14 dB");
    ParameterInputTrim->addPreset(+15.0f, "+15 dB");
    ParameterInputTrim->addPreset(+16.0f, "+16 dB");
    ParameterInputTrim->addPreset(+17.0f, "+17 dB");
    ParameterInputTrim->addPreset(+18.0f, "+18 dB");

    ParameterInputTrim->setSuffix(" dB");
    ParameterInputTrim->setDefaultRealFloat(0.0f, true);
    addCombined(ParameterInputTrim, selInputTrimSwitch, selInputTrim);


    realMinimum = 0.0f;
    realMaximum = 100.0f;
    stepSize = 1.0f;
    scalingFactor = -0.5f;
    decimalPlaces = 0;

    frut::parameters::ParCombined *ParameterStereoLink =
        new frut::parameters::ParCombined(realMinimum, realMaximum, stepSize, scalingFactor, decimalPlaces);
    ParameterStereoLink->setName("Stereo Link");

    ParameterStereoLink->addPreset(0.0f, "0 %");
    ParameterStereoLink->addPreset(25.0f, "25 %");
    ParameterStereoLink->addPreset(50.0f, "50 %");
    ParameterStereoLink->addPreset(70.0f, "70 %");
    ParameterStereoLink->addPreset(80.0f, "80 %");
    ParameterStereoLink->addPreset(90.0f, "90 %");
    ParameterStereoLink->addPreset(100.0f, "100 %");

    ParameterStereoLink->setSuffix(" %");
    ParameterStereoLink->setDefaultRealFloat(100.0f, true);
    addCombined(ParameterStereoLink, selStereoLinkSwitch, selStereoLink);


    // locate directory containing the skins
    File skinDirectory = getSkinDirectory();

    // locate file containing the default skin's name
    File defaultSkinFile = skinDirectory.getChildFile("default_skin.ini");

    // make sure the file exists
    if (!defaultSkinFile.existsAsFile())
    {
        // create file
        defaultSkinFile.create();

        // set "Default" as default skin (using Unicode encoding)
        defaultSkinFile.replaceWithText("Default", true, true);
    }

    // load name of default skin from file
    String defaultSkinName = defaultSkinFile.loadFileAsString();

    frut::parameters::ParString *ParameterSkinName =
        new frut::parameters::ParString(defaultSkinName);
    ParameterSkinName->setName("Skin");
    add(ParameterSkinName, selSkinName);
}


String SqueezerPluginParameters::toString()
{
    String parameterValues;

    parameterValues = String(ProjectInfo::projectName) + " " + String(JucePlugin_VersionString);
#ifdef SQUEEZER_MONO
    parameterValues += " (Mono)";
#else
    parameterValues += " (Stereo)";
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

    parameterValues += ", Link: ";
    parameterValues += getText(selStereoLink);

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
    parameterValues += getText(selCurveType);

    parameterValues += ", ";
    parameterValues += getText(selGainStage);

    parameterValues += ")\nSide-Chain: ";

    parameterValues += getText(selSidechainInput);
    parameterValues += ", ";

    if (getRealInteger(selSidechainHPFCutoff) <= 20)
    {
        parameterValues += "HPF: Bypassed";
    }
    else
    {
        parameterValues += "HPF: ";
        parameterValues += getText(selSidechainHPFCutoff);
    }

    if (getRealInteger(selSidechainLPFCutoff) >= 15000)
    {
        parameterValues += ", LPF: Bypassed";
    }
    else
    {
        parameterValues += ", LPF: ";
        parameterValues += getText(selSidechainLPFCutoff);
    }

    parameterValues += "\nTrim: ";
    parameterValues += getText(selInputTrim);

    parameterValues += ", Output: ";
    parameterValues += getText(selMakeupGain);

    parameterValues += " (";
    parameterValues += getText(selAutoMakeupGain);

    parameterValues += " MU), Wet: ";
    parameterValues += getText(selWetMix);

    return parameterValues + "\n";
}


const File SqueezerPluginParameters::getSkinDirectory()
{
    // On all platforms we want the skins folder to be located with
    // the executable.  On Mac, the executable is *not* the same as
    // the application folder because what looks like an application
    // is really a package (i.e. a folder) with the executable being
    // buried inside.
    //
    // When deploying on a Mac, right-click on the build target and
    // select "Show Package Contents".  Navigate through
    // Contents/MacOS and you will find the Squeezer executable.  Put
    // the squeezer folder here.
    //
    // Thanks to Tod Gentille!

    File applicationDirectory;

#ifdef __APPLE__
    applicationDirectory = File::getSpecialLocation(
                               File::currentExecutableFile).getParentDirectory();
#else
    applicationDirectory = File::getSpecialLocation(
                               File::currentApplicationFile).getParentDirectory();
#endif

    return applicationDirectory.getChildFile("./squeezer/skins/");
}


String SqueezerPluginParameters::getSkinName()
{
    return getText(selSkinName);
}


void SqueezerPluginParameters::setSkinName(const String &strSkinName)
{
    setText(selSkinName, strSkinName);
}
