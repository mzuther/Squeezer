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

namespace frut::widgets
{

Orientation::Orientation( Orientation::orientations ori )
{
   setOrientation( ori );
};


Orientation::Orientation( int angle )
{
   setAngle( angle );
};


Orientation::orientations Orientation::getOrientation() const
{
   Orientation::orientations result;

   switch ( angle_ ) {
      case Orientation::orientations::bottomToTop:

         result = Orientation::orientations::bottomToTop;
         break;

      case Orientation::orientations::leftToRight:

         result = Orientation::orientations::leftToRight;
         break;

      case Orientation::orientations::topToBottom:

         result = Orientation::orientations::topToBottom;
         break;

      case Orientation::orientations::rightToLeft:

         result = Orientation::orientations::rightToLeft;
         break;

      default:

         result = Orientation::orientations::other;
         break;
   }

   return result;
}


void Orientation::setOrientation( Orientation::orientations ori )
{
   setAngle( static_cast<int>( ori ) );
};


int Orientation::getAngle() const
{
   return angle_;
};


void Orientation::setAngle( int angle )
{
   angle_ = angle % 360;
};


bool Orientation::isVertical() const
{
   bool result;

   switch ( angle_ ) {
      case Orientation::orientations::bottomToTop:

         result = true;
         break;

      case Orientation::orientations::topToBottom:

         result = true;
         break;

      default:

         result = false;
         break;
   }

   return result;
}


bool Orientation::isInverted() const
{
   bool result;

   switch ( angle_ ) {
      case Orientation::orientations::topToBottom:

         result = true;
         break;

      case Orientation::orientations::rightToLeft:

         result = true;
         break;

      default:

         result = false;
         break;
   }

   return result;
}


Orientation Orientation::mirror()
{
   return Orientation( angle_ + 180 );
};


Orientation Orientation::turnLeft()
{
   return Orientation( angle_ + 90 );
};


Orientation Orientation::turnRight()
{
   return Orientation( angle_ - 90 );
};


AffineTransform Orientation::getTransform( Rectangle<int> widgetBounds ) const
{
   if ( angle_ == Orientation::orientations::bottomToTop ) {
      return AffineTransform();
   }

   auto pivot = widgetBounds.getCentre().toFloat();

   return AffineTransform::rotation(
             degreesToRadians( static_cast<float>( angle_ ) ),
             pivot.getX(),
             pivot.getY() );
}


Orientation::operator String() const
{
   String output = "Orientation: " + String( angle_ ) + "°";

   switch ( angle_ ) {
      case Orientation::orientations::bottomToTop:

         output += " (bottom to top)";
         break;

      case Orientation::orientations::leftToRight:

         output += " (left to right)";
         break;

      case Orientation::orientations::topToBottom:

         output += " (top to bottom)";
         break;

      case Orientation::orientations::rightToLeft:

         output += " (right to left)";
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

} // namespace
