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
}



void Dither::convertToDouble(
    const AudioBuffer<float> &inputBufferFloat,
    AudioBuffer<double> &outputBufferDouble)
{
    int numberOfChannels = inputBufferFloat.getNumChannels();
    int numberOfSamples = inputBufferFloat.getNumSamples();

    jassert(outputBufferDouble.getNumChannels() == numberOfChannels);
    jassert(numberOfChannels <= numberOfChannels_);

    jassert(outputBufferDouble.getNumSamples() == numberOfSamples);

    for (int currentChannel = 0; currentChannel < numberOfChannels; ++currentChannel)
    {
        for (int currentSample = 0; currentSample < numberOfSamples; ++currentSample)
        {
            double inputValueFloat = inputBufferFloat.getSample(
                                         currentChannel, currentSample);

            // convert input sample
            double outputValueDouble = static_cast<double>(inputValueFloat);

            // store converted input sample
            outputBufferDouble.setSample(
                currentChannel, currentSample, outputValueDouble);
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
            float sampleValue = buffer.getSample(
                                    currentChannel, currentSample);

            // de-normalise input sample
            sampleValue += antiDenormalFloat_;

            // store de-normalised input sample
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
            double sampleValue = buffer.getSample(
                                     currentChannel, currentSample);

            // de-normalise input sample
            sampleValue += antiDenormalDouble_;

            // store de-normalised input sample
            buffer.setSample(
                currentChannel, currentSample, sampleValue);
        }
    }
}


void Dither::denormalizeToDouble(
    const AudioBuffer<float> &inputBufferFloat,
    AudioBuffer<double> &outputBufferDouble)
{
    int numberOfChannels = inputBufferFloat.getNumChannels();
    int numberOfSamples = inputBufferFloat.getNumSamples();

    jassert(outputBufferDouble.getNumChannels() == numberOfChannels);
    jassert(numberOfChannels <= numberOfChannels_);

    jassert(outputBufferDouble.getNumSamples() == numberOfSamples);

    for (int currentChannel = 0; currentChannel < numberOfChannels; ++currentChannel)
    {
        for (int currentSample = 0; currentSample < numberOfSamples; ++currentSample)
        {
            double inputValueFloat = inputBufferFloat.getSample(
                                         currentChannel, currentSample);

            // convert input sample
            double outputValueDouble = static_cast<double>(inputValueFloat);

            // de-normalise input sample
            outputValueDouble += antiDenormalDouble_;

            // store de-normalised input sample
            outputBufferDouble.setSample(
                currentChannel, currentSample, outputValueDouble);
        }
    }
}


// Thanks to Paul Kellet for the code snippet!
// (http://www.musicdsp.org/showone.php?id=77)
float Dither::ditherSample(
    const int currentChannel,
    const double &inputValueDouble)
{
    // can make HP-TRI dither by subtracting previous rand()
    randomNumber_2_.set(currentChannel,
                        randomNumber_1_[currentChannel]);
    randomNumber_1_.set(currentChannel,
                        rand());

    // error feedback
    double outputValue = inputValueDouble + noiseShaping_ *
                         (errorFeedback_1_[currentChannel] +
                          errorFeedback_1_[currentChannel] -
                          errorFeedback_2_[currentChannel]);

    // DC offset and dither
    double tempOutputValue = outputValue + dcOffset_ + ditherAmplitude_ *
                             static_cast<double>(
                                 randomNumber_1_[currentChannel] -
                                 randomNumber_2_[currentChannel]);

    // truncate downwards
    int outputTruncate = static_cast<int>(wordLength_ * tempOutputValue);

    // this is faster than floor()
    if (tempOutputValue < 0.0)
    {
        --outputTruncate;
    }

    // old error feedback
    errorFeedback_2_.set(currentChannel,
                         errorFeedback_1_[currentChannel]);

    // new error feedback
    errorFeedback_1_.set(currentChannel,
                         outputValue - wordLengthInverted_ *
                         static_cast<double>(outputTruncate));

    // return dithered output sample
    return static_cast<float>(outputValue);
}


void Dither::ditherToFloat(
    const AudioBuffer<double> &inputBufferDouble,
    AudioBuffer<float> &outputBufferFloat)
{
    int numberOfChannels = inputBufferDouble.getNumChannels();
    int numberOfSamples = inputBufferDouble.getNumSamples();

    jassert(outputBufferFloat.getNumChannels() == numberOfChannels);
    jassert(numberOfChannels <= numberOfChannels_);

    jassert(outputBufferFloat.getNumSamples() == numberOfSamples);

    for (int currentChannel = 0; currentChannel < numberOfChannels; ++currentChannel)
    {
        for (int currentSample = 0; currentSample < numberOfSamples; ++currentSample)
        {
            double inputValueDouble = inputBufferDouble.getSample(
                                          currentChannel, currentSample);

            float outputValueFloat = ditherSample(
                                         currentChannel, inputValueDouble);

            outputBufferFloat.setSample(
                currentChannel, currentSample, outputValueFloat);
        }
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
