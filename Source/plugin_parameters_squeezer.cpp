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

    WrappedParameterSwitch* ParameterBypass = new WrappedParameterSwitch();
    ParameterBypass->setName("Bypass");

    ParameterBypass->addConstant(0.0f, "Off");
    ParameterBypass->addConstant(1.0f, "On");

    ParameterBypass->setDefaultRealFloat(0.0f, true);
    add(ParameterBypass);

    jassert(arrParameters.size() == selBypass + 1);


    WrappedParameterSwitch* ParameterDesign = new WrappedParameterSwitch();
    ParameterDesign->setName("Design");

    ParameterDesign->addConstant(selDesignModern,  "Modern");
    ParameterDesign->addConstant(selDesignVintage, "Vintage");

    ParameterDesign->setDefaultRealFloat(selDesignModern, true);
    add(ParameterDesign);

    jassert(arrParameters.size() == selDesign + 1);


    WrappedParameterSwitch* ParameterThreshold = new WrappedParameterSwitch();
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

    ParameterThreshold->setDefaultRealFloat(-12.0f, true);
    add(ParameterThreshold);

    jassert(arrParameters.size() == selThreshold + 1);


    WrappedParameterSwitch* ParameterRatio = new WrappedParameterSwitch();
    ParameterRatio->setName("Ratio");

    ParameterRatio->addConstant(1.2f,   "1.2:1");
    ParameterRatio->addConstant(1.5f,   "1.5:1");
    ParameterRatio->addConstant(2.0f,     "2:1");
    ParameterRatio->addConstant(3.0f,     "3:1");
    ParameterRatio->addConstant(4.0f,     "4:1");
    ParameterRatio->addConstant(6.0f,     "6:1");
    ParameterRatio->addConstant(10.0f,   "10:1");

    ParameterRatio->setDefaultRealFloat(2.0f, true);
    add(ParameterRatio);

    jassert(arrParameters.size() == selRatio + 1);


    float fLogFactor = 2.0f;
    float fMinimum = 0.0f;
    float fMaximum = 500.0f;

    WrappedParameterCombined* ParameterAttackRate = new WrappedParameterCombined(fMinimum, fMaximum, fLogFactor);
    ParameterAttackRate->setName("Attack Rate");

    ParameterAttackRate->addConstant(2.0f,     "2 ms");
    ParameterAttackRate->addConstant(5.0f,     "5 ms");
    ParameterAttackRate->addConstant(10.0f,   "10 ms");
    ParameterAttackRate->addConstant(20.0f,   "20 ms");
    ParameterAttackRate->addConstant(50.0f,   "50 ms");
    ParameterAttackRate->addConstant(100.0f, "100 ms");
    ParameterAttackRate->addConstant(200.0f, "200 ms");
    ParameterAttackRate->addConstant(500.0f, "500 ms");

    ParameterAttackRate->setDefaultRealFloat(10.0f, true);
    add(ParameterAttackRate);

    jassert(arrParameters.size() == selAttackRate + 1);


    WrappedParameterSwitch* ParameterAttackMode = new WrappedParameterSwitch();
    ParameterAttackMode->setName("Attack Mode");

    ParameterAttackMode->addConstant(selAttackModeLinear,      "Linear");
    ParameterAttackMode->addConstant(selAttackModeLogarithmic, "Logarithmic");

    ParameterAttackMode->setDefaultRealFloat(selAttackModeLogarithmic, true);
    add(ParameterAttackMode);

    jassert(arrParameters.size() == selAttackMode + 1);


    fLogFactor = 2.0f;
    fMinimum = 0.0f;
    fMaximum = 8000.0f;

    WrappedParameterCombined* ParameterReleaseRate = new WrappedParameterCombined(fMinimum, fMaximum, fLogFactor);
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

    ParameterReleaseRate->setDefaultRealFloat(150.0f, true);
    add(ParameterReleaseRate);

    jassert(arrParameters.size() == selReleaseRate + 1);


    WrappedParameterSwitch* ParameterReleaseMode = new WrappedParameterSwitch();
    ParameterReleaseMode->setName("Release Mode");

    ParameterReleaseMode->addConstant(selReleaseModeLinear,      "Linear");
    ParameterReleaseMode->addConstant(selReleaseModeLogarithmic, "Logarithmic");

    ParameterReleaseMode->setDefaultRealFloat(selReleaseModeLogarithmic, true);
    add(ParameterReleaseMode);

    jassert(arrParameters.size() == selReleaseMode + 1);


    WrappedParameterSwitch* ParameterStereoLink = new WrappedParameterSwitch();
    ParameterStereoLink->setName("Stereo Link");

    ParameterStereoLink->addConstant(0.0f,     "Off");
    ParameterStereoLink->addConstant(50.0f,   "50 %");
    ParameterStereoLink->addConstant(75.0f,   "75 %");
    ParameterStereoLink->addConstant(90.0f,   "90 %");
    ParameterStereoLink->addConstant(100.0f, "100 %");

    ParameterStereoLink->setDefaultRealFloat(100.0f, true);
    add(ParameterStereoLink);

    jassert(arrParameters.size() == selStereoLink + 1);


    WrappedParameterSwitch* ParameterInputGain = new WrappedParameterSwitch();
    ParameterInputGain->setName("Input Gain");

    ParameterInputGain->addConstant(-12.0f, "-12 dB");
    ParameterInputGain->addConstant(-11.0f, "-11 dB");
    ParameterInputGain->addConstant(-10.0f, "-10 dB");
    ParameterInputGain->addConstant(-9.0f,   "-9 dB");
    ParameterInputGain->addConstant(-8.0f,   "-8 dB");
    ParameterInputGain->addConstant(-7.0f,   "-7 dB");
    ParameterInputGain->addConstant(-6.0f,   "-6 dB");
    ParameterInputGain->addConstant(-5.0f,   "-5 dB");
    ParameterInputGain->addConstant(-4.0f,   "-4 dB");
    ParameterInputGain->addConstant(-3.0f,   "-3 dB");
    ParameterInputGain->addConstant(-2.0f,   "-2 dB");
    ParameterInputGain->addConstant(-1.0f,   "-1 dB");
    ParameterInputGain->addConstant(0.0f,     "0 dB");
    ParameterInputGain->addConstant(+1.0f,   "+1 dB");
    ParameterInputGain->addConstant(+2.0f,   "+2 dB");
    ParameterInputGain->addConstant(+3.0f,   "+3 dB");
    ParameterInputGain->addConstant(+4.0f,   "+4 dB");
    ParameterInputGain->addConstant(+5.0f,   "+5 dB");
    ParameterInputGain->addConstant(+6.0f,   "+6 dB");
    ParameterInputGain->addConstant(+7.0f,   "+7 dB");
    ParameterInputGain->addConstant(+8.0f,   "+8 dB");
    ParameterInputGain->addConstant(+9.0f,   "+9 dB");
    ParameterInputGain->addConstant(+10.0f, "+10 dB");
    ParameterInputGain->addConstant(+11.0f, "+11 dB");
    ParameterInputGain->addConstant(+12.0f, "+12 dB");

    ParameterInputGain->setDefaultRealFloat(0.0f, true);
    add(ParameterInputGain);

    jassert(arrParameters.size() == selInputGain + 1);


    WrappedParameterSwitch* ParameterOutputGain = new WrappedParameterSwitch();
    ParameterOutputGain->setName("Output Gain");

    ParameterOutputGain->addConstant(-12.0f, "-12 dB");
    ParameterOutputGain->addConstant(-11.0f, "-11 dB");
    ParameterOutputGain->addConstant(-10.0f, "-10 dB");
    ParameterOutputGain->addConstant(-9.0f,   "-9 dB");
    ParameterOutputGain->addConstant(-8.0f,   "-8 dB");
    ParameterOutputGain->addConstant(-7.0f,   "-7 dB");
    ParameterOutputGain->addConstant(-6.0f,   "-6 dB");
    ParameterOutputGain->addConstant(-5.0f,   "-5 dB");
    ParameterOutputGain->addConstant(-4.0f,   "-4 dB");
    ParameterOutputGain->addConstant(-3.0f,   "-3 dB");
    ParameterOutputGain->addConstant(-2.0f,   "-2 dB");
    ParameterOutputGain->addConstant(-1.0f,   "-1 dB");
    ParameterOutputGain->addConstant(0.0f,     "0 dB");
    ParameterOutputGain->addConstant(+1.0f,   "+1 dB");
    ParameterOutputGain->addConstant(+2.0f,   "+2 dB");
    ParameterOutputGain->addConstant(+3.0f,   "+3 dB");
    ParameterOutputGain->addConstant(+4.0f,   "+4 dB");
    ParameterOutputGain->addConstant(+5.0f,   "+5 dB");
    ParameterOutputGain->addConstant(+6.0f,   "+6 dB");
    ParameterOutputGain->addConstant(+7.0f,   "+7 dB");
    ParameterOutputGain->addConstant(+8.0f,   "+8 dB");
    ParameterOutputGain->addConstant(+9.0f,   "+9 dB");
    ParameterOutputGain->addConstant(+10.0f, "+10 dB");
    ParameterOutputGain->addConstant(+11.0f, "+11 dB");
    ParameterOutputGain->addConstant(+12.0f, "+12 dB");

    ParameterOutputGain->setDefaultRealFloat(0.0f, true);
    add(ParameterOutputGain);

    jassert(arrParameters.size() == selOutputGain + 1);


    WrappedParameterSwitch* ParameterWetMix = new WrappedParameterSwitch();
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

    ParameterWetMix->setDefaultRealFloat(100.0f, true);
    add(ParameterWetMix);

    jassert(arrParameters.size() == selWetMix + 1);
}


SqueezerPluginParameters::~SqueezerPluginParameters()
{
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
