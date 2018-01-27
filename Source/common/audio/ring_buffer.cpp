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
    const String &buffer_name,
    const unsigned int channels,
    const unsigned int length,
    const unsigned int pre_delay,
    const unsigned int chunk_size) :
    ringBufferMemTest_(255)
{
    jassert(channels > 0);
    jassert(length > 0);

    this->clearCallbackClass();

    strBufferName = buffer_name;

    uChannels = channels;
    uLength = length;
    uPreDelay = pre_delay;
    uTotalLength = uLength + uPreDelay;
    uChunkSize = chunk_size;

    // pad memory areas with value of ringBufferMemTest_ to allow
    // detection of memory leaks
    audioData.calloc(uChannels * (uTotalLength + 2));

    for (unsigned int i = 0; i < (uChannels * (uTotalLength + 2)); ++i)
    {
        audioData[i] = ringBufferMemTest_;
    }

    uCurrentPosition = 0;
    uSamplesInBuffer = 0;

    for (unsigned int uChannel = 0; uChannel < uChannels; ++uChannel)
    {
        uChannelOffset.insert(uChannel, uChannel * (uTotalLength + 2) + 1);
    }

    this->clear();
}


template <typename Type>
void RingBuffer<Type>::clear()
{
    uCurrentPosition = 0;

    for (unsigned int uChannel = 0; uChannel < uChannels; ++uChannel)
        for (unsigned int uSample = 0; uSample < uTotalLength; ++uSample)
        {
            audioData[uSample + uChannelOffset[uChannel]] = 0.0;
        }

#ifdef DEBUG

    // detection of memory leaks
    for (unsigned int uChannel = 0; uChannel < uChannels; ++uChannel)
    {
        jassert(audioData[uChannelOffset[uChannel] - 1] ==
                ringBufferMemTest_);
        jassert(audioData[uChannelOffset[uChannel]] !=
                ringBufferMemTest_);
        jassert(audioData[uChannelOffset[uChannel] + uTotalLength] ==
                ringBufferMemTest_);
    }

#endif
}


template <typename Type>
String RingBuffer<Type>::getBufferName()
{
    return strBufferName;
}


template <typename Type>
unsigned int RingBuffer<Type>::getCurrentPosition()
{
    return uCurrentPosition;
}


template <typename Type>
unsigned int RingBuffer<Type>::getSamplesInBuffer()
{
    return uSamplesInBuffer;
}


template <typename Type>
unsigned int RingBuffer<Type>::getBufferLength()
{
    return uLength;
}


template <typename Type>
unsigned int RingBuffer<Type>::getTotalLength()
{
    return uTotalLength;
}


template <typename Type>
unsigned int RingBuffer<Type>::getPreDelay()
{
    return uPreDelay;
}


template <typename Type>
Type RingBuffer<Type>::getSample(
    const unsigned int channel,
    const unsigned int relative_position,
    const unsigned int pre_delay)
{
    jassert(channel < uChannels);
    jassert(relative_position <= uLength);
    jassert(pre_delay <= uPreDelay);

    int nPosition = uCurrentPosition - relative_position;

    if (pre_delay)
    {
        nPosition -= pre_delay;
    }

    while (nPosition < 0)
    {
        // make sure "nPosition" is positive
        nPosition += uTotalLength;
    }

    nPosition %= uTotalLength;

    return audioData[nPosition + uChannelOffset[channel]];
}


template <typename Type>
unsigned int RingBuffer<Type>::addSamples(
    AudioBuffer<Type> &source,
    const unsigned int sourceStartSample,
    const unsigned int numSamples)
{
    if (numSamples <= 0)
    {
        return 0;
    }

    jassert(source.getNumChannels() >= static_cast<int>(uChannels));
    jassert(numSamples <= uLength);
    jassert((sourceStartSample + numSamples) <= static_cast<unsigned int>(
                source.getNumSamples()));

    unsigned int uSamplesLeft = numSamples;
    unsigned int uSamplesFinished = 0;
    unsigned int uProcessedSamples = 0;

    while (uSamplesLeft > 0)
    {
        unsigned int uSamplesToCopy = uChunkSize - uSamplesInBuffer;
        unsigned int uSamplesToCopy_2 = uTotalLength - uCurrentPosition;

        if (uSamplesToCopy_2 < uSamplesToCopy)
        {
            uSamplesToCopy = uSamplesToCopy_2;
        }

        if (uSamplesToCopy > uSamplesLeft)
        {
            uSamplesToCopy = uSamplesLeft;
        }

        for (unsigned int uChannel = 0; uChannel < uChannels; ++uChannel)
        {
            memcpy(audioData + uCurrentPosition + uChannelOffset[uChannel],
                   source.getReadPointer(
                       uChannel,
                       sourceStartSample + uSamplesFinished),
                   sizeof(Type) * uSamplesToCopy);
        }

        uSamplesInBuffer += uSamplesToCopy;
        uProcessedSamples += uSamplesToCopy;

        // buffer is full
        if (uSamplesInBuffer == uChunkSize)
        {
            triggerFullBuffer(source,
                              uChunkSize,
                              sourceStartSample + uSamplesFinished,
                              uProcessedSamples);

            uProcessedSamples = 0;
        }

        uSamplesInBuffer %= uChunkSize;

        uCurrentPosition += uSamplesToCopy;
        uCurrentPosition %= uTotalLength;

        uSamplesLeft -= uSamplesToCopy;
        uSamplesFinished += uSamplesToCopy;
    }

#ifdef DEBUG

    // detection of memory leaks
    for (unsigned int uChannel = 0; uChannel < uChannels; ++uChannel)
    {
        jassert(audioData[uChannelOffset[uChannel] - 1] ==
                ringBufferMemTest_);
        jassert(audioData[uChannelOffset[uChannel]] !=
                ringBufferMemTest_);
        jassert(audioData[uChannelOffset[uChannel] + uTotalLength] ==
                ringBufferMemTest_);
    }

#endif

    return uProcessedSamples;
}


