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
    strSettingsID = "SQUEEZER_SETTINGS";

    WrappedParameterStepped* ParameterThresholdSwitch = new WrappedParameterStepped;
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

    ParameterThresholdSwitch->setDefaultValue("-12 dB");
    add(ParameterThresholdSwitch);

    WrappedParameterStepped* ParameterRatioSwitch = new WrappedParameterStepped;
    ParameterRatioSwitch->setName("Ratio Switch");

    ParameterRatioSwitch->addValue(1.25f,  "1.25:1");
    ParameterRatioSwitch->addValue(1.50f,   "1.5:1");
    ParameterRatioSwitch->addValue(2.00f,     "2:1");
    ParameterRatioSwitch->addValue(3.00f,     "3:1");
    ParameterRatioSwitch->addValue(4.00f,     "4:1");
    ParameterRatioSwitch->addValue(6.00f,     "6:1");
    ParameterRatioSwitch->addValue(10.00f,   "10:1");
    ParameterRatioSwitch->addValue(-1.00f, "Manual");

    ParameterRatioSwitch->setDefaultValue("2:1");
    add(ParameterRatioSwitch);

    WrappedParameterStepped* ParameterAttackRateSwitch = new WrappedParameterStepped;
    ParameterAttackRateSwitch->setName("Attack Rate Switch");

    ParameterAttackRateSwitch->addValue(0.030f,    "30 us");
    ParameterAttackRateSwitch->addValue(0.100f,   "100 us");
    ParameterAttackRateSwitch->addValue(0.300f,   "300 us");
    ParameterAttackRateSwitch->addValue(1.000f,     "1 ms");
    ParameterAttackRateSwitch->addValue(3.000f,     "3 ms");
    ParameterAttackRateSwitch->addValue(10.000f,   "10 ms");
    ParameterAttackRateSwitch->addValue(30.000f,   "30 ms");
    ParameterAttackRateSwitch->addValue(100.000f, "100 ms");
    ParameterAttackRateSwitch->addValue(-1.000f,  "Manual");

    ParameterAttackRateSwitch->setDefaultValue("3 ms");
    add(ParameterAttackRateSwitch);

    WrappedParameterStepped* ParameterReleaseRateSwitch = new WrappedParameterStepped;
    ParameterReleaseRateSwitch->setName("Release Rate Switch");

    ParameterReleaseRateSwitch->addValue(50.0f,    "50 ms");
    ParameterReleaseRateSwitch->addValue(100.0f,  "100 ms");
    ParameterReleaseRateSwitch->addValue(200.0f,  "200 ms");
    ParameterReleaseRateSwitch->addValue(500.0f,  "500 ms");
    ParameterReleaseRateSwitch->addValue(1000.0f,   "1 s");
    ParameterReleaseRateSwitch->addValue(2000.0f,   "2 s");
    ParameterReleaseRateSwitch->addValue(-1.0f,   "Manual");

    ParameterReleaseRateSwitch->setDefaultValue("200 ms");
    add(ParameterReleaseRateSwitch);

    WrappedParameterStepped* ParameterInputGainSwitch = new WrappedParameterStepped;
    ParameterInputGainSwitch->setName("Input Gain Switch");

    ParameterInputGainSwitch->addValue(-12.0f, "-12.0 dB");
    ParameterInputGainSwitch->addValue(-11.5f, "-11.5 dB");
    ParameterInputGainSwitch->addValue(-11.0f, "-11.0 dB");
    ParameterInputGainSwitch->addValue(-10.5f, "-10.5 dB");
    ParameterInputGainSwitch->addValue(-10.0f, "-10.0 dB");
    ParameterInputGainSwitch->addValue(-9.5f,   "-9.5 dB");
    ParameterInputGainSwitch->addValue(-9.0f,   "-9.0 dB");
    ParameterInputGainSwitch->addValue(-8.5f,   "-8.5 dB");
    ParameterInputGainSwitch->addValue(-8.0f,   "-8.0 dB");
    ParameterInputGainSwitch->addValue(-7.5f,   "-7.5 dB");
    ParameterInputGainSwitch->addValue(-7.0f,   "-7.0 dB");
    ParameterInputGainSwitch->addValue(-6.5f,   "-6.5 dB");
    ParameterInputGainSwitch->addValue(-6.0f,   "-6.0 dB");
    ParameterInputGainSwitch->addValue(-5.5f,   "-5.5 dB");
    ParameterInputGainSwitch->addValue(-5.0f,   "-5.0 dB");
    ParameterInputGainSwitch->addValue(-4.5f,   "-4.5 dB");
    ParameterInputGainSwitch->addValue(-4.0f,   "-4.0 dB");
    ParameterInputGainSwitch->addValue(-3.5f,   "-3.5 dB");
    ParameterInputGainSwitch->addValue(-3.0f,   "-3.0 dB");
    ParameterInputGainSwitch->addValue(-2.5f,   "-2.5 dB");
    ParameterInputGainSwitch->addValue(-2.0f,   "-2.0 dB");
    ParameterInputGainSwitch->addValue(-1.5f,   "-1.5 dB");
    ParameterInputGainSwitch->addValue(-1.0f,   "-1.0 dB");
    ParameterInputGainSwitch->addValue(-0.5f,   "-0.5 dB");
    ParameterInputGainSwitch->addValue(0.0f,     "0.0 dB");
    ParameterInputGainSwitch->addValue(+0.5f,   "+0.5 dB");
    ParameterInputGainSwitch->addValue(+1.0f,   "+1.0 dB");
    ParameterInputGainSwitch->addValue(+1.5f,   "+1.5 dB");
    ParameterInputGainSwitch->addValue(+2.0f,   "+2.0 dB");
    ParameterInputGainSwitch->addValue(+2.5f,   "+2.5 dB");
    ParameterInputGainSwitch->addValue(+3.0f,   "+3.0 dB");
    ParameterInputGainSwitch->addValue(+3.5f,   "+3.5 dB");
    ParameterInputGainSwitch->addValue(+4.0f,   "+4.0 dB");
    ParameterInputGainSwitch->addValue(+4.5f,   "+4.5 dB");
    ParameterInputGainSwitch->addValue(+5.0f,   "+5.0 dB");
    ParameterInputGainSwitch->addValue(+5.5f,   "+5.5 dB");
    ParameterInputGainSwitch->addValue(+6.0f,   "+6.0 dB");
    ParameterInputGainSwitch->addValue(+6.5f,   "+6.5 dB");
    ParameterInputGainSwitch->addValue(+7.0f,   "+7.0 dB");
    ParameterInputGainSwitch->addValue(+7.5f,   "+7.5 dB");
    ParameterInputGainSwitch->addValue(+8.0f,   "+8.0 dB");
    ParameterInputGainSwitch->addValue(+8.5f,   "+8.5 dB");
    ParameterInputGainSwitch->addValue(+9.0f,   "+9.0 dB");
    ParameterInputGainSwitch->addValue(+9.5f,   "+9.5 dB");
    ParameterInputGainSwitch->addValue(+10.0f, "+10.0 dB");
    ParameterInputGainSwitch->addValue(+10.5f, "+10.5 dB");
    ParameterInputGainSwitch->addValue(+11.0f, "+11.0 dB");
    ParameterInputGainSwitch->addValue(+11.5f, "+11.5 dB");
    ParameterInputGainSwitch->addValue(+12.0f, "+12.0 dB");

    ParameterInputGainSwitch->setDefaultValue("0 dB");
    add(ParameterInputGainSwitch);

    WrappedParameterStepped* ParameterOutputGainSwitch = new WrappedParameterStepped;
    ParameterOutputGainSwitch->setName("Output Gain Switch");

    ParameterOutputGainSwitch->addValue(-12.0f, "-12.0 dB");
    ParameterOutputGainSwitch->addValue(-11.5f, "-11.5 dB");
    ParameterOutputGainSwitch->addValue(-11.0f, "-11.0 dB");
    ParameterOutputGainSwitch->addValue(-10.5f, "-10.5 dB");
    ParameterOutputGainSwitch->addValue(-10.0f, "-10.0 dB");
    ParameterOutputGainSwitch->addValue(-9.5f,   "-9.5 dB");
    ParameterOutputGainSwitch->addValue(-9.0f,   "-9.0 dB");
    ParameterOutputGainSwitch->addValue(-8.5f,   "-8.5 dB");
    ParameterOutputGainSwitch->addValue(-8.0f,   "-8.0 dB");
    ParameterOutputGainSwitch->addValue(-7.5f,   "-7.5 dB");
    ParameterOutputGainSwitch->addValue(-7.0f,   "-7.0 dB");
    ParameterOutputGainSwitch->addValue(-6.5f,   "-6.5 dB");
    ParameterOutputGainSwitch->addValue(-6.0f,   "-6.0 dB");
    ParameterOutputGainSwitch->addValue(-5.5f,   "-5.5 dB");
    ParameterOutputGainSwitch->addValue(-5.0f,   "-5.0 dB");
    ParameterOutputGainSwitch->addValue(-4.5f,   "-4.5 dB");
    ParameterOutputGainSwitch->addValue(-4.0f,   "-4.0 dB");
    ParameterOutputGainSwitch->addValue(-3.5f,   "-3.5 dB");
    ParameterOutputGainSwitch->addValue(-3.0f,   "-3.0 dB");
    ParameterOutputGainSwitch->addValue(-2.5f,   "-2.5 dB");
    ParameterOutputGainSwitch->addValue(-2.0f,   "-2.0 dB");
    ParameterOutputGainSwitch->addValue(-1.5f,   "-1.5 dB");
    ParameterOutputGainSwitch->addValue(-1.0f,   "-1.0 dB");
    ParameterOutputGainSwitch->addValue(-0.5f,   "-0.5 dB");
    ParameterOutputGainSwitch->addValue(0.0f,     "0.0 dB");
    ParameterOutputGainSwitch->addValue(+0.5f,   "+0.5 dB");
    ParameterOutputGainSwitch->addValue(+1.0f,   "+1.0 dB");
    ParameterOutputGainSwitch->addValue(+1.5f,   "+1.5 dB");
    ParameterOutputGainSwitch->addValue(+2.0f,   "+2.0 dB");
    ParameterOutputGainSwitch->addValue(+2.5f,   "+2.5 dB");
    ParameterOutputGainSwitch->addValue(+3.0f,   "+3.0 dB");
    ParameterOutputGainSwitch->addValue(+3.5f,   "+3.5 dB");
    ParameterOutputGainSwitch->addValue(+4.0f,   "+4.0 dB");
    ParameterOutputGainSwitch->addValue(+4.5f,   "+4.5 dB");
    ParameterOutputGainSwitch->addValue(+5.0f,   "+5.0 dB");
    ParameterOutputGainSwitch->addValue(+5.5f,   "+5.5 dB");
    ParameterOutputGainSwitch->addValue(+6.0f,   "+6.0 dB");
    ParameterOutputGainSwitch->addValue(+6.5f,   "+6.5 dB");
    ParameterOutputGainSwitch->addValue(+7.0f,   "+7.0 dB");
    ParameterOutputGainSwitch->addValue(+7.5f,   "+7.5 dB");
    ParameterOutputGainSwitch->addValue(+8.0f,   "+8.0 dB");
    ParameterOutputGainSwitch->addValue(+8.5f,   "+8.5 dB");
    ParameterOutputGainSwitch->addValue(+9.0f,   "+9.0 dB");
    ParameterOutputGainSwitch->addValue(+9.5f,   "+9.5 dB");
    ParameterOutputGainSwitch->addValue(+10.0f, "+10.0 dB");
    ParameterOutputGainSwitch->addValue(+10.5f, "+10.5 dB");
    ParameterOutputGainSwitch->addValue(+11.0f, "+11.0 dB");
    ParameterOutputGainSwitch->addValue(+11.5f, "+11.5 dB");
    ParameterOutputGainSwitch->addValue(+12.0f, "+12.0 dB");

    ParameterOutputGainSwitch->setDefaultValue("0 dB");
    add(ParameterOutputGainSwitch);
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
