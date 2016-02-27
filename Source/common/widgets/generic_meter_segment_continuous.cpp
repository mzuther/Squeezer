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

#include "generic_meter_segment_continuous.h"

/// Create a new continuous meter segment, complete with peak marker.
///
GenericMeterSegmentContinuous::GenericMeterSegmentContinuous()
{
    // initialise maximum x and y position of component
    maximumX_ = -1;
    maximumY_ = -1;

    // initialise level positions of normal meter
    normalLevelPosition_ = -1.0f;
    normalPeakPosition_ = -1.0f;

    // initialise level positions of discrete meter
    discreteLevelPosition_ = -1.0f;
    discretePeakPosition_ = -1.0f;

    // lowest level of a 24-bit-signal in decibels
    float initialLevel = -144.0f;

    // initialise thresholds and whether this segment the topmost
    // segment
    setThresholdAndRange(initialLevel, 1.0f, false);

    // make sure that segment is drawn after initialisation
    setLevels(initialLevel, initialLevel, initialLevel, initialLevel);
}


/// Set lower threshold, display range and whether the segment is the
/// topmost in a meter bar.
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
float GenericMeterSegmentContinuous::setThresholdAndRange(
    float lowerThreshold, float thresholdRange, bool isTopmost)

{
    // set lower level threshold (in decibels)
    lowerThreshold_ = lowerThreshold;

    // set level range above lower threshold (in decibels)
    thresholdRange_ = thresholdRange;

    // set upper level threshold (in decibels)
    upperThreshold_ = lowerThreshold_ + thresholdRange_;

    // is there a meter segment beyond this?
    isTopmost_ = isTopmost;

    // return upper threshold (may be useful for creating continuous
    // meters)
    return upperThreshold_;
}


/// Set colours of segment and peak marker.
///
/// @param segmentColour colour of the segment
///
/// @param peakMarkerColour colour of the peak marker
///
void GenericMeterSegmentContinuous::setColours(
    const Colour &segmentColour, const Colour &peakMarkerColour)

{
    // initialise segment colour
    segmentColour_ = segmentColour;

    // initialise background colour from segment colour
    backgroundColour_ = segmentColour_.withMultipliedBrightness(0.20f);

    // initialise peak marker's colour
    peakMarkerColour_ = peakMarkerColour;

    // redraw meter segment
    repaint();
}


/// Draw the bar by filling the complete drawing area with the segment
/// and background colours.
///
/// @param g graphics context
///
/// @param levelPosition level position (0.0 and below: hide bar; 1.0:
///        full bar)
///
void GenericMeterSegmentContinuous::drawBar(
    Graphics &g, float levelPosition)

{
    // avoid drawing errors
    if (levelPosition < 0.0f)
    {
        levelPosition = 0.0f;
    }

    // initialise drawing points
    int pos_1 = 0;
    int pos_2 = 0;

    // respect orientation
    switch (orientation_)
    {
    case GenericMeterOrientation::verticalInverted:

        // invert level position
        levelPosition = 1.0f - levelPosition;

        // keep going ...

    case GenericMeterOrientation::vertical:

        // initialise drawing points
        pos_1 = MZ_Juce_Common::round(maximumY_ * levelPosition);
        pos_2 = maximumY_ - pos_1;

        // make sure there is something to draw
        if (pos_2 > 0)
        {
            if (orientation_ == GenericMeterOrientation::vertical)
            {
                // set background colour
                g.setColour(backgroundColour_);
            }
            else
            {
                // set segment colour
                g.setColour(segmentColour_);
            }

            // draw first rectangle
            g.fillRect(1,
                       0,
                       maximumX_ - 1,
                       pos_2 + 1);
        }

        // make sure there is something to draw
        if (pos_1 > 0)
        {
            if (orientation_ == GenericMeterOrientation::vertical)
            {
                // set segment colour
                g.setColour(segmentColour_);
            }
            else
            {
                // set background colour
                g.setColour(backgroundColour_);
            }

            // draw second rectangle
            g.fillRect(1,
                       pos_2,
                       maximumX_ - 1,
                       pos_1 + 1);
        }

        break;

    case GenericMeterOrientation::horizontal:

        // invert level position
        levelPosition = 1.0f - levelPosition;

        // keep going ...

    case GenericMeterOrientation::horizontalInverted:

        // initialise drawing points
        pos_1 = MZ_Juce_Common::round(maximumX_ * levelPosition);
        pos_2 = maximumX_ - pos_1;

        // make sure there is something to draw
        if (pos_2 > 0)
        {
            if (orientation_ == GenericMeterOrientation::horizontalInverted)
            {
                // set background colour
                g.setColour(backgroundColour_);
            }
            else
            {
                // set segment colour
                g.setColour(segmentColour_);
            }

            // draw first rectangle
            g.fillRect(0,
                       1,
                       pos_2 + 1,
                       maximumY_ - 1);
        }

        // make sure there is something to draw
        if (pos_1 > 0)
        {
            if (orientation_ == GenericMeterOrientation::horizontalInverted)
            {
                // set segment colour
                g.setColour(segmentColour_);
            }
            else
            {
                // set background colour
                g.setColour(backgroundColour_);
            }

            // draw second rectangle
            g.fillRect(pos_2,
                       1,
                       pos_1 + 1,
                       maximumY_ - 1);
        }

        break;
    }
}


