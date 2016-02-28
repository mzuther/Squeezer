/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2016 Martin Zuther (http://www.mzuther.de/)

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

#ifndef __FRUT_WIDGET_METER_ORIENTATION_H__
#define __FRUT_WIDGET_METER_ORIENTATION_H__


/// Orientation for meter bars and meter segments.
///
enum GenericMeterOrientation
{
    /// horizontal meter (bottom to top)
    horizontal = 0,

    /// inverted horizontal meter (top to bottom)
    horizontalInverted,

    /// vertical meter (left to right)
    vertical,

    /// inverted vertical meter (right to left)
    verticalInverted
};


#endif  // __FRUT_WIDGET_METER_ORIENTATION_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
