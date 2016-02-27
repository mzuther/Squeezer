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

#include "generic_meter_segment.h"

/// Create a new base meter segment.
///
GenericMeterSegment::GenericMeterSegment()
{
    // set initial orientation
    setOrientation(GenericMeterOrientation::vertical);
}


/// Destructor.
GenericMeterSegment::~GenericMeterSegment()
{
}


/// Get the segment's orientation.
///
/// @return current orientation
///
GenericMeterOrientation GenericMeterSegment::getOrientation()
{
    return orientation_;
}


/// Set the segment's orientation.
///
/// @param orientation new orientation
///
void GenericMeterSegment::setOrientation(
    GenericMeterOrientation orientation)

{
    // update segment's orientation
    orientation_ = orientation;

    // redraw meter segment
    repaint();
}


/// Set normal (average) levels.  *Use this only if you completely
/// disregard discrete (peak) levels!*
///
/// @param normalLevel new normal level
///
/// @param normalLevelPeak new normal peak level
///
void GenericMeterSegment::setNormalLevels(
    float normalLevel, float normalLevelPeak)

{
    // lowest level of a 24-bit-signal in decibels
    float initialLevel = -144.0f;

    setLevels(normalLevel, normalLevelPeak, initialLevel, initialLevel);
}


/// Set discrete (peak) levels.  *Use this only if you completely
/// disregard normal (average) levels!*
///
/// @param discreteLevel new discrete level
///
/// @param discreteLevelPeak new discrete peak level
///
void GenericMeterSegment::setDiscreteLevels(
    float discreteLevel, float discreteLevelPeak)

{
    // lowest level of a 24-bit-signal in decibels
    float initialLevel = -144.0f;

    setLevels(initialLevel, initialLevel, discreteLevel, discreteLevelPeak);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
