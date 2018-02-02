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


class TruePeakMeter :
    public frut::dsp::RateConverter
{
public:
    TruePeakMeter(const int numberOfChannels,
                  const int originalFftBufferSize,
                  const int upsamplingFactor);

    float getLevel(const int channel);

    void setSamples(const frut::audio::RingBuffer<float> &source,
                    const int preDelay);

    void setSamples(const frut::audio::RingBuffer<double> &source,
                    const int preDelay);

protected:
    void processInput();

    Array<float> truePeakLevels_;
    frut::dsp::Dither dither_;

private:
    JUCE_LEAK_DETECTOR(TruePeakMeter);
};


// Local Variables:
// ispell-local-dictionary: "british"
// End:
