/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
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

#include "generic_meter_bar.h"


/// Create a new meter bar (and delete an existing one).  The meter
/// bar can be filled using addSegment() and its cousins.
///
void GenericMeterBar::create()
{
    // this component does not have any transparent areas (increases
    // performance on redrawing)
    setOpaque(true);

    // lowest level of a 24-bit-signal in decibels
    float initialLevel = -144.0f;

    // initialise "normal" levels
    normalLevel_ = initialLevel;
    normalLevelPeak_ = initialLevel;

    // initialise "discrete" levels
    discreteLevel_ = initialLevel;
    discreteLevelPeak_ = initialLevel;

    // initialise meter dimensions
    barWidth_ = 0;
    barHeight_ = 0;

    // initialise segment width (we always use the coordinates for a
    // vertical meter, so this corresponds to the segment height in
    // horizontal meters)
    segmentWidth_ = 10;

    // clear array with meter segments
    meterSegments_.clear();

    // clear array with segment spacings
    segmentSpacing_.clear();

    // set initial orientation
    orientation_ = GenericMeterOrientation::vertical;
    isVertical_ = true;
    isInverted_ = false;
}


/// Add a meter segment to the meter.  This function runs fastest if
/// you change the meter's orientation after all meter segments have
/// been added.
///
/// @param segment segment to add (will be deleted automatically)
///
/// @param segmentHeight height of the segment (in pixels)
///
/// @param spacingBefore spacing before the segment (in pixels)
///
void GenericMeterBar::addSegment(
    GenericMeterSegment *segment,
    int segmentHeight,
    int spacingBefore)

{
    // set levels to current levels; prevents blinking on (re)load
    segment->setLevels(normalLevel_, normalLevelPeak_,
                       discreteLevel_, discreteLevelPeak_);

    // store current orientation
    GenericMeterOrientation orientationOld = orientation_;

    // set to standard orientation to easily add a new meter segment
    setOrientation(GenericMeterOrientation::vertical);

    // add spacing to meter bar's height (no spacing before first
    // meter segment!)
    if (meterSegments_.size() > 0)
    {
        barHeight_ += spacingBefore;
    }

    // store corrected segment spacing
    segmentSpacing_.add(spacingBefore);

    // add segment to meter bar
    meterSegments_.add(segment);

    // set dimensions of meter segment; remember that we always use
    // the coordinates for a vertical meter!
    segment->setBounds(0,
                       barHeight_,
                       segmentWidth_,
                       segmentHeight);

    // add segment height to meter bar's height
    barHeight_ += segmentHeight;

    // re-store old orientation
    setOrientation(orientationOld);

    // show meter segment
    addAndMakeVisible(segment);

    // update dimensions of meter bar
    resized();
}


/// Add a discrete meter segment to the meter.  This function runs
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
/// @param segmentColour colour of the segment
///
/// @param peakMarkerColour colour of the peak marker
///
void GenericMeterBar::addDiscreteSegment(
    float lowerThreshold,
    float thresholdRange,
    bool isTopmost,
    int segmentHeight,
    int spacingBefore,
    const Colour &segmentColour,
    const Colour &peakMarkerColour)

{
    // create new discrete meter segment (will be deleted
    // automatically)
    GenericMeterSegmentDiscrete *segment =
        new GenericMeterSegmentDiscrete();

    // set segment's lower threshold and display range (both in
    // decibels) and whether it is the topmost segment
    segment->setThresholdAndRange(lowerThreshold,
                                  thresholdRange,
                                  isTopmost);

    // set segment colours
    segment->setColours(segmentColour, peakMarkerColour);

    // add segment to meter
    addSegment(
        segment,
        segmentHeight,
        spacingBefore);
}


/// Add a continuous meter segment to the meter.  This function runs
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
/// @param segmentColour colour of the segment
///
/// @param peakMarkerColour colour of the peak marker
///
void GenericMeterBar::addContinuousSegment(
    float lowerThreshold,
    float thresholdRange,
    bool isTopmost,
    int segmentHeight,
    int spacingBefore,
    const Colour &segmentColour,
    const Colour &peakMarkerColour)

