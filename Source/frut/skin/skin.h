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

#pragma once


class Skin
{
public:
    bool loadFromXml(const File &skinFile,
                     const String &rootName,
                     const String &assumedVersionNumber);

    XmlElement *getSetting(const String &tagName);

    XmlElement *getComponent(const String &tagName);

    bool getBoolean(const XmlElement *xmlComponent,
                    const String &attributeName,
                    const bool defaultValue = false);

    int getInteger(const XmlElement *xmlComponent,
                   const String &attributeName,
                   const int defaultValue = 0);

    float getFloat(const XmlElement *xmlComponent,
                   const String &attributeName,
                   const float defaultValue = 0.0f);

    const String getString(const XmlElement *xmlComponent,
                           const String &attributeName,
                           const String &defaultValue = "");

    const Colour getColour(const XmlElement *xmlComponent,
                           const float defaultHue = 0.0f);

    void loadImage(const String &strFilename,
                   Image &image);

    void setBackgroundImage(ImageComponent *background,
                            AudioProcessorEditor *editor);

    Point<int> getPosition(const XmlElement *xmlComponent,
                           const int componentHeight = 0);

    Rectangle<int> getBounds(const XmlElement *xmlComponent,
                             int width = -1,
                             int height = -1);

    void placeComponent(const XmlElement *xmlComponent,
                        Component *component);

    void placeMeterBar(const String &tagName,
                       widget::MeterBar *meterBar);

    void placeAndSkinButton(const String &tagName,
                            ImageButton *button);

    void placeAndSkinSlider(const String &tagName,
                            widget::FrutSlider *slider);

    void placeAndSkinNeedleMeter(const String &tagName,
                                 widget::NeedleMeter *meter);

    void placeAndSkinLabel(const String &tagName,
                           ImageComponent *label);

    void placeAndSkinSignalLed(const String &tagName,
                               widget::SignalLed *label);

    void placeAndSkinStateLabel(const String &tagName,
                                widget::StateLabel *label);

protected:
    ScopedPointer<XmlElement> document_;
    XmlElement *settingsGroup_;
    XmlElement *skinGroup_;
    XmlElement *skinFallback_1_;
    XmlElement *skinFallback_2_;

    File resourcePath_;

    String currentBackgroundName_;
    String currentGroupName_;
    String currentFallbackName_;

    bool originOfYIsBottom_;

    int backgroundWidth_;
    int backgroundHeight_;

private:
    JUCE_LEAK_DETECTOR(Skin);
};
