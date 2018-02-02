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


template <typename Type>
RingBuffer<Type>::RingBuffer(
    const String &bufferName,
    const int numberOfChannels,
    const int numberOfSamples,
    const int preDelay,
    const int chunkSize) :
    ringBufferMemTest_(255)
{
    jassert(numberOfChannels > 0);
    jassert(numberOfSamples > 0);

    this->clearCallbackClass();

    bufferName_ = bufferName;

    numberOfChannels_ = numberOfChannels;
    numberOfSamples_ = numberOfSamples;
    preDelay_ = preDelay;
    totalLength_ = numberOfSamples_ + preDelay_;
    chunkSize_ = chunkSize;

    // pad memory areas with value of ringBufferMemTest_ to allow
    // detection of memory leaks
    audioData_.calloc(numberOfChannels_ * (totalLength_ + 2));

    for (int i = 0; i < (numberOfChannels_ * (totalLength_ + 2)); ++i)
    {
        audioData_[i] = ringBufferMemTest_;
    }

    currentPosition_ = 0;
    samplesInBuffer_ = 0;

    for (int channel = 0; channel < numberOfChannels_; ++channel)
    {
        channelOffsets_.insert(channel, channel * (totalLength_ + 2) + 1);
    }

    this->clear();
}


template <typename Type>
void RingBuffer<Type>::clear()
{
    currentPosition_ = 0;

    for (int channel = 0; channel < numberOfChannels_; ++channel)
        for (int sample = 0; sample < totalLength_; ++sample)
        {
            audioData_[sample + channelOffsets_[channel]] = 0.0;
        }

#ifdef DEBUG

    // detection of memory leaks
    for (int channel = 0; channel < numberOfChannels_; ++channel)
    {
        jassert(audioData_[channelOffsets_[channel] - 1] ==
                ringBufferMemTest_);
        jassert(audioData_[channelOffsets_[channel]] !=
                ringBufferMemTest_);
        jassert(audioData_[channelOffsets_[channel] + totalLength_] ==
                ringBufferMemTest_);
    }

#endif
}


template <typename Type>
String RingBuffer<Type>::getBufferName() const
{
    return bufferName_;
}


template <typename Type>
int RingBuffer<Type>::getNumberOfChannels() const
{
    return numberOfChannels_;
}


template <typename Type>
int RingBuffer<Type>::getCurrentPosition() const
{
    return currentPosition_;
}


template <typename Type>
int RingBuffer<Type>::getSamplesInBuffer() const
{
    return samplesInBuffer_;
}


template <typename Type>
int RingBuffer<Type>::getNumberOfSamples() const
{
    return numberOfSamples_;
}


template <typename Type>
int RingBuffer<Type>::getPreDelay() const
{
    return preDelay_;
}


template <typename Type>
Type RingBuffer<Type>::getSample(
    const int channel,
    const int relativePosition,
    const int preDelay) const
{
    jassert(channel <
            numberOfChannels_);
    jassert(relativePosition <=
            numberOfSamples_);
    jassert(preDelay <=
            preDelay_);

    int nPosition = currentPosition_ - relativePosition;

    if (preDelay)
    {
        nPosition -= preDelay;
    }

    while (nPosition < 0)
    {
        // make sure "nPosition" is positive
        nPosition += totalLength_;
    }

    nPosition %= totalLength_;

    return audioData_[nPosition + channelOffsets_[channel]];
}