template <typename Type>
void RingBuffer<Type>::copyToBuffer(
    AudioBuffer<Type> &destination,
    const unsigned int destStartSample,
    const unsigned int numSamples,
    const unsigned int pre_delay)
{
    if (numSamples <= 0)
    {
        return;
    }

    jassert(destination.getNumChannels() >= static_cast<int>(uChannels));
    jassert(numSamples <= uLength);
    jassert(pre_delay <= uPreDelay);
    jassert((destStartSample + numSamples) <= static_cast<unsigned int>(
                destination.getNumSamples()));

    unsigned int uSamplesLeft = numSamples;
    unsigned int uSamplesFinished = 0;

    int nStartPosition = uCurrentPosition - numSamples - pre_delay;

    // make sure "nStartPosition" is positive
    while (nStartPosition < 0)
    {
        nStartPosition += uTotalLength;
    }

    nStartPosition %= uTotalLength;

    while (uSamplesLeft > 0)
    {
        unsigned int uSamplesToCopy = uTotalLength - nStartPosition;

        if (uSamplesToCopy > uSamplesLeft)
        {
            uSamplesToCopy = uSamplesLeft;
        }

        for (unsigned int uChannel = 0; uChannel < uChannels; ++uChannel)
        {
            memcpy(destination.getWritePointer(
                       uChannel,
                       destStartSample + uSamplesFinished),
                   audioData + nStartPosition + uChannelOffset[uChannel],
                   sizeof(Type) * uSamplesToCopy);
        }

        nStartPosition += uSamplesToCopy;
        nStartPosition %= uTotalLength;

        uSamplesLeft -= uSamplesToCopy;
        uSamplesFinished += uSamplesToCopy;
    }
}


template <typename Type>
Type RingBuffer<Type>::getMagnitude(
    const unsigned int channel,
    const unsigned int numSamples,
    const unsigned int pre_delay)
{
    Type magnitude = 0;

    for (unsigned int uSample = 0; uSample < numSamples; ++uSample)
    {
        magnitude = jmax(magnitude, getSample(channel, uSample, pre_delay));
    }

    return magnitude;
}


template <typename Type>
Type RingBuffer<Type>::getRMSLevel(
    const unsigned int channel,
    const unsigned int numSamples,
    const unsigned int pre_delay)
{
    Type runningSum = 0;

    for (unsigned int uSample = 0; uSample < numSamples; ++uSample)
    {
        Type sampleValue = getSample(channel, uSample, pre_delay);
        runningSum += sampleValue * sampleValue;
    }

    return static_cast<Type>(sqrt(runningSum / numSamples));
}


template <typename Type>
void RingBuffer<Type>::setCallbackClass(
    RingBufferProcessor<Type> *callback_class)
{
    callbackClass_ = callback_class;
}


template <typename Type>
void RingBuffer<Type>::clearCallbackClass()
{
    callbackClass_ = nullptr;
}


template <typename Type>
void RingBuffer<Type>::triggerFullBuffer(
    AudioBuffer<Type> &buffer,
    const unsigned int uChunkSize,
    const unsigned int uBufferPosition,
    const unsigned int uProcessedSamples)
{
    if (callbackClass_)
    {
        callbackClass_->processBufferChunk(buffer,
                                           uChunkSize,
                                           uBufferPosition,
                                           uProcessedSamples);
    }
}


// explicit instantiation of all template instances
template class RingBuffer<float>;
template class RingBuffer<double>;


// Local Variables:
// ispell-local-dictionary: "british"
// End:
