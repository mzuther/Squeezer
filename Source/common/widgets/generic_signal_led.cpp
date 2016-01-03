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

GenericSignalLed::GenericSignalLed(const String &componentName)
{
    // set component name
    setName(componentName);

    // this component blends in with the background
    setOpaque(false);

    // initialise meter segment's brightness (0.0f is dark, 1.0f is
    // fully lit)
    fBrightness = 0.0f;

    imageOff = Image();
    imageLow = Image();
    imageHigh = Image();
}


GenericSignalLed::~GenericSignalLed()
{
    // nothing to do, really
}


void GenericSignalLed::resized()
{
}


void GenericSignalLed::setLevel(float fBrightnessNew)
{
    // re-paint meter segment only when the brightness has changed
    if (fBrightnessNew != fBrightness)
    {
        // update brightness
        fBrightness = fBrightnessNew;

        // redraw component
        repaint(getLocalBounds());
    }
}


void GenericSignalLed::paint(Graphics &g)
{
    if (fBrightness <= 0.0f)
    {
        g.setOpacity(1.0f);
        g.drawImageAt(imageOff, 0, 0, false);
    }
    else if (fBrightness >= 1.0f)
    {
        g.setOpacity(1.0f);
        g.drawImageAt(imageHigh, 0, 0, false);
    }
    else
    {
        g.setOpacity(1.0f);
        g.drawImageAt(imageLow, 0, 0, false);

        g.setOpacity(fBrightness);
        g.drawImageAt(imageHigh, 0, 0, false);
    }
}


void GenericSignalLed::setImages(Image &imageOffNew, Image &imageLowNew, Image &imageHighNew)
{
    imageOff = Image(imageOffNew);
    imageLow = Image(imageLowNew);
    imageHigh = Image(imageHighNew);

    jassert(imageOff.getBounds() == imageLow.getBounds());
    jassert(imageOff.getBounds() == imageHigh.getBounds());

    // redraw component
    repaint(getLocalBounds());
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
