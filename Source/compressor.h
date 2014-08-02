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

#ifndef __COMPRESSOR_H__
#define __COMPRESSOR_H__

class Compressor;
class FilterChebyshev;
class SideChain;

#include "../JuceLibraryCode/JuceHeader.h"
#include "filter_chebyshev.h"
#include "side_chain.h"


static const float fAntiDenormal = 1e-20f;

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
    ~Compressor();

    void resetMeters();

    bool getBypass();
    void setBypass(bool bBypassCompressorNew);

    float getDetectorRmsFilter();
    void setDetectorRmsFilter(float fDetectorRateMilliSecondsNew);

    int getDesign();
    void setDesign(int nDesignNew);

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

    int getDetector();
    void setDetector(int nDetectorTypeNew);

    int getGainStage();
    void setGainStage(int nGainStageTypeNew);

    int getStereoLink();
    void setStereoLink(int nStereoLinkNew);

    bool getAutoMakeupGain();
    void setAutoMakeupGain(bool bAutoMakeupGainNew);

    float getMakeupGain();
    void setMakeupGain(float fMakeupGainNew);

    int getWetMix();
    void setWetMix(int nWetMixNew);

    bool getSidechainFilterState();
    void setSidechainFilterState(bool bSidechainFilterStateNew);

    int getSidechainFilterCutoff();
    void setSidechainFilterCutoff(int nSidechainFilterCutoff);

    float getSidechainFilterGain();
    void setSidechainFilterGain(float fSidechainFilterGain);

    bool getSidechainListen();
    void setSidechainListen(bool bSidechainListenNew);

    float getGainReduction(int nChannel);
    float getGainReductionPeak(int nChannel);

    float getPeakMeterInputLevel(int nChannel);
    float getPeakMeterOutputLevel(int nChannel);

    float getPeakMeterPeakInputLevel(int nChannel);
    float getPeakMeterPeakOutputLevel(int nChannel);

    float getMaximumInputLevel(int nChannel);
    float getMaximumOutputLevel(int nChannel);

    float getAverageMeterInputLevel(int nChannel);
    float getAverageMeterOutputLevel(int nChannel);

    void processBlock(AudioSampleBuffer& buffer);

    juce_UseDebuggingNewOperator
private:
    JUCE_LEAK_DETECTOR(Compressor);

    void updateMeterBallistics();
    void PeakMeterBallistics(float fPeakLevelCurrent, float& fPeakLevelOld, float& fPeakMarkOld, float& fPeakHoldTime);
    void AverageMeterBallistics(float fAverageLevelCurrent, float& fAverageLevelOld);
    void GainReductionMeterPeakBallistics(float fGainReductionPeakCurrent, float& fGainReductionPeakOld, float& fGainReductionHoldTime);
    void LogMeterBallistics(float fMeterInertia, float fTimePassed, float fLevel, float& fReadout);

    SideChain** pSideChain;
    FilterChebyshev** pSidechainFilter;

    AudioSampleBuffer* pMeterInputBuffer;
    AudioSampleBuffer* pMeterOutputBuffer;

    float* pInputSamples;
    float* pSidechainSamples;
    float* pOutputSamples;

    float* pPeakMeterInputLevels;
    float* pPeakMeterOutputLevels;

    float* pPeakMeterPeakInputLevels;
    float* pPeakMeterPeakOutputLevels;

    float* pMaximumInputLevels;
    float* pMaximumOutputLevels;

    float* pAverageMeterInputLevels;
    float* pAverageMeterOutputLevels;

    float* pGainReduction;
    float* pGainReductionPeak;

    float* pPeakMeterPeakInputHoldTime;
    float* pPeakMeterPeakOutputHoldTime;
    float* pGainReductionHoldTime;

    int nChannels;
    int nSampleRate;
    int nMeterBufferSize;
    int nMeterBufferPosition;
    float fCrestFactor;
    int nDesign;

    float fTimePassed;
    float fReleaseCoefLinear;

    bool bBypassCompressor;
    bool bBypassCompressorCombined;
    bool bDesignFeedForward;
    bool bUpwardExpansion;

    int nStereoLink;
    float fStereoLinkOriginal;
    float fStereoLinkOther;

    bool bAutoMakeupGain;
    float fMakeupGain;
    float fMakeupGainDecibel;

    int nWetMix;
    float fWetMix;
    float fDryMix;

    bool bSidechainFilterState;
    int nSidechainFilterCutoff;
    float fSidechainFilterGain;
    double dGainNormalise;
    bool bSidechainListen;
};

#endif  // __COMPRESSOR_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