{
    // create new continuous meter segment (will be deleted
    // automatically)
    GenericMeterSegmentContinuous *segment =
        new GenericMeterSegmentContinuous();

    // set segment's lower threshold and display range (both in
    // decibels) and whether it is the topmost segment
    segment->setThresholdAndRange(lowerThreshold,
                                  thresholdRange,
                                  isTopmost);

    // set segment colours
    segment->setColours(segmentColour, peakMarkerColour);

    // add segment to meter
    addSegment(
        segment,
        segmentHeight,
        spacingBefore);

    // add (slightly) bigger margin to top-most segment
    if (isTopmost)
    {
        Rectangle<int> bounds = segment->getBounds();
        Rectangle<int> newBounds = bounds.withTop(bounds.getY() + 1);

        // update position and size
        segment->setBounds(newBounds);
    }
}


/// Get the meter's orientation.
///
/// @return current orientation
///
GenericMeterOrientation GenericMeterBar::getOrientation()
{
    return orientation_;
}


/// Set the meter's orientation.  In order to save some processing
/// power, you should use this function after all meter segments have
/// been added.
///
/// @param orientation new meter orientation
///
void GenericMeterBar::setOrientation(
    GenericMeterOrientation orientation)

{
    // fast-forward ...
    if (orientation == orientation_)
    {
        return;
    }

    // remember old orientation
    bool isVerticalOld = isVertical_;
    bool isInvertedOld = isInverted_;

    // update orientation
    orientation_ = orientation;

    switch (orientation_)
    {
    case GenericMeterOrientation::vertical:

        // vertical meter
        isVertical_ = true;

        // meter is *not* inverted
        isInverted_ = false;

        break;

    case GenericMeterOrientation::verticalInverted:

        // vertical meter
        isVertical_ = true;

        // meter *is* inverted
        isInverted_ = true;

        break;

    case GenericMeterOrientation::horizontal:

        // horizontal meter
        isVertical_ = false;

        // meter is *not* inverted; however, we have to set this to
        // "true", otherwise the meter segments will be drawn the
        // wrong way round
        isInverted_ = true;

        break;

    case GenericMeterOrientation::horizontalInverted:

        // horizontal meter
        isVertical_ = false;

        // meter *is* inverted; however, we have to set this to
        // "false", otherwise the meter segments will be drawn the
        // wrong way round
        isInverted_ = false;

        break;
    }

    // changed from vertical to horizontal orientation or vice versa
    if (isVertical_ != isVerticalOld)
    {
        // re-arrange meter segments
        for (auto & segment : meterSegments_)
        {
            // swap x <--> y and width <--> height
            segment->setBounds(segment->getY(),
                               segment->getX(),
                               segment->getHeight(),
                               segment->getWidth());
        }

        // update dimensions of meter bar
        resized();
    }

    // changed from inverted to non-inverted orientation or vice versa
    if (isInverted_ != isInvertedOld)
    {
        // initialise position and segment height
        int tempX = 0;
        int tempY = 0;
        int segmentHeight;

        // inverted orientation: start from "bottom" of meter
        if (isInverted_)
        {
            tempY = barHeight_;
        }

        // position meter segments
        for (int index = 0; index < meterSegments_.size(); ++index)
        {
            // get current segment
            GenericMeterSegment *segment = meterSegments_[index];

            // get current segment height
            if (isVertical_)
            {
                segmentHeight = segment->getHeight();
            }
            // horizontal meter
            else
            {
                segmentHeight = segment->getWidth();
            }

            // inverted orientation: subtract from current position
            if (isInverted_)
            {
                // subtract spacing from position (no spacing before
                // first meter segment!)
                if (index > 0)
                {
                    tempY -= segmentSpacing_[index];
                }

                // subtract segment height from position
                tempY -= segmentHeight;
            }
            // non-inverted orientation: add to current position
            else
            {
                // add spacing to position (no spacing before first
                // meter segment)!
                if (index > 0)
                {
                    tempY += segmentSpacing_[index];
                }
            }

            // move meter segments
            if (isVertical_)
            {
                segment->setTopLeftPosition(tempX, tempY);
            }
            // horizontal meter: swap width <--> height
            else
            {
                segment->setTopLeftPosition(tempY, tempX);
            }

            // non-inverted orientation: add height of segment to
            // position
            if (!isInverted_)
            {
                tempY += segmentHeight;
            }
        }
    }

    // update segment orientation
    for (auto & segment : meterSegments_)
    {
        segment->setOrientation(orientation_);
    }
}


/// Invert the current meter.  In order to save some processing power,
/// you should use this function after all meter segments have been
/// added.
///
/// @param invert **true** inverts the meter, **false** reverts to
///        normal orientation
///
void GenericMeterBar::invertMeter(
    bool invert)

