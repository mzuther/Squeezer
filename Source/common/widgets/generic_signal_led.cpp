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

#include "generic_signal_led.h"

/// Create a signal LED.
///
/// @param componentName String containing the component's name
///
GenericSignalLed::GenericSignalLed(const String &componentName)
{
    // set component name
    setName(componentName);

    // this component blends in with the background
    setOpaque(false);

    // initialise LED's level (0.0f is dark, 1.0f is fully lit)
    level = 0.0f;
}


/// Destructor.
///
GenericSignalLed::~GenericSignalLed()
{
    // nothing to do, really
}


/// Set new level.
///
/// @param level new level (0.0 is dark, 1.0 is fully lit)
///
void GenericSignalLed::setLevel(float levelNew)
{
    // re-paint meter segment only when the level has changed
    if (levelNew != level)
    {
        // update level
        level = levelNew;

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
void GenericSignalLed::paint(Graphics &g)
{
    // LED is dark
    if (level <= 0.0f)
    {
        // paint imageOff
        g.setOpacity(1.0f);
        g.drawImageAt(imageOff, 0, 0, false);
    }
    // LED is fully lit
    else if (level >= 1.0f)
    {
        // paint imageHigh
        g.setOpacity(1.0f);
        g.drawImageAt(imageHigh, 0, 0, false);
    }
    // LED is in between
    else
    {
        // paint imageLow
        g.setOpacity(1.0f);
        g.drawImageAt(imageLow, 0, 0, false);

        // blend with imageHigh
        g.setOpacity(level);
        g.drawImageAt(imageHigh, 0, 0, false);
    }
}


/// Set new images.
///
/// @param imageOffNew image is displayed when the LED is dark
///
/// @param imageLowNew image is displayed for levels in between (and
///        mixed with *imageHighNew*)
///
/// @param imageHighNew image is displayed when the LED is fully lit
///
void GenericSignalLed::setImages(Image &imageOffNew, Image &imageLowNew, Image &imageHighNew)
{
    // update images (creates a copy of each image)
    imageOff = imageOffNew.createCopy();
    imageLow = imageLowNew.createCopy();
    imageHigh = imageHighNew.createCopy();

    // assert that all images have the same size
    jassert(imageOff.getBounds() == imageLow.getBounds());
    jassert(imageOff.getBounds() == imageHigh.getBounds());

    // redraw component
    repaint(getLocalBounds());
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
