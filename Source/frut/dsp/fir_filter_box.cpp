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

#if FRUT_DSP_USE_FFTW

namespace frut
{
namespace dsp
{

FIRFilterBox::FIRFilterBox(
   const File resourceDirectory,
   const int numberOfChannels,
   const int fftBufferSize ) :

   frut::dsp::FftwRunner( resourceDirectory, numberOfChannels, fftBufferSize )
{
}


FIRFilterBox::~FIRFilterBox()
{
}


void FIRFilterBox::reset()
{
   FftwRunner::reset();
}


// calculate filter kernel for windowed-sinc low-pass filter
void FIRFilterBox::calculateKernelWindowedSincLPF(
   const double relativeCutoffFrequency )
{
   int samples = fftBufferSize_ + 1;
   double samplesHalf = samples / 2.0;

   // calculate filter kernel
   for ( int i = 0; i < samples; ++i ) {
      if ( i == samplesHalf ) {
         filterKernel_TD_[i] = static_cast<float>(
                                  2.0 * M_PI * relativeCutoffFrequency );
      } else {
         filterKernel_TD_[i] = static_cast<float>(
                                  sin( 2.0 * M_PI * relativeCutoffFrequency * ( i - samplesHalf ) ) / ( i - samplesHalf ) * ( 0.42 - 0.5 * cos( 2.0 * M_PI * i / samples ) + 0.08 * cos( 4.0 * M_PI * i / samples ) ) );
      }
   }

   // normalise filter kernel for unity gain at DC
   double kernelSum = 0.0;

   for ( int i = 0; i < samples; ++i ) {
      kernelSum += filterKernel_TD_[i];
   }

   for ( int i = 0; i < samples; ++i ) {
      filterKernel_TD_[i] = static_cast<float>(
                               filterKernel_TD_[i] / kernelSum );
   }

   // pad filter kernel with zeros
   for ( int i = samples; i < fftSize_; ++i ) {
      filterKernel_TD_[i] = 0.0f;
   }

   // calculate DFT of filter kernel
   fftwf_execute( filterKernelPlan_DFT_ );
}

}
}

#endif  // FRUT_DSP_USE_FFTW
