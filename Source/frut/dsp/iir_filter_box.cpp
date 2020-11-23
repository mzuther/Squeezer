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

namespace frut::dsp
{

IirFilterBox::IirFilterBox(
   const int numberOfChannels,
   const double sampleRate ) :
   BiquadFilter( numberOfChannels ),
   sampleRate_( sampleRate )
{
}


double IirFilterBox::bandwidthToQualityFactor(
   const double octaveBandwidth )
{
   double bw2 = exp2( octaveBandwidth );
   double qualityFactor = sqrt( bw2 ) / ( bw2 - 1.0 );

   return qualityFactor;
}


// Pirkle: "Designing Audio Effect Plug-ins in C++" (2013), p. 182
// 6.6.1 First-Order LPF and HPF
void IirFilterBox::passFilterFirstOrder(
   const double cutoffFrequencyInHz,
   const bool isLowPass )
{
   if ( isLowPass ) {
      double theta_c = 2.0 * M_PI * cutoffFrequencyInHz / sampleRate_;

      double gamma = cos( theta_c ) / ( 1.0 + sin( theta_c ) );

      double a0 = ( 1.0 - gamma ) / 2.0;
      double a1 = a0;
      double a2 = 0.0;

      double b1 = -gamma;
      double b2 = 0.0;

      setCoefficients( a0, a1, a2, b1, b2 );
   } else {
      double theta_c = 2.0 * M_PI * cutoffFrequencyInHz / sampleRate_;

      double gamma = cos( theta_c ) / ( 1.0 + sin( theta_c ) );

      double a0 = ( 1.0 + gamma ) / 2.0;
      double a1 = -a0;
      double a2 = 0.0;

      double b1 = -gamma;
      double b2 = 0.0;

      setCoefficients( a0, a1, a2, b1, b2 );
   }
}


// Pirkle: "Designing Audio Effect Plug-ins in C++" (2013), p. 183
// 6.6.2 Second-Order LPF and HPF
void IirFilterBox::passFilterSecondOrder(
   const double cutoffFrequencyInHz,
   const double qualityFactor,
   const bool isLowPass )
{
   if ( isLowPass ) {
      double theta_c = 2.0 * M_PI * cutoffFrequencyInHz / sampleRate_;
      double d = 1.0 / qualityFactor;

      double beta = 0.5 * ( 1.0 - d / 2.0 * sin( theta_c ) ) /
                    ( 1.0 + d / 2.0 * sin( theta_c ) );
      double gamma = ( 0.5 + beta ) * cos( theta_c );

      double a0 = ( 0.5 + beta - gamma ) / 2.0;
      double a1 = 0.5 + beta - gamma;
      double a2 = a0;

      double b1 = -2.0 * gamma;
      double b2 = 2.0 * beta;

      setCoefficients( a0, a1, a2, b1, b2 );
   } else {
      double theta_c = 2.0 * M_PI * cutoffFrequencyInHz / sampleRate_;
      double d = 1.0 / qualityFactor;

      double beta = 0.5 * ( 1.0 - d / 2.0 * sin( theta_c ) ) /
                    ( 1.0 + d / 2.0 * sin( theta_c ) );
      double gamma = ( 0.5 + beta ) * cos( theta_c );

      double a0 = ( 0.5 + beta + gamma ) / 2.0;
      double a1 = -( 0.5 + beta + gamma );
      double a2 = a0;

      double b1 = -2.0 * gamma;
      double b2 = 2.0 * beta;

      setCoefficients( a0, a1, a2, b1, b2 );
   }
}


// Pirkle: "Designing Audio Effect Plug-ins in C++" (2013), p. 189
// 6.7.2 First-Order Shelving Filters
void IirFilterBox::shelvingFilterFirstOrder(
   const double cutoffFrequencyInHz,
   const double gainInDecibels,
   const bool isLowShelving )
{
   if ( isLowShelving ) {
      double theta_c = 2.0 * M_PI * cutoffFrequencyInHz / sampleRate_;
      double mu = pow( 10, gainInDecibels / 20.0 );

      double beta = 4.0 / ( 1.0 + mu );
      double delta = beta * tan( theta_c / 2.0 );
      double gamma = ( 1.0 - delta ) / ( 1.0 + delta );

      double a0 = ( 1.0 - gamma ) / 2.0;
      double a1 = a0;
      double a2 = 0.0;

      double b1 = -gamma;
      double b2 = 0.0;

      double c0 = mu - 1.0;
      double d0 = 1.0;

      setCoefficients( a0, a1, a2, b1, b2, c0, d0 );
   } else {
      double theta_c = 2.0 * M_PI * cutoffFrequencyInHz / sampleRate_;
      double mu = pow( 10, gainInDecibels / 20.0 );

      double beta = ( 1.0 + mu ) / 4.0;
      double delta = beta * tan( theta_c / 2.0 );
      double gamma = ( 1.0 - delta ) / ( 1.0 + delta );

      double a0 = ( 1.0 + gamma ) / 2.0;
      double a1 = -a0;
      double a2 = 0.0;

      double b1 = -gamma;
      double b2 = 0.0;

      double c0 = mu - 1.0;
      double d0 = 1.0;

      setCoefficients( a0, a1, a2, b1, b2, c0, d0 );
   }
}


// Pirkle: "Designing Audio Effect Plug-ins in C++" (2013), p. 191
// 6.7.3 Second-Order Parametric/Peaking Filter: Non-Constant-Q
void IirFilterBox::peakingFilterVariableQ(
   const double cutoffFrequencyInHz,
   const double gainInDecibels,
   const double qualityFactor )
{
   double theta_c = 2.0 * M_PI * cutoffFrequencyInHz / sampleRate_;
   double mu = pow( 10, gainInDecibels / 20.0 );

   double zeta = 4.0 / ( 1.0 + mu );
   double theta_c_by_quality = theta_c / ( 2.0 * qualityFactor );

   // make sure the tangens gives meaningful results
   double limit = M_PI / 2.0 - 0.15;

   if ( theta_c_by_quality >= limit ) {
      // this results in a higher quality factor (i.e. smaller
      // filter bandwith)
      theta_c_by_quality = limit;

      DBG( "[IIR Filter] clamped down quality factor!" );
   }

   double zeta_tan = zeta * tan( theta_c_by_quality );

   double beta = 0.5 * ( 1.0 - zeta_tan ) / ( 1.0 + zeta_tan );
   double gamma = ( 0.5 + beta ) * cos( theta_c );

   double a0 = 0.5 - beta;
   double a1 = 0.0;
   double a2 = beta - 0.5;

   double b1 = -2.0 * gamma;
   double b2 = 2.0 * beta;

   double c0 = mu - 1.0;
   double d0 = 1.0;

   setCoefficients( a0, a1, a2, b1, b2, c0, d0 );
}


// Pirkle: "Designing Audio Effect Plug-ins in C++" (2013), p. 192
// 6.7.4 Second-Order Parametric/Peaking Filter: Constant-Q
void IirFilterBox::peakingFilterConstantQ(
   const double cutoffFrequencyInHz,
   const double gainInDecibels,
   const double qualityFactor )
{
   double K = tan( M_PI * cutoffFrequencyInHz / sampleRate_ );
   double V0 = pow( 10, gainInDecibels / 20.0 );

   double K_squared = K * K;
   double K_by_quality = K / qualityFactor;

   double temp_d0 = 1.0 + K_by_quality + K_squared;
   double temp_e0 = 1.0 + K_by_quality / V0 + K_squared;

   double alpha = 1.0 + K_by_quality * V0 + K_squared;
   double beta = 2.0 * ( K_squared - 1.0 );
   double gamma = 1.0 - K_by_quality * V0 + K_squared;
   double delta = 1.0 - K_by_quality + K_squared;
   double eta = 1.0 - K_by_quality / V0 + K_squared;

   // boost: initialise filter coefficients
   if ( gainInDecibels > 0.0 ) {
      double a0 = alpha / temp_d0;
      double a1 = beta / temp_d0;
      double a2 = gamma / temp_d0;

      double b1 = a1;
      double b2 = delta / temp_d0;

      setCoefficients( a0, a1, a2, b1, b2 );
      // attenuation: initialise filter coefficients
   } else {
      double a0 = temp_d0 / temp_e0;
      double a1 = beta / temp_e0;
      double a2 = delta / temp_e0;

      double b1 = a1;
      double b2 = eta / temp_e0;

      setCoefficients( a0, a1, a2, b1, b2 );
   }
}

} // namespace
