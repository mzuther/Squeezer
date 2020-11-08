/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2020 Martin Zuther (http://www.mzuther.de/)

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

namespace frut
{
namespace dsp
{

FilterChebyshev::FilterChebyshev(
   double RelativeCutoffFrequency,
   bool IsHighPass,
   double PercentRipple,
   int NumberOfPoles )
{
   PercentRipple_ = PercentRipple;
   NumberOfPoles_ = NumberOfPoles;

   // each filter stage consists of a pair of poles
   jassert( NumberOfPoles_ % 2 == 0 );
   int NumberOfStages = NumberOfPoles_ / 2;

   for ( int Stage = 0; Stage < NumberOfStages; ++Stage ) {
      FilterStages_.add( std::make_unique<FilterChebyshevStage>() );
   }

   changeParameters( RelativeCutoffFrequency, IsHighPass );
   reset();

   // testAlgorithm(false);
}


void FilterChebyshev::changeParameters(
   double RelativeCutoffFrequency,
   bool IsHighPass )
{
   for ( int Stage = 0; Stage < FilterStages_.size(); ++Stage ) {
      // pole pairs start with index 1!
      int PolePair = Stage + 1;

      FilterStages_[Stage]->changeParameters(
         RelativeCutoffFrequency,
         IsHighPass,
         PercentRipple_,
         NumberOfPoles_,
         PolePair );
   }
}


void FilterChebyshev::reset()
{
   for ( int Stage = 0; Stage < FilterStages_.size(); ++Stage ) {
      FilterStages_[Stage]->reset();
   }
}


double FilterChebyshev::filterSample(
   double InputCurrent )
{
   double OutputCurrent = InputCurrent;

   for ( int Stage = 0; Stage < FilterStages_.size(); ++Stage ) {
      OutputCurrent = FilterStages_[Stage]->filterSample(
                         OutputCurrent );
   }

   // output is already de-normalised
   return OutputCurrent;
}


void FilterChebyshev::testAlgorithm(
   bool IsHighPass )
{
   jassert( FilterStages_.size() > 0 );

   double PercentRipple = 0.5;
   double RelativeCutoffFrequency;

   ignoreUnused( PercentRipple );

   DBG( "" );
   DBG( "Chebyshev Filter Coefficients" );
   DBG( "=============================" );

   if ( IsHighPass ) {
      DBG( "Highpass filter, " + String( PercentRipple, 1 ) + "% ripple" );
   } else {
      DBG( "Lowpass filter, " + String( PercentRipple, 1 ) + "% ripple" );
   }

   DBG( "" );

   RelativeCutoffFrequency = 0.010;

   DBG( FilterStages_[0]->testAlgorithm(
           RelativeCutoffFrequency,
           IsHighPass,
           PercentRipple ) );

   DBG( "" );

   RelativeCutoffFrequency = 0.025;

   DBG( FilterStages_[0]->testAlgorithm(
           RelativeCutoffFrequency,
           IsHighPass,
           PercentRipple ) );

   DBG( "" );

   RelativeCutoffFrequency = 0.050;

   DBG( FilterStages_[0]->testAlgorithm(
           RelativeCutoffFrequency,
           IsHighPass,
           PercentRipple ) );

   DBG( "" );

   RelativeCutoffFrequency = 0.075;

   DBG( FilterStages_[0]->testAlgorithm(
           RelativeCutoffFrequency,
           IsHighPass,
           PercentRipple ) );

   DBG( "" );

   RelativeCutoffFrequency = 0.100;

   DBG( FilterStages_[0]->testAlgorithm(
           RelativeCutoffFrequency,
           IsHighPass,
           PercentRipple ) );

   DBG( "" );

   RelativeCutoffFrequency = 0.150;

   DBG( FilterStages_[0]->testAlgorithm(
           RelativeCutoffFrequency,
           IsHighPass,
           PercentRipple ) );

   DBG( "" );

   RelativeCutoffFrequency = 0.200;

   DBG( FilterStages_[0]->testAlgorithm(
           RelativeCutoffFrequency,
           IsHighPass,
           PercentRipple ) );

   DBG( "" );

   RelativeCutoffFrequency = 0.250;

   DBG( FilterStages_[0]->testAlgorithm(
           RelativeCutoffFrequency,
           IsHighPass,
           PercentRipple ) );

   DBG( "" );

   RelativeCutoffFrequency = 0.300;

   DBG( FilterStages_[0]->testAlgorithm(
           RelativeCutoffFrequency,
           IsHighPass,
           PercentRipple ) );

   DBG( "" );

   RelativeCutoffFrequency = 0.350;

   DBG( FilterStages_[0]->testAlgorithm(
           RelativeCutoffFrequency,
           IsHighPass,
           PercentRipple ) );

   DBG( "" );

   RelativeCutoffFrequency = 0.400;

   DBG( FilterStages_[0]->testAlgorithm(
           RelativeCutoffFrequency,
           IsHighPass,
           PercentRipple ) );

   DBG( "" );

   RelativeCutoffFrequency = 0.450;

   DBG( FilterStages_[0]->testAlgorithm(
           RelativeCutoffFrequency,
           IsHighPass,
           PercentRipple ) );

   DBG( "" );
}

}
}
