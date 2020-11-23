/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2020 Martin Zuther (http://www.mzuther.de/)

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

#ifndef FRUT_AUDIO_RING_BUFFER_H
#define FRUT_AUDIO_RING_BUFFER_H

namespace frut::audio
{

template <typename Type>
class RingBufferProcessor;


/// Audio ring buffer.  This ring buffer supports delaying the input
/// samples by a defined pre-delay.  It can also call a callback
/// function every time a certain number of samples have been added.
///
template <typename Type>
class RingBuffer
{
public:
   RingBuffer( const int numberOfChannels,
               const int numberOfSamples,
               const int preDelay,
               const int chunkSize );

   void clear();
   void setCallbackClass( RingBufferProcessor<Type>* callbackClass );

   int getNumberOfChannels() const;
   int getNumberOfSamples() const;
   int getPreDelay() const;


   /// Add audio samples from an AudioBuffer.  **This function will
   /// call the callback function every time the total number of
   /// samples added to this buffer (now or before) exceeds the
   /// "chunk" size.  The write position will be moved.**
   ///
   /// @param source source buffer
   ///
   /// @param sourceStartSample the index in the source buffer to
   ///        start reading from
   ///
   /// @param numberOfSamples number of samples to copy
   ///
   inline void addFrom(
      const AudioBuffer<Type>& source,
      const int sourceStartSample,
      const int numberOfSamples )
   {
      bool updatePosition = true;

      importFrom( source, sourceStartSample,
                  numberOfSamples,
                  updatePosition );
   }


   /// Overwrite audio samples from an AudioBuffer.  **This function
   /// will not call the callback function and leave the write
   /// position alone.**
   ///
   /// @param source source buffer
   ///
   /// @param sourceStartSample the index in the source buffer to
   ///        start reading from
   ///
   /// @param numberOfSamples number of samples to copy
   ///
   inline void overwriteFrom(
      const AudioBuffer<Type>& source,
      const int sourceStartSample,
      const int numberOfSamples )
   {
      bool updatePosition = false;

      importFrom( source, sourceStartSample,
                  numberOfSamples,
                  updatePosition );
   }


   /// Remove audio samples from this ring buffer to an AudioBuffer.
   /// **The read position will be moved.**
   ///
   /// @param destination destination buffer
   ///
   /// @param destStartSample the index in the destination buffer to
   ///        start writing to
   ///
   /// @param numberOfSamples number of samples to copy
   ///
   inline void removeTo(
      AudioBuffer<Type>& destination,
      const int destStartSample,
      const int numberOfSamples )
   {
      bool updatePosition = true;

      exportTo( destination, destStartSample,
                numberOfSamples,
                updatePosition );
   }


   /// Copy audio samples to an AudioBuffer.  **This function will
   /// leave the read position alone.**
   ///
   /// @param destination destination buffer
   ///
   /// @param destStartSample the index in the destination buffer to
   ///        start writing to
   ///
   /// @param numberOfSamples number of samples to copy
   ///
   inline void copyTo(
      AudioBuffer<Type>& destination,
      const int destStartSample,
      const int numberOfSamples )
   {
      bool updatePosition = false;

      exportTo( destination, destStartSample,
                numberOfSamples,
                updatePosition );
   }


   void removeToNull( const int numberOfSamples );

protected:
   void importFrom( const AudioBuffer<Type>& source,
                    const int sourceStartSample,
                    const int numberOfSamples,
                    const bool updatePosition );

   void exportTo( AudioBuffer<Type>& destination,
                  const int destStartSample,
                  const int numberOfSamples,
                  const bool updatePosition );

   RingBufferProcessor<Type>* callbackClass_;
   BufferPosition bufferPosition_;

   Array<int> channelOffsets_;
   HeapBlock<Type> audioData_;

   int numberOfChannels_;
   int chunkSize_;
   int samplesToFilledChunk_;

private:
   JUCE_LEAK_DETECTOR( RingBuffer );

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
   /// @param buffer audio buffer with filled "chunk"
   ///
   /// @return determines whether the audio buffer's contents should
   ///         be copied back to the original RingBuffer.
   ///
   virtual bool processBufferChunk( AudioBuffer<Type>& buffer ) = 0;
};

} // namespace

#endif  // FRUT_AUDIO_RING_BUFFER_H
