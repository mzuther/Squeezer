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


Dither::Dither() :
    antiDenormalFloat_(FLT_MIN),
    antiDenormalDouble_(DBL_MIN)
{
    isInitialized = false;
}


// Thanks to Paul Kellet for the code snippet!
// (http://www.musicdsp.org/showone.php?id=77)
void Dither::initialise(
    const int numberOfChannels,
    const int numberOfBits,
    const double noiseShaping)
{
    jassert(numberOfChannels >= 1);
    jassert(numberOfBits <= 24);

    numberOfChannels_ = numberOfChannels;

    randomNumber_1_.clear();
    randomNumber_2_.clear();

    errorFeedback_1_.clear();
    errorFeedback_2_.clear();

    for (int currentChannel = 0; currentChannel < numberOfChannels_; ++currentChannel)
    {
        // rectangular-PDF random numbers
        randomNumber_1_.add(0);
        randomNumber_2_.add(0);

        // error feedback buffers
        errorFeedback_1_.add(0.0);
        errorFeedback_2_.add(0.0);
    }

    // set to 0.0 for no noise shaping
    noiseShaping_ = noiseShaping;

    // word length (usually 16 bits)
    wordLength_ = pow(2.0, numberOfBits - 1);
    wordLengthInverted_ = 1.0 / wordLength_;

    // dither amplitude (2 LSB)
    ditherAmplitude_ = wordLengthInverted_ / RAND_MAX;

    // remove DC offset
    dcOffset_ = wordLengthInverted_ * 0.5;

    isInitialized = true;
}



void Dither::convertToDouble(
    const AudioBuffer<float> &sourceBufferFloat,
    AudioBuffer<double> &destinationBufferDouble)
{
    int numberOfChannels = sourceBufferFloat.getNumChannels();
    int numberOfSamples = sourceBufferFloat.getNumSamples();

    jassert(destinationBufferDouble.getNumChannels() == numberOfChannels);
    jassert(numberOfChannels <= numberOfChannels_);

    jassert(destinationBufferDouble.getNumSamples() == numberOfSamples);

    for (int currentChannel = 0; currentChannel < numberOfChannels; ++currentChannel)
    {
        for (int currentSample = 0; currentSample < numberOfSamples; ++currentSample)
        {
            // load sample
            float sourceValueFloat = sourceBufferFloat.getSample(
                                         currentChannel, currentSample);

            // convert sample
            double destinationValueDouble = static_cast<double>(
                                                sourceValueFloat);

            // store sample
            destinationBufferDouble.setSample(
                currentChannel, currentSample, destinationValueDouble);
        }
    }
}


void Dither::truncateToFloat(
    const AudioBuffer<double> &sourceBufferDouble,
    AudioBuffer<float> &destinationBufferFloat)
{
    int numberOfChannels = sourceBufferDouble.getNumChannels();
    int numberOfSamples = sourceBufferDouble.getNumSamples();

    jassert(destinationBufferFloat.getNumChannels() == numberOfChannels);
    jassert(numberOfChannels <= numberOfChannels_);

    jassert(destinationBufferFloat.getNumSamples() == numberOfSamples);

    for (int currentChannel = 0; currentChannel < numberOfChannels; ++currentChannel)
    {
        for (int currentSample = 0; currentSample < numberOfSamples; ++currentSample)
        {
            // load sample
            double sourceValueDouble = sourceBufferDouble.getSample(
                                           currentChannel, currentSample);

            // truncate sample
            float destinationValueFloat = static_cast<float>(
                                              sourceValueDouble);

            // store sample
            destinationBufferFloat.setSample(
                currentChannel, currentSample, destinationValueFloat);
        }
    }
}


void Dither::denormalize(
    AudioBuffer<float> &buffer)
{
    int numberOfChannels = buffer.getNumChannels();
    int numberOfSamples = buffer.getNumSamples();

    jassert(numberOfChannels <= numberOfChannels_);

    for (int currentChannel = 0; currentChannel < numberOfChannels; ++currentChannel)
    {
        for (int currentSample = 0; currentSample < numberOfSamples; ++currentSample)
        {
            // load sample
            float sampleValue = buffer.getSample(
                                    currentChannel, currentSample);

            // de-normalise sample
            sampleValue += antiDenormalFloat_;

            // store sample
            buffer.setSample(
                currentChannel, currentSample, sampleValue);
        }
    }
}


