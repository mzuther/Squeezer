/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013-2015 Martin Zuther (http://www.mzuther.de/)

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

MeterBarLevel::MeterBarLevel()
{
    arrHues.add(0.00f);  // red
    arrHues.add(0.18f);  // yellow
    arrHues.add(0.30f);  // green
    arrHues.add(0.58f);  // blue

    for (int n = 0; n < arrHues.size(); n++)
    {
        arrPeakColours.add(Colour(arrHues[n], 1.0f, 1.0f, 0.7f));
    }
}


MeterBarLevel::~MeterBarLevel()
{
}


void MeterBarLevel::create(int crestFactor, int nMainSegmentHeight, Orientation orientation)
{
    GenericMeterBar::create();
    setOrientation(orientation);

    int nCrestFactor = crestFactor * 10;
    int nRange = 20;
    int nTrueLowerThreshold = -nRange;
    int nLowerThreshold = nTrueLowerThreshold + nCrestFactor;

    int nNumberOfBars = 23;

    for (int n = 0; n < nNumberOfBars; n++)
    {
        int nColour;
        int nSegmentHeight;
        int nSpacingBefore;

        if (n == 0)
        {
            nColour = 0;

            // overload marker
            nSegmentHeight = 2 * nMainSegmentHeight - 2;
            nSpacingBefore = 0;
        }
        else if (n == 1)
        {
            nColour = 0;
            nSegmentHeight = nMainSegmentHeight;

            // spacing for overload marker
            nSpacingBefore = 2;
        }
        else if (nTrueLowerThreshold >= -80)
        {
            nColour = 0;
            nSegmentHeight = nMainSegmentHeight;
            nSpacingBefore = 0;
        }
        else if (nTrueLowerThreshold >= -200)
        {
            nColour = 1;
            nSegmentHeight = nMainSegmentHeight;
            nSpacingBefore = 0;
        }
        else
        {
            nColour = 2;
            nSegmentHeight = nMainSegmentHeight;
        }

        float fLowerThreshold = nLowerThreshold * 0.1f;
        float fRange = nRange * 0.1f;
        bool bHasHighestLevel = (n == 0) ? true : false;

        addSegment(fLowerThreshold, fRange, bHasHighestLevel, nSegmentHeight, nSpacingBefore, arrHues[nColour], arrPeakColours[nColour]);

        nTrueLowerThreshold -= nRange;
        nLowerThreshold = nTrueLowerThreshold + nCrestFactor;
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
