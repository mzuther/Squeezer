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

#ifndef FRUT_WIDGETS_NEEDLE_METER_H
#define FRUT_WIDGETS_NEEDLE_METER_H

namespace frut::widgets
{

/// Meter component with a needle that moves according to the input.
///
class NeedleMeter :
   public Component
{
public:
   NeedleMeter();

   void setImages( const Image& imageBackground,
                   const Image& imageNeedle,
                   int needleSpacingLeft,
                   int needleSpacingTop );

   void setValue( float value );

   virtual void paint( Graphics& g ) override;
   virtual void resized() override;

protected:
   int needlePosition_;
   int needleTravelPath_;
   bool isVerticalMeter_;

   int needleSpacingLeft_;
   int needleSpacingTop_;

   Image imageBackground_;
   Image imageNeedle_;

private:
   JUCE_LEAK_DETECTOR( NeedleMeter );
};

} // namespace

#endif  // FRUT_WIDGETS_NEEDLE_METER_H
