/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2016 Martin Zuther (http://www.mzuther.de/)

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

#ifndef __FRUT_AUDIO_RING_BUFFER_H__
#define __FRUT_AUDIO_RING_BUFFER_H__


class RingBufferProcessor;

class RingBuffer
{
public:
    RingBuffer(const String &buffer_name, const unsigned int channels, const unsigned int length, const unsigned int pre_delay, const unsigned int chunk_size);

    void clear();
    void setCallbackClass(RingBufferProcessor *callback_class);

    String getBufferName();
    unsigned int getCurrentPosition();
    unsigned int getSamplesInBuffer();
    unsigned int getBufferLength();
    unsigned int getTotalLength();
    unsigned int getPreDelay();

    float getSample(const unsigned int channel, const unsigned int relative_position, const unsigned int pre_delay);

    unsigned int addSamples(AudioBuffer<float> &source, const unsigned int sourceStartSample, const unsigned int numSamples);
    void copyToBuffer(AudioBuffer<float> &destination, const unsigned int destStartSample, const unsigned int numSamples, const unsigned int pre_delay);

    float getMagnitude(const unsigned int channel, const unsigned int numSamples, const unsigned int pre_delay);
    float getRMSLevel(const unsigned int channel, const unsigned int numSamples, const unsigned int pre_delay);

protected:
    void clearCallbackClass();
    void triggerFullBuffer(AudioBuffer<float> &buffer, const unsigned int uChunkSize, const unsigned int uBufferPosition, const unsigned int uProcessedSamples);

    RingBufferProcessor *pCallbackClass;
    String strBufferName;

    unsigned int uChannels;
    unsigned int uLength;
    unsigned int uPreDelay;
    unsigned int uTotalLength;
    unsigned int uChunkSize;

    unsigned int uCurrentPosition;
    unsigned int uSamplesInBuffer;
    Array<unsigned int> uChannelOffset;

    const float fRingBufferMemTest;

    HeapBlock<float> audioData;

private:
    JUCE_LEAK_DETECTOR(RingBuffer);
};


class RingBufferProcessor
{
public:
    virtual void processBufferChunk(AudioBuffer<float> &buffer, const unsigned int uChunkSize, const unsigned int uBufferPosition, const unsigned int uProcessedSamples) = 0;
};


#endif  // __FRUT_AUDIO_RING_BUFFER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