// copy data from audio buffer to ring buffer
template <typename Type>
int RingBuffer<Type>::addSamples(
    const AudioBuffer<Type> &source,
    const int sourceStartSample,
    const int numberOfSamples)
{
    if (numberOfSamples <= 0)
    {
        return 0;
    }

    jassert(source.getNumChannels() >=
            numberOfChannels_);
    jassert(numberOfSamples <=
            numberOfSamples_);
    jassert((sourceStartSample + numberOfSamples) <=
            source.getNumSamples());

    int samplesLeft = numberOfSamples;
    int samplesFinished = 0;
    int processedSamples = 0;

    while (samplesLeft > 0)
    {
        int samplesToCopy = chunkSize_ - samplesInBuffer_;
        int samplesToCopy_2 = totalLength_ - currentPosition_;

        if (samplesToCopy_2 < samplesToCopy)
        {
            samplesToCopy = samplesToCopy_2;
        }

        if (samplesToCopy > samplesLeft)
        {
            samplesToCopy = samplesLeft;
        }

        for (int channel = 0; channel < numberOfChannels_; ++channel)
        {
            memcpy(audioData_ + currentPosition_ + channelOffsets_[channel],
                   source.getReadPointer(
                       channel,
                       sourceStartSample + samplesFinished),
                   sizeof(Type) * samplesToCopy);
        }

        samplesInBuffer_ += samplesToCopy;
        processedSamples += samplesToCopy;

        // buffer is full
        if (samplesInBuffer_ == chunkSize_)
        {
            triggerFullBuffer(chunkSize_,
                              sourceStartSample + samplesFinished,
                              processedSamples);

            processedSamples = 0;
        }

        samplesInBuffer_ %= chunkSize_;

        currentPosition_ += samplesToCopy;
        currentPosition_ %= totalLength_;

        samplesLeft -= samplesToCopy;
        samplesFinished += samplesToCopy;
    }

#ifdef DEBUG

    // detection of memory leaks
    for (int channel = 0; channel < numberOfChannels_; ++channel)
    {
        jassert(audioData_[channelOffsets_[channel] - 1] ==
                ringBufferMemTest_);
        jassert(audioData_[channelOffsets_[channel]] !=
                ringBufferMemTest_);
        jassert(audioData_[channelOffsets_[channel] + totalLength_] ==
                ringBufferMemTest_);
    }

#endif

    return processedSamples;
}


// copy data from ring buffer to audio buffer
template <typename Type>
void RingBuffer<Type>::getSamples(
    AudioBuffer<Type> &destination,
    const int destStartSample,
    const int numberOfSamples,
    const int preDelay) const
{
    if (numberOfSamples <= 0)
    {
        return;
    }

    jassert(destination.getNumChannels()
            >= numberOfChannels_);
    jassert(numberOfSamples
            <= numberOfSamples_);
    jassert(preDelay
            <= preDelay_);
    jassert((destStartSample + numberOfSamples) <=
            destination.getNumSamples());

    int samplesLeft = numberOfSamples;
    int samplesFinished = 0;

    int nStartPosition = currentPosition_ - numberOfSamples - preDelay;

    // make sure "nStartPosition" is positive
    while (nStartPosition < 0)
    {
        nStartPosition += totalLength_;
    }

    nStartPosition %= totalLength_;

    while (samplesLeft > 0)
    {
        int samplesToCopy = totalLength_ - nStartPosition;

        if (samplesToCopy > samplesLeft)
        {
            samplesToCopy = samplesLeft;
        }

        for (int channel = 0; channel < numberOfChannels_; ++channel)
        {
            memcpy(destination.getWritePointer(
                       channel,
                       destStartSample + samplesFinished),
                   audioData_ + nStartPosition + channelOffsets_[channel],
                   sizeof(Type) * samplesToCopy);
        }

        nStartPosition += samplesToCopy;
        nStartPosition %= totalLength_;

        samplesLeft -= samplesToCopy;
        samplesFinished += samplesToCopy;
    }
}


template <typename Type>
Type RingBuffer<Type>::getMagnitude(
    const int channel,
    const int numberOfSamples,
    const int preDelay) const
{
    Type magnitude = 0;

    for (int sample = 0; sample < numberOfSamples; ++sample)
    {
        Type amplitude = getSample(channel, sample, preDelay);

        if (amplitude < 0)
        {
            amplitude = -amplitude;
        }

        magnitude = jmax(magnitude, getSample(channel, sample, preDelay));
    }

    return magnitude;
}


template <typename Type>
Type RingBuffer<Type>::getRMSLevel(
    const int channel,
    const int numberOfSamples,
    const int preDelay) const
{
    Type runningSum = 0;

    for (int sample = 0; sample < numberOfSamples; ++sample)
    {
        Type sampleValue = getSample(channel, sample, preDelay);
        runningSum += sampleValue * sampleValue;
    }

    return static_cast<Type>(sqrt(runningSum / numberOfSamples));
}


template <typename Type>
void RingBuffer<Type>::setCallbackClass(
    RingBufferProcessor<Type> *callbackClass)
{
    callbackClass_ = callbackClass;
}


template <typename Type>
void RingBuffer<Type>::clearCallbackClass()
{
    callbackClass_ = nullptr;
}


template <typename Type>
void RingBuffer<Type>::triggerFullBuffer(
    const int chunkSize,
    const int bufferPosition,
    const int processedSamples)
{
    if (callbackClass_)
    {
        callbackClass_->processBufferChunk(chunkSize,
                                           bufferPosition,
                                           processedSamples);
    }
}


// explicit instantiation of all template instances
template class RingBuffer<float>;
template class RingBuffer<double>;


// Local Variables:
// ispell-local-dictionary: "british"
// End:
