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
   enum Parameters {
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

   SampleType rmsWindowSizeMilliseconds_;
   int selectedCurve_;
   int selectedGainStage_;

   SampleType threshold_;
   SampleType internalCompressionRatio_;
   SampleType kneeWidth_;

   SampleType attackRateMilliseconds_;
   SampleType attackCoefficient_;

   SampleType releaseRateMilliseconds_;
   SampleType releaseCoefficient_;


public:
   explicit SideChain( int sampleRate ) :
      gainStageFET_( sampleRate ),
      gainStageOptical_( sampleRate )
      /*  Constructor.

          sampleRate: internal sample rate

          return value: none
      */
   {
      gainStageFET_.initialise( 0.0 );
      gainStageOptical_.initialise( 0.0 );

      sampleRate_ = ( SampleType ) sampleRate;
      idealGainReduction_ = 0.0;

      setThreshold( -32.0 );
      setRatio( 2.0 );
      setKneeWidth( 0.0 );

      setRmsWindowSize( 10.0 );
      selectedCurve_ = SideChain<SampleType>::CurveLogSmoothBranching;
      selectedGainStage_ = GainStage<SampleType>::FET;

      setAttackRate( 10.0 );
      setReleaseRate( 100 );
      setCurve( selectedCurve_ );
      setGainStage( selectedGainStage_ );

      // reset (i.e. initialise) all relevant variables
      reset();

#if DEBUG_RELEASE_RATE
      fTimePassed = 1.0 / sampleRate_;

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
      return rmsWindowSizeMilliseconds_;
   }


   void setRmsWindowSize( SampleType rmsWindowSize )
   /*  Set new detector RMS window size.

       rmsWindowSize: new detector RMS window size; set to
       0.0 to disable RMS sensing

       return value: none
   */
   {
      // bypass RMS sensing
      if ( rmsWindowSize <= 0.0 ) {
         rmsWindowSizeMilliseconds_ = 0.0;
         rmsWindowCoefficient_ = 0.0;
      } else {
         rmsWindowSizeMilliseconds_ = rmsWindowSize;

         // logarithmic envelope reaches 90% of the final reading
         // in the given attack time
         SampleType rmsWindowSizeInSamples = sampleRate_ * rmsWindowSizeMilliseconds_ / 1000.0;
         rmsWindowCoefficient_ = exp( log( 0.10 ) / rmsWindowSizeInSamples );
      }
   }


   int getCurve()
   /*  Get current compressor curve type.

       return value: returns compressor curve type
    */
   {
      return selectedCurve_;
   }


   void setCurve( int selectedCurve )
   /*  Set new compressor curve type.

       selectedCurve: new compressor curve type

       return value: none
    */
   {
      selectedCurve_ = selectedCurve;
      intermediateGainReduction_ = 0.0;

      setAttackRate( attackRateMilliseconds_ );
      setReleaseRate( releaseRateMilliseconds_ );
   }


   int getGainStage()
   /*  Get current compressor gain stage type.

       return value: returns compressor gain stage type
    */
   {
      return selectedGainStage_;
   }


   void setGainStage( int selectedGainStage )
   /*  Set new compressor gain stage type.

       selectedGainStage: new compressor gain stage type

       return value: none
    */
   {
      selectedGainStage_ = selectedGainStage;

      // update gain compensation
      setThreshold( threshold_ );

      if ( selectedGainStage_ == GainStage<SampleType>::FET ) {
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
      return threshold_;
   }


   void setThreshold( SampleType threshold )
   /*  Set new threshold.

       threshold: new threshold in decibels

       return value: none
    */
   {
      threshold_ = threshold;

      gainCompensation_ = queryGainComputer( autoGainReferenceLevel_ ) / 2.0;
   }


   SampleType getRatio()
   /*  Get current compression ratio.

       return value: returns the current compression ratio
    */
   {
      return 1.0 / ( 1.0 - internalCompressionRatio_ );
   }


   void setRatio( SampleType compressionRatio )
   /*  Set new compression ratio.

       compressionRatio: new compression ratio

       return value: none
    */
   {
      internalCompressionRatio_ = 1.0 - ( 1.0 / compressionRatio );
      gainCompensation_ = queryGainComputer( autoGainReferenceLevel_ ) / 2.0;
   }


   SampleType getKneeWidth()
   /*  Get current knee width.

       return value: returns the current knee width in decibels
    */
   {
      return kneeWidth_;
   }


   void setKneeWidth( SampleType kneeWidth )
   /*  Set new knee width.

       kneeWidth: new knee width in decibels

       return value: none
    */
   {
      kneeWidth_ = kneeWidth;
      gainCompensation_ = queryGainComputer( autoGainReferenceLevel_ ) / 2.0;
   }


   SampleType getAttackRate()
   /*  Get current attack rate.

       return value: returns the current attack rate in milliseconds
    */
   {
      return attackRateMilliseconds_;
   }


   void setAttackRate( SampleType attackRate )
   /*  Set new attack rate.

       attackRate: new attack rate in milliseconds

       return value: none
    */
   {
      attackRateMilliseconds_ = attackRate;

      if ( attackRateMilliseconds_ <= 0.0 ) {
         attackCoefficient_ = 0.0;
      } else {
         // logarithmic envelope reaches 90% of the final reading in
         // the given attack time
         SampleType attackRateInSamples = sampleRate_ * attackRateMilliseconds_ / 1000.0;
         attackCoefficient_ = exp( log( 0.10 ) / attackRateInSamples );
      }
   }


   SampleType getReleaseRate()
   /*  Get current release rate.

       return value: returns the current release rate in milliseconds
    */
   {
      return releaseRateMilliseconds_;
   }


   void setReleaseRate( SampleType releaseRate )
   /*  Set new release rate.

       nReleaseRateNew: new release rate in milliseconds

       return value: none
    */
   {
      releaseRateMilliseconds_ = releaseRate;

      if ( releaseRateMilliseconds_ <= 0.0 ) {
         releaseCoefficient_ = 0.0;
      } else {
         SampleType releaseRateInSamples = sampleRate_ * releaseRateMilliseconds_ / 1000.0;

         if ( selectedCurve_ == SideChain<SampleType>::CurveLogLin ) {
            // fall time: falls 10 dB per interval defined in release
            // rate (linear)
            releaseCoefficient_ = 10.0 / releaseRateInSamples;
         } else {
            // logarithmic envelope reaches 90% of the final reading
            // in the given release time
            releaseCoefficient_ = exp( log( 0.10 ) / releaseRateInSamples );
         }
      }
   }


   SampleType getGainReduction( bool useAutoMakeupGain )
   /*  Get current gain reduction.

       useAutoMakeupGain: determines whether the gain reduction should be
       level compensated or not

       return value: returns the current gain reduction in decibel
    */
   {
      SampleType tempGainReduction;

      if ( selectedGainStage_ == GainStage<SampleType>::FET ) {
         tempGainReduction = gainStageFET_.processGainReduction( gainReduction_, idealGainReduction_ );
      } else {
         tempGainReduction = gainStageOptical_.processGainReduction( gainReduction_, idealGainReduction_ );
      }

      if ( useAutoMakeupGain ) {

         return tempGainReduction - gainCompensation_;
      } else {
         return tempGainReduction;
      }
   }


   void processSample( SampleType inputLevel )
   /*  Process a single audio sample value.

       inputLevel: current audio sample value in decibels

       return value: current gain reduction in decibels
   */
   {
      // feed input level to gain computer
      idealGainReduction_ = queryGainComputer( inputLevel );

      // filter calculated gain reduction through level detection filter
      SampleType newGainReduction = applyRmsFilter( idealGainReduction_ );

      // feed output from gain computer to level detector
      switch ( selectedCurve_ ) {
         case SideChain<SampleType>::CurveLogLin:
            applyCurveLogLin( newGainReduction );
            break;

         case SideChain<SampleType>::CurveLogSmoothDecoupled:
            applyCurveLogSmoothDecoupled( newGainReduction );
            break;

         case SideChain<SampleType>::CurveLogSmoothBranching:
            applyCurveLogSmoothBranching( newGainReduction );
            break;

         default:
            DBG( "[Squeezer] sidechain::processSample ==> invalid detector" );
            break;
      }

#if DEBUG_RELEASE_RATE

      // reset things during attack phase
      if ( newGainReduction > gainReduction_ ) {
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


   static SampleType level2decibel( SampleType levelLinear )
   /*  Convert level from linear scale to decibels (dB).

       levelLinear: audio level

       return value: returns given level in decibels (dB) when above
       "lowerLimitOnMeter", otherwise "lowerLimitOnMeter"
   */
   {
      // just an inch below the meter's lowest segment
      SampleType lowerLimitOnMeter = -70.01;

      // log(0) is not defined, so return "fMeterMinimumDecibel"
      if ( levelLinear == 0.0 ) {
         return lowerLimitOnMeter;
      } else {
         // calculate decibels from audio level (a factor of 20.0 is
         // needed to calculate *level* ratios, whereas 10.0 is needed
         // for *power* ratios!)
         SampleType levelDecibels = 20.0 * log10( levelLinear );

         // to make meter ballistics look nice for low levels, do not
         // return levels below "fMeterMinimumDecibel"
         if ( levelDecibels < lowerLimitOnMeter ) {
            return lowerLimitOnMeter;
         } else {
            return levelDecibels;
         }
      }
   }


   static SampleType decibel2level( SampleType levelDecibels )
   /*  Convert level from decibels (dB) to linear scale.

       levelDecibels: audio level in decibels (dB)

       return value: given level in linear scale
   */
   {
      // calculate audio level from decibels (a divisor of 20.0 is
      // needed to calculate *level* ratios, whereas 10.0 is needed for
      // *power* ratios!)
      SampleType levelLinear = pow( 10.0, levelDecibels / 20.0 );
      return levelLinear;
   }


private:
   SampleType applyRmsFilter( SampleType detectorInputLevel )
   {
      // bypass RMS sensing
      if ( rmsWindowSizeMilliseconds_ <= 0.0 ) {
         return detectorInputLevel;
      } else {
         SampleType detectorInputLevelSquaredNew = detectorInputLevel * detectorInputLevel;
         detectorOutputLevelSquared_ = ( rmsWindowCoefficient_ * detectorOutputLevelSquared_ ) +
                                       ( 1.0 - rmsWindowCoefficient_ ) * detectorInputLevelSquaredNew;

         SampleType detectorOutputLevel = sqrt( detectorOutputLevelSquared_ );
         return detectorOutputLevel;
      }
   }


   SampleType queryGainComputer( SampleType inputLevel )
   /*  Calculate gain reduction and envelopes from input level.

       inputLevel: current input level in decibels

       return value: calculated gain reduction in decibels
    */
   {
      SampleType aboveThreshold = inputLevel - threshold_;

      if ( kneeWidth_ == 0.0 ) {
         if ( inputLevel <= threshold_ ) {
            return 0.0;
         } else {
            return aboveThreshold * internalCompressionRatio_;
         }
      } else {
         // algorithm adapted from Giannoulis et al., "Digital Dynamic
         // Range Compressor Design - A Tutorial and Analysis", JAES,
         // 60(6):399-408, 2012
         if ( aboveThreshold < -( kneeWidth_ / 2.0 ) ) {
            return 0.0;
         } else if ( aboveThreshold > ( kneeWidth_ / 2.0 ) ) {
            return aboveThreshold * internalCompressionRatio_;
         } else {
            SampleType factor = aboveThreshold + ( kneeWidth_ / 2.0 );
            return factor * factor / ( kneeWidth_ * 2.0 ) * internalCompressionRatio_;
         }
      }
   }


   void applyCurveLogLin( SampleType gainReduction )
   /*  Calculate detector with logarithmic attack and linear release
       ("Linear").

       gainReduction: calculated new gain reduction in decibels

       return value: none
   */
   {
      // apply attack rate if proposed gain reduction is above old gain
      // reduction
      if ( gainReduction >= gainReduction_ ) {
         if ( attackCoefficient_ == 0.0 ) {
            gainReduction_ = gainReduction;
         } else {
            // algorithm adapted from Giannoulis et al., "Digital
            // Dynamic Range Compressor Design - A Tutorial and
            // Analysis", JAES, 60(6):399-408, 2012
            gainReduction_ = ( attackCoefficient_ * gainReduction_ ) +
                             ( 1.0 - attackCoefficient_ ) * gainReduction;
         }

         // otherwise, apply release rate if proposed gain reduction is
         // below old gain reduction
      } else {
         if ( releaseCoefficient_ == 0.0 ) {
            gainReduction_ = gainReduction;
         } else {
            gainReduction_ -= releaseCoefficient_;

            if ( gainReduction_ < gainReduction ) {
               gainReduction_ = gainReduction;
            }
         }
      }
   }


   void applyCurveLogSmoothDecoupled( SampleType gainReduction )
   /*  Calculate smooth decoupled detector ("Smooth").

       gainReduction: calculated gain reduction in decibels

       return value: none
   */
   {
      // algorithm adapted from Giannoulis et al., "Digital Dynamic
      // Range Compressor Design - A Tutorial and Analysis", JAES,
      // 60(6):399-408, 2012
      //
      // apply release envelope
      if ( releaseCoefficient_ == 0.0 ) {
         intermediateGainReduction_ = gainReduction;
      } else {
         intermediateGainReduction_ = ( releaseCoefficient_ * intermediateGainReduction_ ) +
                                      ( 1.0 - releaseCoefficient_ ) * gainReduction;

         // maximally fast peak detection
         if ( gainReduction > intermediateGainReduction_ ) {
            intermediateGainReduction_ = gainReduction;
         }
      }

      // apply attack envelope
      if ( attackCoefficient_ == 0.0 ) {
         gainReduction_ = intermediateGainReduction_;
      } else {
         gainReduction_ = ( attackCoefficient_ * gainReduction_ ) +
                          ( 1.0 - attackCoefficient_ ) * intermediateGainReduction_;
      }
   }


   void applyCurveLogSmoothBranching( SampleType gainReduction )
   /*  Calculate smooth branching detector ("Logarithmic").

       gainReduction: calculated gain reduction in decibels

       return value: none
   */
   {
      // apply attack rate if proposed gain reduction is above old gain
      // reduction
      if ( gainReduction > gainReduction_ ) {
         if ( attackCoefficient_ == 0.0 ) {
            gainReduction_ = gainReduction;
         } else {
            // algorithm adapted from Giannoulis et al., "Digital
            // Dynamic Range Compressor Design - A Tutorial and
            // Analysis", JAES, 60(6):399-408, 2012
            gainReduction_ = ( attackCoefficient_ * gainReduction_ ) +
                             ( 1.0 - attackCoefficient_ ) * gainReduction;
         }

         // otherwise, apply release rate if proposed gain reduction is
         // below old gain reduction
      } else {
         if ( releaseCoefficient_ == 0.0 ) {
            gainReduction_ = gainReduction;
         } else {
            // algorithm adapted from Giannoulis et al., "Digital
            // Dynamic Range Compressor Design - A Tutorial and
            // Analysis", JAES, 60(6):399-408, 2012
            gainReduction_ = ( releaseCoefficient_ * gainReduction_ ) +
                             ( 1.0 - releaseCoefficient_ ) * gainReduction;
         }
      }
   }
};

#endif  // SQUEEZER_SIDE_CHAIN_H
