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

#ifndef FRUT_DSP_IIR_FILTER_BOX_H
#define FRUT_DSP_IIR_FILTER_BOX_H

namespace frut
{
namespace dsp
{

class IirFilterBox :
    public BiquadFilter
{
public:
    IirFilterBox(const int numberOfChannels, const double sampleRate);

    double bandwidthToQualityFactor(const double octaveBandwidth);

    void passFilterFirstOrder(
        const double cutoffFrequencyInHz,
        const bool isLowPass);

    void passFilterSecondOrder(
        const double cutoffFrequencyInHz,
        const double qualityFactor,
        const bool isLowPass);

    void shelvingFilterFirstOrder(
        const double cutoffFrequencyInHz,
        const double gainInDecibels,
        const bool isLowShelving);

    void peakingFilterVariableQ(
        const double cutoffFrequencyInHz,
        const double gainInDecibels,
        const double qualityFactor);

    void peakingFilterConstantQ(
        const double cutoffFrequencyInHz,
        const double gainInDecibels,
        const double qualityFactor);

protected:
    double sampleRate_;

private:
    JUCE_LEAK_DETECTOR(IirFilterBox);
};

}
}

#endif  // FRUT_DSP_IIR_FILTER_BOX_H
