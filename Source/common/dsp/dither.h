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

#pragma once


class Dither
{
public:
    Dither();

    void initialise(const int numberOfChannels,
                    const int numberOfBits,
                    const double noiseShaping = 0.5);

    void convertToDouble(const AudioBuffer<float> &sourceBufferFloat,
                         AudioBuffer<double> &destinationBufferDouble);

    void truncateToFloat(const AudioBuffer<double> &sourceBufferDouble,
                         AudioBuffer<float> &destinationBufferFloat);

    void denormalize(AudioBuffer<float> &buffer);
    void denormalize(AudioBuffer<double> &buffer);

    void denormalizeToDouble(const AudioBuffer<float> &sourceBufferFloat,
                             AudioBuffer<double> &destinationBufferDouble);

    float ditherSample(const int currentChannel,
                       const double &sourceValueDouble);

    void ditherToFloat(const AudioBuffer<double> &sourceBufferDouble,
                       AudioBuffer<float> &destinationBufferFloat);

private:
    Array<int> randomNumber_1_;
    Array<int> randomNumber_2_;

    Array<double> errorFeedback_1_;
    Array<double> errorFeedback_2_;

    const float antiDenormalFloat_;
    const double antiDenormalDouble_;

    bool isInitialized;

    int numberOfChannels_;

    double dcOffset_;
    double ditherAmplitude_;
    double noiseShaping_;
    double wordLength_;
    double wordLengthInverted_;
};


// Local Variables:
// ispell-local-dictionary: "british"
// End:
