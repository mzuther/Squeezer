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

#include "meter_segment.h"


MeterSegment::MeterSegment(const String& componentName, float fThreshold, float fRange, bool bDiscreteLevels, bool bDisplayPeaks, int nColor)
{
    // set component name
    setName(componentName);

    // display peaks on meter?
    displayPeaks = bDisplayPeaks;

    // display discrete levels on meter?
    discreteLevels = bDiscreteLevels;

    // lower threshold, meter segment will be dark below this level
    fLowerThreshold = fThreshold;

    // level range above lower threshold; this affects the brightness
    fThresholdRange = fRange;

    // upper threshold, meter segment will be fully lit above this
    // level
    fUpperThreshold = fThreshold + fThresholdRange;

    // show peak level marker on segment?
    bPeakMarker = false;

    // initialise meter segment's brightness (0.0f is dark, 1.0f is
    // fully lit)
    fBrightness = 0.0f;

    // set meter segment's hue from colour number
    if (nColor == 0)
    {
        // meter segment is red
        fHue = 0.00f;
    }
    else if (nColor == 1)
    {
        // meter segment is yellow
        fHue = 0.18f;
    }
    else if (nColor == 2)
    {
        // meter segment is green
        fHue = 0.30f;
    }
    else
    {
        // meter segment is blue
        fHue = 0.58f;
    }

    // make sure that segment is drawn after initialisation
    setLevels(-9999.9f, -9999.9f);
}


MeterSegment::~MeterSegment()
{
    // nothing to do, really
}


void MeterSegment::paint(Graphics& g)
{
    // get meter segment's screen dimensions
    int width = getWidth();
    int height = getHeight();

    // initialise meter segment's colour from hue and brightness
    g.setColour(Colour(fHue, 1.0f, fBrightness, 1.0f));

    // fill meter segment with solid colour, but leave a border of one
    // pixel for peak marker
    g.fillRect(1, 1, width - 2, height - 2);

    // if peak marker is lit, draw a rectangle around meter segment
    // (width: 1 pixel)
    if (bPeakMarker)
    {
        g.setColour(Colour(fHue, 0.7f, 1.0f, 1.0f));
        g.drawRect(0, 0, width, height);
    }
    else if (!displayPeaks)
    {
        g.setColour(Colours::black);
        g.drawRect(0, 0, width, height);
    }
}


void MeterSegment::visibilityChanged()
{
    // if this function did not exist, the meter segment wouldn't be
    // drawn until the first level change!
}


void MeterSegment::resized()
{
}


void MeterSegment::setLevels(float fLevel, float fLevelPeak)
{
    // store old brightness and peak marker values
    float fBrightnessOld = fBrightness;
    bool bPeakMarkerOld = bPeakMarker;

    // current level lies above upper threshold
    if (fLevel > fUpperThreshold)
    {
        if (discreteLevels)
        {
            // set meter segment to dark
            fBrightness = 0.25f;
        }
        else
        {
            // fully light meter segment
            fBrightness = 0.97f;
        }
    }
    // current level lies on or below lower threshold
    else if (fLevel <= fLowerThreshold)
    {
        // set meter segment to dark
        fBrightness = 0.25f;
    }
    // current level lies within thresholds or on upper threshold
    else
    {
        if (discreteLevels)
        {
            // fully light meter segment
            fBrightness = 0.97f;
        }
        else
        {
            // calculate brightness from current level
            fBrightness = (fLevel - fLowerThreshold) / fThresholdRange;

            // to look well, meter segments should be left with some
            // colour and not have maximum brightness
            fBrightness = fBrightness * 0.72f + 0.25f;
        }
    }

    // peak lies within thresholds or on upper threshold, so show peak
    // marker on segment
    if ((fLevelPeak > fLowerThreshold) && (fLevelPeak <= fUpperThreshold))
    {
        bPeakMarker = displayPeaks;
    }
    // otherwise, do not show peak marker on segment
    else
    {
        bPeakMarker = false;
    }

    // re-paint meter segment only when brightness or peak marker have
    // changed
    if ((fBrightness != fBrightnessOld) || (bPeakMarker != bPeakMarkerOld))
    {
        repaint(getLocalBounds());
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
