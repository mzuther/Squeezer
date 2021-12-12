/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of citrus

   Copyright (c) 2013-2021 Martin Zuther (http://www.mzuther.de/)

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

#ifndef SQUEEZER_COMPRESSOR_H
#define SQUEEZER_COMPRESSOR_H

#include "FrutHeader.h"
#include "side_chain.h"


class Compressor
{
public:
   enum Parameters { // public namespace!
      DesignFeedForward = 0,
      DesignFeedBack,
      NumberOfDesigns,

      KneeHard = 0,
      KneeMedium,
      KneeSoft,
      NumberOfKneeSettings,
   };

   Compressor( int channels,
               int sample_rate );

   void resetMeters();

   bool getBypass();
   void setBypass( bool CompressorIsBypassedNew );

   double getRmsWindowSize();
   void setRmsWindowSize( double RmsWindowSizeMilliSecondsNew );

   int getDesign();
   void setDesign( int CompressorDesignNew );

   double getInputTrim();
   void setInputTrim( double InputTrimNew );

   double getThreshold();
   void setThreshold( double ThresholdNew );

   double getRatio();
   void setRatio( double RatioNew );

   double getKneeWidth();
   void setKneeWidth( double KneeWidthNew );

   double getAttackRate();
   void setAttackRate( double AttackRateNew );

   double getReleaseRate();
   void setReleaseRate( double ReleaseRateNew );

   int getCurve();
   void setCurve( int CurveTypeNew );

   int getGainStage();
   void setGainStage( int GainStageTypeNew );

   int getStereoLink();
   void setStereoLink( int StereoLinkPercentageNew );

   bool getAutoMakeupGain();
   void setAutoMakeupGain( bool UseAutoMakeupGainNew );

   double getMakeupGain();
   void setMakeupGain( double MakeupGainNew );

   int getWetMix();
   void setWetMix( int WetMixPercentageNew );

   bool getSidechainInput();
   void setSidechainInput( bool EnableExternalInputNew );

   int getSidechainHPFCutoff();
   void setSidechainHPFCutoff( int SidechainHPFCutoff );

   int getSidechainLPFCutoff();
   void setSidechainLPFCutoff( int SidechainLPFCutoff );

   bool getSidechainListen();
   void setSidechainListen( bool ListenToSidechainNew );

   double getGainReduction( int CurrentChannel );

   double getPeakMeterInputLevel( int CurrentChannel );
   double getPeakMeterOutputLevel( int CurrentChannel );

   double getAverageMeterInputLevel( int CurrentChannel );
   double getAverageMeterOutputLevel( int CurrentChannel );

   void process( AudioBuffer<double>& MainPlusSideChain );

   static double level2decibel( double levelLinear );
   static double decibel2level( double levelDecibels );

private:
   JUCE_LEAK_DETECTOR( Compressor );

   const double BufferLength;

   void updateMeterBallistics();

   void peakMeterBallistics( double PeakLevelCurrent,
                             double& PeakLevelOld );

   void averageMeterBallistics( double AverageLevelCurrent,
                                double& AverageLevelOld );

   void logMeterBallistics( double MeterInertia,
                            double TimePassed,
                            double Level,
                            double& Readout );

   int NumberOfChannels;
   int SampleRate;
   int MeterBufferPosition;
   int MeterBufferSize;

   AudioBuffer<double> MeterInputBuffer;
   AudioBuffer<double> MeterOutputBuffer;

   OwnedArray<SideChain<double>> SideChainProcessor;
   OwnedArray<frut::dsp::IirFilterBox> SidechainFilter_HPF;
   OwnedArray<frut::dsp::IirFilterBox> SidechainFilter_LPF;

   Array<double> InputSamples;
   Array<double> SidechainSamples;
   Array<double> OutputSamples;

   Array<double> PeakMeterInputLevels;
   Array<double> PeakMeterOutputLevels;

   Array<double> AverageMeterInputLevels;
   Array<double> AverageMeterOutputLevels;

   Array<double> GainReduction;
   Array<double> GainReductionWithMakeup;

   int CompressorDesign;
   double ReleaseCoefLinear;

   bool CompressorIsBypassed;
   bool CompressorIsBypassedCombined;
   bool DesignIsFeedForward;
   bool UseUpwardExpansion;

   int StereoLinkPercentage;
   double StereoLinkWeight;
   double StereoLinkWeightOther;

   double InputTrim;
   bool UseAutoMakeupGain;
   double MakeupGain;
   double MakeupGainDecibel;

   int WetMixPercentage;
   double WetMix;
   double DryMix;

   bool EnableExternalInput;
   bool IsHPFEnabled;
   bool IsLPFEnabled;
   bool ListenToSidechain;

   int SidechainHPFCutoff;
   int SidechainLPFCutoff;
};

#endif  // SQUEEZER_COMPRESSOR_H
