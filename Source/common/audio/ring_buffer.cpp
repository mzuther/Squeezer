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


/// Create a new audio ring buffer.
///
/// @param numberOfChannels number of audio channels
///
/// @param numberOfSamples number of audio samples per channel
///
/// @param preDelay number of samples the buffer output will be
///        delayed
///
/// @param chunkSize number of samples that need to be added to the
///        buffer for the callback to be called
///
template <typename Type>
RingBuffer<Type>::RingBuffer(
    const int numberOfChannels,
    const int numberOfSamples,
    const int preDelay,
    const int chunkSize) :

    ringBufferMemTestByte_(255)
{
    jassert(numberOfChannels > 0);
    jassert(numberOfSamples > 0);
    jassert(isPositiveAndNotGreaterThan(chunkSize, numberOfSamples));

    // initialize ring buffer variables
    numberOfChannels_ = numberOfChannels;
    numberOfSamples_ = numberOfSamples;

    // this ring buffer supports delaying the input samples by a
    // defined pre-delay
    preDelay_ = preDelay;
    totalLength_ = numberOfSamples_ + preDelay_;

    // optionally, every time chunkSize_ samples have been added, the
    // ring buffer will call the function processBufferChunk of a
    // callback class to allow further processing
    chunkSize_ = chunkSize;
    this->setCallbackClass(nullptr);

    // allocate memory for samples and pad memory areas to allow the
    // detection of memory leaks
    int paddedTotalLength = totalLength_ + 2;
    audioData_.calloc(numberOfChannels_ * paddedTotalLength);

    for (int channel = 0; channel < numberOfChannels_; ++channel)
    {
        // initialize the memory offsets of each channel
        int channelOffset = (channel * paddedTotalLength) + 1;
        channelOffsets_.insert(channel, channelOffset);

        // pad each channel with a "sample" of ringBufferMemTestByte_
        // to allow detection of memory leaks
        int channelPadLeft = channelOffset - 1;
        int channelPadRight = channelOffset + totalLength_;

        audioData_[channelPadLeft] = ringBufferMemTestByte_;
        audioData_[channelPadRight] = ringBufferMemTestByte_;
    }

    // clear the ring buffer
    this->clear();
}


/// Clear the ring buffer.
///
template <typename Type>
void RingBuffer<Type>::clear()
{
    // reset the current write position of the buffer
    currentPosition_ = 0;

    // reset the number of samples left until a chunk is full
    samplesToFilledChunk_ = chunkSize_;

    // reset all samples
    for (int channel = 0; channel < numberOfChannels_; ++channel)
        for (int sample = 0; sample < totalLength_; ++sample)
        {
            audioData_[channelOffsets_[channel] + sample] = 0.0;
        }

#ifdef DEBUG

    // detect memory leaks
    for (int channel = 0; channel < numberOfChannels_; ++channel)
    {
        int channelPadLeft = channelOffsets_[channel] - 1;
        int channelPadRight = channelOffsets_[channel] + totalLength_;

        jassert(audioData_[channelPadLeft] ==
                ringBufferMemTestByte_);
        jassert(audioData_[channelPadLeft + 1] !=
                ringBufferMemTestByte_);
        jassert(audioData_[channelPadRight - 1] !=
                ringBufferMemTestByte_);
        jassert(audioData_[channelPadRight] ==
                ringBufferMemTestByte_);
    }

#endif
}


/// Set class that defines the callback function.
///
/// @param callbackClass class that defines the callback function
///        (must be derived from RingBufferProcessor)
///
template <typename Type>
void RingBuffer<Type>::setCallbackClass(
    RingBufferProcessor<Type> *callbackClass)
{
    callbackClass_ = callbackClass;
}


/// Get number of audio channels.
///
/// @return number of audio channels
///
template <typename Type>
int RingBuffer<Type>::getNumberOfChannels() const
{
    return numberOfChannels_;
}


/// Get current write position in buffer (ranges from zero to number
/// of samples + pre-delay).
///
/// @return current write position
///
template <typename Type>
int RingBuffer<Type>::getCurrentPosition() const
{
    return currentPosition_;
}


/// Get number of audio samples per channel (excluding pre-delay).
///
/// @return number of audio samples
///
template <typename Type>
int RingBuffer<Type>::getNumberOfSamples() const
{
    return numberOfSamples_;
}


