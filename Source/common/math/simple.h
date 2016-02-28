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

#ifndef __FRUT_MATH_SIMPLE_H__
#define __FRUT_MATH_SIMPLE_H__


/// Simple class for version info and common functions.
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
    static int round(float x)
    {
        // value is greater than (or exactly) zero
        if (x >= 0.0f)
        {
            // round away from zero
            x += 0.5f;
        }
        // value is below zero
        else
        {
            // round away from zero
            x -= 0.5f;
        }

        // remove remainder and return it
        return static_cast<int>(x);
    }


    /// Round given value up.  Will always round **away** from zero!
    ///
    /// @param x value to be rounded
    ///
    /// @return rounded value
    ///
    static int roundUp(float x)
    {
        int floor = static_cast<int>(x);

        // value is exactly zero
        if (x == 0.0f)
        {
            // return value
            return x;
        }
        // value is greater than zero
        else if (x > 0.0f)
        {
            // check whether value needs to be rounded
            if ((x - floor) > 0.0f)
            {
                // round away from zero
                floor += 1;
            }
        }
        // value is below zero
        else
        {
            // check whether value needs to be rounded
            if ((x - floor) < 0.0f)
            {
                // round away from zero
                floor -= 1;
            }
        }

        // return rounded value
        return floor;
    }


    /// Round given value up.  Will always round **down to** zero!
    ///
    /// @param x value to be rounded
    ///
    /// @return rounded value
    ///
    static int roundDown(float x)
    {
        // remove remainder and return it
        return static_cast<int>(x);
    }
};


#endif  // __FRUT_MATH_SIMPLE_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
