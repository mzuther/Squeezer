/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2018 Martin Zuther (http://www.mzuther.de/)

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


// Thanks to Paul Kellet for the code snippet!
// (http://www.musicdsp.org/showone.php?id=77)

Dither::Dither(
    int NumberOfBits,
    double NoiseShaping)
{
    initialise(NumberOfBits, NoiseShaping);
}


void Dither::initialise(
    int NumberOfBits,
    double NoiseShaping)
{
    jassert(NumberOfBits <= 24);

    // rectangular-PDF random numbers
    RandomNumber_1_ = 0;
    RandomNumber_2_ = 0;

    // error feedback buffers
    ErrorFeedback_1_ = 0.0;
    ErrorFeedback_2_ = 0.0;

    // set to 0.0 for no noise shaping
    NoiseShaping_ = NoiseShaping;

    // word length (usually 16 bits)
    WordLength_ = pow(2.0, NumberOfBits - 1);
    WordLengthInverted_ = 1.0 / WordLength_;

    // dither amplitude (2 LSB)
    DitherAmplitude_ = WordLengthInverted_ / RAND_MAX;

    // remove DC offset
    DcOffset_ = WordLengthInverted_ * 0.5;
}



float Dither::dither(
    double Input)
{
    // can make HP-TRI dither by subtracting previous rand()
    RandomNumber_2_ = RandomNumber_1_;
    RandomNumber_1_ = rand();

    // error feedback
    double Output = Input + NoiseShaping_ *
                    (ErrorFeedback_1_ + ErrorFeedback_1_ - ErrorFeedback_2_);

    // DC offset and dither
    double TempOutput = Output + DcOffset_ + DitherAmplitude_ *
                        (double)(RandomNumber_1_ - RandomNumber_2_);

    // truncate downwards
    int OutputTruncate = (int)(WordLength_ * TempOutput);

    if (TempOutput < 0.0)
    {
        // this is faster than floor()
        --OutputTruncate;
    }

    // old error feedback
    ErrorFeedback_2_ = ErrorFeedback_1_;

    // new error feedback
    ErrorFeedback_1_ = Output - WordLengthInverted_ * (double) OutputTruncate;

    return (float) Output;
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
