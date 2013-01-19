/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose compressor with a touch of lemon

   Copyright (c) 2013 Martin Zuther (http://www.mzuther.de/)

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

#ifndef __GAIN_REDUCER_H__
#define __GAIN_REDUCER_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "plugin_processor_squeezer.h"


//==============================================================================
/**
*/
class GainReducer
{
public:
    GainReducer(int nSampleRate);
    ~GainReducer();

    void setSampleRate(int nSampleRate);
    void reset();

    int getDesign();
    void setDesign(int nDesignNew);

    bool getLogarithmic();
    void setLogarithmic(bool bLogarithmicNew);

    float getThreshold();
    void setThreshold(float fThresholdNew);

    float getRatio();
    void setRatio(float fRatioNew);

    int getAttackRate();
    void setAttackRate(int nAttackRateNew);

    int getReleaseRate();
    void setReleaseRate(int nReleaseRateNew);

    float getGainReduction(bool useGainCompensation);

    void processSample(float fSampleValue);

    static float level2decibel(float fLevel);
    static float decibel2level(float fDecibels);
private:
    JUCE_LEAK_DETECTOR(GainReducer);

    float fTimePassed;
    float fCrestFactorAutoGain;
    float fGainReduction;
    float fGainCompensation;

    int nDesign;
    bool bLogarithmicEnvelope;

    float fThreshold;
    float fRatio;

    int nAttackRate;
    float fAttackCoefficient;

    int nReleaseRate;
    float fReleaseCoefficient;

    float calculateFinalGainReduction(float fInputLevel);
    void applyLinearEnvelope(float fGainReductionFinal);
    void applyLogarithmicEnvelope(float fGainReductionFinal);

    static float fMeterMinimumDecibel;
};


#endif  // __GAIN_REDUCER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
