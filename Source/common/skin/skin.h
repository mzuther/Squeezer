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

#ifndef __FRUT_SKIN_H__
#define __FRUT_SKIN_H__


class Skin
{
public:
    bool loadFromXml(File &skinFile,
                     const String &rootName);

    int getIntegerSetting(const String &tagName,
                          const String &attributeName,
                          int defaultValue = 0);

    float getFloatSetting(
        const String &tagName,
        const String &attributeName,
        float defaultValue = 0.0f);

    const String getStringSetting(
        const String &tagName,
        const String &attributeName,
        const String &defaultValue = "");

    const Colour getColourSetting(
        const String &tagName,
        float defaultHue = 0.0f);

    void placeComponent(Component *component,
                        const String &tagName);

    void placeMeterBar(widget::MeterBar *meterBar,
                       const String &tagName);

    void placeAndSkinButton(ImageButton *button,
                            const String &tagName);

    void placeAndSkinNeedleMeter(widget::NeedleMeter *meter,
                                 const String &tagName);

    void placeAndSkinLabel(ImageComponent *label,
                           const String &tagName);

    void placeAndSkinSignalLed(widget::SignalLed *label,
                               const String &tagName);

    void placeAndSkinStateLabel(widget::StateLabel *label,
                                const String &tagName);

    void setBackgroundImage(ImageComponent *background,
                            AudioProcessorEditor *editor);

protected:
    XmlElement *getComponentFromXml(const String &tagName);

    XmlElement *getSetting(const String &tagName);

    ScopedPointer<XmlElement> document_;
    XmlElement *settingsGroup_;
    XmlElement *skinGroup_;
    XmlElement *skinFallback_1_;
    XmlElement *skinFallback_2_;

    File resourcePath_;

    String currentBackgroundName_;
    String currentGroupName_;
    String currentFallbackName_;

private:
    JUCE_LEAK_DETECTOR(Skin);
};

#endif   // __FRUT_SKIN_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
