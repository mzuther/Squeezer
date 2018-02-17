/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2018 Martin Zuther (http://www.mzuther.de/)

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


/// Create a signal LED.
///
SignalLed::SignalLed()
{
    // this component blends in with the background
    setOpaque(false);

    // initialise LED's level (0.0f is dark, 1.0f is fully lit)
    level_ = 0.0f;
}


/// Set new level.
///
/// @param level new level (0.0 is dark, 1.0 is fully lit)
///
void SignalLed::setLevel(
    float level)

{
    // re-paint meter segment only when the level has changed
    if (level != level_)
    {
        // update level
        level_ = level;

        // redraw component
        repaint(getLocalBounds());
    }
}


/// Draw content of component.
///
/// The paint() method gets called when a region of a component needs
/// redrawing, either because the component's repaint() method has
/// been called, or because something has happened on the screen that
/// means a section of a window needs to be redrawn.
void SignalLed::paint(
    Graphics &g)

{
    // LED is dark
    if (level_ <= 0.0f)
    {
        // only use imageOff_
        g.setOpacity(1.0f);
        g.drawImageAt(imageOff_, 0, 0, false);
    }
    // LED is fully lit
    else if (level_ >= 1.0f)
    {
        // only use imageHigh_
        g.setOpacity(1.0f);
        g.drawImageAt(imageHigh_, 0, 0, false);
    }
    // LED is in between
    else
    {
        // blend imageLow_ and imageHigh_
        g.setOpacity(1.0f);
        g.drawImageAt(imageLow_, 0, 0, false);

        g.setOpacity(level_);
        g.drawImageAt(imageHigh_, 0, 0, false);
    }
}


/// Set new images.
///
/// @param imageOff image is displayed when the LED is dark
///
/// @param imageLow image is displayed for levels in between (and
///        mixed with *imageHigh*)
///
/// @param imageHigh image is displayed when the LED is fully lit
///
void SignalLed::setImages(
    const Image &imageOff,
    const Image &imageLow,
    const Image &imageHigh)

{
    // update images (creates a copy of each image)
    imageOff_ = imageOff.createCopy();
    imageLow_ = imageLow.createCopy();
    imageHigh_ = imageHigh.createCopy();

    // assert that all images have the same size
    jassert(imageOff_.getBounds() == imageLow_.getBounds());
    jassert(imageOff_.getBounds() == imageHigh_.getBounds());

    // redraw component
    repaint(getLocalBounds());
}
