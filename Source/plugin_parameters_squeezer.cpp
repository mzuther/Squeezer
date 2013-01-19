/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose compressor with a touch of lemon

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

    WrappedParameterSwitch* ParameterBypassSwitch = new WrappedParameterSwitch;
    ParameterBypassSwitch->setName("Bypass Switch");

    ParameterBypassSwitch->addValue(0.0f, "Off");
    ParameterBypassSwitch->addValue(1.0f, "On");

    ParameterBypassSwitch->setDefaultRealFloat(0.0f, true);
    add(ParameterBypassSwitch);

    jassert(arrParameters.size() == selBypassSwitch + 1);


    WrappedParameterSwitch* ParameterDesignSwitch = new WrappedParameterSwitch;
    ParameterDesignSwitch->setName("Design Switch");

    ParameterDesignSwitch->addValue(selDesignModern,  "Modern");
    ParameterDesignSwitch->addValue(selDesignVintage, "Vintage");

    ParameterDesignSwitch->setDefaultRealFloat(selDesignModern, true);
    add(ParameterDesignSwitch);

    jassert(arrParameters.size() == selDesignSwitch + 1);


    WrappedParameterSwitch* ParameterThresholdSwitch = new WrappedParameterSwitch;
    ParameterThresholdSwitch->setName("Threshold Switch");

    ParameterThresholdSwitch->addValue(-48.0f, "-48 dB");
    ParameterThresholdSwitch->addValue(-46.0f, "-46 dB");
    ParameterThresholdSwitch->addValue(-44.0f, "-44 dB");
    ParameterThresholdSwitch->addValue(-42.0f, "-42 dB");
    ParameterThresholdSwitch->addValue(-40.0f, "-40 dB");
    ParameterThresholdSwitch->addValue(-38.0f, "-38 dB");
    ParameterThresholdSwitch->addValue(-36.0f, "-36 dB");
    ParameterThresholdSwitch->addValue(-34.0f, "-34 dB");
    ParameterThresholdSwitch->addValue(-32.0f, "-32 dB");
    ParameterThresholdSwitch->addValue(-30.0f, "-30 dB");
    ParameterThresholdSwitch->addValue(-28.0f, "-28 dB");
    ParameterThresholdSwitch->addValue(-26.0f, "-26 dB");
    ParameterThresholdSwitch->addValue(-24.0f, "-24 dB");
    ParameterThresholdSwitch->addValue(-22.0f, "-22 dB");
    ParameterThresholdSwitch->addValue(-20.0f, "-20 dB");
    ParameterThresholdSwitch->addValue(-18.0f, "-18 dB");
    ParameterThresholdSwitch->addValue(-16.0f, "-16 dB");
    ParameterThresholdSwitch->addValue(-14.0f, "-14 dB");
    ParameterThresholdSwitch->addValue(-12.0f, "-12 dB");
    ParameterThresholdSwitch->addValue(-10.0f, "-10 dB");
    ParameterThresholdSwitch->addValue(-8.0f,   "-8 dB");
    ParameterThresholdSwitch->addValue(-6.0f,   "-6 dB");
    ParameterThresholdSwitch->addValue(-4.0f,   "-4 dB");
    ParameterThresholdSwitch->addValue(-2.0f,   "-2 dB");
    ParameterThresholdSwitch->addValue(0.0f,     "0 dB");
    ParameterThresholdSwitch->addValue(+2.0f,   "+2 dB");
    ParameterThresholdSwitch->addValue(+4.0f,   "+4 dB");
    ParameterThresholdSwitch->addValue(+6.0f,   "+6 dB");
    ParameterThresholdSwitch->addValue(+8.0f,   "+8 dB");
    ParameterThresholdSwitch->addValue(+10.0f, "+10 dB");
    ParameterThresholdSwitch->addValue(+12.0f, "+12 dB");
    ParameterThresholdSwitch->addValue(+14.0f, "+14 dB");
    ParameterThresholdSwitch->addValue(+16.0f, "+16 dB");
    ParameterThresholdSwitch->addValue(+18.0f, "+18 dB");

    ParameterThresholdSwitch->setDefaultRealFloat(-12.0f, true);
    add(ParameterThresholdSwitch);

    jassert(arrParameters.size() == selThresholdSwitch + 1);


    WrappedParameterSwitch* ParameterRatioSwitch = new WrappedParameterSwitch;
    ParameterRatioSwitch->setName("Ratio Switch");

    ParameterRatioSwitch->addValue(1.2f,   "1.2:1");
    ParameterRatioSwitch->addValue(1.5f,   "1.5:1");
    ParameterRatioSwitch->addValue(2.0f,     "2:1");
    ParameterRatioSwitch->addValue(3.0f,     "3:1");
    ParameterRatioSwitch->addValue(4.0f,     "4:1");
    ParameterRatioSwitch->addValue(6.0f,     "6:1");
    ParameterRatioSwitch->addValue(10.0f,   "10:1");

    ParameterRatioSwitch->setDefaultRealFloat(2.0f, true);
    add(ParameterRatioSwitch);

    jassert(arrParameters.size() == selRatioSwitch + 1);


    WrappedParameterSwitch* ParameterAttackRateSwitch = new WrappedParameterSwitch;
    ParameterAttackRateSwitch->setName("Attack Rate Switch");

    ParameterAttackRateSwitch->addValue(2.0f,     "2 ms");
    ParameterAttackRateSwitch->addValue(5.0f,     "5 ms");
    ParameterAttackRateSwitch->addValue(10.0f,   "10 ms");
    ParameterAttackRateSwitch->addValue(20.0f,   "20 ms");
    ParameterAttackRateSwitch->addValue(50.0f,   "50 ms");
    ParameterAttackRateSwitch->addValue(100.0f, "100 ms");
    ParameterAttackRateSwitch->addValue(200.0f, "200 ms");

    ParameterAttackRateSwitch->setDefaultRealFloat(10.0f, true);
    add(ParameterAttackRateSwitch);

    jassert(arrParameters.size() == selAttackRateSwitch + 1);


    WrappedParameterSwitch* ParameterAttackTypeSwitch = new WrappedParameterSwitch;
    ParameterAttackTypeSwitch->setName("Attack Type Switch");

    ParameterAttackTypeSwitch->addValue(selAttackTypeLinear,      "Linear");
    ParameterAttackTypeSwitch->addValue(selAttackTypeLogarithmic, "Logarithmic");

    ParameterAttackTypeSwitch->setDefaultRealFloat(selAttackTypeLogarithmic, true);
    add(ParameterAttackTypeSwitch);

    jassert(arrParameters.size() == selAttackTypeSwitch + 1);


    WrappedParameterSwitch* ParameterReleaseRateSwitch = new WrappedParameterSwitch;
    ParameterReleaseRateSwitch->setName("Release Rate Switch");

    ParameterReleaseRateSwitch->addValue(50.0f,    "50 ms");
    ParameterReleaseRateSwitch->addValue(75.0f,    "75 ms");
    ParameterReleaseRateSwitch->addValue(100.0f,  "100 ms");
    ParameterReleaseRateSwitch->addValue(125.0f,  "125 ms");
    ParameterReleaseRateSwitch->addValue(150.0f,  "150 ms");
    ParameterReleaseRateSwitch->addValue(175.0f,  "175 ms");
    ParameterReleaseRateSwitch->addValue(200.0f,  "200 ms");
    ParameterReleaseRateSwitch->addValue(250.0f,  "250 ms");
    ParameterReleaseRateSwitch->addValue(375.0f,  "375 ms");
    ParameterReleaseRateSwitch->addValue(500.0f,  "500 ms");
    ParameterReleaseRateSwitch->addValue(750.0f,  "750 ms");
    ParameterReleaseRateSwitch->addValue(1000.0f, "1.0 s");
    ParameterReleaseRateSwitch->addValue(1500.0f, "1.5 s");
    ParameterReleaseRateSwitch->addValue(2000.0f, "2.0 s");
    ParameterReleaseRateSwitch->addValue(4000.0f, "4.0 s");
    ParameterReleaseRateSwitch->addValue(8000.0f, "8.0 s");

    ParameterReleaseRateSwitch->setDefaultRealFloat(150.0f, true);
    add(ParameterReleaseRateSwitch);

    jassert(arrParameters.size() == selReleaseRateSwitch + 1);


    float fLogFactor = 2.0f;
    WrappedParameterContinuous* ParameterReleaseRateContinuous = new WrappedParameterContinuous(0.0f, 8000.0f, fLogFactor);
    ParameterReleaseRateContinuous->setName("Release Rate Continuous");

    ParameterReleaseRateContinuous->setDefaultRealFloat(150.0f, true);
    add(ParameterReleaseRateContinuous);

    jassert(arrParameters.size() == selReleaseRateContinuous + 1);


    WrappedParameterSwitch* ParameterReleaseTypeSwitch = new WrappedParameterSwitch;
    ParameterReleaseTypeSwitch->setName("Release Type Switch");

    ParameterReleaseTypeSwitch->addValue(selReleaseTypeLinear,      "Linear");
    ParameterReleaseTypeSwitch->addValue(selReleaseTypeLogarithmic, "Logarithmic");

    ParameterReleaseTypeSwitch->setDefaultRealFloat(selReleaseTypeLogarithmic, true);
    add(ParameterReleaseTypeSwitch);

    jassert(arrParameters.size() == selReleaseTypeSwitch + 1);


    WrappedParameterSwitch* ParameterStereoLinkSwitch = new WrappedParameterSwitch;
    ParameterStereoLinkSwitch->setName("Stereo Link Switch");

    ParameterStereoLinkSwitch->addValue(0.0f,     "Off");
    ParameterStereoLinkSwitch->addValue(50.0f,   "50 %");
    ParameterStereoLinkSwitch->addValue(75.0f,   "75 %");
    ParameterStereoLinkSwitch->addValue(90.0f,   "90 %");
    ParameterStereoLinkSwitch->addValue(100.0f, "100 %");

    ParameterStereoLinkSwitch->setDefaultRealFloat(100.0f, true);
    add(ParameterStereoLinkSwitch);

    jassert(arrParameters.size() == selStereoLinkSwitch + 1);


    WrappedParameterSwitch* ParameterInputGainSwitch = new WrappedParameterSwitch;
    ParameterInputGainSwitch->setName("Input Gain Switch");

    ParameterInputGainSwitch->addValue(-12.0f, "-12 dB");
    ParameterInputGainSwitch->addValue(-11.0f, "-11 dB");
    ParameterInputGainSwitch->addValue(-10.0f, "-10 dB");
    ParameterInputGainSwitch->addValue(-9.0f,   "-9 dB");
    ParameterInputGainSwitch->addValue(-8.0f,   "-8 dB");
    ParameterInputGainSwitch->addValue(-7.0f,   "-7 dB");
    ParameterInputGainSwitch->addValue(-6.0f,   "-6 dB");
    ParameterInputGainSwitch->addValue(-5.0f,   "-5 dB");
    ParameterInputGainSwitch->addValue(-4.0f,   "-4 dB");
    ParameterInputGainSwitch->addValue(-3.0f,   "-3 dB");
    ParameterInputGainSwitch->addValue(-2.0f,   "-2 dB");
    ParameterInputGainSwitch->addValue(-1.0f,   "-1 dB");
    ParameterInputGainSwitch->addValue(0.0f,     "0 dB");
    ParameterInputGainSwitch->addValue(+1.0f,   "+1 dB");
    ParameterInputGainSwitch->addValue(+2.0f,   "+2 dB");
    ParameterInputGainSwitch->addValue(+3.0f,   "+3 dB");
    ParameterInputGainSwitch->addValue(+4.0f,   "+4 dB");
    ParameterInputGainSwitch->addValue(+5.0f,   "+5 dB");
    ParameterInputGainSwitch->addValue(+6.0f,   "+6 dB");
    ParameterInputGainSwitch->addValue(+7.0f,   "+7 dB");
    ParameterInputGainSwitch->addValue(+8.0f,   "+8 dB");
    ParameterInputGainSwitch->addValue(+9.0f,   "+9 dB");
    ParameterInputGainSwitch->addValue(+10.0f, "+10 dB");
    ParameterInputGainSwitch->addValue(+11.0f, "+11 dB");
    ParameterInputGainSwitch->addValue(+12.0f, "+12 dB");

    ParameterInputGainSwitch->setDefaultRealFloat(0.0f, true);
    add(ParameterInputGainSwitch);

    jassert(arrParameters.size() == selInputGainSwitch + 1);


    WrappedParameterSwitch* ParameterOutputGainSwitch = new WrappedParameterSwitch;
    ParameterOutputGainSwitch->setName("Output Gain Switch");

    ParameterOutputGainSwitch->addValue(-12.0f, "-12 dB");
    ParameterOutputGainSwitch->addValue(-11.0f, "-11 dB");
    ParameterOutputGainSwitch->addValue(-10.0f, "-10 dB");
    ParameterOutputGainSwitch->addValue(-9.0f,   "-9 dB");
    ParameterOutputGainSwitch->addValue(-8.0f,   "-8 dB");
    ParameterOutputGainSwitch->addValue(-7.0f,   "-7 dB");
    ParameterOutputGainSwitch->addValue(-6.0f,   "-6 dB");
    ParameterOutputGainSwitch->addValue(-5.0f,   "-5 dB");
    ParameterOutputGainSwitch->addValue(-4.0f,   "-4 dB");
    ParameterOutputGainSwitch->addValue(-3.0f,   "-3 dB");
    ParameterOutputGainSwitch->addValue(-2.0f,   "-2 dB");
    ParameterOutputGainSwitch->addValue(-1.0f,   "-1 dB");
    ParameterOutputGainSwitch->addValue(0.0f,     "0 dB");
    ParameterOutputGainSwitch->addValue(+1.0f,   "+1 dB");
    ParameterOutputGainSwitch->addValue(+2.0f,   "+2 dB");
    ParameterOutputGainSwitch->addValue(+3.0f,   "+3 dB");
    ParameterOutputGainSwitch->addValue(+4.0f,   "+4 dB");
    ParameterOutputGainSwitch->addValue(+5.0f,   "+5 dB");
    ParameterOutputGainSwitch->addValue(+6.0f,   "+6 dB");
    ParameterOutputGainSwitch->addValue(+7.0f,   "+7 dB");
    ParameterOutputGainSwitch->addValue(+8.0f,   "+8 dB");
    ParameterOutputGainSwitch->addValue(+9.0f,   "+9 dB");
    ParameterOutputGainSwitch->addValue(+10.0f, "+10 dB");
    ParameterOutputGainSwitch->addValue(+11.0f, "+11 dB");
    ParameterOutputGainSwitch->addValue(+12.0f, "+12 dB");

    ParameterOutputGainSwitch->setDefaultRealFloat(0.0f, true);
    add(ParameterOutputGainSwitch);

    jassert(arrParameters.size() == selOutputGainSwitch + 1);


    WrappedParameterSwitch* ParameterWetMixSwitch = new WrappedParameterSwitch;
    ParameterWetMixSwitch->setName("Wet Mix Switch");

    ParameterWetMixSwitch->addValue(0.0f,   "Bypass");
    ParameterWetMixSwitch->addValue(5.0f,      "5 %");
    ParameterWetMixSwitch->addValue(10.0f,    "10 %");
    ParameterWetMixSwitch->addValue(15.0f,    "15 %");
    ParameterWetMixSwitch->addValue(20.0f,    "20 %");
    ParameterWetMixSwitch->addValue(25.0f,    "25 %");
    ParameterWetMixSwitch->addValue(30.0f,    "30 %");
    ParameterWetMixSwitch->addValue(40.0f,    "40 %");
    ParameterWetMixSwitch->addValue(50.0f,    "50 %");
    ParameterWetMixSwitch->addValue(75.0f,    "75 %");
    ParameterWetMixSwitch->addValue(100.0f,  "100 %");

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
