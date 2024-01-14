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

#ifndef FRUT_WIDGETS_SIGNAL_LED_H
#define FRUT_WIDGETS_SIGNAL_LED_H

namespace frut::widgets
{

/// Signal LED component.  This widget loads three images of the same
/// size which will be displayed according to the meter's level.
///
/// | %Level     | %Display                            |
/// | :--------: | ----------------------------------- |
/// | <= 0.0     | *imageOff*                          |
/// | in between | blend of *imageLow* and *imageHigh* |
/// | >= 1.0     | *imageHigh*                         |
///
class SignalLed :
   public Component
{
public:
   SignalLed();

   virtual void paint( Graphics& g ) override;

   void setLevel( float level );
   void setImages( const Image& imageOff,
                   const Image& imageLow,
                   const Image& imageHigh );

protected:
   float level_;

   Image imageOff_;
   Image imageLow_;
   Image imageHigh_;

private:
   JUCE_LEAK_DETECTOR( SignalLed );
};

} // namespace

#endif  // FRUT_WIDGETS_SIGNAL_LED_H
