/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

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

#ifndef __SIDE_CHAIN_H__
#define __SIDE_CHAIN_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "compressor.h"
#include "plugin_processor_squeezer.h"


//==============================================================================
/**
*/
class SideChain
{
public:
    SideChain(int nSampleRate);
    ~SideChain();

    void setSampleRate(int nSampleRate);
    void reset();

    float getLevelDetectionRate();
    void setLevelDetectionRate(float fDetectorRateMilliSecondsNew);

    int getDetector();
    void setDetector(int nDetectorTypeNew);

    float getThreshold();
    void setThreshold(float fThresholdNew);

    float getRatio();
    void setRatio(float fRatioNew);

    float getKneeWidth();
    void setKneeWidth(float fKneeWidthNew);

    int getAttackRate();
    void setAttackRate(int nAttackRateNew);

    int getReleaseRate();
    void setReleaseRate(int nReleaseRateNew);

    float getGainReduction(bool bAutoMakeupGain);

    void processSample(float fSampleValue);

    static float level2decibel(float fLevel);
    static float decibel2level(float fDecibels);
private:
    JUCE_LEAK_DETECTOR(SideChain);

    static const int nNumberOfFactors = 60;

    float fSampleRate;
    float fCrestFactorAutoGain;
    float fGainReduction;
    float fGainReductionIntermediate;
    float fGainCompensation;

    float fDetectorCoefficient;
    float fDetectorOutputLevelSquared;

    float fDetectorRateMilliSeconds;
    int nDetectorType;
    float fThreshold;
    float fRatioInternal;
    float fKneeWidth;
    float fKneeWidthHalf;
    float fKneeWidthDouble;

    int nAttackRate;
    float fAttackCoefficient;

    int nReleaseRate;
    float fReleaseCoefficient;
    float* fReleaseCoefficientsOptical;

    float fOpticalDivisorA;
    float fOpticalDivisorB;

    float queryGainComputer(float fInputLevel);
    float applyLevelDetectionFilter(float fDetectorInputLevel);
    void applyDetectorLinear(float fGainReductionNew);
    void applyDetectorSmoothDecoupled(float fGainReductionNew);
    void applyDetectorSmoothBranching(float fGainReductionNew);
    void applyDetectorOptical(float fGainReductionNew);

    static float fMeterMinimumDecibel;
};


#endif  // __SIDE_CHAIN_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
