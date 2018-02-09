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


template <typename Type>
class RingBufferProcessor;


/// Audio ring buffer with pre-delay.  Can call a callback function
/// every time a certain number of samples have been added.
///
template <typename Type>
class RingBuffer
{
public:
    RingBuffer(const int numberOfChannels,
               const int numberOfSamples,
               const int preDelay,
               const int chunkSize);

    void clear();
    void setCallbackClass(RingBufferProcessor<Type> *callbackClass);

    int getNumberOfChannels() const;
    int getCurrentPosition() const;
    int getNumberOfSamples() const;
    int getPreDelay() const;

    void addSamples(const AudioBuffer<Type> &source,
                    const int startSample,
                    const int numberOfSamples);

    Type getSample(const int channel,
                   const int offset,
                   const bool applyPreDelay) const;

    void getSamples(AudioBuffer<Type> &destination,
                    const int destStartSample,
                    const int numberOfSamples,
                    const bool applyPreDelay) const;

    Type getMagnitude(const int channel,
                      const int numberOfSamples) const;

    Type getRMSLevel(const int channel,
                     const int numberOfSamples) const;

protected:
    RingBufferProcessor<Type> *callbackClass_;

    Array<int> channelOffsets_;
    HeapBlock<Type> audioData_;

    int numberOfChannels_;
    int numberOfSamples_;

    int currentPosition_;
    int totalLength_;

    int chunkSize_;
    int samplesToFilledChunk_;

    int preDelay_;

private:
    JUCE_LEAK_DETECTOR(RingBuffer);

    const Type ringBufferMemTestByte_;
};


/// Used to receive callbacks every time a certain number of samples
/// have been added to a RingBuffer.
///
template <typename Type>
class RingBufferProcessor
{
public:
    /// Called every time a certain number of samples have been added
    /// to a RingBuffer.
    ///
    /// @param ringBuffer ring buffer with filled "chunk"
    ///
    /// @param chunkSize number of samples in the "chunk"
    ///
    virtual void processBufferChunk(RingBuffer<Type> *ringBuffer,
                                    const int chunkSize) = 0;
};


// Local Variables:
// ispell-local-dictionary: "british"
// End:
