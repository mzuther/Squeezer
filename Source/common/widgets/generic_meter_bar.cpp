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

/// Create container for an empty meter bar.  Call create() to create
/// a new meter bar.
///
GenericMeterBar::GenericMeterBar()
{
    // this component does not have any transparent areas (increases
    // performance on redrawing)
    setOpaque(true);
}


/// Destructor.
///
GenericMeterBar::~GenericMeterBar()
{
}


/// Create a new meter bar (and delete an existing one).  The meter
/// bar has to be filled using addSegment().
///
void GenericMeterBar::create()
{
    // lowest level of a 24-bit-signal in decibels
    float initialLevel = -144.0f;

    // initialise "normal" levels
    normalLevel = initialLevel;
    discreteLevel = initialLevel;

    // initialise "discrete" levels
    normalLevelPeak = initialLevel;
    discreteLevelPeak = initialLevel;

    // initialise meter dimensions
    barWidth = 0;
    barHeight = 0;

    // initialise segment width (we always use the coordinates for a
    // vertical meter, so this corresponds to the segment height in
    // horizontal meters)
    segmentWidth = 10;

    // clear array with meter segments
    p_arrMeterSegments.clear();

    // clear array with segment spacings
    arrSegmentSpacing.clear();

    // set initial orientation
    barOrientation = orientationVertical;
    isOrientationVertical = true;
    isOrientationInverted = false;
}


/// Add a GenericMeterSegment to the meter.  This function runs
/// fastest if you change the meter's orientation after all meter
/// segments have been added.
///
/// @param lowerThreshold lower threshold level (in decibels)
///
/// @param thresholdRange difference between lower and upper level
///        threshold (in decibels)
///
/// @param isTopmost if set to **true**, the segment has no upper
///        level threshold
///
/// @param segmentHeight height of the segment (in pixels)
///
/// @param spacingBefore spacing before the segment (in pixels)
///
/// @param segmentHue hue of the segment (0.0 to 1.0)
///
/// @param colPeakMarker colour of the peak marker
///
void GenericMeterBar::addSegment(float lowerThreshold, float thresholdRange, bool isTopmost, int segmentHeight, int spacingBefore, float segmentHue, const Colour &colPeakMarker)
{
    // store current orientation
    GenericMeterBar::Orientation barOrientationOld = barOrientation;

    // set to standard orientation to easily add a new meter segment
    setOrientation(orientationVertical);

    // create new meter segment
    GenericMeterSegment *currentSegment = new GenericMeterSegment();

    // set segment's lower threshold and display range (both in
    // decibels) and whether it is the topmost segment
    currentSegment->setThresholds(lowerThreshold, thresholdRange, isTopmost);

    // set segment's hue and peak marker colour
    currentSegment->setColour(segmentHue, colPeakMarker);

    // set levels to current levels; prevents blinking on (re)load
    currentSegment->setLevels(normalLevel, discreteLevel, normalLevelPeak, discreteLevelPeak);

    // meter segment outlines overlap (better do it here than relying
    // on the user to remember this....)
    segmentHeight += 1;
    spacingBefore -= 1;

    // add spacing to meter bar's height (no spacing before first
    // meter segment!)
    if (p_arrMeterSegments.size() > 0)
    {
        barHeight += spacingBefore;
    }

    // store corrected segment spacing
    arrSegmentSpacing.add(spacingBefore);

    // add segment to meter bar
    p_arrMeterSegments.add(currentSegment);

    // we always use the coordinates for a vertical meter!
    int tempX = 0;
    int tempY = barHeight;
    int tempWidth = segmentWidth;
    int tempHeight = segmentHeight;

    // set dimensions of meter segment
    currentSegment->setBounds(tempX, tempY, tempWidth, tempHeight);

    // add segment height to meter bar's height
    barHeight += segmentHeight;

    // update dimensions of meter bar
    resized();

    // show meter segment
    addAndMakeVisible(currentSegment);

    // re-store old orientation
    setOrientation(barOrientationOld);
}


/// Get the meter's orientation.
///
/// @return orientation as type **GenericMeterBar::Orientation**
///
GenericMeterBar::Orientation GenericMeterBar::getOrientation()
{
    return barOrientation;
}


