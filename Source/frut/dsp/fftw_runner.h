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

#if FRUT_DSP_USE_FFTW

#ifndef FRUT_DSP_FFTW_RUNNER_H
#define FRUT_DSP_FFTW_RUNNER_H

namespace frut
{
namespace dsp
{

class FftwRunner
{
public:
    FftwRunner(const int numberOfChannels,
               const int fftBufferSize);

    virtual ~FftwRunner();

    virtual void reset();
    void convolveWithKernel(const int channel,
                            const float oversamplingRate = 1.0f);

protected:
    DynamicLibrary dynamicLibraryFFTW;

    float *filterKernel_TD_;
    fftwf_complex *filterKernel_FD_;
    fftwf_plan filterKernelPlan_DFT_;

    float *audioSamples_TD_;
    fftwf_complex *audioSamples_FD_;
    fftwf_plan audioSamplesPlan_DFT_;
    fftwf_plan audioSamplesPlan_IDFT_;

    int numberOfChannels_;
    int fftBufferSize_;

    int fftSize_;
    int halfFftSizePlusOne_;

    AudioBuffer<float> fftSampleBuffer_;
    AudioBuffer<float> fftOverlapAddSamples_;

#if (defined (_WIN32) || defined (_WIN64))
    float *(*fftwf_alloc_real)(size_t);
    fftwf_complex *(*fftwf_alloc_complex)(size_t);
    void (*fftwf_free)(void *);

    fftwf_plan(*fftwf_plan_dft_r2c_1d)(int, float *, fftwf_complex *, unsigned);
    fftwf_plan(*fftwf_plan_dft_c2r_1d)(int, fftwf_complex *, float *, unsigned);
    void (*fftwf_destroy_plan)(fftwf_plan);

    void (*fftwf_execute)(const fftwf_plan);
#endif

private:
    JUCE_LEAK_DETECTOR(FftwRunner);
};

}
}

#endif  // FRUT_DSP_FFTW_RUNNER_H

#endif  // FRUT_DSP_USE_FFTW
