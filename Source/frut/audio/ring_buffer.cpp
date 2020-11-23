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

namespace frut::audio
{

/// Create a new audio ring buffer.
///
/// @param numberOfChannels number of audio channels
///
/// @param numberOfSamples number of audio samples per channel
///
/// @param preDelay number of samples the buffer output will be
///        delayed (can be larger than numberOfSamples)
///
/// @param chunkSize number of samples that need to be added to the
///        buffer for the callback to be called
///
template <typename Type>
RingBuffer<Type>::RingBuffer(
   const int numberOfChannels,
   const int numberOfSamples,
   const int preDelay,
   const int chunkSize ) :

   bufferPosition_( numberOfSamples, preDelay ),
   ringBufferMemTestByte_( 255 )
{
   jassert( numberOfChannels > 0 );
   jassert( numberOfSamples > 0 );
   jassert( isPositiveAndNotGreaterThan( chunkSize,
                                         numberOfSamples ) );

   // initialize number of audio channels
   numberOfChannels_ = numberOfChannels;

   // get total buffer length (number of samples + pre-delay)
   int totalLength = bufferPosition_.getTotalBufferLength();

   // every time chunkSize_ samples have been added, the ring buffer
   // will call the function processBufferChunk of a callback class
   // to allow further processing; disable this functionality for now
   chunkSize_ = chunkSize;
   this->setCallbackClass( nullptr );

   // allocate memory for samples and pad memory areas to allow the
   // detection of memory leaks
   int paddedTotalLength = totalLength + 2;
   audioData_.calloc( numberOfChannels_ * paddedTotalLength );

   for ( int channel = 0; channel < numberOfChannels_; ++channel ) {
      // initialize the memory offsets of each channel
      int channelOffset = ( channel * paddedTotalLength ) + 1;
      channelOffsets_.insert( channel, channelOffset );

      // pad each channel with a "sample" of ringBufferMemTestByte_
      // to allow detection of memory leaks
      int channelPadLeft = channelOffset - 1;
      int channelPadRight = channelOffset + totalLength;

      audioData_[channelPadLeft] = ringBufferMemTestByte_;
      audioData_[channelPadRight] = ringBufferMemTestByte_;
   }

   // clear the ring buffer
   this->clear();
}


/// Clear ring buffer.
///
template <typename Type>
void RingBuffer<Type>::clear()
{
   // reset current read and write positions of the buffer
   bufferPosition_.reset();

   // reset number of samples left until a chunk is full
   samplesToFilledChunk_ = chunkSize_;

   // get total buffer length (number of samples + pre-delay)
   int totalLength = bufferPosition_.getTotalBufferLength();

   // reset all samples
   for ( int channel = 0; channel < numberOfChannels_; ++channel ) {
      for ( int sample = 0; sample < totalLength; ++sample ) {
         audioData_[channelOffsets_[channel] + sample] = 0;
      }
   }

#ifdef DEBUG

   // detect memory leaks
   for ( int channel = 0; channel < numberOfChannels_; ++channel ) {
      int channelPadLeft = channelOffsets_[channel] - 1;
      int channelPadRight = channelOffsets_[channel] + totalLength;

      jassert( audioData_[channelPadLeft] ==
               ringBufferMemTestByte_ );
      jassert( audioData_[channelPadLeft + 1] !=
               ringBufferMemTestByte_ );
      jassert( audioData_[channelPadRight - 1] !=
               ringBufferMemTestByte_ );
      jassert( audioData_[channelPadRight] ==
               ringBufferMemTestByte_ );
   }

#endif // DEBUG
}


/// Set class that defines the callback function.
///
/// @param callbackClass class that defines the callback function
///        (must be derived from RingBufferProcessor)
///
template <typename Type>
void RingBuffer<Type>::setCallbackClass(
   RingBufferProcessor<Type>* callbackClass )
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


/// Get number of audio samples per channel (excluding pre-delay).
///
/// @return number of audio samples
///
template <typename Type>
int RingBuffer<Type>::getNumberOfSamples() const
{
   return bufferPosition_.getNumberOfSamples();
}


/// Get number of samples the buffer output will be delayed.
///
/// @return pre-delay in samples
///
template <typename Type>
int RingBuffer<Type>::getPreDelay() const
{
   return bufferPosition_.getPreDelay();
}


/// Import audio samples from an AudioBuffer.  **This function will
/// call the callback function every time the total number of samples
/// added to this buffer (now or before) exceeds the "chunk" size.**
///
/// @param source source buffer
///
/// @param sourceStartSample the index in the source buffer to start
///        reading from
///
/// @param numberOfSamples number of samples to copy
///
/// @param updatePosition when true, the buffer's write position will
///        be updated
///
template <typename Type>
void RingBuffer<Type>::importFrom(
   const AudioBuffer<Type>& source,
   const int sourceStartSample,
   const int numberOfSamples,
   const bool updatePosition )
{
   jassert( source.getNumChannels() ==
            numberOfChannels_ );
   jassert( isPositiveAndNotGreaterThan( sourceStartSample + numberOfSamples,
                                         source.getNumSamples() ) );

   // number of processed samples
   int processedSamples = 0;

   // number of samples left to process
   int unprocessedSamples = numberOfSamples;

   // keep processing until all input samples have been processed
   while ( unprocessedSamples > 0 ) {
      // number of samples to copy in this pass
      int samplesToCopy;

      // positions of two sample blocks for storing samples
      int startIndex_1, blockSize_1;
      int startIndex_2, blockSize_2;

      if ( updatePosition ) {
         // determine number of samples to copy in this pass
         samplesToCopy = jmin( unprocessedSamples,
                               samplesToFilledChunk_ );

         // get positions of sample blocks; the write position will
         // be updated
         bufferPosition_.queue(
            samplesToCopy,
            startIndex_1, blockSize_1,
            startIndex_2, blockSize_2 );
      } else {
         // determine number of samples to copy in this pass
         samplesToCopy = unprocessedSamples;

         // get positions of sample blocks; leave write position
         // alone
         bufferPosition_.lookBackFromWritePosition(
            samplesToCopy,
            startIndex_1, blockSize_1,
            startIndex_2, blockSize_2 );
      }

      for ( int channel = 0; channel < numberOfChannels_; ++channel ) {
         // determine position of first sample block
         int sourceStart = sourceStartSample + processedSamples;
         int channelOffset = channelOffsets_[channel];

         // get pointer to destination audio data
         Type* destAudioData = audioData_.get();
         jassert( destAudioData != nullptr );
         destAudioData += channelOffset;

         // get pointer to source audio data
         const Type* sourceAudioData = source.getReadPointer( channel );
         jassert( sourceAudioData );

         // copy first sample block from external buffer
         memcpy( destAudioData + startIndex_1,
                 sourceAudioData + sourceStart,
                 sizeof( Type ) * blockSize_1 );

         // do we need to copy samples to the second block?
         if ( blockSize_2 > 0 ) {
            // determine position of second sample block
            sourceStart += blockSize_1;

            // copy second sample block from external buffer
            memcpy( destAudioData + startIndex_2,
                    sourceAudioData + sourceStart,
                    sizeof( Type ) * blockSize_2 );
         }
      }

      // increase number of processed samples
      processedSamples += samplesToCopy;

      // decrease number of samples left to process
      unprocessedSamples -= samplesToCopy;

      // update write position?
      if ( ! updatePosition ) {

         continue;
      }

      // decrease number of samples left to fill chunk
      samplesToFilledChunk_ -= samplesToCopy;

      // chunk is full
      if ( samplesToFilledChunk_ == 0 ) {
         // reset of samples left to fill chunk
         samplesToFilledChunk_ = chunkSize_;

         // run callback (if any)
         if ( callbackClass_ ) {
            AudioBuffer<Type> buffer( numberOfChannels_, chunkSize_ );

            copyTo( buffer, 0, chunkSize_ );

            // process buffer chunk
            bool writeBack = callbackClass_->processBufferChunk( buffer );

            if ( writeBack ) {
               overwriteFrom( buffer, 0, chunkSize_ );
            }
         }
      }
   }

#ifdef DEBUG

   // get total buffer length (number of samples + pre-delay)
   int totalLength = bufferPosition_.getTotalBufferLength();

   // detect memory leaks
   for ( int channel = 0; channel < numberOfChannels_; ++channel ) {
      int channelPadLeft = channelOffsets_[channel] - 1;
      int channelPadRight = channelOffsets_[channel] + totalLength;

      jassert( audioData_[channelPadLeft] ==
               ringBufferMemTestByte_ );
      jassert( audioData_[channelPadRight] ==
               ringBufferMemTestByte_ );
   }

#endif // DEBUG
}


/// Export audio samples to an AudioBuffer.
///
/// @param destination destination buffer
///
/// @param destStartSample the index in the destination buffer to start
///        writing to
///
/// @param numberOfSamples number of samples to copy
///
/// @param updatePosition when true, the buffer's read position will
///        be updated
///
template <typename Type>
void RingBuffer<Type>::exportTo(
   AudioBuffer<Type>& destination,
   const int destStartSample,
   const int numberOfSamples,
   const bool updatePosition )
{
   jassert( destination.getNumChannels() ==
            numberOfChannels_ );
   jassert( isPositiveAndNotGreaterThan( destStartSample + numberOfSamples,
                                         destination.getNumSamples() ) );

   // positions of two sample blocks for storing samples
   int startIndex_1, blockSize_1;
   int startIndex_2, blockSize_2;

   if ( updatePosition ) {
      // get positions of sample blocks; the read position will be
      // updated
      bufferPosition_.dequeue(
         numberOfSamples,
         startIndex_1, blockSize_1,
         startIndex_2, blockSize_2 );
   } else {
      // get positions of sample blocks; leave read position alone
      bufferPosition_.lookBackFromWritePosition(
         numberOfSamples,
         startIndex_1, blockSize_1,
         startIndex_2, blockSize_2 );
   }

   for ( int channel = 0; channel < numberOfChannels_; ++channel ) {
      // determine position of first sample block
      int destStart = destStartSample;
      int channelOffset = channelOffsets_[channel];

      // get pointer to destination audio data
      Type* destAudioData = destination.getWritePointer( channel );
      jassert( destAudioData );

      // get pointer to source audio data
      const Type* sourceAudioData = audioData_.get();
      jassert( sourceAudioData != nullptr );
      sourceAudioData += channelOffset;

      // copy first sample block to external buffer
      memcpy( destAudioData + destStart,
              sourceAudioData + startIndex_1,
              sizeof( Type ) * blockSize_1 );

      // do we need to copy samples to the second block?
      if ( blockSize_2 > 0 ) {
         // determine position of second sample block
         destStart += blockSize_1;

         // copy second sample block to external buffer
         memcpy( destAudioData + destStart,
                 sourceAudioData + startIndex_2,
                 sizeof( Type ) * blockSize_2 );
      }
   }
}


/// Remove audio samples from this ring buffer without actually
/// copying it to an AudioBuffer.  **The only thing this function does
/// is move the read position.  Used correctly, this will prevent the
/// "overwriting unread data" debug message from appearing.**
///
/// @param numberOfSamples number of samples to copy
///
template <typename Type>
void RingBuffer<Type>::removeToNull(
   const int numberOfSamples )
{
   // simulate read (update read position)
   bufferPosition_.simulateDequeue( numberOfSamples );
}


// explicit instantiation of all template instances
template class RingBuffer<float>;
template class RingBuffer<double>;

} // namespace

