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

#pragma once


class BiquadFilter
{
public:
    BiquadFilter(const int numberOfChannels);
    ~BiquadFilter();

    void neutral();
    void resetDelays();

    void processSample(double &sampleValue, const int channel);
    void processInPlace(AudioBuffer<double> &buffer);
    AudioBuffer<double> process(const AudioBuffer<double> &inputBuffer);

    void setCoefficients(const double a0, const double a1, const double a2,
                         const double b0, const double b1,
                         const double c0 = 1.0, const double d0 = 0.0,
                         const bool showCoefficients = false);

protected:
    void processSampleInternal(double &sampleValue,
                               double &x0, double &x1, double &x2,
                               double &y1, double &y2);

private:
    int numberOfChannels_;

    double a0_;
    double a1_;
    double a2_;

    double b1_;
    double b2_;

    // necessary for some audio filters
    double c0_;
    double d0_;

    Array<double> x0_;
    Array<double> x1_;
    Array<double> x2_;

    Array<double> y1_;
    Array<double> y2_;

    JUCE_LEAK_DETECTOR(BiquadFilter);
};

// Local Variables:
// ispell-local-dictionary: "british"
// End:
