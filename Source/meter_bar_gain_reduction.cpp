/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of lemon.

   Copyright (c) 2013 Martin Zuther (http://www.mzuther.de/)

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

MeterBarGainReduction::MeterBarGainReduction(const String& componentName, int pos_x, int pos_y, int width, int number_of_bars, int segment_height)
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

    MeterArray = new MeterSegment*[nNumberOfBars];

    bUpwardExpansion = false;
    initialiseMeter();
}


MeterBarGainReduction::~MeterBarGainReduction()
{
    deleteMeter();

    delete [] MeterArray;
    MeterArray = NULL;

    deleteAllChildren();
}


void MeterBarGainReduction::initialiseMeter()
{
    float fRange = 1.0f;
    int nColor = 3;
    int nThreshold;
    int nThresholdDiff;

    if (bUpwardExpansion)
    {
        nThreshold = (nNumberOfBars - 1) * 10;
        nThresholdDiff = -10;
    }
    else
    {
        nThreshold = 0;
        nThresholdDiff = 10;
    }

    for (int n = 0; n < nNumberOfBars; n++)
    {
        MeterArray[n] = new MeterSegment("MeterSegment #" + String(n) + " (" + getName() + ")", nThreshold * 0.1f, fRange, true, nColor);
        addAndMakeVisible(MeterArray[n]);

        nThreshold += nThresholdDiff;
    }
}


void MeterBarGainReduction::deleteMeter()
{
    for (int n = 0; n < nNumberOfBars; n++)
    {
        removeChildComponent(MeterArray[n]);

        delete MeterArray[n];
        MeterArray[n] = NULL;
    }
}


void MeterBarGainReduction::visibilityChanged()
{
    setBounds(nPosX, nPosY, nWidth, nHeight);

    int x = 0;
    int y = 0;

    for (int n = 0; n < nNumberOfBars; n++)
    {
        MeterArray[n]->setBounds(x, y, nWidth, nSegmentHeight + 1);
        y += nSegmentHeight;
    }
}


void MeterBarGainReduction::paint(Graphics& g)
{
    g.fillAll(Colours::black);
}


void MeterBarGainReduction::resized()
{
}


void MeterBarGainReduction::setUpwardExpansion(bool upward_expansion)
{
    if (bUpwardExpansion != upward_expansion)
    {
        bUpwardExpansion = upward_expansion;

        deleteMeter();
        initialiseMeter();
        visibilityChanged();
    }
}


void MeterBarGainReduction::setGainReduction(float gainReduction, float gainReductionPeak)
{
    if ((gainReduction != fGainReduction) || (gainReductionPeak != fGainReductionPeak))
    {
        fGainReduction = gainReduction;
        fGainReductionPeak = gainReductionPeak;

        for (int n = 0; n < nNumberOfBars; n++)
        {
            MeterArray[n]->setLevels(-9999.9f, fGainReduction, -9999.9f, fGainReductionPeak);
        }
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
