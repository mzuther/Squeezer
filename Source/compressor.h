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
class SideChain;

#include "../JuceLibraryCode/JuceHeader.h"
#include "side_chain.h"


//============================================================================
class Compressor
{
public:
    //==========================================================================

    enum Parameters  // public namespace!
    {
        LevelDetectionPeak = 0,
        LevelDetectionRMS,
        NumberOfLevelDetections,

        DesignFeedForward = 0,
        DesignFeedBack,
        NumberOfDesigns,

        DetectorLinear = 0,
        DetectorSmoothBranching,
        DetectorSmoothDecoupled,
        NumberOfDetectors,
    };

    Compressor(int channels, int sample_rate);
    ~Compressor();

    bool getBypass();
    void setBypass(bool bBypassCompressorNew);

    int getLevelDetection();
    void setLevelDetection(int nLevelDetectionTypeNew);

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

    int getStereoLink();
    void setStereoLink(int nStereoLinkNew);

    bool getAutoMakeupGain();
    void setAutoMakeupGain(bool bAutoMakeupGainNew);

    float getMakeupGain();
    void setMakeupGain(float fMakeupGainNew);

    int getWetMix();
    void setWetMix(int nWetMixNew);

    float getGainReduction(int nChannel);

    void processBlock(AudioSampleBuffer& buffer);

    juce_UseDebuggingNewOperator
private:
    JUCE_LEAK_DETECTOR(Compressor);

    SideChain** pSideChain;

    float* pInputSamples;
    float* pOutputSamples;

    int nChannels;
    float fCrestFactor;
    int nDesign;

    bool bBypassCompressor;
    bool bBypassCompressorCombined;
    bool bDesignFeedForward;

    int nStereoLink;
    float fStereoLinkOriginal;
    float fStereoLinkOther;

    bool bAutoMakeupGain;
    float fMakeupGain;
    float fMakeupGainDecibel;

    int nWetMix;
    float fWetMix;
    float fDryMix;
};

#endif  // __COMPRESSOR_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