/// Get the meter's orientation.  In order to save some processing
/// power, you should use this function after all meter segments have
/// been added.
///
/// @param barOrientationNew new meter orientation
///
void GenericMeterBar::setOrientation(GenericMeterBar::Orientation barOrientationNew)
{
    // fast-forward ...
    if (barOrientationNew == barOrientation)
    {
        return;
    }

    // remember old orientation
    bool isOrientationVerticalOld = isOrientationVertical;
    bool isOrientationInvertedOld = isOrientationInverted;

    // update orientation
    barOrientation = barOrientationNew;

    switch (barOrientation)
    {
    case orientationVertical:

        // vertical meter
        isOrientationVertical = true;

        // meter is *not* inverted
        isOrientationInverted = false;

        break;

    case orientationVerticalInverted:

        // vertical meter
        isOrientationVertical = true;

        // meter *is* inverted
        isOrientationInverted = true;

        break;

    case orientationHorizontal:

        // horizontal meter
        isOrientationVertical = false;

        // meter is *not* inverted; however, we have to set this to
        // "true", otherwise the meter segments will be drawn the
        // wrong way round
        isOrientationInverted = true;

        break;

    case orientationHorizontalInverted:

        // horizontal meter
        isOrientationVertical = false;

        // meter *is* inverted; however, we have to set this to
        // "false", otherwise the meter segments will be drawn the
        // wrong way round
        isOrientationInverted = false;

        break;
    }

    // changed from vertical to horizontal orientation or vice versa
    if (isOrientationVertical != isOrientationVerticalOld)
    {
        // re-arrange meter segments
        for (int segmentIndex = 0; segmentIndex < p_arrMeterSegments.size(); segmentIndex++)
        {
            // get current segment
            GenericMeterSegment *currentSegment = p_arrMeterSegments[segmentIndex];

            // get segment's current position and size
            int tempX = currentSegment->getX();
            int tempY = currentSegment->getY();
            int tempWidth = currentSegment->getWidth();
            int tempHeight = currentSegment->getHeight();

            // swap x <--> y and width <--> height
            currentSegment->setBounds(tempY, tempX, tempHeight, tempWidth);
        }

        // set dimensions of meter bar
        resized();
    }

    // changed from inverted to non-inverted orientation or vice versa
    if (isOrientationInverted != isOrientationInvertedOld)
    {
        // initialise position and segment height
        int tempX = 0;
        int tempY = 0;
        int segmentHeight;

        // inverted orientation: start from "bottom" of meter
        if (isOrientationInverted)
        {
            tempY = barHeight;
        }

        // position meter segments
        for (int segmentIndex = 0; segmentIndex < p_arrMeterSegments.size(); segmentIndex++)
        {
            // get current segment
            GenericMeterSegment *currentSegment = p_arrMeterSegments[segmentIndex];

            // get current segment height
            if (isOrientationVertical)
            {
                segmentHeight = currentSegment->getHeight();
            }
            // horizontal meter
            else
            {
                segmentHeight = currentSegment->getWidth();
            }

            // inverted orientation: subtract from current position
            if (isOrientationInverted)
            {
                // subtract spacing from position (no spacing before
                // first meter segment!)
                if (segmentIndex > 0)
                {
                    tempY -= arrSegmentSpacing[segmentIndex];
                }

                // subtract segment height from position
                tempY -= segmentHeight;
            }
            // non-inverted orientation: add to current position
            else
            {
                // add spacing to position (no spacing before first
                // meter segment)!
                if (segmentIndex > 0)
                {
                    tempY += arrSegmentSpacing[segmentIndex];
                }
            }

            // move meter segments
            if (isOrientationVertical)
            {
                currentSegment->setTopLeftPosition(tempX, tempY);
            }
            // horizontal meter: swap width <--> height
            else
            {
                currentSegment->setTopLeftPosition(tempY, tempX);
            }

            // non-inverted orientation: add height of segment to
            // position
            if (!isOrientationInverted)
            {
                tempY += segmentHeight;
            }
        }
    }
}


/// Invert the current meter.  In order to save some processing power,
/// you should use this function after all meter segments have been
/// added.
///
/// @param isOrientationInvertedNew **true** inverts the meter,
///        **false** reverts to normal orientation
///
void GenericMeterBar::invertMeter(bool isOrientationInvertedNew)
{
    // convert boolean to enum
    if (isOrientationVertical)
    {
        if (isOrientationInvertedNew)
        {
            setOrientation(orientationVerticalInverted);
        }
        else
        {
            setOrientation(orientationVertical);
        }
    }
    else
    {
        if (isOrientationInvertedNew)
        {
            setOrientation(orientationHorizontalInverted);
        }
        else
        {
            setOrientation(orientationHorizontal);
        }
    }
}


/// Find out if the current meter is inverted.
///
/// @return **true** if meter is inverted, **false** if meter has
///         normal orientation
///
bool GenericMeterBar::isMeterInverted()
{
    return isOrientationInverted;
}


