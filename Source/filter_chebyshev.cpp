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

#include "filter_chebyshev.h"
#include "math.h"


//==============================================================================

FilterChebyshev::FilterChebyshev(double dRelativeCutoffFrequency, bool isHighPass, double dPercentRipple, int nNumberOfPoles)
{
    jassert(nNumberOfPoles % 2 == 0);

    nNumberOfStages = nNumberOfPoles / 2;
    dRipple = dPercentRipple;

    // TODO: normalise coefficients
    for (int nStage = 0; nStage < nNumberOfStages; nStage++)
    {
        // pole pairs start with index 1!
        int nPolePair = nStage + 1;

        p_arrFilterStages.add(new FilterChebyshevStage(dRelativeCutoffFrequency, isHighPass, dRipple, nNumberOfPoles, nPolePair));
    }

    changeParameters(dRelativeCutoffFrequency, isHighPass);
}


FilterChebyshev::~FilterChebyshev()
{
}


double FilterChebyshev::filterSample(double dInputCurrent)
{
    double dOutputCurrent = dInputCurrent;

    for (int nStage = 0; nStage < nNumberOfStages; nStage++)
    {
        dOutputCurrent = p_arrFilterStages[nStage]->filterSample(dOutputCurrent);
    }

    // output is already de-normalised
    return dOutputCurrent;
}


void FilterChebyshev::changeParameters(double dRelativeCutoffFrequency, bool isHighPass)
{
    int nNumberOfPoles = nNumberOfStages * 2;

    for (int nStage = 0; nStage < nNumberOfStages; nStage++)
    {
        // pole pairs start with index 1!
        int nPolePair = nStage + 1;

        p_arrFilterStages[nStage]->changeParameters(dRelativeCutoffFrequency, isHighPass, dRipple, nNumberOfPoles, nPolePair);
    }

    reset();
}


void FilterChebyshev::reset()
{
    for (int nStage = 0; nStage < nNumberOfStages; nStage++)
    {
        p_arrFilterStages[nStage]->reset();
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
