/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012-2015 Martin Zuther (http://www.mzuther.de/)

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

#include "audio_ring_buffer.h"

AudioRingBuffer::AudioRingBuffer(const String &buffer_name, const unsigned int channels, const unsigned int length, const unsigned int pre_delay, const unsigned int chunk_size)
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

    // pad memory areas with RING_BUFFER_MEM_TEST to allow detection of
    // memory leaks
    pAudioData = (float *) malloc(uChannels * (uTotalLength + 2) * sizeof(float));

    for (unsigned int i = 0; i < (uChannels * (uTotalLength + 2)); i++)
    {
        pAudioData[i] = RING_BUFFER_MEM_TEST;
    }

    uCurrentPosition = 0;
    uSamplesInBuffer = 0;
    uChannelOffset = new unsigned int[uChannels];

    for (unsigned int uChannel = 0; uChannel < uChannels; uChannel++)
    {
        uChannelOffset[uChannel] = uChannel * (uTotalLength + 2) + 1;
    }

    this->clear();
}


AudioRingBuffer::~AudioRingBuffer()
{
    delete [] uChannelOffset;
    uChannelOffset = nullptr;

    free(pAudioData);
}


void AudioRingBuffer::clear()
{
    uCurrentPosition = 0;

    for (unsigned int uChannel = 0; uChannel < uChannels; uChannel++)
        for (unsigned int uSample = 0; uSample < uTotalLength; uSample++)
        {
            pAudioData[uSample + uChannelOffset[uChannel]] = 0.0f;
        }

#ifdef DEBUG

    // detection of memory leaks
    for (unsigned int uChannel = 0; uChannel < uChannels; uChannel++)
    {
        jassert(pAudioData[uChannelOffset[uChannel] - 1] == RING_BUFFER_MEM_TEST);
        jassert(pAudioData[uChannelOffset[uChannel]] != RING_BUFFER_MEM_TEST);
        jassert(pAudioData[uChannelOffset[uChannel] + uTotalLength] == RING_BUFFER_MEM_TEST);
    }

#endif
}


String AudioRingBuffer::getBufferName()
{
    return strBufferName;
}


unsigned int AudioRingBuffer::getCurrentPosition()
{
    return uCurrentPosition;
}


unsigned int AudioRingBuffer::getSamplesInBuffer()
{
    return uSamplesInBuffer;
}


unsigned int AudioRingBuffer::getBufferLength()
{
    return uLength;
}


unsigned int AudioRingBuffer::getTotalLength()
{
    return uTotalLength;
}


unsigned int AudioRingBuffer::getPreDelay()
{
    return uPreDelay;
}


float AudioRingBuffer::getSample(const unsigned int channel, const unsigned int relative_position, const unsigned int pre_delay)
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
        nPosition += uTotalLength;    // make sure "nPosition" is positive
    }

    nPosition %= uTotalLength;

    return pAudioData[nPosition + uChannelOffset[channel]];
}


unsigned int AudioRingBuffer::addSamples(AudioSampleBuffer &source, const unsigned int sourceStartSample, const unsigned int numSamples)
{
    if (numSamples <= 0)
    {
        return 0;
    }

    jassert(source.getNumChannels() >= (int) uChannels);
    jassert(numSamples <= uLength);
    jassert((sourceStartSample + numSamples) <= (unsigned int) source.getNumSamples());

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

        for (unsigned int uChannel = 0; uChannel < uChannels; uChannel++)
        {
            memcpy(pAudioData + uCurrentPosition + uChannelOffset[uChannel], source.getReadPointer(uChannel, sourceStartSample + uSamplesFinished), sizeof(float) * uSamplesToCopy);
        }

        uSamplesInBuffer += uSamplesToCopy;

        uProcessedSamples += uSamplesToCopy;
        bool bBufferFull = (uSamplesInBuffer == uChunkSize);
        uSamplesInBuffer %= uChunkSize;

        uCurrentPosition += uSamplesToCopy;
        uCurrentPosition %= uTotalLength;

        uSamplesLeft -= uSamplesToCopy;

        if (bBufferFull)
        {
            triggerFullBuffer(source, uChunkSize, sourceStartSample + uSamplesFinished, uProcessedSamples);
            uProcessedSamples = 0;
        }

        uSamplesFinished += uSamplesToCopy;
    }

#ifdef DEBUG

    // detection of memory leaks
    for (unsigned int uChannel = 0; uChannel < uChannels; uChannel++)
    {
        jassert(pAudioData[uChannelOffset[uChannel] - 1] == RING_BUFFER_MEM_TEST);
        jassert(pAudioData[uChannelOffset[uChannel]] != RING_BUFFER_MEM_TEST);
        jassert(pAudioData[uChannelOffset[uChannel] + uTotalLength] == RING_BUFFER_MEM_TEST);
    }

#endif

    return uProcessedSamples;
}


void AudioRingBuffer::copyToBuffer(AudioSampleBuffer &destination, const unsigned int destStartSample, const unsigned int numSamples, const unsigned int pre_delay)
{
    if (numSamples <= 0)
    {
        return;
    }

    jassert(destination.getNumChannels() >= (int) uChannels);
    jassert(numSamples <= uLength);
    jassert(pre_delay <= uPreDelay);
    jassert((destStartSample + numSamples) <= (unsigned int) destination.getNumSamples());

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

        for (unsigned int uChannel = 0; uChannel < uChannels; uChannel++)
        {
            memcpy(destination.getWritePointer(uChannel, destStartSample + uSamplesFinished), pAudioData + nStartPosition + uChannelOffset[uChannel], sizeof(float) * uSamplesToCopy);
        }

        nStartPosition += uSamplesToCopy;
        nStartPosition %= uTotalLength;

        uSamplesLeft -= uSamplesToCopy;
        uSamplesFinished += uSamplesToCopy;
    }
}


float AudioRingBuffer::getMagnitude(const unsigned int channel, const unsigned int numSamples, const unsigned int pre_delay)
{
    float fMagnitude = 0.0f;

    for (unsigned int uSample = 0; uSample < numSamples; uSample++)
    {
        float fSampleValue = fabsf(getSample(channel, uSample, pre_delay));

        if (fSampleValue > fMagnitude)
        {
            fMagnitude = fSampleValue;
        }
    }

    return fMagnitude;
}


float AudioRingBuffer::getRMSLevel(const unsigned int channel, const unsigned int numSamples, const unsigned int pre_delay)
{
    double dRunningSum = 0.0;

    for (unsigned int uSample = 0; uSample < numSamples; uSample++)
    {
        float fSampleValue = getSample(channel, uSample, pre_delay);
        dRunningSum += fSampleValue * fSampleValue;
    }

    return (float) sqrt(dRunningSum / numSamples);
}


void AudioRingBuffer::setCallbackClass(AudioRingBufferProcessor *callback_class)
{
    pCallbackClass = callback_class;
}


void AudioRingBuffer::clearCallbackClass()
{
    pCallbackClass = nullptr;
}


void AudioRingBuffer::triggerFullBuffer(AudioSampleBuffer &buffer, const unsigned int uChunkSize, const unsigned int uBufferPosition, const unsigned int uProcessedSamples)
{
    if (pCallbackClass)
    {
        pCallbackClass->processBufferChunk(buffer, uChunkSize, uBufferPosition, uProcessedSamples);
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
