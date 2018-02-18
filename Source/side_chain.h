/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013-2018 Martin Zuther (http://www.mzuther.de/)

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

#ifndef SQUEEZER_SIDE_CHAIN_H
#define SQUEEZER_SIDE_CHAIN_H

#define DEBUG_RELEASE_RATE 0


#include "FrutHeader.h"
#include "gain_stage_fet.h"
#include "gain_stage_optical.h"


class SideChain
{
public:
    enum Parameters  // public namespace!
    {
        DetectorLinear = 0,
        DetectorSmoothDecoupled,
        DetectorSmoothBranching,
        NumberOfDetectors,
    };

    SideChain(int nSampleRate);

    void reset();

    double getDetectorRmsFilter();
    void setDetectorRmsFilter(double dDetectorRateMilliSecondsNew);

    int getDetector();
    void setDetector(int nDetectorTypeNew);

    int getGainStage();
    void setGainStage(int nGainStageTypeNew);

    double getThreshold();
    void setThreshold(double dThresholdNew);

    double getRatio();
    void setRatio(double dRatioNew);

    double getKneeWidth();
    void setKneeWidth(double dKneeWidthNew);

    double getAttackRate();
    void setAttackRate(double dAttackRateNew);

    int getReleaseRate();
    void setReleaseRate(int nReleaseRateNew);

    double getGainReduction(bool bAutoMakeupGain);

    void processSample(double dSampleValue);

    static double level2decibel(double dLevel);
    static double decibel2level(double dDecibels);
private:
    JUCE_LEAK_DETECTOR(SideChain);

#if DEBUG_RELEASE_RATE
    double dTimePassed;

    double dDebugFinalValue90;
    double dDebugTimeInReleasePhase;
#endif

    GainStageFET gainStageFET;
    GainStageOptical gainStageOptical;

    double dSampleRate;
    double dCrestFactorAutoGain;
    double dGainReduction;
    double dGainReductionIdeal;
    double dGainReductionIntermediate;
    double dGainCompensation;

    double dDetectorCoefficient;
    double dDetectorOutputLevelSquared;

    double dDetectorRateMilliSeconds;
    int nDetectorType;
    int nGainStageType;

    double dThreshold;
    double dRatioInternal;
    double dKneeWidth;
    double dKneeWidthHalf;
    double dKneeWidthDouble;

    double dAttackRate;
    double dAttackCoefficient;

    int nReleaseRate;
    double dReleaseCoefficient;

    double queryGainComputer(double dInputLevel);
    double applyLevelDetectionFilter(double dDetectorInputLevel);
    void applyDetectorLinear(double dGainReductionNew);
    void applyDetectorSmoothDecoupled(double dGainReductionNew);
    void applyDetectorSmoothBranching(double dGainReductionNew);
};

#endif  // SQUEEZER_SIDE_CHAIN_H
