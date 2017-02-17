/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013-2016 Martin Zuther (http://www.mzuther.de/)

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

#ifndef __METER_BAR_GAIN_REDUCTION_H__
#define __METER_BAR_GAIN_REDUCTION_H__

#include "FrutHeader.h"


class MeterBarGainReduction :
    public frut::widget::MeterBar
{
public:
    void create(frut::widget::Orientation orientation,
                bool discreteMeter,
                int mainSegmentHeight,
                const Array<Colour> &segmentColours);

    void setUpwardExpansion(bool upwardExpansion);

protected:
    /// Colour ID selector for meter segments.
    enum colourSelector  // protected namespace
    {
        /// normal (usually blue)
        normal = 0,

        /// notify (usually yellow)
        notify
    };

    bool upwardExpansion_;

private:
    JUCE_LEAK_DETECTOR(MeterBarGainReduction);
};


#endif  // __METER_BAR_GAIN_REDUCTION_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
