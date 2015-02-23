/* ----------------------------------------------------------------------------

   MZ common JUCE
   ==============
   Common classes for use with the JUCE library

   Copyright (c) 2010-2015 Martin Zuther (http://www.mzuther.de/)

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

#include "generic_meter_bar.h"

GenericMeterBar::GenericMeterBar()
{
    // this component does not have any transparent areas (increases
    // performance on redrawing)
    setOpaque(true);
}


GenericMeterBar::~GenericMeterBar()
{
}


void GenericMeterBar::create()
{
    setOrientation(orientationVertical);

    nWidth = 0;
    nHeight = 0;

    fNormalLevel = -144.0f;
    fDiscreteLevel = -144.0f;

    fNormalLevelPeak = -144.0f;
    fDiscreteLevelPeak = -144.0f;

    p_arrMeterSegments.clear();
    nNumberOfBars = 0;
}


void GenericMeterBar::setOrientation(GenericMeterBar::Orientation orientationNew)
{
    orientation = orientationNew;
}


GenericMeterBar::Orientation GenericMeterBar::getOrientation()
{
    return orientation;
}


void GenericMeterBar::addSegment(float fLowerThreshold, float fDisplayRange, bool bHasHighestLevel, int nSegmentHeight, int nSpacingBefore, float fHue, const Colour &colPeak)
{
    GenericMeterSegment *pMeterSegment = new GenericMeterSegment();
    pMeterSegment->setThresholds(fLowerThreshold, fDisplayRange, bHasHighestLevel);
    pMeterSegment->setColour(fHue, colPeak);
    pMeterSegment->setLevels(fNormalLevel, fDiscreteLevel, fNormalLevelPeak, fDiscreteLevelPeak);

    // meter segment outlines overlap
    nSegmentHeight += 1;
    nSpacingBefore -= 1;

    // no spacing before first meter segment
    if (nNumberOfBars > 0)
    {
        nHeight += nSpacingBefore;
    }

    int x = 0;
    int y = nHeight;
    int width = nWidth;
    int height = nSegmentHeight;

    if (orientation == orientationVertical)
    {
        pMeterSegment->setBounds(x, y, width, height);
    }
    else
    {
        pMeterSegment->setBounds(y, x, height, width);
    }

    p_arrMeterSegments.add(pMeterSegment);
    nNumberOfBars += 1;

    nHeight += nSegmentHeight;

    if (orientation == orientationVertical)
    {
        setSize(nWidth, nHeight);
    }
    else
    {
        setSize(nHeight, nWidth);
    }

    addAndMakeVisible(pMeterSegment);
}


void GenericMeterBar::paint(Graphics &g)
{
    g.fillAll(Colours::black);
}


void GenericMeterBar::resized()
{
    if (orientation == orientationVertical)
    {
        int nWidthNew = getWidth();

        if (nWidth != nWidthNew)
        {
            nWidth = nWidthNew;
            setSize(nWidth, nHeight);

            for (int n = 0; n < nNumberOfBars; n++)
            {
                int nCurrentHeight = p_arrMeterSegments[n]->getHeight();
                p_arrMeterSegments[n]->setSize(nWidth, nCurrentHeight);
            }
        }
    }
    else
    {
        int nWidthNew = getHeight();

        if (nWidth != nWidthNew)
        {
            nWidth = nWidthNew;
            setSize(nHeight, nWidth);

            for (int n = 0; n < nNumberOfBars; n++)
            {
                int nCurrentHeight = p_arrMeterSegments[n]->getWidth();
                p_arrMeterSegments[n]->setSize(nCurrentHeight, nWidth);
            }
        }
    }
}


// use this only if you completely disregard discrete levels!
void GenericMeterBar::setNormalLevels(float normalLevel, float normalLevelPeak)
{
    if ((normalLevel != fNormalLevel) || (normalLevelPeak != fNormalLevelPeak))
    {
        fNormalLevel = normalLevel;
        fNormalLevelPeak = normalLevelPeak;

        for (int n = 0; n < nNumberOfBars; n++)
        {
            p_arrMeterSegments[n]->setNormalLevels(fNormalLevel, fNormalLevelPeak);
        }
    }
}


// use this only if you completely disregard normal levels!
void GenericMeterBar::setDiscreteLevels(float discreteLevel, float discreteLevelPeak)
{
    if ((discreteLevel != fDiscreteLevel) || (discreteLevelPeak != fDiscreteLevelPeak))
    {
        fDiscreteLevel = discreteLevel;
        fDiscreteLevelPeak = discreteLevelPeak;

        for (int n = 0; n < nNumberOfBars; n++)
        {
            p_arrMeterSegments[n]->setDiscreteLevels(fDiscreteLevel, fDiscreteLevelPeak);
        }
    }
}


void GenericMeterBar::setLevels(float normalLevel, float discreteLevel, float normalLevelPeak, float discreteLevelPeak)
{
    if ((normalLevel != fNormalLevel) || (discreteLevel != fDiscreteLevel) || (normalLevelPeak != fNormalLevelPeak) || (discreteLevelPeak != fDiscreteLevelPeak))
    {
        fNormalLevel = normalLevel;
        fDiscreteLevel = discreteLevel;

        fNormalLevelPeak = normalLevelPeak;
        fDiscreteLevelPeak = discreteLevelPeak;

        for (int n = 0; n < nNumberOfBars; n++)
        {
            p_arrMeterSegments[n]->setLevels(fNormalLevel, fDiscreteLevel, fNormalLevelPeak, fDiscreteLevelPeak);
        }
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
