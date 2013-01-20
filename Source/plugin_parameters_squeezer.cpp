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

    WrappedParameterSwitch* ParameterBypassSwitch = new WrappedParameterSwitch();
    ParameterBypassSwitch->setName("Bypass Switch");

    ParameterBypassSwitch->addConstant(0.0f, "Off");
    ParameterBypassSwitch->addConstant(1.0f, "On");

    ParameterBypassSwitch->setDefaultRealFloat(0.0f, true);
    add(ParameterBypassSwitch);

    jassert(arrParameters.size() == selBypassSwitch + 1);


    WrappedParameterSwitch* ParameterDesignSwitch = new WrappedParameterSwitch();
    ParameterDesignSwitch->setName("Design Switch");

    ParameterDesignSwitch->addConstant(selDesignModern,  "Modern");
    ParameterDesignSwitch->addConstant(selDesignVintage, "Vintage");

    ParameterDesignSwitch->setDefaultRealFloat(selDesignModern, true);
    add(ParameterDesignSwitch);

    jassert(arrParameters.size() == selDesignSwitch + 1);


    WrappedParameterSwitch* ParameterThresholdSwitch = new WrappedParameterSwitch();
    ParameterThresholdSwitch->setName("Threshold Switch");

    ParameterThresholdSwitch->addConstant(-48.0f, "-48 dB");
    ParameterThresholdSwitch->addConstant(-46.0f, "-46 dB");
    ParameterThresholdSwitch->addConstant(-44.0f, "-44 dB");
    ParameterThresholdSwitch->addConstant(-42.0f, "-42 dB");
    ParameterThresholdSwitch->addConstant(-40.0f, "-40 dB");
    ParameterThresholdSwitch->addConstant(-38.0f, "-38 dB");
    ParameterThresholdSwitch->addConstant(-36.0f, "-36 dB");
    ParameterThresholdSwitch->addConstant(-34.0f, "-34 dB");
    ParameterThresholdSwitch->addConstant(-32.0f, "-32 dB");
    ParameterThresholdSwitch->addConstant(-30.0f, "-30 dB");
    ParameterThresholdSwitch->addConstant(-28.0f, "-28 dB");
    ParameterThresholdSwitch->addConstant(-26.0f, "-26 dB");
    ParameterThresholdSwitch->addConstant(-24.0f, "-24 dB");
    ParameterThresholdSwitch->addConstant(-22.0f, "-22 dB");
    ParameterThresholdSwitch->addConstant(-20.0f, "-20 dB");
    ParameterThresholdSwitch->addConstant(-18.0f, "-18 dB");
    ParameterThresholdSwitch->addConstant(-16.0f, "-16 dB");
    ParameterThresholdSwitch->addConstant(-14.0f, "-14 dB");
    ParameterThresholdSwitch->addConstant(-12.0f, "-12 dB");
    ParameterThresholdSwitch->addConstant(-10.0f, "-10 dB");
    ParameterThresholdSwitch->addConstant(-8.0f,   "-8 dB");
    ParameterThresholdSwitch->addConstant(-6.0f,   "-6 dB");
    ParameterThresholdSwitch->addConstant(-4.0f,   "-4 dB");
    ParameterThresholdSwitch->addConstant(-2.0f,   "-2 dB");
    ParameterThresholdSwitch->addConstant(0.0f,     "0 dB");
    ParameterThresholdSwitch->addConstant(+2.0f,   "+2 dB");
    ParameterThresholdSwitch->addConstant(+4.0f,   "+4 dB");
    ParameterThresholdSwitch->addConstant(+6.0f,   "+6 dB");
    ParameterThresholdSwitch->addConstant(+8.0f,   "+8 dB");
    ParameterThresholdSwitch->addConstant(+10.0f, "+10 dB");
    ParameterThresholdSwitch->addConstant(+12.0f, "+12 dB");
    ParameterThresholdSwitch->addConstant(+14.0f, "+14 dB");
    ParameterThresholdSwitch->addConstant(+16.0f, "+16 dB");
    ParameterThresholdSwitch->addConstant(+18.0f, "+18 dB");

    ParameterThresholdSwitch->setDefaultRealFloat(-12.0f, true);
    add(ParameterThresholdSwitch);

    jassert(arrParameters.size() == selThresholdSwitch + 1);


    WrappedParameterSwitch* ParameterRatioSwitch = new WrappedParameterSwitch();
    ParameterRatioSwitch->setName("Ratio Switch");

    ParameterRatioSwitch->addConstant(1.2f,   "1.2:1");
    ParameterRatioSwitch->addConstant(1.5f,   "1.5:1");
    ParameterRatioSwitch->addConstant(2.0f,     "2:1");
    ParameterRatioSwitch->addConstant(3.0f,     "3:1");
    ParameterRatioSwitch->addConstant(4.0f,     "4:1");
    ParameterRatioSwitch->addConstant(6.0f,     "6:1");
    ParameterRatioSwitch->addConstant(10.0f,   "10:1");

    ParameterRatioSwitch->setDefaultRealFloat(2.0f, true);
    add(ParameterRatioSwitch);

    jassert(arrParameters.size() == selRatioSwitch + 1);


    WrappedParameterSwitch* ParameterAttackRateSwitch = new WrappedParameterSwitch();
    ParameterAttackRateSwitch->setName("Attack Rate Switch");

    ParameterAttackRateSwitch->addConstant(2.0f,     "2 ms");
    ParameterAttackRateSwitch->addConstant(5.0f,     "5 ms");
    ParameterAttackRateSwitch->addConstant(10.0f,   "10 ms");
    ParameterAttackRateSwitch->addConstant(20.0f,   "20 ms");
    ParameterAttackRateSwitch->addConstant(50.0f,   "50 ms");
    ParameterAttackRateSwitch->addConstant(100.0f, "100 ms");
    ParameterAttackRateSwitch->addConstant(200.0f, "200 ms");
    ParameterAttackRateSwitch->addConstant(500.0f, "500 ms");

    ParameterAttackRateSwitch->setDefaultRealFloat(10.0f, true);
    add(ParameterAttackRateSwitch);

    jassert(arrParameters.size() == selAttackRateSwitch + 1);


    WrappedParameterSwitch* ParameterAttackModeSwitch = new WrappedParameterSwitch();
    ParameterAttackModeSwitch->setName("Attack Mode Switch");

    ParameterAttackModeSwitch->addConstant(selAttackModeLinear,      "Linear");
    ParameterAttackModeSwitch->addConstant(selAttackModeLogarithmic, "Logarithmic");

    ParameterAttackModeSwitch->setDefaultRealFloat(selAttackModeLogarithmic, true);
    add(ParameterAttackModeSwitch);

    jassert(arrParameters.size() == selAttackModeSwitch + 1);


    float fLogFactor = 2.0f;
    WrappedParameterCombined* ParameterReleaseRateSwitch = new WrappedParameterCombined(0.0f, 8000.0f, fLogFactor);
    ParameterReleaseRateSwitch->setName("Release Rate Switch");

    ParameterReleaseRateSwitch->addConstant(50.0f,    "50 ms");
    ParameterReleaseRateSwitch->addConstant(75.0f,    "75 ms");
    ParameterReleaseRateSwitch->addConstant(100.0f,  "100 ms");
    ParameterReleaseRateSwitch->addConstant(125.0f,  "125 ms");
    ParameterReleaseRateSwitch->addConstant(150.0f,  "150 ms");
    ParameterReleaseRateSwitch->addConstant(175.0f,  "175 ms");
    ParameterReleaseRateSwitch->addConstant(200.0f,  "200 ms");
    ParameterReleaseRateSwitch->addConstant(250.0f,  "250 ms");
    ParameterReleaseRateSwitch->addConstant(375.0f,  "375 ms");
    ParameterReleaseRateSwitch->addConstant(500.0f,  "500 ms");
    ParameterReleaseRateSwitch->addConstant(750.0f,  "750 ms");
    ParameterReleaseRateSwitch->addConstant(1000.0f, "1.0 s");
    ParameterReleaseRateSwitch->addConstant(1500.0f, "1.5 s");
    ParameterReleaseRateSwitch->addConstant(2000.0f, "2.0 s");
    ParameterReleaseRateSwitch->addConstant(4000.0f, "4.0 s");
    ParameterReleaseRateSwitch->addConstant(8000.0f, "8.0 s");

    ParameterReleaseRateSwitch->setDefaultRealFloat(150.0f, true);
    add(ParameterReleaseRateSwitch);

    jassert(arrParameters.size() == selReleaseRateSwitch + 1);


    fLogFactor = 2.0f;
    WrappedParameterContinuous* ParameterReleaseRateContinuous = new WrappedParameterContinuous(0.0f, 8000.0f, fLogFactor);
    ParameterReleaseRateContinuous->setName("Release Rate Continuous");

    ParameterReleaseRateContinuous->setDefaultRealFloat(150.0f, true);
    add(ParameterReleaseRateContinuous);

    jassert(arrParameters.size() == selReleaseRateContinuous + 1);


    WrappedParameterSwitch* ParameterReleaseModeSwitch = new WrappedParameterSwitch();
    ParameterReleaseModeSwitch->setName("Release Mode Switch");

    ParameterReleaseModeSwitch->addConstant(selReleaseModeLinear,      "Linear");
    ParameterReleaseModeSwitch->addConstant(selReleaseModeLogarithmic, "Logarithmic");

    ParameterReleaseModeSwitch->setDefaultRealFloat(selReleaseModeLogarithmic, true);
    add(ParameterReleaseModeSwitch);

    jassert(arrParameters.size() == selReleaseModeSwitch + 1);


    WrappedParameterSwitch* ParameterStereoLinkSwitch = new WrappedParameterSwitch();
    ParameterStereoLinkSwitch->setName("Stereo Link Switch");

    ParameterStereoLinkSwitch->addConstant(0.0f,     "Off");
    ParameterStereoLinkSwitch->addConstant(50.0f,   "50 %");
    ParameterStereoLinkSwitch->addConstant(75.0f,   "75 %");
    ParameterStereoLinkSwitch->addConstant(90.0f,   "90 %");
    ParameterStereoLinkSwitch->addConstant(100.0f, "100 %");

    ParameterStereoLinkSwitch->setDefaultRealFloat(100.0f, true);
    add(ParameterStereoLinkSwitch);

    jassert(arrParameters.size() == selStereoLinkSwitch + 1);


    WrappedParameterSwitch* ParameterInputGainSwitch = new WrappedParameterSwitch();
    ParameterInputGainSwitch->setName("Input Gain Switch");

    ParameterInputGainSwitch->addConstant(-12.0f, "-12 dB");
    ParameterInputGainSwitch->addConstant(-11.0f, "-11 dB");
    ParameterInputGainSwitch->addConstant(-10.0f, "-10 dB");
    ParameterInputGainSwitch->addConstant(-9.0f,   "-9 dB");
    ParameterInputGainSwitch->addConstant(-8.0f,   "-8 dB");
    ParameterInputGainSwitch->addConstant(-7.0f,   "-7 dB");
    ParameterInputGainSwitch->addConstant(-6.0f,   "-6 dB");
    ParameterInputGainSwitch->addConstant(-5.0f,   "-5 dB");
    ParameterInputGainSwitch->addConstant(-4.0f,   "-4 dB");
    ParameterInputGainSwitch->addConstant(-3.0f,   "-3 dB");
    ParameterInputGainSwitch->addConstant(-2.0f,   "-2 dB");
    ParameterInputGainSwitch->addConstant(-1.0f,   "-1 dB");
    ParameterInputGainSwitch->addConstant(0.0f,     "0 dB");
    ParameterInputGainSwitch->addConstant(+1.0f,   "+1 dB");
    ParameterInputGainSwitch->addConstant(+2.0f,   "+2 dB");
    ParameterInputGainSwitch->addConstant(+3.0f,   "+3 dB");
    ParameterInputGainSwitch->addConstant(+4.0f,   "+4 dB");
    ParameterInputGainSwitch->addConstant(+5.0f,   "+5 dB");
    ParameterInputGainSwitch->addConstant(+6.0f,   "+6 dB");
    ParameterInputGainSwitch->addConstant(+7.0f,   "+7 dB");
    ParameterInputGainSwitch->addConstant(+8.0f,   "+8 dB");
    ParameterInputGainSwitch->addConstant(+9.0f,   "+9 dB");
    ParameterInputGainSwitch->addConstant(+10.0f, "+10 dB");
    ParameterInputGainSwitch->addConstant(+11.0f, "+11 dB");
    ParameterInputGainSwitch->addConstant(+12.0f, "+12 dB");

    ParameterInputGainSwitch->setDefaultRealFloat(0.0f, true);
    add(ParameterInputGainSwitch);

    jassert(arrParameters.size() == selInputGainSwitch + 1);


    WrappedParameterSwitch* ParameterOutputGainSwitch = new WrappedParameterSwitch();
    ParameterOutputGainSwitch->setName("Output Gain Switch");

    ParameterOutputGainSwitch->addConstant(-12.0f, "-12 dB");
    ParameterOutputGainSwitch->addConstant(-11.0f, "-11 dB");
    ParameterOutputGainSwitch->addConstant(-10.0f, "-10 dB");
    ParameterOutputGainSwitch->addConstant(-9.0f,   "-9 dB");
    ParameterOutputGainSwitch->addConstant(-8.0f,   "-8 dB");
    ParameterOutputGainSwitch->addConstant(-7.0f,   "-7 dB");
    ParameterOutputGainSwitch->addConstant(-6.0f,   "-6 dB");
    ParameterOutputGainSwitch->addConstant(-5.0f,   "-5 dB");
    ParameterOutputGainSwitch->addConstant(-4.0f,   "-4 dB");
    ParameterOutputGainSwitch->addConstant(-3.0f,   "-3 dB");
    ParameterOutputGainSwitch->addConstant(-2.0f,   "-2 dB");
    ParameterOutputGainSwitch->addConstant(-1.0f,   "-1 dB");
    ParameterOutputGainSwitch->addConstant(0.0f,     "0 dB");
    ParameterOutputGainSwitch->addConstant(+1.0f,   "+1 dB");
    ParameterOutputGainSwitch->addConstant(+2.0f,   "+2 dB");
    ParameterOutputGainSwitch->addConstant(+3.0f,   "+3 dB");
    ParameterOutputGainSwitch->addConstant(+4.0f,   "+4 dB");
    ParameterOutputGainSwitch->addConstant(+5.0f,   "+5 dB");
    ParameterOutputGainSwitch->addConstant(+6.0f,   "+6 dB");
    ParameterOutputGainSwitch->addConstant(+7.0f,   "+7 dB");
    ParameterOutputGainSwitch->addConstant(+8.0f,   "+8 dB");
    ParameterOutputGainSwitch->addConstant(+9.0f,   "+9 dB");
    ParameterOutputGainSwitch->addConstant(+10.0f, "+10 dB");
    ParameterOutputGainSwitch->addConstant(+11.0f, "+11 dB");
    ParameterOutputGainSwitch->addConstant(+12.0f, "+12 dB");

    ParameterOutputGainSwitch->setDefaultRealFloat(0.0f, true);
    add(ParameterOutputGainSwitch);

    jassert(arrParameters.size() == selOutputGainSwitch + 1);


    WrappedParameterSwitch* ParameterWetMixSwitch = new WrappedParameterSwitch();
    ParameterWetMixSwitch->setName("Wet Mix Switch");

    ParameterWetMixSwitch->addConstant(0.0f,   "Bypass");
    ParameterWetMixSwitch->addConstant(5.0f,      "5 %");
    ParameterWetMixSwitch->addConstant(10.0f,    "10 %");
    ParameterWetMixSwitch->addConstant(15.0f,    "15 %");
    ParameterWetMixSwitch->addConstant(20.0f,    "20 %");
    ParameterWetMixSwitch->addConstant(25.0f,    "25 %");
    ParameterWetMixSwitch->addConstant(30.0f,    "30 %");
    ParameterWetMixSwitch->addConstant(40.0f,    "40 %");
    ParameterWetMixSwitch->addConstant(50.0f,    "50 %");
    ParameterWetMixSwitch->addConstant(75.0f,    "75 %");
    ParameterWetMixSwitch->addConstant(100.0f,  "100 %");

    ParameterWetMixSwitch->setDefaultRealFloat(100.0f, true);
    add(ParameterWetMixSwitch);

    jassert(arrParameters.size() == selWetMixSwitch + 1);
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
