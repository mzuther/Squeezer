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

#ifndef FRUT_WIDGETS_ORIENTATION_H
#define FRUT_WIDGETS_ORIENTATION_H

namespace frut
{
namespace widgets
{

/// Orientation for widgets.
///
class Orientation
{
public:
   enum orientations { // public namespace!
      bottomToTop = 0,
      leftToRight = 90,
      topToBottom = 180,
      rightToLeft = 270,

      /// widget's orientation does not match any direction
      other = -1
   };

   explicit Orientation( Orientation::orientations ori );
   explicit Orientation( int angle );

   Orientation::orientations getOrientation() const;
   void setOrientation( Orientation::orientations ori );

   int getAngle() const;
   void setAngle( int angle );

   bool isVertical() const;
   bool isInverted() const;

   Orientation mirror();
   Orientation turnLeft();
   Orientation turnRight();

   AffineTransform getTransform( Rectangle<int> widgetBounds ) const;
   operator String() const;

private:
   int angle_;
};

bool operator==( const Orientation& a,
                 const Orientation& b );

bool operator!=( const Orientation& a,
                 const Orientation& b );

bool operator==( const Orientation& a,
                 Orientation::orientations ori );

bool operator!=( const Orientation& a,
                 Orientation::orientations ori );

}
}

#endif  // FRUT_WIDGETS_ORIENTATION_H
