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

#include "plugin_parameters_squeezer.h"


// The methods of this class may be called on the audio thread, so
// they are absolutely time-critical!

SqueezerPluginParameters::SqueezerPluginParameters()
{
    PluginParameters();

    strSettingsID = "SQUEEZER_SETTINGS";

    ParameterBypass = new WrappedParameterToggleSwitch("Off", "On");
    ParameterBypass->setName("Bypass");
    ParameterBypass->setDefaultBoolean(false, true);
    add(ParameterBypass, selBypass);


    ParameterDesign = new WrappedParameterSwitch();
    ParameterDesign->setName("Design");

    ParameterDesign->addConstant(Compressor::DesignModern,  "Modern");
    ParameterDesign->addConstant(Compressor::DesignVintage, "Vintage");

    ParameterDesign->setDefaultRealFloat(Compressor::DesignModern, true);
    add(ParameterDesign, selDesign);


    ParameterDetector = new WrappedParameterSwitch();
    ParameterDetector->setName("Detector");

    ParameterDetector->addConstant(Compressor::DetectorLinear,          "Linear");
    ParameterDetector->addConstant(Compressor::DetectorSmoothBranching, "Logarithmic");
    ParameterDetector->addConstant(Compressor::DetectorSmoothDecoupled, "S-Curve");

    ParameterDetector->setDefaultRealFloat(Compressor::DetectorSmoothBranching, true);
    add(ParameterDetector, selDetector);


    float fMinimum = -48.0f;
    float fMaximum = +18.0f;
    float fResolution = 0.5f;
    float fLogFactor = 0.0f;
    int nDecimalPlaces = 1;

    ParameterThreshold = new WrappedParameterCombined(fMinimum, fMaximum, fResolution, fLogFactor, nDecimalPlaces);
    ParameterThreshold->setName("Threshold");

    ParameterThreshold->addConstant(-48.0f, "-48 dB");
    ParameterThreshold->addConstant(-46.0f, "-46 dB");
    ParameterThreshold->addConstant(-44.0f, "-44 dB");
    ParameterThreshold->addConstant(-42.0f, "-42 dB");
    ParameterThreshold->addConstant(-40.0f, "-40 dB");
    ParameterThreshold->addConstant(-38.0f, "-38 dB");
    ParameterThreshold->addConstant(-36.0f, "-36 dB");
    ParameterThreshold->addConstant(-34.0f, "-34 dB");
    ParameterThreshold->addConstant(-32.0f, "-32 dB");
    ParameterThreshold->addConstant(-30.0f, "-30 dB");
    ParameterThreshold->addConstant(-28.0f, "-28 dB");
    ParameterThreshold->addConstant(-26.0f, "-26 dB");
    ParameterThreshold->addConstant(-24.0f, "-24 dB");
    ParameterThreshold->addConstant(-22.0f, "-22 dB");
    ParameterThreshold->addConstant(-20.0f, "-20 dB");
    ParameterThreshold->addConstant(-18.0f, "-18 dB");
    ParameterThreshold->addConstant(-16.0f, "-16 dB");
    ParameterThreshold->addConstant(-14.0f, "-14 dB");
    ParameterThreshold->addConstant(-12.0f, "-12 dB");
    ParameterThreshold->addConstant(-10.0f, "-10 dB");
    ParameterThreshold->addConstant(-8.0f,   "-8 dB");
    ParameterThreshold->addConstant(-6.0f,   "-6 dB");
    ParameterThreshold->addConstant(-4.0f,   "-4 dB");
    ParameterThreshold->addConstant(-2.0f,   "-2 dB");
    ParameterThreshold->addConstant(0.0f,     "0 dB");
    ParameterThreshold->addConstant(+2.0f,   "+2 dB");
    ParameterThreshold->addConstant(+4.0f,   "+4 dB");
    ParameterThreshold->addConstant(+6.0f,   "+6 dB");
    ParameterThreshold->addConstant(+8.0f,   "+8 dB");
    ParameterThreshold->addConstant(+10.0f, "+10 dB");
    ParameterThreshold->addConstant(+12.0f, "+12 dB");
    ParameterThreshold->addConstant(+14.0f, "+14 dB");
    ParameterThreshold->addConstant(+16.0f, "+16 dB");
    ParameterThreshold->addConstant(+18.0f, "+18 dB");

    ParameterThreshold->setSuffix(" dB");
    ParameterThreshold->setDefaultRealFloat(-12.0f, true);
    addCombined(ParameterThreshold, selThreshold, selThresholdSwitch);


    fMinimum = 1.1f;
    fMaximum = 10.0f;
    fResolution = 0.1f;
    fLogFactor = 1.0f;
    nDecimalPlaces = 1;

    ParameterRatio = new WrappedParameterCombined(fMinimum, fMaximum, fResolution, fLogFactor, nDecimalPlaces);
    ParameterRatio->setName("Ratio");

    ParameterRatio->addConstant(1.2f,   "1.2:1");
    ParameterRatio->addConstant(1.5f,   "1.5:1");
    ParameterRatio->addConstant(2.0f,     "2:1");
    ParameterRatio->addConstant(3.0f,     "3:1");
    ParameterRatio->addConstant(4.0f,     "4:1");
    ParameterRatio->addConstant(6.0f,     "6:1");
    ParameterRatio->addConstant(10.0f,   "10:1");

    ParameterRatio->setSuffix(":1");
    ParameterRatio->setDefaultRealFloat(2.0f, true);
    addCombined(ParameterRatio, selRatio, selRatioSwitch);


    fMinimum = 0.0f;
    fMaximum = 48.0f;
    fResolution = 6.0f;
    fLogFactor = 0.0f;
    nDecimalPlaces = 0;

    ParameterKneeWidth = new WrappedParameterCombined(fMinimum, fMaximum, fResolution, fLogFactor, nDecimalPlaces);
    ParameterKneeWidth->setName("Knee Width");

    ParameterKneeWidth->addConstant(0.0f,  "Hard");
    ParameterKneeWidth->addConstant(12.0f, "Medium");
    ParameterKneeWidth->addConstant(48.0f, "Soft");

    ParameterKneeWidth->setSuffix(" dB");
    ParameterKneeWidth->setDefaultRealFloat(0.0f, true);
    addCombined(ParameterKneeWidth, selKneeWidth, selKneeWidthSwitch);


    fMinimum = 0.0f;
    fMaximum = 500.0f;
    fResolution = 1.0f;
    fLogFactor = 2.0f;
    nDecimalPlaces = 0;

    ParameterAttackRate = new WrappedParameterCombined(fMinimum, fMaximum, fResolution, fLogFactor, nDecimalPlaces);
    ParameterAttackRate->setName("Attack Rate");

    ParameterAttackRate->addConstant(2.0f,     "2 ms");
    ParameterAttackRate->addConstant(5.0f,     "5 ms");
    ParameterAttackRate->addConstant(10.0f,   "10 ms");
    ParameterAttackRate->addConstant(20.0f,   "20 ms");
    ParameterAttackRate->addConstant(50.0f,   "50 ms");
    ParameterAttackRate->addConstant(100.0f, "100 ms");
    ParameterAttackRate->addConstant(200.0f, "200 ms");
    ParameterAttackRate->addConstant(500.0f, "500 ms");

    ParameterAttackRate->setSuffix(" ms");
    ParameterAttackRate->setDefaultRealFloat(10.0f, true);
    addCombined(ParameterAttackRate, selAttackRate, selAttackRateSwitch);


    fMinimum = 0.0f;
    fMaximum = 8000.0f;
    fResolution = 1.0f;
    fLogFactor = 3.0f;
    nDecimalPlaces = 0;

    ParameterReleaseRate = new WrappedParameterCombined(fMinimum, fMaximum, fResolution, fLogFactor, nDecimalPlaces);
    ParameterReleaseRate->setName("Release Rate");

    ParameterReleaseRate->addConstant(50.0f,    "50 ms");
    ParameterReleaseRate->addConstant(75.0f,    "75 ms");
    ParameterReleaseRate->addConstant(100.0f,  "100 ms");
    ParameterReleaseRate->addConstant(125.0f,  "125 ms");
    ParameterReleaseRate->addConstant(150.0f,  "150 ms");
    ParameterReleaseRate->addConstant(175.0f,  "175 ms");
    ParameterReleaseRate->addConstant(200.0f,  "200 ms");
    ParameterReleaseRate->addConstant(250.0f,  "250 ms");
    ParameterReleaseRate->addConstant(375.0f,  "375 ms");
    ParameterReleaseRate->addConstant(500.0f,  "500 ms");
    ParameterReleaseRate->addConstant(750.0f,  "750 ms");
    ParameterReleaseRate->addConstant(1000.0f, "1.0 s");
    ParameterReleaseRate->addConstant(1500.0f, "1.5 s");
    ParameterReleaseRate->addConstant(2000.0f, "2.0 s");
    ParameterReleaseRate->addConstant(4000.0f, "4.0 s");
    ParameterReleaseRate->addConstant(8000.0f, "8.0 s");

    ParameterReleaseRate->setSuffix(" ms");
    ParameterReleaseRate->setDefaultRealFloat(150.0f, true);
    addCombined(ParameterReleaseRate, selReleaseRate, selReleaseRateSwitch);


    fMinimum = 0.0f;
    fMaximum = 100.0f;
    fResolution = 1.0f;
    fLogFactor = 0.0f;
    nDecimalPlaces = 0;

    ParameterStereoLink = new WrappedParameterCombined(fMinimum, fMaximum, fResolution, fLogFactor, nDecimalPlaces);
    ParameterStereoLink->setName("Stereo Link");

    ParameterStereoLink->addConstant(0.0f,     "Off");
    ParameterStereoLink->addConstant(50.0f,   "50 %");
    ParameterStereoLink->addConstant(75.0f,   "75 %");
    ParameterStereoLink->addConstant(90.0f,   "90 %");
    ParameterStereoLink->addConstant(100.0f, "100 %");

    ParameterStereoLink->setSuffix(" %");
    ParameterStereoLink->setDefaultRealFloat(100.0f, true);
    addCombined(ParameterStereoLink, selStereoLink, selStereoLinkSwitch);


    ParameterAutoMakeupGain = new WrappedParameterToggleSwitch("Off", "On");
    ParameterAutoMakeupGain->setName("Auto Make-Up Gain");
    ParameterAutoMakeupGain->setDefaultBoolean(false, true);
    add(ParameterAutoMakeupGain, selAutoMakeupGain);


    fMinimum = -24.0f;
    fMaximum = +24.0f;
    fResolution = 0.5f;
    fLogFactor = 0.0f;
    nDecimalPlaces = 1;

    ParameterMakeupGain = new WrappedParameterCombined(fMinimum, fMaximum, fResolution, fLogFactor, nDecimalPlaces);
    ParameterMakeupGain->setName("Make-Up Gain");

    ParameterMakeupGain->addConstant(-12.0f, "-12 dB");
    ParameterMakeupGain->addConstant(-11.0f, "-11 dB");
    ParameterMakeupGain->addConstant(-10.0f, "-10 dB");
    ParameterMakeupGain->addConstant(-9.0f,   "-9 dB");
    ParameterMakeupGain->addConstant(-8.0f,   "-8 dB");
    ParameterMakeupGain->addConstant(-7.0f,   "-7 dB");
    ParameterMakeupGain->addConstant(-6.0f,   "-6 dB");
    ParameterMakeupGain->addConstant(-5.0f,   "-5 dB");
    ParameterMakeupGain->addConstant(-4.0f,   "-4 dB");
    ParameterMakeupGain->addConstant(-3.0f,   "-3 dB");
    ParameterMakeupGain->addConstant(-2.0f,   "-2 dB");
    ParameterMakeupGain->addConstant(-1.0f,   "-1 dB");
    ParameterMakeupGain->addConstant(0.0f,     "0 dB");
    ParameterMakeupGain->addConstant(+1.0f,   "+1 dB");
    ParameterMakeupGain->addConstant(+2.0f,   "+2 dB");
    ParameterMakeupGain->addConstant(+3.0f,   "+3 dB");
    ParameterMakeupGain->addConstant(+4.0f,   "+4 dB");
    ParameterMakeupGain->addConstant(+5.0f,   "+5 dB");
    ParameterMakeupGain->addConstant(+6.0f,   "+6 dB");
    ParameterMakeupGain->addConstant(+7.0f,   "+7 dB");
    ParameterMakeupGain->addConstant(+8.0f,   "+8 dB");
    ParameterMakeupGain->addConstant(+9.0f,   "+9 dB");
    ParameterMakeupGain->addConstant(+10.0f, "+10 dB");
    ParameterMakeupGain->addConstant(+11.0f, "+11 dB");
    ParameterMakeupGain->addConstant(+12.0f, "+12 dB");

    ParameterMakeupGain->setSuffix(" dB");
    ParameterMakeupGain->setDefaultRealFloat(0.0f, true);
    addCombined(ParameterMakeupGain, selMakeupGain, selMakeupGainSwitch);


    fMinimum = 0.0f;
    fMaximum = 100.0f;
    fResolution = 1.0f;
    fLogFactor = 1.0f;
    nDecimalPlaces = 0;

    ParameterWetMix = new WrappedParameterCombined(fMinimum, fMaximum, fResolution, fLogFactor, nDecimalPlaces);
    ParameterWetMix->setName("Wet Mix");

    ParameterWetMix->addConstant(0.0f,   "Bypass");
    ParameterWetMix->addConstant(5.0f,      "5 %");
    ParameterWetMix->addConstant(10.0f,    "10 %");
    ParameterWetMix->addConstant(15.0f,    "15 %");
    ParameterWetMix->addConstant(20.0f,    "20 %");
    ParameterWetMix->addConstant(25.0f,    "25 %");
    ParameterWetMix->addConstant(30.0f,    "30 %");
    ParameterWetMix->addConstant(40.0f,    "40 %");
    ParameterWetMix->addConstant(50.0f,    "50 %");
    ParameterWetMix->addConstant(75.0f,    "75 %");
    ParameterWetMix->addConstant(100.0f,  "100 %");

    ParameterWetMix->setSuffix(" %");
    ParameterWetMix->setDefaultRealFloat(100.0f, true);
    addCombined(ParameterWetMix, selWetMix, selWetMixSwitch);
}


SqueezerPluginParameters::~SqueezerPluginParameters()
{
    delete ParameterBypass;
    ParameterBypass = NULL;

    delete ParameterDesign;
    ParameterDesign = NULL;

    delete ParameterThreshold;
    ParameterThreshold = NULL;

    delete ParameterRatio;
    ParameterRatio = NULL;

    delete ParameterKneeWidth;
    ParameterKneeWidth = NULL;

    delete ParameterAttackRate;
    ParameterAttackRate = NULL;

    delete ParameterReleaseRate;
    ParameterReleaseRate = NULL;

    delete ParameterDetector;
    ParameterDetector = NULL;

    delete ParameterStereoLink;
    ParameterStereoLink = NULL;

    delete ParameterMakeupGain;
    ParameterMakeupGain = NULL;

    delete ParameterAutoMakeupGain;
    ParameterAutoMakeupGain = NULL;

    delete ParameterWetMix;
    ParameterWetMix = NULL;
}


int SqueezerPluginParameters::getNumParameters(bool bIncludeHiddenParameters)
{
    if (bIncludeHiddenParameters)
    {
        return nNumParametersComplete;
    }
    else
    {
        return nNumParametersRevealed;
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
