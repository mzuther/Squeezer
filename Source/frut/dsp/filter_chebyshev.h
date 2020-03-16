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

#ifndef FRUT_DSP_FILTER_CHEBYSHEV_H
#define FRUT_DSP_FILTER_CHEBYSHEV_H

namespace frut
{
namespace dsp
{

class FilterChebyshev
{
public:
    FilterChebyshev(double RelativeCutoffFrequency,
                    bool IsHighPass,
                    double PercentRipple,
                    int NumberOfPoles);

    void changeParameters(double RelativeCutoffFrequency,
                          bool IsHighPass);

    void reset();

    double filterSample(double InputCurrent);
    void testAlgorithm(bool IsHighPass);

private:
    OwnedArray<FilterChebyshevStage> FilterStages_;

    int NumberOfPoles_;
    double PercentRipple_;
};

}
}

#endif  // FRUT_DSP_FILTER_CHEBYSHEV_H
