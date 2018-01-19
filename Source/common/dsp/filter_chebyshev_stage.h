/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013-2018 Martin Zuther (http://www.mzuther.de/)

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

#ifndef __FRUT_DSP_FILTER_CHEBYSHEV_STAGE_H__
#define __FRUT_DSP_FILTER_CHEBYSHEV_STAGE_H__


class FilterChebyshevStage
{
public:
    FilterChebyshevStage();

    void changeParameters(double RelativeCutoffFrequency,
                          bool IsHighPass,
                          double PercentRipple,
                          int NumberOfPoles,
                          int PolePair);

    void reset();
    double filterSample(double InputCurrent);

    String testAlgorithm(double RelativeCutoffFrequency,
                         bool IsHighPass,
                         double PercentRipple);
    String getCoefficients();

private:
    const double AntiDenormalDouble_;

    double RelativeCutoffFrequency_;
    bool IsHighPass_;

    double Coeff_A0_;
    double Coeff_A1_;
    double Coeff_A2_;

    double Coeff_B1_;
    double Coeff_B2_;

    double InputPrevious_1_;
    double InputPrevious_2_;

    double OutputPrevious_1_;
    double OutputPrevious_2_;
};

#endif  // __FRUT_DSP_FILTER_CHEBYSHEV_STAGE_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
