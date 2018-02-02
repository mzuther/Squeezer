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


template <typename Type>
class RingBuffer
{
public:
    RingBuffer(const String &bufferName,
               const int numberOfChannels,
               const int numberOfSamples,
               const int preDelay,
               const int chunkSize);

    void clear();
    void setCallbackClass(RingBufferProcessor<Type> *callbackClass);

    String getBufferName() const;
    int getNumberOfChannels() const;
    int getCurrentPosition() const;
    int getSamplesInBuffer() const;
    int getNumberOfSamples() const;
    int getPreDelay() const;

    Type getSample(const int channel,
                   const int relativePosition,
                   const int preDelay) const;

    int addSamples(const AudioBuffer<Type> &source,
                   const int startSample,
                   const int numberOfSamples);

    void getSamples(AudioBuffer<Type> &destination,
                    const int destStartSample,
                    const int numberOfSamples,
                    const int preDelay) const;

    Type getMagnitude(const int channel,
                      const int numberOfSamples,
                      const int preDelay) const;

    Type getRMSLevel(const int channel,
                     const int numberOfSamples,
                     const int preDelay) const;

protected:
    void clearCallbackClass();

    void triggerFullBuffer(const int chunkSize,
                           const int bufferPosition,
                           const int processedSamples);

    RingBufferProcessor<Type> *callbackClass_;

    Array<int> channelOffsets_;
    HeapBlock<Type> audioData_;

    String bufferName_;

    int numberOfChannels_;
    int numberOfSamples_;
    int preDelay_;
    int totalLength_;
    int chunkSize_;

    int currentPosition_;
    int samplesInBuffer_;

private:
    JUCE_LEAK_DETECTOR(RingBuffer);

    const Type ringBufferMemTest_;
};


template <typename Type>
class RingBufferProcessor
{
public:
    virtual void processBufferChunk(const int chunkSize,
                                    const int bufferPosition,
                                    const int processedSamples) = 0;
};


// Local Variables:
// ispell-local-dictionary: "british"
// End:
