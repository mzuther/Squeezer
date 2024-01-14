/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2024 Martin Zuther (http://www.mzuther.de/)

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

#if FRUT_DSP_USE_FFTW

namespace frut::dsp
{

TruePeakMeter::TruePeakMeter(
   const File resourceDirectory,
   const int numberOfChannels,
   const int originalFftBufferSize,
   const int upsamplingFactor ) :

   frut::dsp::RateConverter( resourceDirectory,
                             numberOfChannels,
                             originalFftBufferSize,
                             upsamplingFactor )
{
}


TruePeakMeter::~TruePeakMeter()
{
}


void TruePeakMeter::reset()
{
   RateConverter::reset();

   truePeakLevels_.clear();
}


float TruePeakMeter::getLevel(
   const int channel )
{
   jassert( isPositiveAndBelow( channel, numberOfChannels_ ) );

   return truePeakLevels_[channel];
}


void TruePeakMeter::copyFrom(
   const AudioBuffer<float>& source,
   const int numberOfSamples )
{
   jassert( source.getNumChannels() ==
            numberOfChannels_ );
   jassert( source.getNumSamples() >=
            numberOfSamples );
   jassert( originalFftBufferSize_ ==
            numberOfSamples );

   // copy data from external buffer
   for ( int channel = 0; channel < numberOfChannels_; ++channel ) {
      sampleBufferOriginal_.copyFrom( channel, 0,
                                      source,
                                      channel, 0,
                                      numberOfSamples );
   }

   // process input data
   processInput();
}


void TruePeakMeter::processInput()
{
   // upsample buffer (overwrites contents of sample buffer)
   upsample();

   // evaluate true peak level
   for ( int channel = 0; channel < numberOfChannels_; ++channel ) {
      float truePeakLevel = fftSampleBuffer_.getMagnitude(
                               channel, 0, fftBufferSize_ );

      truePeakLevels_.set( channel, truePeakLevel );
   }
}

} // namespace

#endif  // FRUT_DSP_USE_FFTW
