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

#ifndef FRUT_DSP_TRUE_PEAK_METER_H
#define FRUT_DSP_TRUE_PEAK_METER_H

namespace frut
{
namespace dsp
{

class TruePeakMeter :
    public frut::dsp::RateConverter
{
public:
    TruePeakMeter(const int numberOfChannels,
                  const int originalFftBufferSize,
                  const int upsamplingFactor);

    float getLevel(const int channel);

    void copyFrom(const AudioBuffer<float> &source,
                  const int numberOfSamples);

protected:
    void processInput();

    Array<float> truePeakLevels_;

private:
    JUCE_LEAK_DETECTOR(TruePeakMeter);
};

}
}

#endif  // FRUT_DSP_TRUE_PEAK_METER_H

#endif  // FRUT_DSP_USE_FFTW
