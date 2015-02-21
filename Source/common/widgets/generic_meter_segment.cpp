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

#include "generic_meter_segment.h"


GenericMeterSegment::GenericMeterSegment(float threshold, float displayRange)
//
//  The meter segment's state depends on two levels, the "normal" level
//  and the "discrete" level:
//
//  * normal level >= upper threshold
//
//    segment is fully lit
//
//
//  * lower threshold < discrete level <= upper threshold
//
//    segment is fully lit
//
//
//  * lower threshold < normal level <= upper threshold
//
//    level affects segment's brightness
//
//
//  * otherwise
//
//    segment is dark
//
//
//  If either the "normal" or the "discrete" level lies between the
//  upper and lower threshold (or on the lower threshold), the
//  segment's peak marker is lit.
//
{
    // initialise thresholds
    setThresholds(threshold, displayRange);

    // initialise meter segment's brightness (0.0f is dark, 1.0f is
    // fully lit)
    fBrightness = 0.0f;
    fBrightnessOutline = 0.0f;

    // initialise meter segment's hue
    fHue = 0.0f;

    // peak level marker is hidden
    bPeakMarker = false;

    // make sure that segment is drawn after initialisation
    setLevels(-9999.9f, -9999.9f, -9999.9f, -9999.9f);
}


GenericMeterSegment::~GenericMeterSegment()
{
}


float GenericMeterSegment::setThresholds(float threshold, float displayRange)
{
    // lower level threshold
    lowerThreshold = threshold;

    // level range above lower threshold
    thresholdRange = displayRange;

    // upper level threshold
    upperThreshold = lowerThreshold + thresholdRange;

    return upperThreshold;
}


void GenericMeterSegment::setColour(float fHueNew, const Colour &colPeakNew)
{
    fHue = fHueNew;
    colPeak = colPeakNew;

    // redraw meter segment
    repaint();
}


void GenericMeterSegment::paint(Graphics &g)
{
    // get meter segment's screen dimensions
    int width = getWidth();
    int height = getHeight();

    // initialise meter segment's outline colour from hue and
    // brightness
    g.setColour(Colour(fHue, 1.0f, fBrightnessOutline, 1.0f));

    // outline meter segment with solid colour, but leave a border of
    // one pixel for peak marker
    g.drawRect(1, 1, width - 2, height - 2);

    // initialise meter segment's fill colour from hue and brightness
    g.setColour(Colour(fHue, 1.0f, fBrightness, 1.0f));

    // fill remaining meter segment with solid colour
    g.fillRect(2, 2, width - 4, height - 4);

    // if peak marker is lit, draw a rectangle around meter segment
    // (width: 1 pixel)
    if (bPeakMarker)
    {
        g.setColour(colPeak);
        g.drawRect(0, 0, width, height);
    }
}


void GenericMeterSegment::visibilityChanged()
{
    // if this function did not exist, the meter segment wouldn't be
    // drawn until the first level change!
}


void GenericMeterSegment::resized()
{
}


// use this only if you completely disregard discrete levels!
void GenericMeterSegment::setNormalLevels(float normalLevel, float normalLevelPeak)
{
    setLevels(normalLevel, -9999.9f, normalLevelPeak, -9999.9f);
}


// use this only if you completely disregard normal levels!
void GenericMeterSegment::setDiscreteLevels(float discreteLevel, float discreteLevelPeak)
{
    setLevels(-9999.9f, discreteLevel, -9999.9f, discreteLevelPeak);
}


void GenericMeterSegment::setLevels(float normalLevel, float discreteLevel, float normalLevelPeak, float discreteLevelPeak)
{
    // store old brightness and peak marker values
    float fBrightnessOld = fBrightness;
    bool bPeakMarkerOld = bPeakMarker;

    // normal level lies on or above upper threshold, so fully light
    // meter segment
    if (normalLevel >= upperThreshold)
    {
        fBrightness = 0.97f;
        fBrightnessOutline = 0.90f;
    }
    // discrete level lies within thresholds or on upper threshold, so
    // fully light meter segment
    else if ((discreteLevel > lowerThreshold) && (discreteLevel <= upperThreshold))
    {
        fBrightness = 0.97f;
        fBrightnessOutline = 0.90f;
    }
    // normal level lies on or below lower threshold, so set meter
    // segment to dark
    else if (normalLevel <= lowerThreshold)
    {
        fBrightness = 0.25f;
        fBrightnessOutline = 0.30f;
    }
    // normal level lies within thresholds, so calculate brightness
    // from current level
    else
    {
        fBrightness = (normalLevel - lowerThreshold) / thresholdRange;
        fBrightnessOutline = fBrightness;

        // to look well, meter segments should be left with some
        // colour and not have maximum brightness
        fBrightness = fBrightness * 0.72f + 0.25f;
        fBrightnessOutline = fBrightnessOutline * 0.60f + 0.30f;
    }

    // discrete peak level lies within thresholds or on lower
    // threshold, so light peak marker
    if ((discreteLevelPeak >= lowerThreshold) && (discreteLevelPeak < upperThreshold))
    {
        bPeakMarker = true;
    }
    // normal peak level lies within thresholds or on lower threshold,
    // so light peak marker
    else if ((normalLevelPeak >= lowerThreshold) && (normalLevelPeak < upperThreshold))
    {
        bPeakMarker = true;
    }
    // otherwise, hide peak marker
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
