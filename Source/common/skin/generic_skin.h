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

#ifndef __GENERIC_SKIN_H__
#define __GENERIC_SKIN_H__

class GenericSkin;

#include "JuceHeader.h"
#include "../widgets/generic_horizontal_meter.h"
#include "../widgets/generic_meter_bar.h"
#include "../widgets/generic_signal_led.h"
#include "../widgets/generic_state_label.h"


class GenericSkin
{
public:
    GenericSkin();

    bool loadFromXml(File &fileSkin, const String &strXmlRootName);
    void placeComponent(Component *component, String strXmlTag);
    void placeMeterBar(GenericMeterBar *meterBar, String strXmlTag);
    void placeAndSkinButton(ImageButton *button, String strXmlTag);
    void placeAndSkinHorizontalMeter(GenericHorizontalMeter *meter, String strXmlTag);
    void placeAndSkinLabel(ImageComponent *label, String strXmlTag);
    void placeAndSkinSignalLed(GenericSignalLed *label, String strXmlTag);
    void placeAndSkinStateLabel(GenericStateLabel *label, String strXmlTag);
    void setBackgroundImage(ImageComponent *background, AudioProcessorEditor *editor);

protected:
    XmlElement *getComponentFromXml(String strXmlTag);

    ScopedPointer<XmlElement> xml;
    XmlElement *xmlSkinGroup;
    XmlElement *xmlSkinFallback_1;
    XmlElement *xmlSkinFallback_2;

    File fileResourcePath;

    String strBackgroundSelector;
    String strSkinGroup;
    String strSkinFallback_1;

private:
    JUCE_LEAK_DETECTOR(GenericSkin);
};

#endif   // __GENERIC_SKIN_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