/// Draw a level marker (i. e. a line) using the specified colour.
///
/// @param g graphics context
///
/// @param markerColour colour to use for the marker
///
/// @param levelPosition level position (0.0 and below: hide marker;
///        1.0: marker at top)
///
void GenericMeterSegmentContinuous::drawMarker(
    Graphics &g, const Colour &markerColour, float levelPosition)

{
    // avoid drawing errors
    if (levelPosition < 0.0f)
    {
        return;
    }

    // initialise drawing points
    int pos_1 = 0;
    int pos_2 = 0;

    // respect orientation
    switch (orientation_)
    {
    case GenericMeterOrientation::verticalInverted:

        // invert level position
        levelPosition = 1.0f - levelPosition;

        // keep going ...

    case GenericMeterOrientation::vertical:

        // initialise drawing points
        pos_1 = MZ_Juce_Common::roundDown((maximumY_ + 1) * levelPosition);

        if (pos_1 > maximumY_)
        {
            pos_1 = maximumY_;
        }

        pos_2 = maximumY_ - pos_1;

        // set line colour
        g.setColour(markerColour);

        // draw marker
        g.drawRect(1,
                   pos_2,
                   maximumX_ - 1,
                   1);

        break;

    case GenericMeterOrientation::horizontal:

        // invert level position
        levelPosition = 1.0f - levelPosition;

        // keep going ...

    case GenericMeterOrientation::horizontalInverted:

        // initialise drawing points
        pos_1 = MZ_Juce_Common::roundDown((maximumX_ + 1) * levelPosition);

        if (pos_1 > maximumX_)
        {
            pos_1 = maximumX_;
        }

        pos_2 = maximumX_ - pos_1;

        // set line colour
        g.setColour(markerColour);

        // draw marker
        g.drawRect(pos_2,
                   1,
                   1,
                   maximumY_ - 1);

        break;
    }
}


/// Paint segment.
///
/// @param g the graphics context that must be used to do the drawing
///        operations
///
void GenericMeterSegmentContinuous::paint(
    Graphics &g)

{
    // draw meter bar
    drawBar(g, normalLevelPosition_);

    // draw discrete level marker
    if (discreteLevelPosition_ >= 0.0f)
    {
        drawMarker(g, segmentColour_, discreteLevelPosition_);
    }

    // draw normal peak marker
    if (normalPeakPosition_ >= 0.0f)
    {
        drawMarker(g, peakMarkerColour_, normalPeakPosition_);
    }

    // draw discrete peak marker
    if (discretePeakPosition_ >= 0.0f)
    {
        drawMarker(g, peakMarkerColour_, discretePeakPosition_);
    }
}


