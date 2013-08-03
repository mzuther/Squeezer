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

#include "filter_chebyshev.h"
#include "math.h"


//==============================================================================

FilterChebyshev::FilterChebyshev(float fRelativeCutoffFrequency, bool isHighPass, float fPercentRipple, int nNumberOfPoles)
{
    jassert(nNumberOfPoles % 2 == 0);

    nNumberOfStages = nNumberOfPoles / 2;
    fRipple = fPercentRipple;

    pFilterStages = new FilterChebyshevStage*[nNumberOfStages];

    for (int nStage = 0; nStage < nNumberOfStages; nStage++)
    {
        // pole pairs start with index 1!
        int nPolePair = nStage + 1;

        pFilterStages[nStage] = new FilterChebyshevStage(fRelativeCutoffFrequency, isHighPass, fRipple, nNumberOfPoles, nPolePair);
    }

    changeParameters(fRelativeCutoffFrequency, isHighPass);
}


FilterChebyshev::~FilterChebyshev()
{
    for (int nStage = 0; nStage < nNumberOfStages; nStage++)
    {
        delete pFilterStages[nStage];
        pFilterStages[nStage] = NULL;
    }

    delete[] pFilterStages;
    pFilterStages = NULL;
}


double FilterChebyshev::filterSample(double dInputCurrent)
{
    double dOutputCurrent = dInputCurrent / dGainNormalise;

    for (int nStage = 0; nStage < nNumberOfStages; nStage++)
    {
        dOutputCurrent = pFilterStages[nStage]->filterSample(dOutputCurrent);
    }

    return dOutputCurrent;
}


void FilterChebyshev::changeParameters(float fRelativeCutoffFrequency, bool isHighPass)
{
    int nNumberOfPoles = nNumberOfStages * 2;

    for (int nStage = 0; nStage < nNumberOfStages; nStage++)
    {
        // pole pairs start with index 1!
        int nPolePair = nStage + 1;

        pFilterStages[nStage]->changeParameters(fRelativeCutoffFrequency, isHighPass, fRipple, nNumberOfPoles, nPolePair);
    }

    reset();
    dGainNormalise = 1.0;

    for (int n = 0; n < (10 * nNumberOfPoles); n++)
    {
        filterSample(1.0);
    }

    dGainNormalise = filterSample(1.0);
    reset();
}


void FilterChebyshev::reset()
{
    for (int nStage = 0; nStage < nNumberOfStages; nStage++)
    {
        pFilterStages[nStage]->reset();
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
