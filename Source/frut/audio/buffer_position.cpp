/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2021 Martin Zuther (http://www.mzuther.de/)

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

/// Create helper class instance.
///
/// @param numberOfSamples number of audio samples per channel
///
/// @param preDelay number of samples the buffer output will be
///        delayed
///
BufferPosition::BufferPosition(
   const int numberOfSamples,
   const int preDelay )
{
   jassert( numberOfSamples > 0 );
   jassert( preDelay >= 0 );

   // initialise pre-delay
   preDelay_ = preDelay;

   // initialise number of samples necessary for buffer (buffer
   // length + pre-delay)
   totalBufferLength_ = numberOfSamples + preDelay_;

   // reset buffer positions
   reset();
}


/// Reset buffer positions.
///
void BufferPosition::reset()
{
   // initialise read position
   readPosition_ = 0;

   // initialise remaining number of samples from read position to
   // end of buffer
   readPositionToWrap_ = totalBufferLength_ - readPosition_;

   // initialise write position (this automatically applies
   // pre-delay!)
   writePosition_ = preDelay_;

   // initialise remaining number of samples from write position to
   // end of buffer
   writePositionToWrap_ = totalBufferLength_ - writePosition_;

   // initialise number of samples stored in buffer
   storedSamples_ = 0;
}


/// Get buffer length (excluding pre-delay).
///
/// @return number of audio samples
///
int BufferPosition::getNumberOfSamples() const
{
   return totalBufferLength_ - preDelay_;
}


/// Get total buffer length (including pre-delay).
///
/// @return number of audio samples
///
int BufferPosition::getTotalBufferLength() const
{
   return totalBufferLength_;
}


/// Get number of samples the buffer output will be delayed.
///
/// @return pre-delay in samples
///
int BufferPosition::getPreDelay() const
{
   return preDelay_;
}


/// Get current read position.
///
/// @return read position in samples
///
int BufferPosition::getCurrentReadPosition() const
{
   return readPosition_;
}


/// Get current write position.
///
/// @return write position in samples
///
int BufferPosition::getCurrentWritePosition() const
{
   return writePosition_;
}

/// Get positions of two sample blocks for storing a specified number
/// of samples.
///
/// @param numberOfSamples number of samples to store in the buffer
///
/// @param startIndex_1 on exit, this will contain the index position
///        where the first block of data should be written
///
/// @param blockSize_1 on exit, this indicates how many samples should
///        be written to the first block
///
/// @param startIndex_2 on exit, this will contain the index position
///        where data that didn't fit into the first block should be
///        written
///
/// @param blockSize_2 on exit, this indicates how many samples (if
///        any) should be written to the second block
///
/// @param updatePosition when true, the buffer's write position will
///        be updated
///
void BufferPosition::store(
   const int numberOfSamples,
   int& startIndex_1,
   int& blockSize_1,
   int& startIndex_2,
   int& blockSize_2,
   const bool updatePosition )
{
   jassert( isPositiveAndNotGreaterThan( numberOfSamples,
                                         totalBufferLength_ ) );

   // calculate position and length of first block from write
   // position
   startIndex_1 = writePosition_;
   blockSize_1 = jmin( numberOfSamples, writePositionToWrap_ );

   // calculate position and length of second block (data that didn't
   // fit into the first block)
   startIndex_2 = 0;
   blockSize_2 = numberOfSamples - blockSize_1;

   // update write position
   if ( updatePosition ) {
      // update write position and wrap around at end of buffer
      writePosition_ = negativeAwareModulo(
                          writePosition_ + numberOfSamples,
                          totalBufferLength_ );

      // update remaining number of samples to end of buffer
      writePositionToWrap_ = totalBufferLength_ - writePosition_;

      // increase number of samples in buffer
      storedSamples_ += numberOfSamples;

      // check for data corruption
      if ( storedSamples_ > totalBufferLength_ ) {
         DBG( "[BufferPosition] overwriting unread data!" );
      }
   }
}


/// Get positions of two sample blocks for retrieving a specified
/// number of samples.
///
/// @param numberOfSamples number of samples to retrieve from the
///        buffer
///
/// @param startIndex_1 on exit, this will contain the index position
///        from where the first block of data should be retrieved
///
/// @param blockSize_1 on exit, this indicates how many samples should
///        be retrieved from the first block
///
/// @param startIndex_2 on exit, this will contain the index position
///        from where data that didn't fit into the first block should
///        be retrieved
///
/// @param blockSize_2 on exit, this indicates how many samples (if
///        any) should be retrieved from the second block
///
/// @param updatePosition when true, the buffer's read position will
///        be updated
///
void BufferPosition::retrieve(
   const int numberOfSamples,
   int& startIndex_1,
   int& blockSize_1,
   int& startIndex_2,
   int& blockSize_2,
   const bool updatePosition )
{
   jassert( isPositiveAndNotGreaterThan( numberOfSamples,
                                         totalBufferLength_ ) );

   // calculate position and length of first block from read position
   startIndex_1 = readPosition_;
   blockSize_1 = jmin( numberOfSamples, readPositionToWrap_ );

   // calculate position and length of second block (data that didn't
   // fit into the first block)
   startIndex_2 = 0;
   blockSize_2 = numberOfSamples - blockSize_1;

   // update write position
   if ( updatePosition ) {
      // update read position and wrap around at end of buffer
      readPosition_ = negativeAwareModulo(
                         readPosition_ + numberOfSamples,
                         totalBufferLength_ );

      // update remaining number of samples to end of buffer
      readPositionToWrap_ = totalBufferLength_ - readPosition_;

      // decrease number of samples in buffer
      storedSamples_ -= numberOfSamples;

      // check for data corruption
      if ( storedSamples_ < 0 ) {
         DBG( "[BufferPosition] reading undefined data!" );
      }
   }
}


/// Simulate dequeue.  **The only thing this function does is move the
/// read position.  Used correctly, this will prevent the "overwriting
/// unread data" debug message from appearing.**
///
/// @param numberOfSamples number of samples to retrieve from the
///        buffer
///
void BufferPosition::simulateDequeue(
   const int numberOfSamples )
{
   // update read position and wrap around at end of buffer
   readPosition_ = negativeAwareModulo(
                      readPosition_ + numberOfSamples,
                      totalBufferLength_ );

   // update remaining number of samples to end of buffer
   readPositionToWrap_ = totalBufferLength_ - readPosition_;

   // decrease number of samples in buffer
   storedSamples_ -= numberOfSamples;

   // check for data corruption
   if ( storedSamples_ < 0 ) {
      DBG( "[BufferPosition] reading undefined data!" );
   }
}


/// Get positions of two sample blocks for storing or retrieving a
/// specified number of samples **looking backwards from the current
/// write position.  The write and read positions will be left
/// alone.**
///
/// @param numberOfSamples number of samples to store to or retrieve
///        from the buffer
///
/// @param startIndex_1 on exit, this will contain the index position
///        of the first block of data
///
/// @param blockSize_1 on exit, this indicates how many samples should
///        be stored to or retrieved from the first block
///
/// @param startIndex_2 on exit, this will contain the index position
///        for data that didn't fit into the first block
///
/// @param blockSize_2 on exit, this indicates how many samples (if
///        any) should be stored to or retrieved from the second block
///
void BufferPosition::lookBackFromWritePosition(
   const int numberOfSamples,
   int& startIndex_1,
   int& blockSize_1,
   int& startIndex_2,
   int& blockSize_2 )
{
   jassert( isPositiveAndNotGreaterThan( numberOfSamples,
                                         totalBufferLength_ ) );

   // calculate beginning of data based on the **write** position and
   // wrap around at start of buffer
   int writePositionTemp = negativeAwareModulo(
                              writePosition_ - numberOfSamples,
                              totalBufferLength_ );

   // calculate remaining number of samples from **write** position
   // to end of buffer
   int writePositionTempToWrap = totalBufferLength_ - writePositionTemp;

   // calculate position and length of first block from **write**
   // position
   startIndex_1 = writePositionTemp;
   blockSize_1 = jmin( numberOfSamples, writePositionTempToWrap );

   // calculate position and length of second block (data that didn't
   // fit into the first block)
   startIndex_2 = 0;
   blockSize_2 = numberOfSamples - blockSize_1;
}

} // namespace
