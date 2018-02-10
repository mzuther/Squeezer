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


TruePeakMeter::TruePeakMeter(
    const int numberOfChannels,
    const int originalFftBufferSize,
    const int upsamplingFactor) :

    frut::dsp::RateConverter(numberOfChannels,
                             originalFftBufferSize,
                             upsamplingFactor)
{
    dither_.initialise(numberOfChannels_, 24);
}


float TruePeakMeter::getLevel(
    const int channel)
{
    jassert(channel >= 0);
    jassert(channel < numberOfChannels_);

    return truePeakLevels_[channel];
}


void TruePeakMeter::setSamples(
    const frut::audio::RingBuffer<float> &source,
    const int numberOfSamples)
{
    jassert(fftSampleBuffer_.getNumChannels() ==
            source.getNumberOfChannels());
    jassert(originalFftBufferSize_ ==
            numberOfSamples);
    jassert(source.getNumberOfSamples() >=
            numberOfSamples);

    // copy data from ring buffer to sample buffer
    source.getSamples(sampleBufferOriginal_, 0, numberOfSamples, true);

    // process input data
    processInput();
}


void TruePeakMeter::setSamples(
    const frut::audio::RingBuffer<double> &source,
    const int numberOfSamples)
{
    jassert(fftSampleBuffer_.getNumChannels() ==
            source.getNumberOfChannels());
    jassert(originalFftBufferSize_ ==
            numberOfSamples);
    jassert(source.getNumberOfSamples() >=
            numberOfSamples);

    int numberOfChannels = source.getNumberOfChannels();
    AudioBuffer<double> processBuffer(numberOfChannels, numberOfSamples);

    // copy data from ring buffer to audio buffer
    source.getSamples(processBuffer, 0, numberOfSamples, true);

    // dither output to float and store in internal buffer
    dither_.ditherToFloat(processBuffer, sampleBufferOriginal_);

    // process input data
    processInput();
}


void TruePeakMeter::processInput()
{
    // upsample buffer (overwrites contents of sample buffer)
    upsample();

    // evaluate true peak level
    for (int channel = 0; channel < numberOfChannels_; ++channel)
    {
        float truePeakLevel = fftSampleBuffer_.getMagnitude(
                                  channel, 0, fftBufferSize_);

        truePeakLevels_.set(channel, truePeakLevel);
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
