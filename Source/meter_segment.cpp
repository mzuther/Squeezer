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

#include "meter_segment.h"


MeterSegment::MeterSegment(const String &componentName, float fThreshold, float fRange, bool bDisplayPeakMeter, int nColor)
{
    // set component name
    setName(componentName);

    // display additional peak meter?
    displayPeakMeter = bDisplayPeakMeter;

    // lower threshold, meter segment will be dark below this level
    fLowerThreshold = fThreshold;

    // level range above lower threshold; this affects the brightness
    fThresholdRange = fRange;

    // upper threshold, meter segment will be fully lit above this
    // level
    fUpperThreshold = fThreshold + fThresholdRange;

    // show peak level marker on segment?
    nPeakMarker = PEAK_MARKER_NONE;

    // initialise meter segment's brightness (0.0f is dark, 1.0f is
    // fully lit)
    fBrightness = 0.0f;
    fBrightnessOutline = 0.0f;

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
        fHue = 0.34f;
    }
    else
    {
        // meter segment is blue
        fHue = 0.61f;
    }

    // make sure that segment is drawn after initialisation
    setLevels(-9999.9f, -9999.9f, -9999.9f, -9999.9f);
}


MeterSegment::~MeterSegment()
{
    // nothing to do, really
}


void MeterSegment::paint(Graphics &g)
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

    // if peak marker is lit, draw a white rectangle around meter
    // segment (width: 1 pixel)
    if (nPeakMarker != PEAK_MARKER_NONE)
    {
        g.setColour(Colour(fHue, 1.0f, 1.0f, 0.7f));
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


void MeterSegment::setLevels(float fPeakLevel, float fAverageLevel, float fPeakLevelPeak, float fAverageLevelPeak)
{
    // store old brightness and peak marker values
    float fBrightnessOld = fBrightness;
    int nPeakMarkerOld = nPeakMarker;

    // current average level lies on or above upper threshold, so
    // fully light meter segment
    if (fAverageLevel >= fUpperThreshold)
    {
        fBrightness = 0.97f;
        fBrightnessOutline = 0.90f;
    }
    // peak metering is enabled and current peak level lies within
    // thresholds or on upper threshold, so fully light meter segment
    else if (displayPeakMeter && (fPeakLevel > fLowerThreshold) && (fPeakLevel <= fUpperThreshold))
    {
        fBrightness = 0.97f;
        fBrightnessOutline = 0.90f;
    }
    // current average level lies on or below lower threshold, so set
    // meter segment to dark
    else if (fAverageLevel <= fLowerThreshold)
    {
        fBrightness = 0.25f;
        fBrightnessOutline = 0.30f;
    }
    // current average level lies within thresholds, so calculate
    // brightness from current level
    else
    {
        fBrightness = (fAverageLevel - fLowerThreshold) / fThresholdRange;
        fBrightnessOutline = fBrightness;

        // to look well, meter segments should be left with some
        // colour and not have maximum brightness
        fBrightness = fBrightness * 0.72f + 0.25f;
        fBrightnessOutline = fBrightnessOutline * 0.60f + 0.30f;
    }

    // peak metering is enabled and meter's peak level peak lies
    // within thresholds or on upper threshold, so show peak marker on
    // segment
    if (displayPeakMeter && (fPeakLevelPeak > fLowerThreshold) && (fPeakLevelPeak <= fUpperThreshold))
    {
        nPeakMarker = PEAK_MARKER_PEAK;
    }
    // meter's average level peak lies within thresholds or on upper
    // threshold, so show peak marker on segment
    else if ((fAverageLevelPeak > fLowerThreshold) && (fAverageLevelPeak <= fUpperThreshold))
    {
        nPeakMarker = PEAK_MARKER_AVERAGE;
    }
    // otherwise, do not show peak marker on segment
    else
    {
        nPeakMarker = PEAK_MARKER_NONE;
    }

    // re-paint meter segment only when brightness or peak marker have
    // changed
    if ((fBrightness != fBrightnessOld) || (nPeakMarker != nPeakMarkerOld))
    {
        repaint(getLocalBounds());
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