/// Get number of samples the buffer output will be delayed.
///
/// @return pre-delay in samples
///
template <typename Type>
int RingBuffer<Type>::getPreDelay() const
{
    return preDelay_;
}


/// Copy audio samples from an AudioBuffer to this ring buffer.
/// **This function will call the callback function every time the
/// total number of samples added to this buffer (now or before)
/// exceeds the "chunk" size.**
///
/// @param source source buffer
///
/// @param sourceStartSample the offset in the source buffer to start
///        reading from
///
/// @param numberOfSamples number of samples to copy
///
template <typename Type>
void RingBuffer<Type>::addSamples(
    const AudioBuffer<Type> &source,
    const int sourceStartSample,
    const int numberOfSamples)
{
    jassert(source.getNumChannels() == numberOfChannels_);
    jassert(isPositiveAndNotGreaterThan(numberOfSamples, numberOfSamples_));
    jassert(isPositiveAndNotGreaterThan(sourceStartSample + numberOfSamples,
                                        source.getNumSamples()));

    // number of processed samples
    int processedSamples = 0;

    // number of unprocessed samples
    int unprocessedSamples = numberOfSamples;

    // keep processing while there are still samples
    while (unprocessedSamples > 0)
    {
        // number of samples left until the buffer wraps around
        int samplesToBufferWrap = totalLength_ - currentPosition_;

        // number of samples to copy in this pass
        int samplesToCopy = jmin(unprocessedSamples,
                                 samplesToFilledChunk_,
                                 samplesToBufferWrap);

        for (int channel = 0; channel < numberOfChannels_; ++channel)
        {
            memcpy(audioData_ + channelOffsets_[channel] + currentPosition_,
                   source.getReadPointer(channel,
                                         sourceStartSample + processedSamples),
                   sizeof(Type) * samplesToCopy);
        }

        // update current write position
        currentPosition_ += samplesToCopy;
        currentPosition_ %= totalLength_;

        // update number of processed samples
        processedSamples += samplesToCopy;

        // update number of unprocessed samples
        unprocessedSamples -= samplesToCopy;

        // update number of samples left to fill chunk
        samplesToFilledChunk_ -= samplesToCopy;

        // chunk is full
        if (samplesToFilledChunk_ == 0)
        {
            // reset of samples left to fill chunk
            samplesToFilledChunk_ = chunkSize_;

            // run callback (if any)
            if (callbackClass_)
            {
                callbackClass_->processBufferChunk(this, chunkSize_);
            }
        }
    }

#ifdef DEBUG

    // detect memory leaks
    for (int channel = 0; channel < numberOfChannels_; ++channel)
    {
        int channelPadLeft = channelOffsets_[channel] - 1;
        int channelPadRight = channelOffsets_[channel] + totalLength_;

        jassert(audioData_[channelPadLeft] ==
                ringBufferMemTestByte_);
        jassert(audioData_[channelPadRight] ==
                ringBufferMemTestByte_);
    }

#endif
}


/// Get the value of a single sample.
///
/// @param channel audio channel to query
///
/// @param offset offset within the channel (with pre-delay applied);
///        ranges from zero (start of buffer) to the number of samples
///        in the buffer
///
/// @param applyPreDelay delay output by pre-delay
///
/// @return sample value
///
template <typename Type>
Type RingBuffer<Type>::getSample(
    const int channel,
    const int offset,
    const bool applyPreDelay) const
{
    jassert(channel < numberOfChannels_);
    jassert(isPositiveAndNotGreaterThan(offset, numberOfSamples_));

    // the samples that have already been written lie to the *left* of
    // the current write position
    int position = currentPosition_ - numberOfSamples_ + offset;

    if (applyPreDelay)
    {
        // apply pre-delay
        position -= preDelay_;
    }

    // make sure "position" is positive
    while (position < 0)
    {
        position += totalLength_;
    }

    // wrap "position" to buffer length
    position %= totalLength_;

    Type sampleValue = audioData_[channelOffsets_[channel] + position];
    return sampleValue;
}


