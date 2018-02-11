/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2018 Martin Zuther (http://www.mzuther.de/)

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

#pragma once


/// Base class for meter segment components.
///
/// @see MeterBar
///
class MeterSegment :
    public Component
{
public:
    MeterSegment();
    virtual ~MeterSegment();

    virtual void setNormalLevels(float normalLevel,
                                 float normalLevelPeak);

    virtual void setDiscreteLevels(float discreteLevel,
                                   float discreteLevelPeak);

    /// Set discrete (peak) and normal (average) levels.
    ///
    /// @param normalLevel new normal level
    ///
    /// @param normalLevelPeak new normal peak level
    ///
    /// @param discreteLevel new discrete level
    ///
    /// @param discreteLevelPeak new discrete peak level
    ///
    virtual void setLevels(float normalLevel,
                           float normalLevelPeak,
                           float discreteLevel,
                           float discreteLevelPeak) = 0 ;

    virtual widget::Orientation getOrientation();
    virtual void setOrientation(widget::Orientation orientation);

protected:
    widget::Orientation orientation_;

};


// Local Variables:
// ispell-local-dictionary: "british"
// End:
