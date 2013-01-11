/* ----------------------------------------------------------------------------

   traKmeter
   =========
   Loudness meter for correctly setting up tracking and mixing levels

   Copyright (c) 2012 Martin Zuther (http://www.mzuther.de/)

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

    MeterArray = new MeterSegment*[nNumberOfBars];

    int nThreshold = 0;
    float fRange = 1.0f;
    int nColor = 1;
    bool bDiscreteLevels = false;

    for (int n = 0; n < nNumberOfBars; n++)
    {
        MeterArray[n] = new MeterSegment("MeterSegment #" + String(n) + " (" + componentName + ")", nThreshold * 0.1f, fRange, bDiscreteLevels, false, nColor);
        addAndMakeVisible(MeterArray[n]);

        nThreshold += 10;
    }
}


MeterBarGainReduction::~MeterBarGainReduction()
{
    for (int n = 0; n < nNumberOfBars; n++)
    {
        removeChildComponent(MeterArray[n]);
        delete MeterArray[n];
        MeterArray[n] = NULL;
    }

    delete [] MeterArray;
    MeterArray = NULL;

    deleteAllChildren();
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


void MeterBarGainReduction::setGainReduction(float gainReduction)
{
    if (gainReduction != fGainReduction)
    {
        fGainReduction = gainReduction;

        for (int n = 0; n < nNumberOfBars; n++)
        {
            MeterArray[n]->setLevels(fGainReduction, 0.0f);
        }
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
