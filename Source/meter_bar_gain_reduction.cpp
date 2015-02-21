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

#include "meter_bar_gain_reduction.h"

MeterBarGainReduction::MeterBarGainReduction(const String &componentName, int pos_x, int pos_y, int width, int number_of_bars, int segment_height)
{
    setName(componentName);

    // this component does not have any transparent areas (increases
    // performance on redrawing)
    setOpaque(true);

    nNumberOfBars = number_of_bars;
    nSegmentHeight = segment_height;

    nPosX = pos_x;
    nPosY = pos_y;
    nWidth = width;
    nHeight = nNumberOfBars * nSegmentHeight + 1;

    fGainReduction = 0.0f;
    fGainReductionPeak = 0.0f;

    bUpwardExpansion = false;

    int nThreshold = 0;
    int nThresholdDiff = 10;
    float fRange = 1.0f;

    Array<float> arrHues;

    arrHues.add(0.00f);  // red
    arrHues.add(0.18f);  // yellow
    arrHues.add(0.30f);  // green
    arrHues.add(0.58f);  // blue

    for (int n = 0; n < nNumberOfBars; n++)
    {
        int nColor;

        if (n % 6 == 5)
        {
            nColor = 1;
        }
        else
        {
            nColor = 3;
        }

        GenericMeterSegment *pMeterSegment = p_arrMeterSegments.add(new GenericMeterSegment("GenericMeterSegment #" + String(n) + " (" + getName() + ")", nThreshold * 0.1f, fRange, true));
        pMeterSegment->setColour(arrHues[nColor], Colour(arrHues[nColor], 1.0f, 1.0f, 0.7f));

        addAndMakeVisible(pMeterSegment);

        nThreshold += nThresholdDiff;
    }
}


MeterBarGainReduction::~MeterBarGainReduction()
{
}


void MeterBarGainReduction::visibilityChanged()
{
    setBounds(nPosX, nPosY, nWidth, nHeight);

    int y = 0;

    for (int n = 0; n < nNumberOfBars; n++)
    {
        int nIndex = n;

        // upward expansion: turn meter upside down
        if (bUpwardExpansion)
        {
            nIndex = nNumberOfBars - n - 1;
        }

        p_arrMeterSegments[nIndex]->setBounds(0, y, nWidth, nSegmentHeight + 1);
        y += nSegmentHeight;
    }
}


void MeterBarGainReduction::paint(Graphics &g)
{
    g.fillAll(Colours::black);
}


void MeterBarGainReduction::resized()
{
}


void MeterBarGainReduction::setUpwardExpansion(bool upward_expansion)
{
    bUpwardExpansion = upward_expansion;

    // re-position meter segments
    visibilityChanged();
}


void MeterBarGainReduction::setGainReduction(float gainReduction, float gainReductionPeak)
{
    if ((gainReduction != fGainReduction) || (gainReductionPeak != fGainReductionPeak))
    {
        fGainReduction = gainReduction;
        fGainReductionPeak = gainReductionPeak;

        for (int n = 0; n < nNumberOfBars; n++)
        {
            p_arrMeterSegments[n]->setLevels(-9999.9f, fGainReduction, -9999.9f, fGainReductionPeak);
        }
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
