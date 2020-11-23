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

#ifndef FRUT_MATH_SIMPLE_MATH_H
#define FRUT_MATH_SIMPLE_MATH_H

namespace frut::math
{

/// Simple class for common math functions.
///
class SimpleMath
{
public:
   /// Round given value to nearest integer.  Will always round
   /// **away** from zero!
   ///
   /// @param x value to be rounded
   ///
   /// @return rounded value
   ///
   static int round( float x )
   {
      // value is greater than (or exactly) zero
      if ( x >= 0.0f ) {
         // round away from zero
         x += 0.5f;
         // value is below zero
      } else {
         // round away from zero
         x -= 0.5f;
      }

      // remove remainder and return it
      return static_cast<int>( x );
   }


   /// Round given value up.  Will always round **away** from zero!
   ///
   /// @param x value to be rounded
   ///
   /// @return rounded value
   ///
   static int roundUp( float x )
   {
      int floor = static_cast<int>( x );

      // value is exactly zero
      if ( x == 0.0f ) {
         // return value
         return floor;
         // value is greater than zero
      } else if ( x > 0.0f ) {
         // check whether value needs to be rounded
         if ( ( x - floor ) > 0.0f ) {
            // round away from zero
            floor += 1;
         }

         // value is below zero
      } else {
         // check whether value needs to be rounded
         if ( ( x - floor ) < 0.0f ) {
            // round away from zero
            floor -= 1;
         }
      }

      // return rounded value
      return floor;
   }


   /// Round given value down.  Will always round **down to** zero!
   ///
   /// @param x value to be rounded
   ///
   /// @return rounded value
   ///
   static int roundDown( float x )
   {
      // remove remainder and return it
      return static_cast<int>( x );
   }


   /// Round given float to nearest integer with given number of
   /// decimal places.  Will always round **away** from zero!
   ///
   /// @param x value to be rounded
   ///
   /// @param decimal_places number of decimal places for formatting
   ///        the value
   ///
   /// @return rounded value
   ///
   static float roundFloat( float x, int decimal_places )
   {
      jassert( decimal_places >= 0 );

      float decimal_factor = powf( 10, ( float ) decimal_places );
      x = ( float ) math::SimpleMath::round( x * decimal_factor );
      return x / decimal_factor;
   }


   /// Round given float up with given number of decimal places.
   /// Will always round **away** from zero!
   ///
   /// @param x value to be rounded
   ///
   /// @param decimal_places number of decimal places for formatting
   ///        the value
   ///
   /// @return rounded value
   ///
   static float roundUpFloat( float x, int decimal_places )
   {
      jassert( decimal_places >= 0 );

      float decimal_factor = powf( 10, ( float ) decimal_places );
      x = ( float ) math::SimpleMath::roundUp( x * decimal_factor );
      return x / decimal_factor;
   }


   /// Round given float down with given number of decimal places.
   /// Will always round **down to** zero!
   ///
   /// @param x value to be rounded
   ///
   /// @param decimal_places number of decimal places for formatting
   ///        the value
   ///
   /// @return rounded value
   ///
   static float roundDownFloat( float x, int decimal_places )
   {
      jassert( decimal_places >= 0 );

      float decimal_factor = powf( 10, ( float ) decimal_places );
      x = ( float ) math::SimpleMath::roundDown( x * decimal_factor );
      return x / decimal_factor;
   }


   /// Round given float to nearest step.  Will always round **away**
   /// from zero!
   ///
   /// @param x value to be stepped
   ///
   /// @param step_size values are quantised using this value.  For
   ///        example, a step size of 0.2 will result in the values
   ///        0.0, 0.2, 0.4, 0.6, and so on.
   ///
   /// @return stepped value
   ///
   static float stepFloat( float x, float step_size )
   {
      jassert( step_size != 0.0f );

      float decimal_factor = 1.0f / step_size;
      x = ( float ) math::SimpleMath::round( x * decimal_factor );
      return x / decimal_factor;
   }


   /// Round given float up to next step.  Will always round **away**
   /// from zero!
   ///
   /// @param x value to be stepped
   ///
   /// @param step_size values are quantised using this value.  For
   ///        example, a step size of 0.2 will result in the values
   ///        0.0, 0.2, 0.4, 0.6, and so on.
   ///
   /// @return stepped value
   ///
   static float stepUpFloat( float x, float step_size )
   {
      jassert( step_size != 0.0f );

      float decimal_factor = 1.0f / step_size;
      x = ( float ) math::SimpleMath::roundUp( x * decimal_factor );
      return x / decimal_factor;
   }


   /// Round given float down to previous step.  Will always round
   /// **down to** zero!
   ///
   /// @param x value to be stepped
   ///
   /// @param step_size values are quantised using this value.  For
   ///        example, a step size of 0.2 will result in the values
   ///        0.0, 0.2, 0.4, 0.6, and so on.
   ///
   /// @return stepped value
   ///
   static float stepDownFloat( float x, float step_size )
   {
      jassert( step_size != 0.0f );

      float decimal_factor = 1.0f / step_size;
      x = ( float ) math::SimpleMath::roundDown( x * decimal_factor );
      return x / decimal_factor;
   }
};

} // namespace

#endif  // FRUT_MATH_SIMPLE_MATH_H