{
    // convert boolean to enum
    if (isVertical_)
    {
        if (invert)
        {
            setOrientation(GenericMeterOrientation::verticalInverted);
        }
        else
        {
            setOrientation(GenericMeterOrientation::vertical);
        }
    }
    else
    {
        if (invert)
        {
            setOrientation(GenericMeterOrientation::horizontalInverted);
        }
        else
        {
            setOrientation(GenericMeterOrientation::horizontal);
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
    return isInverted_;
}


/// Get current segment width.
///
/// @return segment width for vertical meters and segment height for
///         horizontal meters.
///
int GenericMeterBar::getSegmentWidth()
{
    return segmentWidth_;
}


/// Set segment width.  Sets segment width for vertical meters and
/// segment height for horizontal meters.
///
/// @param segmentWidth new segment width (or height, see above)
///
void GenericMeterBar::setSegmentWidth(
    int segmentWidth)

{
    // update segment width
    segmentWidth_ = segmentWidth;

    // update meter bar's width
    barWidth_ = segmentWidth_;

    // update meter segments
    for (auto & segment : meterSegments_)
    {
        // set dimensions of meter segment
        if (isVertical_)
        {
            segment->setSize(segmentWidth_, segment->getHeight());
        }
        // horizontal meter
        else
        {
            segment->setSize(segment->getWidth(), segmentWidth_);
        }
    }

    // update dimensions of meter bar
    resized();
}


/// Paint meter bar.
///
/// @param g graphics context
///
void GenericMeterBar::paint(
    Graphics &g)

{
    // fill background with black (disabled peak markers will be drawn
    // in black)
    g.fillAll(Colours::black);
}


/// This function overrides the meter bar's dimensions!
///
void GenericMeterBar::resized()
{
    // override dimensions of meter bar
    if (isVertical_)
    {
        setSize(barWidth_, barHeight_);
    }
    else
    {
        // horizontal meter: swap width <--> height
        setSize(barHeight_, barWidth_);
    }
}


/// Set normal (average) levels.  Use this only if you completely
/// disregard discrete (peak) levels!
///
/// @param normalLevel new normal level
///
/// @param normalLevelPeak new normal peak level
///
void GenericMeterBar::setNormalLevels(
    float normalLevel, float normalLevelPeak)

{
    // "normal" levels have changed
    if ((normalLevel_ != normalLevel) ||
            (normalLevelPeak_ != normalLevelPeak))
    {
        // update levels
        normalLevel_ = normalLevel;
        normalLevelPeak_ = normalLevelPeak;

        // update meter segments
        for (auto & segment : meterSegments_)
        {
            segment->setNormalLevels(
                normalLevel_, normalLevelPeak_);
        }
    }
}


/// Set discrete (peak) levels.  Use this only if you completely
/// disregard normal (average) levels!
///
/// @param discreteLevel new discrete level
///
/// @param discreteLevelPeak new discrete peak level
///
void GenericMeterBar::setDiscreteLevels(
    float discreteLevel, float discreteLevelPeak)

{
    // "discrete" levels have changed
    if ((discreteLevel_ != discreteLevel) ||
            (discreteLevelPeak_ != discreteLevelPeak))
    {
        // update levels
        discreteLevel_ = discreteLevel;
        discreteLevelPeak_ = discreteLevelPeak;

        // update meter segments
        for (auto & segment : meterSegments_)
        {
            segment->setDiscreteLevels(
                discreteLevel_, discreteLevelPeak_);
        }
    }
}


/// Set discrete (peak) and normal (average) levels.
///
/// @param normalLevel new normal level
///
/// @param normalLevelPeak new normal peak level
///
/// @param discreteLevel new discrete level
///
/// @param discreteLevelPeak new discrete peak level
///
void GenericMeterBar::setLevels(
    float normalLevel, float normalLevelPeak,
    float discreteLevel, float discreteLevelPeak)

{
    // "normal" or "discrete" levels have changed
    if ((normalLevel_ != normalLevel) ||
            (normalLevelPeak_ != normalLevelPeak) ||
            (discreteLevel_ != discreteLevel) ||
            (discreteLevelPeak_ != discreteLevelPeak))
    {
        // update "normal" levels
        normalLevel_ = normalLevel;
        normalLevelPeak_ = normalLevelPeak;

        // update "discrete" levels
        discreteLevel_ = discreteLevel;
        discreteLevelPeak_ = discreteLevelPeak;

        // update meter bars
        for (auto & segment : meterSegments_)
        {
            segment->setLevels(
                normalLevel_, normalLevelPeak_,
                discreteLevel_, discreteLevelPeak_);
        }
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
