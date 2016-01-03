/* ----------------------------------------------------------------------------

   MZ common JUCE
   ==============
   Common classes for use with the JUCE library

   Copyright (c) 2010-2016 Martin Zuther (http://www.mzuther.de/)

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

#ifndef __AVERAGER_H__
#define __AVERAGER_H__

#include "JuceHeader.h"


class Averager
{
public:
    Averager(int number_of_samples, float initial_value);
    ~Averager();

    void addSample(float newSample);
    bool isValid();
    float getSimpleMovingAverage();

private:
    JUCE_LEAK_DETECTOR(Averager);

    bool bIsValid;
    int nNumberOfSamples;
    float fNumberOfSamples;
    int nCurrentSample;
    float fSum;

    Array<float> fSamples;
};

#endif   // __AVERAGER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
