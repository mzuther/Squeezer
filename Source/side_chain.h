/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of citrus

   Copyright (c) 2013-2020 Martin Zuther (http://www.mzuther.de/)

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


template <typename SampleType>
class SideChain
{
public:
   enum Parameters { // public namespace!
      CurveLogLin = 0,
      CurveLogSmoothDecoupled,
      CurveLogSmoothBranching,
      NumberOfCurves,
   };


private:
   JUCE_LEAK_DETECTOR( SideChain );

#if DEBUG_RELEASE_RATE
   SampleType timePassed_;

   SampleType debugFinalValue90_;
   SampleType debugTimeInReleasePhase_;
#endif // DEBUG_RELEASE_RATE

   GainStageFET<SampleType> gainStageFET_;
   GainStageOptical<SampleType> gainStageOptical_;

   SampleType sampleRate_;
   SampleType autoGainReferenceLevel_;
   SampleType gainReduction_;
   SampleType idealGainReduction_;
   SampleType intermediateGainReduction_;
   SampleType gainCompensation_;

   SampleType rmsWindowCoefficient_;
   SampleType detectorOutputLevelSquared_;

   SampleType rmsWindowSizeMilliSeconds_;
   int nCurveType;
   int nGainStageType;

   SampleType dThreshold;
   SampleType dRatioInternal;
   SampleType kneeWidth_;

   SampleType dAttackRate;
   SampleType dAttackCoefficient;

   int nReleaseRate;
   SampleType dReleaseCoefficient;


public:
   explicit SideChain( int nSampleRate ) :
      gainStageFET_( nSampleRate ),
      gainStageOptical_( nSampleRate )
      /*  Constructor.

          nSampleRate: internal sample rate

          return value: none
      */
   {
      gainStageFET_.initialise( 0.0 );
      gainStageOptical_.initialise( 0.0 );

      sampleRate_ = ( SampleType ) nSampleRate;
      idealGainReduction_ = 0.0;

      setThreshold( -32.0 );
      setRatio( 2.0 );
      setKneeWidth( 0.0 );

      setRmsWindowSize( 10.0 );
      nCurveType = SideChain<SampleType>::CurveLogSmoothBranching;
      nGainStageType = GainStage<SampleType>::FET;

      setAttackRate( 10.0 );
      setReleaseRate( 100 );
      setCurve( nCurveType );
      setGainStage( nGainStageType );

      // reset (i.e. initialise) all relevant variables
      reset();

#if DEBUG_RELEASE_RATE
      fTimePassed = 1.0 / ( SampleType ) nSampleRate;

      debugFinalValue90_ = -1.0;
      debugTimeInReleasePhase_ = 0.0;
#endif // DEBUG_RELEASE_RATE
   }


   void reset()
   /*  Reset all relevant variables.

       return value: none
   */
   {
      gainReduction_ = 0.0;
      gainCompensation_ = 0.0;
      detectorOutputLevelSquared_ = 0.0;

      autoGainReferenceLevel_ = 0.0;
   }


   SampleType getRmsWindowSize()
   /*  Get current detector RMS window size.

       return value: returns current current detector RMS window size
   */
   {
      return rmsWindowSizeMilliSeconds_;
   }


   void setRmsWindowSize( SampleType dRmsWindowSizeMilliSecondsNew )
   /*  Set new detector RMS window size.

       dRmsWindowSizeMilliSecondsNew: new detector RMS window size; set
       to 0.0 to disable RMS sensing

       return value: none
   */
   {
      // bypass RMS sensing
      if ( dRmsWindowSizeMilliSecondsNew <= 0.0 ) {
         rmsWindowSizeMilliSeconds_ = 0.0;
         rmsWindowCoefficient_ = 0.0;
      } else {
         rmsWindowSizeMilliSeconds_ = dRmsWindowSizeMilliSecondsNew;
         SampleType dRmsWindowSizeSeconds = rmsWindowSizeMilliSeconds_ / 1000.0;

         // logarithmic envelope reaches 90% of the final reading
         // in the given attack time
         rmsWindowCoefficient_ = exp( log( 0.10 ) / ( dRmsWindowSizeSeconds * sampleRate_ ) );
      }
   }


   int getCurve()
   /*  Get current compressor curve type.

       return value: returns compressor curve type
    */
   {
      return nCurveType;
   }


   void setCurve( int nCurveTypeNew )
   /*  Set new compressor curve type.

       nCurveTypeNew: new compressor curve type

       return value: none
    */
   {
      nCurveType = nCurveTypeNew;
      intermediateGainReduction_ = 0.0;

      setAttackRate( dAttackRate );
      setReleaseRate( nReleaseRate );
   }


   int getGainStage()
   /*  Get current compressor gain stage type.

       return value: returns compressor gain stage type
    */
   {
      return nGainStageType;
   }


   void setGainStage( int nGainStageTypeNew )
   /*  Set new compressor gain stage type.

       nGainStageTypeNew: new compressor gain stage type

       return value: none
    */
   {
      nGainStageType = nGainStageTypeNew;

      // update gain compensation
      setThreshold( dThreshold );

      if ( nGainStageType == GainStage<SampleType>::FET ) {
         gainStageFET_.initialise( gainReduction_ );
      } else {
         gainStageOptical_.initialise( gainReduction_ );
      }
   }


   SampleType getThreshold()
   /*  Get current threshold.

       return value: returns the current threshold in decibels
    */
   {
      return dThreshold;
   }


   void setThreshold( SampleType dThresholdNew )
   /*  Set new threshold.

       dThresholdNew: new threshold in decibels

       return value: none
    */
   {
      dThreshold = dThresholdNew;

      gainCompensation_ = queryGainComputer( autoGainReferenceLevel_ ) / 2.0;
   }


   SampleType getRatio()
   /*  Get current compression ratio.

       return value: returns the current compression ratio
    */
   {
      return 1.0 / ( 1.0 - dRatioInternal );
   }


   void setRatio( SampleType dRatioNew )
   /*  Set new compression ratio.

       dRatioNew: new compression ratio

       return value: none
    */
   {
      dRatioInternal = 1.0 - ( 1.0 / dRatioNew );
      gainCompensation_ = queryGainComputer( autoGainReferenceLevel_ ) / 2.0;
   }


   SampleType getKneeWidth()
   /*  Get current knee width.

       return value: returns the current knee width in decibels
    */
   {
      return kneeWidth_;
   }


   void setKneeWidth( SampleType dKneeWidthNew )
   /*  Set new knee width.

       dKneeWidthNew: new knee width in decibels

       return value: none
    */
   {
      kneeWidth_ = dKneeWidthNew;
      gainCompensation_ = queryGainComputer( autoGainReferenceLevel_ ) / 2.0;
   }


   SampleType getAttackRate()
   /*  Get current attack rate.

       return value: returns the current attack rate in milliseconds
    */
   {
      return dAttackRate;
   }


   void setAttackRate( SampleType dAttackRateNew )
   /*  Set new attack rate.

       dAttackRateNew: new attack rate in milliseconds

       return value: none
    */
   {
      dAttackRate = dAttackRateNew;

      if ( dAttackRate <= 0.0 ) {
         dAttackCoefficient = 0.0;
      } else {
         SampleType dAttackRateSeconds = dAttackRate / 1000.0;

         // logarithmic envelope reaches 90% of the final reading in
         // the given attack time
         dAttackCoefficient = exp( log( 0.10 ) / ( dAttackRateSeconds * sampleRate_ ) );
      }
   }


   int getReleaseRate()
   /*  Get current release rate.

       return value: returns the current release rate in milliseconds
    */
   {
      return nReleaseRate;
   }


   void setReleaseRate( int nReleaseRateNew )
   /*  Set new release rate.

       nReleaseRateNew: new release rate in milliseconds

       return value: none
    */
   {
      nReleaseRate = nReleaseRateNew;

      if ( nReleaseRate <= 0 ) {
         dReleaseCoefficient = 0.0;
      } else {
         SampleType dReleaseRateSeconds = nReleaseRate / 1000.0;

         if ( nCurveType == SideChain<SampleType>::CurveLogLin ) {
            // fall time: falls 10 dB per interval defined in release
            // rate (linear)
            dReleaseCoefficient = 10.0 / ( dReleaseRateSeconds * sampleRate_ );
         } else {
            // logarithmic envelope reaches 90% of the final reading
            // in the given release time
            dReleaseCoefficient = exp( log( 0.10 ) / ( dReleaseRateSeconds * sampleRate_ ) );
         }
      }
   }


   SampleType getGainReduction( bool bAutoMakeupGain )
   /*  Get current gain reduction.

       bAutoMakeupGain: determines whether the gain reduction should be
       level compensated or not

       return value: returns the current gain reduction in decibel
    */
   {
      SampleType dGainReductionTemp;

      if ( nGainStageType == GainStage<SampleType>::FET ) {
         dGainReductionTemp = gainStageFET_.processGainReduction( gainReduction_, idealGainReduction_ );
      } else {
         dGainReductionTemp = gainStageOptical_.processGainReduction( gainReduction_, idealGainReduction_ );
      }

      if ( bAutoMakeupGain ) {

         return dGainReductionTemp - gainCompensation_;
      } else {
         return dGainReductionTemp;
      }
   }


   void processSample( SampleType dInputLevel )
   /*  Process a single audio sample value.

       dInputLevel: current audio sample value in decibels

       return value: current gain reduction in decibels
   */
   {
      // feed input level to gain computer
      idealGainReduction_ = queryGainComputer( dInputLevel );

      // filter calculated gain reduction through level detection filter
      SampleType dGainReductionNew = applyRmsFilter( idealGainReduction_ );

      // feed output from gain computer to level detector
      switch ( nCurveType ) {
         case SideChain<SampleType>::CurveLogLin:
            applyCurveLogLin( dGainReductionNew );
            break;

         case SideChain<SampleType>::CurveLogSmoothDecoupled:
            applyCurveLogSmoothDecoupled( dGainReductionNew );
            break;

         case SideChain<SampleType>::CurveLogSmoothBranching:
            applyCurveLogSmoothBranching( dGainReductionNew );
            break;

         default:
            DBG( "[Squeezer] sidechain::processSample ==> invalid detector" );
            break;
      }

#if DEBUG_RELEASE_RATE

      // reset things during attack phase
      if ( dGainReductionNew > gainReduction_ ) {
         debugFinalValue90_ = -1.0;
         debugTimeInReleasePhase_ = 0.0;
         // we're in release phase
      } else {
         // release phase has just started
         if ( debugFinalValue90_ < 0.0 ) {
            // only measure "real" data
            if ( gainReduction_ >= 1.0 ) {
               // determine time when the envelope reaches 90% of the
               // final reading
               debugFinalValue90_ = gainReduction_ * 0.1;

               // reset time measurement
               debugTimeInReleasePhase_ = 0.0;
            }

            // test for 90% of the final reading
         } else if ( gainReduction_ <= debugFinalValue90_ ) {
            // display measured time
            DBG( String( debugTimeInReleasePhase_ * 1000.0, 1 ) + " ms" );

            // reset things
            debugFinalValue90_ = -1.0;
            debugTimeInReleasePhase_ = 0.0;
            // update time since start of release phase
         } else {
            debugTimeInReleasePhase_ += timePassed_;
         }
      }

#endif // DEBUG_RELEASE_RATE
   }


   static SampleType level2decibel( SampleType dLevel )
   /*  Convert level from linear scale to decibels (dB).

       dLevel: audio level

       return value: returns given level in decibels (dB) when above
       "dMeterMinimumDecibel", otherwise "dMeterMinimumDecibel"
   */
   {
      // just an inch below the meter's lowest segment
      SampleType dMeterMinimumDecibel = -70.01;

      // log(0) is not defined, so return "fMeterMinimumDecibel"
      if ( dLevel == 0.0 ) {
         return dMeterMinimumDecibel;
      } else {
         // calculate decibels from audio level (a factor of 20.0 is
         // needed to calculate *level* ratios, whereas 10.0 is needed
         // for *power* ratios!)
         SampleType dDecibels = 20.0 * log10( dLevel );

         // to make meter ballistics look nice for low levels, do not
         // return levels below "fMeterMinimumDecibel"
         if ( dDecibels < dMeterMinimumDecibel ) {
            return dMeterMinimumDecibel;
         } else {
            return dDecibels;
         }
      }
   }


   static SampleType decibel2level( SampleType dDecibels )
   /*  Convert level from decibels (dB) to linear scale.

       dLevel: audio level in decibels (dB)

       return value: given level in linear scale
   */
   {
      // calculate audio level from decibels (a divisor of 20.0 is
      // needed to calculate *level* ratios, whereas 10.0 is needed for
      // *power* ratios!)
      SampleType dLevel = pow( 10.0, dDecibels / 20.0 );
      return dLevel;
   }


private:
   SampleType applyRmsFilter( SampleType dDetectorInputLevel )
   {
      // bypass RMS sensing
      if ( rmsWindowSizeMilliSeconds_ <= 0.0 ) {
         return dDetectorInputLevel;
      } else {
         SampleType dDetectorInputLevelSquared = dDetectorInputLevel * dDetectorInputLevel;
         SampleType dDetectorOutputLevelSquaredOld = detectorOutputLevelSquared_;

         detectorOutputLevelSquared_ = ( rmsWindowCoefficient_ * dDetectorOutputLevelSquaredOld ) + ( 1.0 - rmsWindowCoefficient_ ) * dDetectorInputLevelSquared;

         SampleType dDetectorOutputLevel = sqrt( detectorOutputLevelSquared_ );
         return dDetectorOutputLevel;
      }
   }


   SampleType queryGainComputer( SampleType dInputLevel )
   /*  Calculate gain reduction and envelopes from input level.

       dInputLevel: current input level in decibels

       return value: calculated gain reduction in decibels
    */
   {
      SampleType dAboveThreshold = dInputLevel - dThreshold;

      if ( kneeWidth_ == 0.0 ) {
         if ( dInputLevel <= dThreshold ) {
            return 0.0;
         } else {
            return dAboveThreshold * dRatioInternal;
         }
      } else {
         // algorithm adapted from Giannoulis et al., "Digital Dynamic
         // Range Compressor Design - A Tutorial and Analysis", JAES,
         // 60(6):399-408, 2012
         if ( dAboveThreshold < -( kneeWidth_ / 2.0 ) ) {
            return 0.0;
         } else if ( dAboveThreshold > ( kneeWidth_ / 2.0 ) ) {
            return dAboveThreshold * dRatioInternal;
         } else {
            SampleType factor = dAboveThreshold + ( kneeWidth_ / 2.0 );
            return factor * factor / ( kneeWidth_ * 2.0 ) * dRatioInternal;
         }
      }
   }


   void applyCurveLogLin( SampleType dGainReductionNew )
   /*  Calculate detector with logarithmic attack and linear release
       ("Linear").

       dGainReductionNew: calculated new gain reduction in decibels

       return value: none
   */
   {
      // apply attack rate if proposed gain reduction is above old gain
      // reduction
      if ( dGainReductionNew >= gainReduction_ ) {
         if ( dAttackCoefficient == 0.0 ) {
            gainReduction_ = dGainReductionNew;
         } else {
            // algorithm adapted from Giannoulis et al., "Digital
            // Dynamic Range Compressor Design - A Tutorial and
            // Analysis", JAES, 60(6):399-408, 2012

            SampleType dGainReductionOld = gainReduction_;
            gainReduction_ = ( dAttackCoefficient * dGainReductionOld ) + ( 1.0 - dAttackCoefficient ) * dGainReductionNew;
         }

         // otherwise, apply release rate if proposed gain reduction is
         // below old gain reduction
      } else {
         if ( dReleaseCoefficient == 0.0 ) {
            gainReduction_ = dGainReductionNew;
         } else {
            gainReduction_ -= dReleaseCoefficient;

            if ( gainReduction_ < dGainReductionNew ) {
               gainReduction_ = dGainReductionNew;
            }
         }
      }
   }


   void applyCurveLogSmoothDecoupled( SampleType dGainReductionNew )
   /*  Calculate smooth decoupled detector ("Smooth").

       dGainReductionNew: calculated gain reduction in decibels

       return value: none
   */
   {
      // algorithm adapted from Giannoulis et al., "Digital Dynamic
      // Range Compressor Design - A Tutorial and Analysis", JAES,
      // 60(6):399-408, 2012
      //
      // apply release envelope
      if ( dReleaseCoefficient == 0.0 ) {
         intermediateGainReduction_ = dGainReductionNew;
      } else {
         SampleType dGainReductionIntermediateOld = intermediateGainReduction_;
         intermediateGainReduction_ = ( dReleaseCoefficient * dGainReductionIntermediateOld ) + ( 1.0 - dReleaseCoefficient ) * dGainReductionNew;

         // maximally fast peak detection
         if ( dGainReductionNew > intermediateGainReduction_ ) {
            intermediateGainReduction_ = dGainReductionNew;
         }
      }

      // apply attack envelope
      if ( dAttackCoefficient == 0.0 ) {
         gainReduction_ = intermediateGainReduction_;
      } else {
         SampleType dGainReductionOld = gainReduction_;
         gainReduction_ = ( dAttackCoefficient * dGainReductionOld ) + ( 1.0 - dAttackCoefficient ) * intermediateGainReduction_;
      }
   }


   void applyCurveLogSmoothBranching( SampleType dGainReductionNew )
   /*  Calculate smooth branching detector ("Logarithmic").

       dGainReductionNew: calculated gain reduction in decibels

       return value: none
   */
   {
      // apply attack rate if proposed gain reduction is above old gain
      // reduction
      if ( dGainReductionNew > gainReduction_ ) {
         if ( dAttackCoefficient == 0.0 ) {
            gainReduction_ = dGainReductionNew;
         } else {
            // algorithm adapted from Giannoulis et al., "Digital
            // Dynamic Range Compressor Design - A Tutorial and
            // Analysis", JAES, 60(6):399-408, 2012

            SampleType dGainReductionOld = gainReduction_;
            gainReduction_ = ( dAttackCoefficient * dGainReductionOld ) + ( 1.0 - dAttackCoefficient ) * dGainReductionNew;
         }

         // otherwise, apply release rate if proposed gain reduction is
         // below old gain reduction
      } else {
         if ( dReleaseCoefficient == 0.0 ) {
            gainReduction_ = dGainReductionNew;
         } else {
            // algorithm adapted from Giannoulis et al., "Digital
            // Dynamic Range Compressor Design - A Tutorial and
            // Analysis", JAES, 60(6):399-408, 2012

            SampleType dGainReductionOld = gainReduction_;
            gainReduction_ = ( dReleaseCoefficient * dGainReductionOld ) + ( 1.0 - dReleaseCoefficient ) * dGainReductionNew;
         }
      }
   }
};

#endif  // SQUEEZER_SIDE_CHAIN_H
