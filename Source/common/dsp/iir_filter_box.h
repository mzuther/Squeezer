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

#ifndef __FRUT_DSP_IIR_FILTER_BOX_H__
#define __FRUT_DSP_IIR_FILTER_BOX_H__


class IirFilterBox :
    public BiquadFilter
{
public:
    IirFilterBox(int numberOfChannels, double sampleRate);

    double bandwidthToQualityFactor(double octaveBandwidth);

    void passFilterFirstOrder(
        double cutoffFrequencyInHz,
        bool isLowPass);

    void passFilterSecondOrder(
        double cutoffFrequencyInHz,
        double qualityFactor,
        bool isLowPass);

    void shelvingFilterFirstOrder(
        double cutoffFrequencyInHz,
        double gainInDecibels,
        bool isLowShelving);

    void peakingFilterVariableQ(
        double cutoffFrequencyInHz,
        double gainInDecibels,
        double qualityFactor);

    void peakingFilterConstantQ(
        double cutoffFrequencyInHz,
        double gainInDecibels,
        double qualityFactor);

protected:
    double sampleRate_;

private:
    JUCE_LEAK_DETECTOR(IirFilterBox);
};

#endif  // __FRUT_DSP_IIR_FILTER_BOX_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