void Dither::denormalize(
    AudioBuffer<double> &buffer)
{
    int numberOfChannels = buffer.getNumChannels();
    int numberOfSamples = buffer.getNumSamples();

    jassert(numberOfChannels <= numberOfChannels_);

    for (int currentChannel = 0; currentChannel < numberOfChannels; ++currentChannel)
    {
        for (int currentSample = 0; currentSample < numberOfSamples; ++currentSample)
        {
            // load sample
            double sampleValue = buffer.getSample(
                                     currentChannel, currentSample);

            // de-normalise sample
            sampleValue += antiDenormalDouble_;

            // store sample
            buffer.setSample(
                currentChannel, currentSample, sampleValue);
        }
    }
}


void Dither::denormalizeToDouble(
    const AudioBuffer<float> &sourceBufferFloat,
    AudioBuffer<double> &destinationBufferDouble)
{
    int numberOfChannels = sourceBufferFloat.getNumChannels();
    int numberOfSamples = sourceBufferFloat.getNumSamples();

    jassert(destinationBufferDouble.getNumChannels() == numberOfChannels);
    jassert(numberOfChannels <= numberOfChannels_);

    jassert(destinationBufferDouble.getNumSamples() == numberOfSamples);

    for (int currentChannel = 0; currentChannel < numberOfChannels; ++currentChannel)
    {
        for (int currentSample = 0; currentSample < numberOfSamples; ++currentSample)
        {
            // load sample
            float sourceValueFloat = sourceBufferFloat.getSample(
                                         currentChannel, currentSample);

            // convert sample
            double destinationValueDouble = static_cast<double>(
                                                sourceValueFloat);

            // de-normalise sample
            destinationValueDouble += antiDenormalDouble_;

            // store sample
            destinationBufferDouble.setSample(
                currentChannel, currentSample, destinationValueDouble);
        }
    }
}


// Thanks to Paul Kellet for the code snippet!
// (http://www.musicdsp.org/showone.php?id=77)
float Dither::ditherSample(
    const int currentChannel,
    const double &sourceValueDouble)
{
    jassert(isInitialized);

    // can make HP-TRI dither by subtracting previous rand()
    randomNumber_2_.set(currentChannel,
                        randomNumber_1_[currentChannel]);
    randomNumber_1_.set(currentChannel,
                        rand());

    // error feedback
    double destinationValue = sourceValueDouble + noiseShaping_ *
                              (errorFeedback_1_[currentChannel] +
                               errorFeedback_1_[currentChannel] -
                               errorFeedback_2_[currentChannel]);

    // DC offset and dither
    double tempDestinationValue = destinationValue + dcOffset_ +
                                  ditherAmplitude_ * static_cast<double>(
                                      randomNumber_1_[currentChannel] -
                                      randomNumber_2_[currentChannel]);

    // truncate downwards
    int destinationTruncate = static_cast<int>(
                                  wordLength_ * tempDestinationValue);

    // this is faster than floor()
    if (tempDestinationValue < 0.0)
    {
        --destinationTruncate;
    }

    // old error feedback
    errorFeedback_2_.set(currentChannel,
                         errorFeedback_1_[currentChannel]);

    // new error feedback
    errorFeedback_1_.set(currentChannel,
                         destinationValue - wordLengthInverted_ *
                         static_cast<double>(destinationTruncate));

    // return dithered destination sample
    return static_cast<float>(destinationValue);
}


void Dither::ditherToFloat(
    const AudioBuffer<double> &sourceBufferDouble,
    AudioBuffer<float> &destinationBufferFloat)
{
    int numberOfChannels = sourceBufferDouble.getNumChannels();
    int numberOfSamples = sourceBufferDouble.getNumSamples();

    jassert(destinationBufferFloat.getNumChannels() == numberOfChannels);
    jassert(numberOfChannels <= numberOfChannels_);

    jassert(destinationBufferFloat.getNumSamples() == numberOfSamples);

    for (int currentChannel = 0; currentChannel < numberOfChannels; ++currentChannel)
    {
        for (int currentSample = 0; currentSample < numberOfSamples; ++currentSample)
        {
            // load sample
            double sourceValueDouble = sourceBufferDouble.getSample(
                                           currentChannel, currentSample);

            // dither sample
            float destinationValueFloat = ditherSample(currentChannel,
                                          sourceValueDouble);

            // store sample
            destinationBufferFloat.setSample(
                currentChannel, currentSample, destinationValueFloat);
        }
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
