/* ----------------------------------------------------------------------------

   MZ common JUCE
   ==============
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

#ifndef __GENERIC_METER_SEGMENT_H__
#define __GENERIC_METER_SEGMENT_H__

#include "JuceHeader.h"


/// Base class for Meter segment components.
///
/// @see GenericMeterBar
///
class GenericMeterSegment : public Component
{
public:
    /// Destructor.
    virtual ~GenericMeterSegment() { } ;

    /// Set normal (average) levels.  *Use this only if you completely
    /// disregard discrete (peak) levels!*
    ///
    /// @param normalLevelNew new normal level
    ///
    /// @param normalLevelPeakNew new normal peak level
    ///
    virtual void setNormalLevels(float normalLevelNew,
                                 float normalLevelPeakNew) { } ;

    /// Set discrete (peak) levels.  *Use this only if you completely
    /// disregard normal (average) levels!*
    ///
    /// @param discreteLevelNew new discrete level
    ///
    /// @param discreteLevelPeakNew new discrete peak level
    ///
    virtual void setDiscreteLevels(float discreteLevelNew,
                                   float discreteLevelPeakNew) { } ;

    /// Set discrete (peak) and normal (average) levels.
    ///
    /// @param normalLevelNew new normal level
    ///
    /// @param normalLevelPeakNew new normal peak level
    ///
    /// @param discreteLevelNew new discrete level
    ///
    /// @param discreteLevelPeakNew new discrete peak level
    ///
    virtual void setLevels(float normalLevelNew,
                           float normalLevelPeakNew,
                           float discreteLevelNew,
                           float discreteLevelPeakNew) { } ;
};


#endif  // __GENERIC_METER_SEGMENT_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
