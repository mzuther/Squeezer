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

#include "filter_chebyshev_stage.h"
#include "math.h"


//==============================================================================

FilterChebyshevStage::FilterChebyshevStage(double dRelativeCutoffFrequency, bool isHighPass, double dPercentRipple, int nNumberOfPoles, int nPolePair) :
    dAntiDenormal(DBL_MIN)
{
    changeParameters(dRelativeCutoffFrequency, isHighPass, dPercentRipple, nNumberOfPoles, nPolePair);
}


FilterChebyshevStage::~FilterChebyshevStage()
{
}


void FilterChebyshevStage::changeParameters(double dRelativeCutoffFrequency, bool isHighPass, double dPercentRipple, int nNumberOfPoles, int nPolePair)
{
    // This function has been derived from "The Scientist and Engineer's
    // Guide to Digital Signal Processing." (http://www.dspguide.com/)
    // under the following copyright notice: "All these programs may be
    // copied, distributed, and used for any noncommercial purpose."
    //
    // dRelativeCutoffFrequency:  rel. cutoff frequency (0.0 to 0.5)
    // isHighPass:                Boolean, false --> LPF, true --> HPF
    // dPercentRipple:            percent ripple (0 to 29)
    // nNumberOfPoles:            number of poles (2, 4, ..., 20)
    // nPolePair:                 pole pair (1, 2, ..., nNumberOfPoles / 2)

    // calculate the pole location on the unit circle
    double RP = -cos(M_PI / (nNumberOfPoles * 2.0) + (nPolePair - 1.0) * M_PI / nNumberOfPoles);
    double IP = sin(M_PI / (nNumberOfPoles * 2.0) + (nPolePair - 1.0) * M_PI / nNumberOfPoles);

    // warp from a circle to an ellipse
    if (dPercentRipple > 0)
    {
        double ES = sqrt(pow(100.0 / (100.0 - dPercentRipple), 2.0) - 1.0);
        double VX = (1.0 / nNumberOfPoles) * log((1.0 / ES) + sqrt(1.0 / pow(ES, 2.0) + 1.0));
        double KX = (1.0 / nNumberOfPoles) * log((1.0 / ES) + sqrt(1.0 / pow(ES, 2.0) - 1.0));
        KX = (exp(KX) + exp(-KX)) / 2.0;
        RP = RP * ((exp(VX) - exp(-VX)) / 2.0) / KX;
        IP = IP * ((exp(VX) + exp(-VX)) / 2.0) / KX;
    }

    // s-domain to z-domain conversion
    double T = 2.0 * tan(0.5);
    double W = 2.0 * M_PI * dRelativeCutoffFrequency;
    double M = pow(RP, 2.0) + pow(IP, 2.0);
    double D = 4.0 - (4.0 * RP * T) + (M * pow(T, 2.0));
    double X0 = pow(T, 2.0) / D;
    double X1 = 2.0 * pow(T, 2.0) / D;
    double X2 = pow(T, 2.0) / D;
    double Y1 = (8.0 - 2.0 * M * pow(T, 2.0)) / D;
    double Y2 = (-4.0 - 4.0 * RP * T - M * pow(T, 2.0)) / D;
    double K;

    // LP TO LP, or LP TO HP transform
    if (isHighPass)
    {
        K = -cos((W + 1.0) / 2.0) / cos((W - 1.0) / 2.0);
    }
    else
    {
        K = sin((1.0 - W) / 2.0) / sin((1.0 + W) / 2.0);
    }

    D = 1.0 + Y1 * K - Y2 * pow(K, 2.0);
    coeff_A0 = (X0 - X1 * K + X2 * pow(K, 2.0)) / D;
    coeff_A1 = (-2.0 * X0 * K + X1 + X1 * pow(K, 2.0) - 2.0 * X2 * K) / D;
    coeff_A2 = (X0 * pow(K, 2.0) - X1 * K + X2) / D;
    coeff_B1 = (2.0 * K + Y1 + Y1 * pow(K, 2.0) - 2.0 * Y2 * K) / D;
    coeff_B2 = (-pow(K, 2.0) - Y1 * K + Y2) / D;

    if (isHighPass)
    {
        coeff_A1 = -coeff_A1;
        coeff_B1 = -coeff_B1;
    }
}


double FilterChebyshevStage::filterSample(double dInputCurrent)
{
    double dOutputCurrent = coeff_A0 * dInputCurrent;
    dOutputCurrent += coeff_A1 * dInputPrevious_1;
    dOutputCurrent += coeff_A2 * dInputPrevious_2;
    dOutputCurrent += coeff_B1 * dOutputPrevious_1;
    dOutputCurrent += coeff_B2 * dOutputPrevious_2;

    // de-normalise output
    dOutputCurrent += dAntiDenormal;

    dInputPrevious_1 = dInputCurrent;
    dInputPrevious_2 = dInputPrevious_1;

    dOutputPrevious_1 = dOutputCurrent;
    dOutputPrevious_2 = dOutputPrevious_1;

    return dOutputCurrent;
}


void FilterChebyshevStage::reset()
{
    dInputPrevious_1 = 0.0;
    dInputPrevious_2 = 0.0;

    dOutputPrevious_1 = 0.0;
    dOutputPrevious_2 = 0.0;
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
