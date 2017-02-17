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

#include "meter_bar_level.h"

void MeterBarLevel::create(int crestFactor,
                           frut::widget::Orientation orientation,
                           bool discreteMeter,
                           int mainSegmentHeight,
                           const Array<Colour> &segmentColours)

{
    frut::widget::MeterBar::create();

    crestFactor *= 10;

    int levelRange = 20;
    int trueLowerThreshold = -levelRange;
    int lowerThreshold = trueLowerThreshold + crestFactor;

    int numberOfBars = 23;

    for (int n = 0; n < numberOfBars; ++n)
    {
        int colourId;
        int segmentHeight;
        int spacingBefore = 0;

        if (n == 0)
        {
            colourId = colourSelector::overload;

            // overload marker
            segmentHeight = 2 * mainSegmentHeight - 2;
        }
        else if (n == 1)
        {
            colourId = colourSelector::overload;
            segmentHeight = mainSegmentHeight;

            // spacing for overload marker
            spacingBefore = 2;
        }
        else if (trueLowerThreshold >= -80)
        {
            colourId = colourSelector::overload;
            segmentHeight = mainSegmentHeight;
        }
        else if (trueLowerThreshold >= -200)
        {
            colourId = colourSelector::warning;
            segmentHeight = mainSegmentHeight;
        }
        else
        {
            colourId = colourSelector::fine;
            segmentHeight = mainSegmentHeight;
        }

        bool hasHighestLevel = (n == 0) ? true : false;

        if (discreteMeter)
        {
            // meter segment outlines overlap
            spacingBefore -= 1;
            segmentHeight += 1;

            addDiscreteSegment(
                lowerThreshold * 0.1f,
                levelRange * 0.1f,
                hasHighestLevel,
                segmentHeight,
                spacingBefore,
                segmentColours[colourId],
                segmentColours[colourId].withMultipliedBrightness(0.7f));
        }
        else
        {
            // meter segment outlines must not overlap

            addContinuousSegment(
                lowerThreshold * 0.1f,
                levelRange * 0.1f,
                (levelRange * 0.1f) / segmentHeight,
                hasHighestLevel,
                segmentHeight,
                spacingBefore,
                segmentColours[colourId],
                segmentColours[colourId].withMultipliedBrightness(0.7f));
        }

        trueLowerThreshold -= levelRange;
        lowerThreshold = trueLowerThreshold + crestFactor;
    }

    // set orientation here to save some processing power
    setOrientation(orientation);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
