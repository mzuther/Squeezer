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
namespace widgets
{

/// Default constructor.
///
Orientation::Orientation()
{
   setAngle( Orientation::orientations::vertical );
};


Orientation::Orientation( Orientation::orientations newOrientation )
{
   setOrientation( newOrientation );
};


Orientation::Orientation( int newAngle )
{
   setAngle( newAngle );
};


Orientation::orientations Orientation::getOrientation() const
{
   switch ( angle ) {
      case Orientation::orientations::vertical:

         return Orientation::orientations::vertical;
         break;

      case Orientation::orientations::horizontal:

         return Orientation::orientations::horizontal;
         break;

      case Orientation::orientations::verticalInverted:

         return Orientation::orientations::verticalInverted;
         break;

      case Orientation::orientations::horizontalInverted:

         return Orientation::orientations::horizontalInverted;
         break;

      default:

         return Orientation::orientations::other;
         break;
   }
}


void Orientation::setOrientation( Orientation::orientations newOrientation )
{
   setAngle( static_cast<int>( newOrientation ) );
};


int Orientation::getAngle() const
{
   return angle;
};


void Orientation::setAngle( int newAngle )
{
   angle = newAngle % 360;
};


bool Orientation::isVertical() const
{
   switch ( angle ) {
      case Orientation::orientations::vertical:

         return true;
         break;

      case Orientation::orientations::verticalInverted:

         return true;
         break;

      default:

         return false;
         break;
   }
}


bool Orientation::isInverted() const
{
   switch ( angle ) {
      case Orientation::orientations::verticalInverted:

         return true;
         break;

      case Orientation::orientations::horizontalInverted:

         return true;
         break;

      default:

         return false;
         break;
   }
}


Orientation Orientation::mirror()
{
   return Orientation( angle + 180 );
};


Orientation Orientation::turnLeft()
{
   return Orientation( angle + 90 );
};


Orientation Orientation::turnRight()
{
   return Orientation( angle - 90 );
};


AffineTransform Orientation::getTransform( Rectangle<int> bounds ) const
{
   if ( angle == Orientation::orientations::vertical ) {
      return AffineTransform();
   }

   auto pivot = bounds.getCentre().toFloat();

   return AffineTransform::rotation(
             degreesToRadians( static_cast<float>( angle ) ),
             pivot.getX(),
             pivot.getY() );
}


Orientation::operator String() const
{
   String output = "Orientation: " + String( angle ) + "°";

   switch ( angle ) {
      case Orientation::orientations::vertical:

         output += " (vertical)";
         break;

      case Orientation::orientations::horizontal:

         output += " (horizontal)";
         break;

      case Orientation::orientations::verticalInverted:

         output += " (vertical inverted)";
         break;

      case Orientation::orientations::horizontalInverted:

         output += " (horizontal inverted)";
         break;

      default:

         // nothing to be done, really
         break;
   }

   return output;
}


bool operator==( const Orientation& a,
                 const Orientation& b )
{
   return ( a.getAngle() == b.getAngle() );
}


bool operator!=( const Orientation& a,
                 const Orientation& b )
{
   return ! ( a == b );
}


bool operator==( const Orientation& a,
                 Orientation::orientations ori )
{
   return ( a.getOrientation() == ori );
}


bool operator!=( const Orientation& a,
                 Orientation::orientations ori )
{
   return ! ( a == ori );
}


}
}
