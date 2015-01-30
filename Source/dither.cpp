/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2014 Martin Zuther (http://www.mzuther.de/)

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

#include "dither.h"

// Thanks to Paul Kellet from mda for the code snippet!
// (http://www.musicdsp.org/showone.php?id=77)


Dither::Dither(int number_of_bits, double noise_shaping)
{
    initialise(number_of_bits, noise_shaping);
}


Dither::~Dither()
{
    // nothing to do, really
}


void Dither::initialise(int number_of_bits, double noise_shaping)
{
    jassert(number_of_bits <= 24);

    // rectangular-PDF random numbers
    nRandomNumber_1 = 0;
    nRandomNumber_2 = 0;

    // error feedback buffers
    dErrorFeedback_1 = 0.0;
    dErrorFeedback_2 = 0.0;

    // set to 0.0 for no noise shaping
    dNoiseShaping = noise_shaping;

    // word length (usually 16 bits)
    dWordLength = pow(2.0, number_of_bits - 1);
    dWordLengthInverted = 1.0 / dWordLength;

    // dither amplitude (2 LSB)
    dDitherAmplitude = dWordLengthInverted / RAND_MAX;

    // remove DC offset
    dDcOffset = dWordLengthInverted * 0.5;
}



float Dither::dither(double dInput)
{
    // can make HP-TRI dither by subtracting previous rand()
    nRandomNumber_2 = nRandomNumber_1;
    nRandomNumber_1 = rand();

    // error feedback
    double dOutput = dInput + dNoiseShaping * (dErrorFeedback_1 + dErrorFeedback_1 - dErrorFeedback_2);

    // DC offset and dither
    double dTmp = dOutput + dDcOffset + dDitherAmplitude * (double)(nRandomNumber_1 - nRandomNumber_2);

    // truncate downwards
    int nOutputTruncate = (int)(dWordLength * dTmp);

    if (dTmp < 0.0)
    {
        // this is faster than floor()
        nOutputTruncate--;
    }

    // old error feedback
    dErrorFeedback_2 = dErrorFeedback_1;

    // new error feedback
    dErrorFeedback_1 = dOutput - dWordLengthInverted * (double) nOutputTruncate;

    return (float) dOutput;
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
