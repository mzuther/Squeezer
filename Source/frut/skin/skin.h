/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2020 Martin Zuther (http://www.mzuther.de/)

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

#ifndef FRUT_SKIN_SKIN_H
#define FRUT_SKIN_SKIN_H

namespace frut
{
namespace skin
{

class Skin
{
public:
    bool loadFromXml(const File &skinFile,
                     const String &rootName,
                     const String &assumedVersionNumber);

    static File getDefaultSkinFile(const File &skinDirectory);

    static String getDefaultSkin(const File &skinDirectory);
    static void setDefaultSkin(const String &defaultSkinName,
                               const File &skinDirectory);

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
                           const Colour defaultColour = Colours::red,
                           const String valuePrefix = "");

    std::unique_ptr<Drawable> loadImage(const String &strFilename);

    void loadImage(const String &strFilename,
                   Image &image);

    void setBackground(std::unique_ptr<Drawable> &background,
                       AudioProcessorEditor *editor);

    Point<int> getPosition(const XmlElement *xmlComponent,
                           const int componentHeight = 0);

    Rectangle<int> getBounds(const XmlElement *xmlComponent,
                             int width = -1,
                             int height = -1);

    void placeComponent(const XmlElement *xmlComponent,
                        Component *component);

    void placeMeterBar(const String &tagName,
                       widgets::MeterBar *meterBar);

    void placeAndSkinButton(const String &tagName,
                            DrawableButton *button);

    void placeAndSkinSlider(const String &tagName,
                            widgets::FrutSlider *slider);

    void placeAndSkinSlider(const String &tagName,
                            widgets::SliderCombined *slider);

    void placeAndSkinNeedleMeter(const String &tagName,
                                 widgets::NeedleMeter *meter);

    void placeAndSkinLabel(const String &tagName,
                           ImageComponent *label);

    void placeAndSkinSignalLed(const String &tagName,
                               widgets::SignalLed *label);

    void placeAndSkinStateLabel(const String &tagName,
                                widgets::StateLabel *label);

protected:
    std::unique_ptr<XmlElement> document_;

    std::unique_ptr<Drawable> createBogusImage(const String &warningText,
            int width,
            int height);

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

}
}

#endif  // FRUT_SKIN_SKIN_H
