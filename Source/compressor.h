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
class GainReducer;

#include "../JuceLibraryCode/JuceHeader.h"
#include "gain_reducer.h"


//============================================================================
class Compressor
{
public:
    //==========================================================================

    Compressor(int channels, int sample_rate);
    ~Compressor();

    bool getBypass();
    void setBypass(bool bBypassCompressorNew);

    int getDesign();
    void setDesign(int nDesignNew);

    float getThreshold();
    void setThreshold(float fThresholdNew);

    float getRatio();
    void setRatio(float fRatioNew);

    int getAttackRate();
    void setAttackRate(int nAttackRateNew);

    int getReleaseRate();
    void setReleaseRate(int nReleaseRateNew);

    bool getLogarithmicAttack();
    void setLogarithmicAttack(bool bLogarithmicAttackNew);

    bool getLogarithmicRelease();
    void setLogarithmicRelease(bool bLogarithmicReleaseNew);

    int getStereoLink();
    void setStereoLink(int nStereoLinkNew);

    float getInputGain();
    void setInputGain(float fInputGainNew);

    float getOutputGain();
    void setOutputGain(float fOutputGainNew);

    int getWetMix();
    void setWetMix(int nWetMixNew);

    float getGainReduction(int nChannel);

    void processBlock(AudioSampleBuffer& buffer);

    juce_UseDebuggingNewOperator
private:
    JUCE_LEAK_DETECTOR(Compressor);

    GainReducer** pGainReducer;

    float* pInputSamples;
    float* pOutputSamples;

    int nChannels;
    float fCrestFactor;

    bool bBypassCompressor;
    bool bBypassCompressorCombined;
    bool bDesignModern;

    int nStereoLink;
    float fStereoLinkOriginal;
    float fStereoLinkOther;

    float fInputGain;
    float fInputGainDecibel;

    float fOutputGain;
    float fOutputGainDecibel;

    int nWetMix;
    float fWetMix;
    float fDryMix;
};

#endif  // __COMPRESSOR_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
