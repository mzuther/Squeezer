/* ----------------------------------------------------------------------------

   MZ common JUCE
   ==============
   Common classes for use with the JUCE library

   Copyright (c) 2010-2015 Martin Zuther (http://www.mzuther.de/)

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

#include "averager.h"


Averager::Averager(int number_of_samples, float initial_value)
{
    jassert(number_of_samples > 2);

    bIsValid = false;
    nNumberOfSamples = number_of_samples;
    fNumberOfSamples = float(nNumberOfSamples);
    nCurrentSample = 0;

    pSamples = new float[nNumberOfSamples];

    for (int n = 0; n < nNumberOfSamples; n++)
    {
        pSamples[n] = initial_value;
    }

    fSum = initial_value * fNumberOfSamples;
}

Averager::~Averager()
{
    delete [] pSamples;
    pSamples = nullptr;
}

void Averager::addSample(float newSample)
{
    fSum -= pSamples[nCurrentSample];
    pSamples[nCurrentSample] = newSample;
    fSum += pSamples[nCurrentSample];

    nCurrentSample++;

    if (nCurrentSample >= nNumberOfSamples)
    {
        bIsValid = true;
        nCurrentSample = 0;
    }
}


bool Averager::isValid()
{
    return bIsValid;
}


float Averager::getSimpleMovingAverage()
{
    return fSum / fNumberOfSamples;
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
