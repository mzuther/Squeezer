/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2020 Martin Zuther (http://www.mzuther.de/)

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

namespace frut
{
namespace dsp
{

FilterChebyshevStage::FilterChebyshevStage()
{
    Coeff_A0_ = 0.0;
    Coeff_A1_ = 0.0;
    Coeff_A2_ = 0.0;

    Coeff_B1_ = 0.0;
    Coeff_B2_ = 0.0;

    RelativeCutoffFrequency_ = 0.0;
    IsHighPass_ = false;

    reset();
}


void FilterChebyshevStage::changeParameters(
    double RelativeCutoffFrequency,
    bool IsHighPass,
    double PercentRipple,
    int NumberOfPoles,
    int PolePair)
{
    // This function has been derived from "The Scientist and Engineer's
    // Guide to Digital Signal Processing." (http://www.dspguide.com/)
    // under the following copyright notice: "All these programs may be
    // copied, distributed, and used for any noncommercial purpose."
    //
    // RelativeCutoffFrequency:  rel. cutoff frequency (0.0 to 0.5)
    //
    // IsHighPass:               Boolean, false ==> LPF, true ==> HPF
    //
    // PercentRipple:            percent ripple (0 to 29); when the
    //                           ripple is set to 0%, the filter
    //                           becomes a Butterworth (or maximally
    //                           flat) filter
    //
    // NumberOfPoles:            number of poles (2, 4, ..., 20)
    //
    // PolePair:                 pole pair (1, 2, ..., NumberOfPoles / 2)

    // calculate the pole location on the unit circle
    RelativeCutoffFrequency_ = RelativeCutoffFrequency;
    IsHighPass_ = IsHighPass;

    double RP = -cos(M_PI / (NumberOfPoles * 2.0) +
                     (PolePair - 1.0) * M_PI / NumberOfPoles);
    double IP = sin(M_PI / (NumberOfPoles * 2.0) +
                    (PolePair - 1.0) * M_PI / NumberOfPoles);

    // warp from a circle to an ellipse
    if (PercentRipple > 0)
    {
        double ES = sqrt(pow(100.0 / (100.0 - PercentRipple), 2.0) - 1.0);
        double VX = (1.0 / NumberOfPoles) * log((1.0 / ES) +
                                                sqrt(1.0 / pow(ES, 2.0) + 1.0));
        double KX = (1.0 / NumberOfPoles) * log((1.0 / ES) +
                                                sqrt(1.0 / pow(ES, 2.0) - 1.0));

        KX = (exp(KX) + exp(-KX)) / 2.0;
        RP = RP * ((exp(VX) - exp(-VX)) / 2.0) / KX;
        IP = IP * ((exp(VX) + exp(-VX)) / 2.0) / KX;
    }

    // s-domain to z-domain conversion
    double T = 2.0 * tan(0.5);
    double W = 2.0 * M_PI * RelativeCutoffFrequency_;
    double M = pow(RP, 2.0) + pow(IP, 2.0);
    double D = 4.0 - (4.0 * RP * T) + (M * pow(T, 2.0));

    double X0 = pow(T, 2.0) / D;
    double X1 = 2.0 * pow(T, 2.0) / D;
    double X2 = pow(T, 2.0) / D;

    double Y1 = (8.0 - 2.0 * M * pow(T, 2.0)) / D;
    double Y2 = (-4.0 - 4.0 * RP * T - M * pow(T, 2.0)) / D;

    double K;

    // LP TO LP, or LP TO HP transform
    if (IsHighPass_)
    {
        K = -cos(W / 2.0 + 0.5) / cos(W / 2.0 - 0.5);
    }
    else
    {
        K = sin(0.5 - W / 2.0) / sin(0.5 + W / 2.0);
    }

    D = 1.0 + Y1 * K - Y2 * pow(K, 2.0);

    Coeff_A0_ = (X0 - X1 * K + X2 * pow(K, 2.0)) / D;
    Coeff_A1_ = (-2.0 * X0 * K + X1 + X1 * pow(K, 2.0) - 2.0 * X2 * K) / D;
    Coeff_A2_ = (X0 * pow(K, 2.0) - X1 * K + X2) / D;

    Coeff_B1_ = (2.0 * K + Y1 + Y1 * pow(K, 2.0) - 2.0 * Y2 * K) / D;
    Coeff_B2_ = (-pow(K, 2.0) - Y1 * K + Y2) / D;

    if (IsHighPass_)
    {
        Coeff_A1_ = -Coeff_A1_;
        Coeff_B1_ = -Coeff_B1_;
    }

    // normalise coefficients
    double SA = 0.0;
    double SB = 0.0;

    if (IsHighPass_)
    {
        SA +=  Coeff_A0_;
        SA += -Coeff_A1_;
        SA +=  Coeff_A2_;

        SB += -Coeff_B1_;
        SB +=  Coeff_B2_;
    }
    else
    {
        SA += Coeff_A0_;
        SA += Coeff_A1_;
        SA += Coeff_A2_;

        SB += Coeff_B1_;
        SB += Coeff_B2_;
    }

    double Gain = SA / (1.0 - SB);

    Coeff_A0_ /= Gain;
    Coeff_A1_ /= Gain;
    Coeff_A2_ /= Gain;
}


void FilterChebyshevStage::reset()
{
    InputPrevious_1_ = 0.0;
    InputPrevious_2_ = 0.0;

    OutputPrevious_1_ = 0.0;
    OutputPrevious_2_ = 0.0;
}


double FilterChebyshevStage::filterSample(
    double InputCurrent)
{
    double OutputCurrent = Coeff_A0_ * InputCurrent;
    OutputCurrent += Coeff_A1_ * InputPrevious_1_;
    OutputCurrent += Coeff_A2_ * InputPrevious_2_;

    OutputCurrent += Coeff_B1_ * OutputPrevious_1_;
    OutputCurrent += Coeff_B2_ * OutputPrevious_2_;

    InputPrevious_1_ = InputCurrent;
    InputPrevious_2_ = InputPrevious_1_;

    OutputPrevious_1_ = OutputCurrent;
    OutputPrevious_2_ = OutputPrevious_1_;

    return OutputCurrent;
}


String FilterChebyshevStage::testAlgorithm(
    double RelativeCutoffFrequency,
    bool IsHighPass,
    double PercentRipple)
{
    // store old settings and coefficients
    double Cutoff = RelativeCutoffFrequency_;
    bool HighPass = IsHighPass_;

    double A0 = Coeff_A0_;
    double A1 = Coeff_A1_;
    double A2 = Coeff_A2_;

    double B1 = Coeff_B1_;
    double B2 = Coeff_B2_;

    // get coefficients for test
    changeParameters(
        RelativeCutoffFrequency,
        IsHighPass,
        PercentRipple,
        2,
        1);

    String Result = getCoefficients();

    // restore old settings and coefficients
    RelativeCutoffFrequency_ = Cutoff;
    IsHighPass_ = HighPass;

    Coeff_A0_ = A0;
    Coeff_A1_ = A1;
    Coeff_A2_ = A2;

    Coeff_B1_ = B1;
    Coeff_B2_ = B2;

    return Result;
}


String FilterChebyshevStage::getCoefficients()
{
    String Result;

    Result += "fc: " + String::formatted("%1.3f", RelativeCutoffFrequency_);
    Result += "   ";
    Result += "A0: " + String::formatted("% E", Coeff_A0_);
    Result += "\n";

    Result += "            ";
    Result += "A1: " + String::formatted("% E", Coeff_A1_);
    Result += "   ";
    Result += "B1: " + String::formatted("% E", Coeff_B1_);
    Result += "\n";

    Result += "            ";
    Result += "A2: " + String::formatted("% E", Coeff_A2_);
    Result += "   ";
    Result += "B2: " + String::formatted("% E", Coeff_B2_);
    Result += "\n";

    return Result;
}

}
}
