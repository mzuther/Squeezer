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

void MeterBarLevel::create(int crestFactor, Orientation orientation,
                           bool discreteMeter, int mainSegmentHeight,
                           int segmentWidth)

{
    GenericMeterBar::create();

    segmentColours_.clear();

    segmentColours_.add(Colour(0.00f, 1.0f, 1.0f, 1.0f));  // red
    segmentColours_.add(Colour(0.18f, 1.0f, 1.0f, 1.0f));  // yellow
    segmentColours_.add(Colour(0.30f, 1.0f, 1.0f, 1.0f));  // green
    segmentColours_.add(Colour(0.58f, 1.0f, 1.0f, 1.0f));  // blue

    setSegmentWidth(segmentWidth);

    int nCrestFactor = crestFactor * 10;
    int nRange = 20;
    int nTrueLowerThreshold = -nRange;
    int nLowerThreshold = nTrueLowerThreshold + nCrestFactor;

    int nNumberOfBars = 23;

    for (int n = 0; n < nNumberOfBars; ++n)
    {
        int nColour;
        int segmentHeight;
        int nSpacingBefore;

        if (n == 0)
        {
            nColour = 0;

            // overload marker
            segmentHeight = 2 * mainSegmentHeight - 2;
            nSpacingBefore = 0;
        }
        else if (n == 1)
        {
            nColour = 0;
            segmentHeight = mainSegmentHeight;

            // spacing for overload marker
            nSpacingBefore = 2;
        }
        else if (nTrueLowerThreshold >= -80)
        {
            nColour = 0;
            segmentHeight = mainSegmentHeight;
            nSpacingBefore = 0;
        }
        else if (nTrueLowerThreshold >= -200)
        {
            nColour = 1;
            segmentHeight = mainSegmentHeight;
            nSpacingBefore = 0;
        }
        else
        {
            nColour = 2;
            segmentHeight = mainSegmentHeight;
        }

        float fLowerThreshold = nLowerThreshold * 0.1f;
        float fRange = nRange * 0.1f;
        bool hasHighestLevel = (n == 0) ? true : false;

        if (discreteMeter)
        {
            addDiscreteSegment(
                fLowerThreshold,
                fRange,
                hasHighestLevel,
                segmentHeight,
                nSpacingBefore,
                segmentColours_[nColour],
                segmentColours_[nColour].withAlpha(0.7f));
        }
        else
        {
            addContinuousSegment(
                fLowerThreshold,
                fRange,
                hasHighestLevel,
                segmentHeight,
                nSpacingBefore,
                segmentColours_[nColour],
                segmentColours_[nColour].withAlpha(0.7f));
        }

        nTrueLowerThreshold -= nRange;
        nLowerThreshold = nTrueLowerThreshold + nCrestFactor;
    }

    // set orientation here to save some processing power
    setOrientation(orientation);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
