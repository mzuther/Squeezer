/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2024 Martin Zuther (http://www.mzuther.de/)

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

namespace frut::dsp
{

BiquadFilter::BiquadFilter( const int numberOfChannels )
{
   numberOfChannels_ = numberOfChannels;
   neutral();

   resetDelays();
}


BiquadFilter::~BiquadFilter()
{
}


void BiquadFilter::neutral()
{
   setCoefficients( 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, false );
}


// reset filter storage
void BiquadFilter::resetDelays()
{
   for ( int channel = 0; channel < numberOfChannels_; ++channel ) {
      // indices are added if not already present!
      x0_.set( channel, 0.0 );
      x1_.set( channel, 0.0 );
      x2_.set( channel, 0.0 );

      y1_.set( channel, 0.0 );
      y2_.set( channel, 0.0 );
   }
}


// set filter coefficients
void BiquadFilter::setCoefficients(
   const double a0,
   const double a1,
   const double a2,

   const double b1,
   const double b2,

   const bool showCoefficients )
{
   double c0 = 1.0;
   double d0 = 0.0;

   setCoefficients(
      a0, a1, a2,
      b1, b2,
      c0, d0,
      showCoefficients );
}


// set filter coefficients
void BiquadFilter::setCoefficients(
   const double a0,
   const double a1,
   const double a2,

   const double b1,
   const double b2,

   const double c0,
   const double d0,

   const bool showCoefficients )
{
   a0_ = a0;
   a1_ = a1;
   a2_ = a2;

   b1_ = b1;
   b2_ = b2;

   c0_ = c0;
   d0_ = d0;

   if ( showCoefficients ) {
      DBG( "" );
      DBG( "a0: " + String( a0_ ) );
      DBG( "a1: " + String( a1_ ) );
      DBG( "a2: " + String( a2_ ) );

      DBG( "" );
      DBG( "b1: " + String( b1_ ) );
      DBG( "b2: " + String( b2_ ) );

      DBG( "" );
      DBG( "c0: " + String( c0_ ) );
      DBG( "d0: " + String( d0_ ) );
   }
}


void BiquadFilter::processSampleInternal(
   double& sampleValue,

   double& x0,
   double& x1,
   double& x2,

   double& y1,
   double& y2 )
{
   x0 = sampleValue;

   double y0 = a0_ * x0
               + a1_ * x1
               + a2_ * x2
               - b1_ * y1
               - b2_ * y2;

   // order IS important ...
   x2 = x1;
   x1 = x0;

   y2 = y1;
   y1 = y0;

   // save processed sample
   if ( d0_ != 0.0 ) {
      sampleValue = c0_ * y0 + d0_ * x0;
   } else {
      sampleValue = y0;
   }
}


void BiquadFilter::processSample(
   double& sampleValue,
   const int channel )
{
   jassert( channel >= 0 );
   jassert( channel <= numberOfChannels_ );

   double& x0 = x0_.getReference( channel );
   double& x1 = x1_.getReference( channel );
   double& x2 = x2_.getReference( channel );

   double& y1 = y1_.getReference( channel );
   double& y2 = y2_.getReference( channel );

   processSampleInternal( sampleValue, x0, x1, x2, y1, y2 );
}


void BiquadFilter::processInPlace(
   AudioBuffer<double>& buffer )
{
   jassert( buffer.getNumChannels() == numberOfChannels_ );

   for ( int channel = 0; channel < numberOfChannels_; ++channel ) {
      double& x0 = x0_.getReference( channel );
      double& x1 = x1_.getReference( channel );
      double& x2 = x2_.getReference( channel );

      double& y1 = y1_.getReference( channel );
      double& y2 = y2_.getReference( channel );

      for ( int sampleId = 0; sampleId < buffer.getNumSamples(); ++sampleId ) {
         double sampleValue = buffer.getSample( channel, sampleId );
         processSampleInternal( sampleValue, x0, x1, x2, y1, y2 );
         buffer.setSample( channel, sampleId, sampleValue );
      }
   }
}


AudioBuffer<double> BiquadFilter::process(
   const AudioBuffer<double>& inputBuffer )
{
   jassert( inputBuffer.getNumChannels() == numberOfChannels_ );

   AudioBuffer<double> outputBuffer( numberOfChannels_,
                                     inputBuffer.getNumSamples() );

   for ( int channel = 0; channel < numberOfChannels_; ++channel ) {
      double& x0 = x0_.getReference( channel );
      double& x1 = x1_.getReference( channel );
      double& x2 = x2_.getReference( channel );

      double& y1 = y1_.getReference( channel );
      double& y2 = y2_.getReference( channel );

      for ( int sampleId = 0; sampleId < inputBuffer.getNumSamples(); ++sampleId ) {
         double sampleValue = inputBuffer.getSample( channel, sampleId );
         processSampleInternal( sampleValue, x0, x1, x2, y1, y2 );
         outputBuffer.setSample( channel, sampleId, sampleValue );
      }
   }

   return outputBuffer;
}

} // namespace