/// Called when this component's visibility changes.
///
/// If this function did not exist, the meter segment wouldn't be
/// drawn until the first level change!
void GenericMeterSegmentContinuous::visibilityChanged()
{
}


/// Called when this component's size has been changed.
///
void GenericMeterSegmentContinuous::resized()
{
    // update maximum x and y position of component
    maximumX_ = getWidth() - 1;
    maximumY_ = getHeight() - 1;
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
void GenericMeterSegmentContinuous::setLevels(
    float normalLevel, float normalLevelPeak,
    float discreteLevel, float discreteLevelPeak)

{
    // store old brightness and peak marker values
    float normalLevelPositionOld = normalLevelPosition_;
    float normalPeakPositionOld = normalPeakPosition_;

    float discreteLevelPositionOld = discreteLevelPosition_;
    float discretePeakPositionOld = discretePeakPosition_;

    // normal level lies on or above upper threshold, so fully light
    // meter segment
    if (normalLevel >= upperThreshold_)
    {
        normalLevelPosition_ = 1.0f;
    }
    // normal level lies below lower threshold, so set meter segment
    // to dark
    else if (normalLevel < lowerThreshold_)
    {
        normalLevelPosition_ = -1.0f;
    }
    // normal level lies within thresholds or on lower threshold, so
    // calculate position from current level
    else
    {
        normalLevelPosition_ = (normalLevel - lowerThreshold_) /
                               thresholdRange_;
    }

    // discrete level lies below lower threshold
    if (discreteLevel < lowerThreshold_)
    {
        // hide level marker
        discreteLevelPosition_ = -1.0f;
    }
    // discrete level lies on or above lower threshold and below upper
    // threshold, so calculate position from current level
    else if (discreteLevel < upperThreshold_)
    {
        discreteLevelPosition_ = (discreteLevel - lowerThreshold_) /
                                 thresholdRange_;
    }
    // discrete level lies on or above upper threshold
    else
    {
        // there is no meter segment beyond this
        if (isTopmost_)
        {
            // draw level marker!
            discreteLevelPosition_ = 1.0f;
        }
        // otherwise
        else
        {
            // hide level marker!
            discreteLevelPosition_ = -1.0f;
        }
    }

    // normal peak level lies below lower threshold
    if (normalLevelPeak < lowerThreshold_)
    {
        // hide level marker
        normalPeakPosition_ = -1.0f;
    }
    // normal peak level lies on or above lower threshold and below
    // upper threshold, so calculate position from current level
    else if (normalLevelPeak < upperThreshold_)
    {
        normalPeakPosition_ = (normalLevelPeak - lowerThreshold_) /
                              thresholdRange_;
    }
    // normal peak level lies on or above upper threshold
    else
    {
        // there is no meter segment beyond this
        if (isTopmost_)
        {
            // draw level marker!
            normalPeakPosition_ = 1.0f;
        }
        // otherwise
        else
        {
            // hide marker!
            normalPeakPosition_ = -1.0f;
        }
    }

    // discrete peak level lies below lower threshold
    if (discreteLevelPeak < lowerThreshold_)
    {
        // hide level marker
        discretePeakPosition_ = -1.0f;
    }
    // discrete peak level lies on or above lower threshold and below
    // upper threshold, so calculate position from current level
    else if (discreteLevelPeak < upperThreshold_)
    {
        discretePeakPosition_ = (discreteLevelPeak - lowerThreshold_) /
                                thresholdRange_;
    }
    // discrete peak level lies on or above upper threshold
    else
    {
        // there is no meter segment beyond this
        if (isTopmost_)
        {
            // draw level marker!
            discretePeakPosition_ = 1.0f;
        }
        // otherwise
        else
        {
            // hide level marker!
            discretePeakPosition_ = -1.0f;
        }
    }

    // re-paint meter segment only if something has changed
    if ((normalLevelPosition_ != normalLevelPositionOld) ||
            (normalPeakPosition_ != normalPeakPositionOld) ||
            (discreteLevelPosition_ != discreteLevelPositionOld) ||
            (discretePeakPosition_ != discretePeakPositionOld))
    {
        repaint();
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
