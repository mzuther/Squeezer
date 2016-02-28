/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013-2016 Martin Zuther (http://www.mzuther.de/)

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

#ifndef __COMPRESSOR_H__
#define __COMPRESSOR_H__

class SideChain;

#include "FrutHeader.h"
#include "filter_chebyshev.h"
#include "side_chain.h"


//============================================================================
class Compressor
{
public:
    //==========================================================================

    enum Parameters  // public namespace!
    {
        DetectorRmsFilterPeak = 0,
        DetectorRmsFilterFast,
        DetectorRmsFilterMedium,
        DetectorRmsFilterSlow,
        NumberOfDetectorRmsFilters,

        DesignFeedForward = 0,
        DesignFeedBack,
        NumberOfDesigns,

        DetectorLinear = 0,
        DetectorSmoothDecoupled,
        DetectorSmoothBranching,
        NumberOfDetectors,

        GainStageFET = 0,
        GainStageOptical,
        NumberOfGainStages,
    };

    Compressor(int channels, int sample_rate);

    void resetMeters();

    bool getBypass();
    void setBypass(bool bBypassCompressorNew);

    double getDetectorRmsFilter();
    void setDetectorRmsFilter(double dDetectorRateMilliSecondsNew);

    int getDesign();
    void setDesign(int nDesignNew);

    double getThreshold();
    void setThreshold(double dThresholdNew);

    double getRatio();
    void setRatio(double dRatioNew);

    double getKneeWidth();
    void setKneeWidth(double dKneeWidthNew);

    int getAttackRate();
    void setAttackRate(int nAttackRateNew);

    int getReleaseRate();
    void setReleaseRate(int nReleaseRateNew);

    int getDetector();
    void setDetector(int nDetectorTypeNew);

    int getGainStage();
    void setGainStage(int nGainStageTypeNew);

    int getStereoLink();
    void setStereoLink(int nStereoLinkNew);

    bool getAutoMakeupGain();
    void setAutoMakeupGain(bool bAutoMakeupGainNew);

    double getMakeupGain();
    void setMakeupGain(double dMakeupGainNew);

    int getWetMix();
    void setWetMix(int nWetMixNew);

    bool getSidechainFilterState();
    void setSidechainFilterState(bool bSidechainFilterStateNew);

    int getSidechainFilterCutoff();
    void setSidechainFilterCutoff(int nSidechainFilterCutoff);

    double getSidechainFilterGain();
    void setSidechainFilterGain(double dSidechainFilterGain);

    bool getSidechainListen();
    void setSidechainListen(bool bSidechainListenNew);

    double getGainReduction(int nChannel);
    double getGainReductionPeak(int nChannel);

    double getPeakMeterInputLevel(int nChannel);
    double getPeakMeterOutputLevel(int nChannel);

    double getPeakMeterPeakInputLevel(int nChannel);
    double getPeakMeterPeakOutputLevel(int nChannel);

    double getMaximumInputLevel(int nChannel);
    double getMaximumOutputLevel(int nChannel);

    double getAverageMeterInputLevel(int nChannel);
    double getAverageMeterOutputLevel(int nChannel);

    void processBlock(AudioSampleBuffer &buffer);

private:
    JUCE_LEAK_DETECTOR(Compressor);

    const float fAntiDenormal;
    const double dAntiDenormal;
    const double dBufferLength;

    void updateMeterBallistics();
    void PeakMeterBallistics(double dPeakLevelCurrent, double &dPeakLevelOld, double &dPeakMarkOld, double &dPeakHoldTime);
    void AverageMeterBallistics(double dAverageLevelCurrent, double &dAverageLevelOld);
    void GainReductionMeterPeakBallistics(double dGainReductionPeakCurrent, double &dGainReductionPeakOld, double &dGainReductionHoldTime);
    void LogMeterBallistics(double dMeterInertia, double dTimePassed, double dLevel, double &dReadout);

    int nChannels;
    int nSampleRate;
    int nMeterBufferPosition;
    int nMeterBufferSize;

    AudioSampleBuffer MeterInputBuffer;
    AudioSampleBuffer MeterOutputBuffer;

    frut::audio::Dither dither;
    OwnedArray<SideChain> p_arrSideChain;
    OwnedArray<FilterChebyshev> p_arrSidechainFilter;

    Array<double> arrInputSamples;
    Array<double> arrSidechainSamples;
    Array<double> arrOutputSamples;

    Array<double> arrPeakMeterInputLevels;
    Array<double> arrPeakMeterOutputLevels;

    Array<double> arrPeakMeterPeakInputLevels;
    Array<double> arrPeakMeterPeakOutputLevels;

    Array<double> arrMaximumInputLevels;
    Array<double> arrMaximumOutputLevels;

    Array<double> arrAverageMeterInputLevels;
    Array<double> arrAverageMeterOutputLevels;

    Array<double> arrGainReduction;
    Array<double> arrGainReductionPeak;

    Array<double> arrPeakMeterPeakInputHoldTime;
    Array<double> arrPeakMeterPeakOutputHoldTime;
    Array<double> arrGainReductionHoldTime;

    double dCrestFactor;
    int nDesign;

    double dReleaseCoefLinear;

    bool bBypassCompressor;
    bool bBypassCompressorCombined;
    bool bDesignFeedForward;
    bool bUpwardExpansion;

    int nStereoLink;
    double dStereoLinkOriginal;
    double dStereoLinkOther;

    bool bAutoMakeupGain;
    double dMakeupGain;
    double dMakeupGainDecibel;

    int nWetMix;
    double dWetMix;
    double dDryMix;

    bool bSidechainFilterState;
    int nSidechainFilterCutoff;
    double dSidechainFilterGain;
    double dGainNormalise;
    bool bSidechainListen;
};

#endif  // __COMPRESSOR_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