/// Copy samples from this ring buffer to an AudioBuffer.
///
/// @param destination destination buffer
///
/// @param destStartSample the offset in the destination buffer to start
///        writing to
///
/// @param numberOfSamples number of samples to copy
///
/// @param applyPreDelay delay output by pre-delay
///
template <typename Type>
void RingBuffer<Type>::getSamples(
    AudioBuffer<Type> &destination,
    const int destStartSample,
    const int numberOfSamples,
    const bool applyPreDelay) const
{
    jassert(numberOfChannels_ == destination.getNumChannels());
    jassert(isPositiveAndNotGreaterThan(numberOfSamples, numberOfSamples_));
    jassert(isPositiveAndNotGreaterThan(destStartSample + numberOfSamples,
                                        destination.getNumSamples()));

    // number of processed samples
    int processedSamples = 0;

    // number of unprocessed samples
    int unprocessedSamples = numberOfSamples;

    // source position; the samples that have already been written lie
    // to the *left* of the current write position
    int position = currentPosition_ - numberOfSamples;

    if (applyPreDelay)
    {
        // apply pre-delay
        position -= preDelay_;
    }

    // make sure "position" is positive
    while (position < 0)
    {
        position += totalLength_;
    }

    // wrap "position" to buffer length
    position %= totalLength_;

    // keep processing while there are still samples
    while (unprocessedSamples > 0)
    {
        // number of samples left until the buffer wraps around
        int samplesToBufferWrap = totalLength_ - position;

        // number of samples to copy in this pass
        int samplesToCopy = jmin(unprocessedSamples,
                                 samplesToBufferWrap);

        for (int channel = 0; channel < numberOfChannels_; ++channel)
        {
            memcpy(destination.getWritePointer(
                       channel, destStartSample + processedSamples),
                   audioData_ + channelOffsets_[channel] + position,
                   sizeof(Type) * samplesToCopy);
        }

        // update source position
        position += samplesToCopy;
        position %= totalLength_;

        // update number of processed samples
        processedSamples += samplesToCopy;

        // update number of unprocessed samples
        unprocessedSamples -= samplesToCopy;
    }
}


/// Finds the highest absolute sample value within a channel.
///
/// @param channel audio channel to process
///
/// @param numberOfSamples number of samples to process, starting from
///        the **beginning** of the buffer
///
/// @return highest absolute sample value
///
template <typename Type>
Type RingBuffer<Type>::getMagnitude(
    const int channel,
    const int numberOfSamples) const
{
    jassert(isPositiveAndBelow(channel, numberOfChannels_));
    jassert(isPositiveAndNotGreaterThan(numberOfSamples, numberOfSamples_));

    // the samples that have already been written lie to the *left* of
    // the current write position
    int bufferStart = currentPosition_ - numberOfSamples_;

    // apply pre-delay
    bufferStart -= preDelay_;

    // make sure "bufferStart" is positive
    while (bufferStart < 0)
    {
        bufferStart += totalLength_;
    }

    Type magnitude = 0;
    int channelOffset = channelOffsets_[channel];

    for (int sample = 0; sample < numberOfSamples; ++sample)
    {
        // wrap "position" to buffer length
        int position = (bufferStart + sample) % totalLength_;

        Type amplitude = audioData_[channelOffset + position];

        if (amplitude < 0)
        {
            amplitude = -amplitude;
        }

        magnitude = jmax(magnitude, amplitude);
    }

    return magnitude;
}


/// Finds the root mean squared level for a channel.
///
/// @param channel audio channel to process
///
/// @param numberOfSamples number of samples to process
///
/// @return root mean squared level
///
template <typename Type>
Type RingBuffer<Type>::getRMSLevel(
    const int channel,
    const int numberOfSamples) const
{
    jassert(isPositiveAndBelow(channel, numberOfChannels_));
    jassert(isPositiveAndNotGreaterThan(numberOfSamples, numberOfSamples_));

    // the samples that have already been written lie to the *left* of
    // the current write position
    int bufferStart = currentPosition_ - numberOfSamples_;

    // apply pre-delay
    bufferStart -= preDelay_;

    // make sure "bufferStart" is positive
    while (bufferStart < 0)
    {
        bufferStart += totalLength_;
    }

    Type runningSum = 0;
    int channelOffset = channelOffsets_[channel];

    for (int sample = 0; sample < numberOfSamples; ++sample)
    {
        // wrap "position" to buffer length
        int position = (bufferStart + sample) % totalLength_;

        Type sampleValue = audioData_[channelOffset + position];
        runningSum += sampleValue * sampleValue;
    }

    return static_cast<Type>(sqrt(runningSum / numberOfSamples));
}


// explicit instantiation of all template instances
template class RingBuffer<float>;
template class RingBuffer<double>;


// Local Variables:
// ispell-local-dictionary: "british"
// End:
