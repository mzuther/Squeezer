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

MeterBarGainReduction::MeterBarGainReduction()
{
    arrHues.add(0.00f);  // red
    arrHues.add(0.18f);  // yellow
    arrHues.add(0.30f);  // green
    arrHues.add(0.58f);  // blue

    for (int n = 0; n < arrHues.size(); ++n)
    {
        arrPeakColours.add(Colour(arrHues[n], 1.0f, 1.0f, 0.7f));
    }
}


void MeterBarGainReduction::create(int nMainSegmentWidth, int nMainSegmentHeight, Orientation orientation)
{
    GenericMeterBar::create();
    setSegmentWidth(nMainSegmentWidth);

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

        addSegment(fTrueLowerThreshold, fRange, bHasHighestLevel, nMainSegmentHeight, nSpacingBefore, arrHues[nColour], arrPeakColours[nColour]);

        nTrueLowerThreshold += nRange;
    }

    // set orientation here to save some processing power
    setOrientation(orientation);
}


void MeterBarGainReduction::setUpwardExpansion(bool upward_expansion)
{
    bUpwardExpansion = upward_expansion;
    invertMeter(bUpwardExpansion);
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
