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


/// Meter segment component.  This widget consists of a coloured
/// filled rectangle (meter segment) surrounded by a small coloured
/// rectangle (peak marker).  Both rectangles react to level changes
/// with a change in colour or visibility.
///
/// @see GenericMeterBar
///
class GenericMeterSegment : public Component
{
public:
    virtual ~GenericMeterSegment() { } ;

    virtual void setNormalLevels(float normalLevelNew,
                                 float normalLevelPeakNew) { } ;

    virtual void setDiscreteLevels(float discreteLevelNew,
                                   float discreteLevelPeakNew) { } ;

    virtual void setLevels(float normalLevelNew,
                           float normalLevelPeakNew,
                           float discreteLevelNew,
                           float discreteLevelPeakNew) { } ;
};


#endif  // __GENERIC_METER_SEGMENT_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
