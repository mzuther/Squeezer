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

#include "meter_bar_gain_reduction.h"


void MeterBarGainReduction::create(Orientation orientation, bool discreteMeter,
                                   int segmentHeight, int segmentWidth)

{
    GenericMeterBar::create();

    segmentColours_.clear();

    segmentColours_.add(Colour(0.00f, 1.0f, 1.0f, 1.0f));  // red
    segmentColours_.add(Colour(0.18f, 1.0f, 1.0f, 1.0f));  // yellow
    segmentColours_.add(Colour(0.30f, 1.0f, 1.0f, 1.0f));  // green
    segmentColours_.add(Colour(0.58f, 1.0f, 1.0f, 1.0f));  // blue

    setSegmentWidth(segmentWidth);

    setUpwardExpansion(false);

    int nTrueLowerThreshold = 0;
    int nRange = 10;
    int nNumberOfBars = 24;

    for (int n = 0; n < nNumberOfBars; ++n)
    {
        int nColour;

        if (n % 6 == 5)
        {
            nColour = 1;
        }
        else
        {
            nColour = 3;
        }

        float fTrueLowerThreshold = nTrueLowerThreshold * 0.1f;
        float fRange = nRange * 0.1f;

        bool bHasHighestLevel = (n == (nNumberOfBars - 1)) ? true : false;
        int nSpacingBefore = 0;

        if (discreteMeter)
        {
            addDiscreteSegment(
                fTrueLowerThreshold,
                fRange,
                bHasHighestLevel,
                segmentHeight,
                nSpacingBefore,
                segmentColours_[nColour],
                segmentColours_[nColour].withAlpha(0.7f));
        }
        else
        {
            addContinuousSegment(
                fTrueLowerThreshold,
                fRange,
                bHasHighestLevel,
                segmentHeight,
                nSpacingBefore,
                segmentColours_[nColour],
                segmentColours_[nColour].withAlpha(0.7f));
        }

        nTrueLowerThreshold += nRange;
    }

    // set orientation here to save some processing power
    setOrientation(orientation);
}


void MeterBarGainReduction::setUpwardExpansion(bool upwardExpansion)
{
    upwardExpansion_ = upwardExpansion;
    invertMeter(upwardExpansion_);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