/// Get current segment width.
///
/// @return segment width for vertical meters and segment height for
///         horizontal meters.
///
int GenericMeterBar::getSegmentWidth()
{
    return segmentWidth;
}


/// Set segment width.  Sets segment width for vertical meters and
/// segment height for horizontal meters.
///
/// @param segmentWidthNew new segment width
///
void GenericMeterBar::setSegmentWidth(int segmentWidthNew)
{
    // update segment width
    segmentWidth = segmentWidthNew;

    // update meter bar's width
    barWidth = segmentWidth;

    // update meter segments
    for (int segmentIndex = 0; segmentIndex < p_arrMeterSegments.size(); segmentIndex++)
    {
        // get current segment
        GenericMeterSegment *currentSegment = p_arrMeterSegments[segmentIndex];

        // set dimensions of meter segment
        if (isOrientationVertical)
        {
            currentSegment->setSize(segmentWidth, currentSegment->getHeight());
        }
        else
        {
            // horizontal meter
            int segmentHeight = segmentWidth;
            currentSegment->setSize(currentSegment->getWidth(), segmentHeight);
        }
    }

    // update dimensions of meter bar
    resized();
}


/// Paint meter bar.
///
void GenericMeterBar::paint(Graphics &g)
{
    // fill background with black (disabled peak markers will be drawn
    // in black)
    g.fillAll(Colours::black);
}


/// Overrides dimensions of meter bar.
///
void GenericMeterBar::resized()
{
    // override dimensions of meter bar
    if (isOrientationVertical)
    {
        setSize(barWidth, barHeight);
    }
    else
    {
        // horizontal meter: swap width <--> height
        setSize(barHeight, barWidth);
    }
}


/// Set normal (average) levels.  Use this only if you completely
/// disregard discrete (peak) levels!
///
/// @param normalLevelNew new normal level
///
/// @param normalLevelPeakNew new normal peak level
///
void GenericMeterBar::setNormalLevels(float normalLevelNew, float normalLevelPeakNew)
{
    // "normal" levels have changed
    if ((normalLevel != normalLevelNew) || (normalLevelPeak != normalLevelPeakNew))
    {
        // update levels
        normalLevel = normalLevelNew;
        normalLevelPeak = normalLevelPeakNew;

        // update meter segments
        for (int segmentIndex = 0; segmentIndex < p_arrMeterSegments.size(); segmentIndex++)
        {
            p_arrMeterSegments[segmentIndex]->setNormalLevels(normalLevel, normalLevelPeak);
        }
    }
}


/// Set discrete (peak) levels.  Use this only if you completely
/// disregard normal (average) levels!
///
/// @param discreteLevelNew new discrete level
///
/// @param discreteLevelPeakNew new discrete peak level
///
void GenericMeterBar::setDiscreteLevels(float discreteLevelNew, float discreteLevelPeakNew)
{
    // "discrete" levels have changed
    if ((discreteLevel != discreteLevelNew) || (discreteLevelPeak != discreteLevelPeakNew))
    {
        // update levels
        discreteLevel = discreteLevelNew;
        discreteLevelPeak = discreteLevelPeakNew;

        // update meter segments
        for (int segmentIndex = 0; segmentIndex < p_arrMeterSegments.size(); segmentIndex++)
        {
            p_arrMeterSegments[segmentIndex]->setDiscreteLevels(discreteLevel, discreteLevelPeak);
        }
    }
}


/// Set discrete (peak) and normal (average) levels.
///
/// @param normalLevelNew new normal level
///
/// @param discreteLevelNew new discrete level
///
/// @param normalLevelPeakNew new normal peak level
///
/// @param discreteLevelPeakNew new discrete peak level
///
void GenericMeterBar::setLevels(float normalLevelNew, float discreteLevelNew, float normalLevelPeakNew, float discreteLevelPeakNew)
{
    // "normal" or "discrete" levels have changed
    if ((normalLevel != normalLevelNew) || (discreteLevel != discreteLevelNew) || (normalLevelPeak != normalLevelPeakNew) || (discreteLevelPeak != discreteLevelPeakNew))
    {
        // update "normal" levels
        normalLevel = normalLevelNew;
        discreteLevel = discreteLevelNew;

        // update "discrete" levels
        normalLevelPeak = normalLevelPeakNew;
        discreteLevelPeak = discreteLevelPeakNew;

        // update meter bars
        for (int segmentIndex = 0; segmentIndex < p_arrMeterSegments.size(); segmentIndex++)
        {
            p_arrMeterSegments[segmentIndex]->setLevels(normalLevel, discreteLevel, normalLevelPeak, discreteLevelPeak);
        }
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
