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

#define DEBUG_RELEASE_RATE 0


#include "../JuceLibraryCode/JuceHeader.h"
#include "compressor.h"
#include "gain_stage_fet.h"
#include "gain_stage_optical.h"
#include "plugin_processor_squeezer.h"


//==============================================================================
/**
*/
class SideChain
{
public:
    SideChain(int nSampleRate);
    ~SideChain();

    void reset();

    float getDetectorRmsFilter();
    void setDetectorRmsFilter(float fDetectorRateMilliSecondsNew);

    int getDetector();
    void setDetector(int nDetectorTypeNew);

    int getGainStage();
    void setGainStage(int nGainStageTypeNew);

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

#if DEBUG_RELEASE_RATE
    float fTimePassed;

    float fDebugFinalValue90;
    float fDebugTimeInReleasePhase;
#endif

    GainStage* pGainStageCurrent;
    GainStageFET* pGainStageFET;
    GainStageOptical* pGainStageOptical;

    float fSampleRate;
    float fCrestFactorAutoGain;
    float fGainReduction;
    float fGainReductionIdeal;
    float fGainReductionIntermediate;
    float fGainCompensation;

    float fDetectorCoefficient;
    float fDetectorOutputLevelSquared;

    float fDetectorRateMilliSeconds;
    int nDetectorType;
    int nGainStageType;

    float fThreshold;
    float fRatioInternal;
    float fKneeWidth;
    float fKneeWidthHalf;
    float fKneeWidthDouble;

    int nAttackRate;
    float fAttackCoefficient;

    int nReleaseRate;
    float fReleaseCoefficient;

    float queryGainComputer(float fInputLevel);
    float applyLevelDetectionFilter(float fDetectorInputLevel);
    void applyDetectorLinear(float fGainReductionNew);
    void applyDetectorSmoothDecoupled(float fGainReductionNew);
    void applyDetectorSmoothBranching(float fGainReductionNew);
};


#endif  // __SIDE_CHAIN_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
