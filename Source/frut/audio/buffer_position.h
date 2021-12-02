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

#ifndef FRUT_AUDIO_BUFFER_POSITION_H
#define FRUT_AUDIO_BUFFER_POSITION_H

namespace frut::audio
{

/// Helper class for audio RingBuffer.  This class keeps track of the
/// current read and write positions and also delays the input by a
/// defined pre-delay.
///
class BufferPosition
{
public:
   BufferPosition( const int numberOfSamples,
                   const int preDelay );

   void reset();

   int getNumberOfSamples() const;
   int getTotalBufferLength() const;
   int getPreDelay() const;

   int getCurrentReadPosition() const;
   int getCurrentWritePosition() const;


   /// Get positions of two sample blocks for storing a specified
   /// number of samples.  **The write position will be updated.**
   ///
   /// @param numberOfSamples number of samples to store in the
   ///        buffer
   ///
   /// @param startIndex_1 on exit, this will contain the index
   ///        position where the first block of data should be
   ///        written
   ///
   /// @param blockSize_1 on exit, this indicates how many samples
   ///        should be written to the first block
   ///
   /// @param startIndex_2 on exit, this will contain the index
   ///        position where data that didn't fit into the first
   ///        block should be written
   ///
   /// @param blockSize_2 on exit, this indicates how many samples
   ///        (if any) should be written to the second block
   ///
   inline void queue(
      const int numberOfSamples,
      int& startIndex_1,
      int& blockSize_1,
      int& startIndex_2,
      int& blockSize_2 )
   {
      bool updatePosition = true;

      store( numberOfSamples,
             startIndex_1, blockSize_1,
             startIndex_2, blockSize_2,
             updatePosition );
   }


   /// Get positions of two sample blocks for storing a specified
   /// number of samples.  **The write position will be left alone.**
   ///
   /// @param numberOfSamples number of samples to store in the
   ///        buffer
   ///
   /// @param startIndex_1 on exit, this will contain the index
   ///        position where the first block of data should be
   ///        written
   ///
   /// @param blockSize_1 on exit, this indicates how many samples
   ///        should be written to the first block
   ///
   /// @param startIndex_2 on exit, this will contain the index
   ///        position where data that didn't fit into the first
   ///        block should be written
   ///
   /// @param blockSize_2 on exit, this indicates how many samples
   ///        (if any) should be written to the second block
   ///
   inline void overwrite(
      const int numberOfSamples,
      int& startIndex_1,
      int& blockSize_1,
      int& startIndex_2,
      int& blockSize_2 )
   {
      bool updatePosition = false;

      store( numberOfSamples,
             startIndex_1, blockSize_1,
             startIndex_2, blockSize_2,
             updatePosition );
   }


   /// Get positions of two sample blocks for retrieving a specified
   /// number of samples.  **The read position will be updated.**
   ///
   /// @param numberOfSamples number of samples to retrieve from the
   ///        buffer
   ///
   /// @param startIndex_1 on exit, this will contain the index
   ///        position from where the first block of data should be
   ///        retrieved
   ///
   /// @param blockSize_1 on exit, this indicates how many samples
   ///        should be retrieved from the first block
   ///
   /// @param startIndex_2 on exit, this will contain the index
   ///        position from where data that didn't fit into the first
   ///        block should be retrieved
   ///
   /// @param blockSize_2 on exit, this indicates how many samples
   ///        (if any) should be retrieved from the second block
   ///
   inline void dequeue(
      const int numberOfSamples,
      int& startIndex_1,
      int& blockSize_1,
      int& startIndex_2,
      int& blockSize_2 )
   {
      bool updatePosition = true;

      retrieve( numberOfSamples,
                startIndex_1, blockSize_1,
                startIndex_2, blockSize_2,
                updatePosition );
   }


   /// Get positions of two sample blocks for retrieving a specified
   /// number of samples.  **The read position will be left alone.**
   ///
   /// @param numberOfSamples number of samples to retrieve from the
   ///        buffer
   ///
   /// @param startIndex_1 on exit, this will contain the index
   ///        position from where the first block of data should be
   ///        retrieved
   ///
   /// @param blockSize_1 on exit, this indicates how many samples
   ///        should be retrieved from the first block
   ///
   /// @param startIndex_2 on exit, this will contain the index
   ///        position from where data that didn't fit into the first
   ///        block should be retrieved
   ///
   /// @param blockSize_2 on exit, this indicates how many samples
   ///        (if any) should be retrieved from the second block
   ///
   inline void copy(
      const int numberOfSamples,
      int& startIndex_1,
      int& blockSize_1,
      int& startIndex_2,
      int& blockSize_2 )
   {
      bool updatePosition = false;

      retrieve( numberOfSamples,
                startIndex_1, blockSize_1,
                startIndex_2, blockSize_2,
                updatePosition );
   }


   void simulateDequeue( const int numberOfSamples );

   void lookBackFromWritePosition( const int numberOfSamples,
                                   int& startIndex_1,
                                   int& blockSize_1,
                                   int& startIndex_2,
                                   int& blockSize_2 );

protected:
   void store( const int numberOfSamples,
               int& startIndex_1,
               int& blockSize_1,
               int& startIndex_2,
               int& blockSize_2,
               const bool updatePosition );

   void retrieve( const int numberOfSamples,
                  int& startIndex_1,
                  int& blockSize_1,
                  int& startIndex_2,
                  int& blockSize_2,
                  const bool updatePosition );

   int totalBufferLength_;
   int preDelay_;

   int readPosition_;
   int writePosition_;

   int readPositionToWrap_;
   int writePositionToWrap_;

   int storedSamples_;

private:
   JUCE_LEAK_DETECTOR( BufferPosition );
};

} // namespace

#endif  // FRUT_AUDIO_BUFFER_POSITION_H
