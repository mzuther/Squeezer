/* ----------------------------------------------------------------------------

   MZ common JUCE
   ==============
   Common classes for use with the JUCE library

   Copyright (c) 2010-2016 Martin Zuther (http://www.mzuther.de/)

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

#include "generic_meter_segment_discrete.h"

/// Create a new meter segment, complete with peak marker.
///
/// > #### Internals
/// >
/// > The meter segment's state depends on two levels, the normal
/// > (usually average) level and the discrete (usually peak) level:
/// >
/// > 1. normal level >= upper threshold
/// >
/// >    segment is fully lit
/// >
/// > 2. lower threshold <= discrete level < upper threshold
/// >
/// >    segment is fully lit
/// >
/// > 3. normal level < lower threshold
/// >
/// >    segment is dark
/// >
/// > 4. otherwise
/// >
/// >    level affects segment's brightness
/// >
/// > The segment's peak marker is lit when any level peak lies
/// > between the upper and lower threshold (or on the lower
/// > threshold).  If **isTopmost** is **true**, however, the
/// > segment's peak marker is lit when any level peak reaches or
/// > exceeds the lower threshold.
///
/// @param lowerThreshold lower threshold level (in decibels)
///
/// @param thresholdRange difference between lower and upper level
///        threshold (in decibels)
///
/// @param isTopmost if set to **true**, the segment has no upper
///        level threshold
///
GenericMeterSegmentDiscrete::GenericMeterSegmentDiscrete(
    float lowerThresholdNew, float thresholdRangeNew, bool isTopmostNew)
{
    // initialise thresholds and whether this segment the topmost
    // segment
    setThresholds(lowerThresholdNew, thresholdRangeNew, isTopmostNew);

    // initialise meter segment's brightness (0.0f is dark, 1.0f is
    // fully lit)
    segmentBrightness = 0.0f;
    outlineBrightness = 0.0f;

    // initialise meter segment's hue
    segmentHue = 0.0f;

    // lowest level of a 24-bit-signal in decibels
    float initialLevel = -144.0f;

    // make sure that segment is drawn after initialisation
    setLevels(initialLevel, initialLevel, initialLevel, initialLevel);
}


/// Set thresholds and whether the segment is the topmost in a meter
/// bar.
///
/// @param lowerThreshold lower threshold level (in decibels)
///
/// @param thresholdRange difference between lower and upper level
///        threshold (in decibels)
///
/// @param isTopmost if set to **true**, the segment has no upper
///        level threshold
///
/// @return return new upper threshold of segment (may be useful for
///         creating continuous meters)
///
float GenericMeterSegmentDiscrete::setThresholds(
    float lowerThresholdNew, float thresholdRangeNew, bool isTopmostNew)
{
    // set lower level threshold (in decibels)
    lowerThreshold = lowerThresholdNew;

    // set level range above lower threshold (in decibels)
    thresholdRange = thresholdRangeNew;

    // set upper level threshold (in decibels)
    upperThreshold = lowerThreshold + thresholdRange;

    // peak level marker is currently off
    lightPeakMarker = false;

    // is there a meter segment beyond this?
    isTopmost = isTopmostNew;

    // return upper threshold (may be useful for creating continuous
    // meters)
    return upperThreshold;
}


/// Set colours of segment and peak marker.
///
/// @param segmentHue hue of the segment (0.0 to 1.0)
///
/// @param colPeakMarker colour of the peak marker
///
void GenericMeterSegmentDiscrete::setColour(float segmentHueNew,
        const Colour &colPeakMarkerNew)
{
    // set meter segment's hue
    segmentHue = segmentHueNew;

    // set peak marker's colour
    colPeakMarker = colPeakMarkerNew;

    // redraw meter segment
    repaint();
}


/// Paint segment.
///
/// @param g the graphics context that must be used to do the drawing
///        operations
///
void GenericMeterSegmentDiscrete::paint(Graphics &g)
{
    // get meter segment's dimensions
    int tempWidth = getWidth();
    int tempHeight = getHeight();

    // set meter segment's colour from hue and brightness
    g.setColour(Colour(segmentHue, 1.0f, segmentBrightness, 1.0f));

    // fill meter segment, but leave a border of two pixels for
    // outline and peak marker
    g.fillRect(2, 2, tempWidth - 4, tempHeight - 4);

    // set meter segment's outline colour from hue and brightness
    g.setColour(Colour(segmentHue, 1.0f, outlineBrightness, 1.0f));

    // draw outline, but leave a border of one pixel for peak marker
    g.drawRect(1, 1, tempWidth - 2, tempHeight - 2);

    // if peak marker is lit, draw peak marker around meter segment
    // (width: 1 pixel)
    if (lightPeakMarker)
    {
        g.setColour(colPeakMarker);
        g.drawRect(0, 0, tempWidth, tempHeight);
    }
}


/// Called when this component's visibility changes.
///
/// If this function did not exist, the meter segment wouldn't be
/// drawn until the first level change!
void GenericMeterSegmentDiscrete::visibilityChanged()
{
}


/// Called when this component's size has been changed.
///
void GenericMeterSegmentDiscrete::resized()
{
}


/// Set normal (average) levels.  Use this only if you completely
/// disregard discrete (peak) levels!
///
/// @param normalLevelNew new normal level
///
/// @param normalLevelPeakNew new normal peak level
///
void GenericMeterSegmentDiscrete::setNormalLevels(float normalLevelNew,
        float normalLevelPeakNew)
{
    // lowest level of a 24-bit-signal in decibels
    float initialLevel = -144.0f;

    setLevels(normalLevelNew, normalLevelPeakNew, initialLevel, initialLevel);
}


/// Set discrete (peak) levels.  Use this only if you completely
/// disregard normal (average) levels!
///
/// @param discreteLevelNew new discrete level
///
/// @param discreteLevelPeakNew new discrete peak level
///
void GenericMeterSegmentDiscrete::setDiscreteLevels(float discreteLevelNew,
        float discreteLevelPeakNew)
{
    // lowest level of a 24-bit-signal in decibels
    float initialLevel = -144.0f;

    setLevels(initialLevel, initialLevel, discreteLevelNew, discreteLevelPeakNew);
}


/// Set discrete (peak) and normal (average) levels.
///
/// @param normalLevelNew new normal level
///
/// @param normalLevelPeakNew new normal peak level
///
/// @param discreteLevelNew new discrete level
///
/// @param discreteLevelPeakNew new discrete peak level
///
void GenericMeterSegmentDiscrete::setLevels(
    float normalLevelNew, float normalLevelPeakNew, float discreteLevelNew,
    float discreteLevelPeakNew)
{
    // store old brightness and peak marker values
    float segmentBrightnessOld = segmentBrightness;
    bool lightPeakMarkerOld = lightPeakMarker;

    // normal level lies on or above upper threshold, so fully light
    // meter segment
    if (normalLevelNew >= upperThreshold)
    {
        segmentBrightness = 0.97f;
        outlineBrightness = 0.90f;
    }
    // discrete level lies within thresholds or on lower threshold, so
    // fully light meter segment
    else if ((discreteLevelNew >= lowerThreshold) && (discreteLevelNew < upperThreshold))
    {
        segmentBrightness = 0.97f;
        outlineBrightness = 0.90f;
    }
    // normal level lies below lower threshold, so set meter segment
    // to dark
    else if (normalLevelNew < lowerThreshold)
    {
        segmentBrightness = 0.25f;
        outlineBrightness = 0.30f;
    }
    // normal level lies within thresholds or on lower threshold, so
    // calculate brightness from current level
    else
    {
        segmentBrightness = (normalLevelNew - lowerThreshold) / thresholdRange;
        outlineBrightness = segmentBrightness;

        // to look well, meter segments should be left with some
        // colour and not have maximum brightness
        segmentBrightness = segmentBrightness * 0.72f + 0.25f;
        outlineBrightness = outlineBrightness * 0.60f + 0.30f;
    }

    // there is no meter segment beyond this; light peak marker if
    // peak level is on or above lower threshold
    if (isTopmost)
    {
        // light peak marker ("normal" peak level)
        if (normalLevelPeakNew >= lowerThreshold)
        {
            lightPeakMarker = true;
        }
        // light peak marker ("discrete" peak level)
        else if (discreteLevelPeakNew >= lowerThreshold)
        {
            lightPeakMarker = true;
        }
        // peak marker is off
        else
        {
            lightPeakMarker = false;
        }
    }
    // otherwise, light peak marker if peak level lies within
    // thresholds or on lower threshold
    else
    {
        // light peak marker ("normal" peak level)
        if ((normalLevelPeakNew >= lowerThreshold) && (normalLevelPeakNew < upperThreshold))
        {
            lightPeakMarker = true;
        }
        // light peak marker ("discrete" peak level)
        else if ((discreteLevelPeakNew >= lowerThreshold) && (discreteLevelPeakNew < upperThreshold))
        {
            lightPeakMarker = true;
        }
        // peak marker is off
        else
        {
            lightPeakMarker = false;
        }
    }

    // re-paint meter segment only when brightness or peak marker have
    // changed
    if ((segmentBrightness != segmentBrightnessOld) || (lightPeakMarker != lightPeakMarkerOld))
    {
        repaint();
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
