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

#ifndef SQUEEZER_COMPRESSOR_CURVE_H
#define SQUEEZER_COMPRESSOR_CURVE_H

template <typename FloatType> class CompressorCurve;

#include "FrutHeader.h"
#include "compressor_curve_log_lin.h"
#include "compressor_curve_log_smooth_branching.h"
#include "compressor_curve_log_smooth_decoupled.h"


template <typename FloatType>
class CompressorCurve
{
public:
   enum Parameters {
      LogLin = 0,
      LogSmoothDecoupled,
      LogSmoothBranching,
      NumberOfCurves,
   };

protected:
   int selectedCurve_;
   FloatType sampleRate_;

   FloatType attackRateMilliseconds_;
   FloatType releaseRateMilliseconds_;

   FloatType attackCoefficient_;
   FloatType releaseCoefficient_;

   JUCE_LEAK_DETECTOR( CompressorCurve );

public:
   // Destructor.
   virtual ~CompressorCurve()
   {}


   int getCurve()
   /*  Get current compressor curve type.

       return value: returns compressor curve type
    */
   {
      return selectedCurve_;
   }


   FloatType getReleaseRate()
   /*  Get current release rate.

       return value: returns the current release rate in milliseconds
    */
   {
      return releaseRateMilliseconds_;
   }


   FloatType getAttackRate()
   /*  Get current attack rate.

       return value: returns the current attack rate in milliseconds
    */
   {
      return attackRateMilliseconds_;
   }


   static std::unique_ptr<CompressorCurve<FloatType>> create( int selectedCurve,
                                                              FloatType sampleRate,
                                                              FloatType attackRate,
                                                              FloatType releaseRate )
   {
      std::unique_ptr<CompressorCurve<FloatType>> curve;

      switch ( selectedCurve ) {
         case CompressorCurve<FloatType>::LogLin:
            curve = std::make_unique<CompressorCurveLogLin<FloatType>>( sampleRate );
            break;

         case CompressorCurve<FloatType>::LogSmoothBranching:
            curve = std::make_unique<CompressorCurveLogSmoothBranching<FloatType>>( sampleRate );
            break;

         case CompressorCurve<FloatType>::LogSmoothDecoupled:
            curve = std::make_unique<CompressorCurveLogSmoothDecoupled<FloatType>>( sampleRate );
            break;

         default:
            DBG( "[Squeezer] invalid compressor curve" );
            return nullptr;
            break;
      }

      curve->reset();
      curve->setAttackRate( attackRate );
      curve->setReleaseRate( releaseRate );

      return curve;
   }


   virtual void reset() = 0;
   virtual FloatType processSample( FloatType gainReduction ) = 0;

   virtual void setAttackRate( FloatType attackRate ) = 0;
   virtual void setReleaseRate( FloatType releaseRate ) = 0;

protected:
   explicit CompressorCurve( FloatType sampleRate ) :
      selectedCurve_( -1 ),
      sampleRate_( sampleRate ),
      attackRateMilliseconds_( 0.0 ),
      releaseRateMilliseconds_( 0.0 ),
      attackCoefficient_( 0.0 ),
      releaseCoefficient_( 0.0 )
   {}


   FloatType calculateLogAttackCoefficient( FloatType attackRate )
   /*  Calculate logarithmic attack coefficient from attack rate.

       attackRate: attack rate in milliseconds

       return value: logarithmic attack coefficient
    */
   {
      if ( attackRate <= 0.0 ) {
         return 0.0;
      } else {
         // logarithmic envelope reaches 90% of the final reading in
         // the given attack time
         auto attackRateInSamples = sampleRate_ * attackRate / 1000.0;
         return exp( log( 0.10 ) / attackRateInSamples );
      }
   }


   FloatType calculateLinearReleaseCoefficient( FloatType releaseRate )
   /*  Calculate linear release coefficient from attack rate.

       releaseRate: release rate in milliseconds

       return value: linear release coefficient
    */
   {
      if ( releaseRate <= 0.0 ) {
         return 0.0;
      } else {
         // fall time: falls 10 dB per interval defined in release
         // rate (linear)
         auto releaseRateInSamples = sampleRate_ * releaseRate / 1000.0;
         return 10.0 / releaseRateInSamples;
      }
   }


   FloatType calculateLogReleaseCoefficient( FloatType releaseRate )
   /*  Calculate logarithmic release coefficient from attack rate.

       releaseRate: release rate in milliseconds

       return value: logarithmic release coefficient
    */
   {
      if ( releaseRate <= 0.0 ) {
         return 0.0;
      } else {
         // logarithmic envelope reaches 90% of the final reading in
         // the given release time
         auto releaseRateInSamples = sampleRate_ * releaseRate / 1000.0;
         return exp( log( 0.10 ) / releaseRateInSamples );
      }
   }
};

#endif  // SQUEEZER_COMPRESSOR_CURVE_H
