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

#if FRUT_DSP_USE_FFTW

#ifndef FRUT_DSP_FIR_FILTER_BOX_H
#define FRUT_DSP_FIR_FILTER_BOX_H

namespace frut::dsp
{

class FIRFilterBox :
   public FftwRunner
{
public:
   FIRFilterBox( const File resourceDirectory,
                 const int numberOfChannels,
                 const int fftBufferSize );

   virtual ~FIRFilterBox();

   virtual void reset() override;
   void calculateKernelWindowedSincLPF(
      const double relativeCutoffFrequency );

private:
   JUCE_LEAK_DETECTOR( FIRFilterBox );
};

} // namespace

#endif  // FRUT_DSP_FIR_FILTER_BOX_H

#endif  // FRUT_DSP_USE_FFTW
