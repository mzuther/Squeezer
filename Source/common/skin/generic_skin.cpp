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

#include "generic_skin.h"


bool GenericSkin::loadFromXml(
    File &skinFile,
    const String &rootName)

{
    Logger::outputDebugString(
        String("[Skin] loading file \"") +
        skinFile.getFileName() + "\"");

    document_ = XmlDocument::parse(skinFile);

    settingsGroup_ = nullptr;
    skinGroup_ = nullptr;
    skinFallback_1_ = nullptr;
    skinFallback_2_ = nullptr;

    if (document_ == nullptr)
    {
        Logger::outputDebugString(
            String("[Skin] file \"") +
            skinFile.getFullPathName() +
            "\" not found");

        return false;
    }

    settingsGroup_ = document_->getChildByName("settings");
    skinFallback_2_ = document_->getChildByName("default");

    if ((!document_->hasTagName(rootName)) ||
            (skinFallback_2_ == nullptr))
    {
        Logger::outputDebugString(
            "[Skin] XML file not valid");

        document_ = nullptr;

        return false;
    }
    else
    {
        skinGroup_ = document_->getChildByName(currentGroupName_);
        skinFallback_1_ = document_->getChildByName(currentFallbackName_);

        String resourcePathName = document_->getStringAttribute("path");
        resourcePath_ = File(skinFile.getSiblingFile(resourcePathName));

        if (!resourcePath_.isDirectory())
        {
            Logger::outputDebugString(
                String("[Skin] directory \"") +
                resourcePath_.getFullPathName() +
                "\" not found");

            document_ = nullptr;

            return false;
        }
    }

    return true;
}


XmlElement *GenericSkin::getSetting(
    const String &tagName)

{
    if (settingsGroup_ == nullptr)
    {
        Logger::outputDebugString(
            String("[Skin] XML element \"settings\" not found"));

        return nullptr;
    }

    XmlElement *xmlSetting = settingsGroup_->getChildByName(tagName);

    if (xmlSetting == nullptr)
    {
        Logger::outputDebugString(
            String("[Skin] XML element \"") +
            tagName +
            "\" not found");

        return nullptr;
    }

    return xmlSetting;
}


int GenericSkin::getIntegerSetting(
    const String &tagName,
    const String &attributeName,
    int defaultValue)

{
    XmlElement *xmlSetting = getSetting(tagName);

    if (xmlSetting == nullptr)
    {
        return defaultValue;
    }
    else
    {
        return xmlSetting->getIntAttribute(attributeName, defaultValue);
    }
}


float GenericSkin::getFloatSetting(
    const String &tagName,
    const String &attributeName,
    float defaultValue)

{
    XmlElement *xmlSetting = getSetting(tagName);

    if (xmlSetting == nullptr)
    {
        return defaultValue;
    }
    else
    {
        double result = xmlSetting->getDoubleAttribute(
                            attributeName, defaultValue);

        return static_cast<float>(result);
    }
}


const String GenericSkin::getStringSetting(
    const String &tagName,
    const String &attributeName,
    const String &defaultValue)

{
    XmlElement *xmlSetting = getSetting(tagName);

    if (xmlSetting == nullptr)
    {
        return defaultValue;
    }
    else
    {
        return xmlSetting->getStringAttribute(attributeName, defaultValue);
    }
}


const Colour GenericSkin::getColourSetting(
    const String &tagName,
    float defaultHue)

{
    float hue = getFloatSetting(tagName,
                                "hue",
                                defaultHue);

    float saturation = getFloatSetting(tagName,
                                       "saturation",
                                       1.00f);

    float brightness = getFloatSetting(tagName,
                                       "brightness",
                                       1.00f);

    float alpha = getFloatSetting(tagName,
                                  "alpha",
                                  1.00f);

    // initialise HSBA colour
    return Colour(hue, saturation, brightness, alpha);
}


XmlElement *GenericSkin::getComponentFromXml(
    const String &tagName)

{
    XmlElement *xmlComponent;

    // suppress unnecessary warnings and save some time
    if (document_ == nullptr)
    {
        xmlComponent = nullptr;
    }
    else if ((skinGroup_ != nullptr) &&
             (skinGroup_->getChildByName(tagName) != nullptr))
    {
        xmlComponent = skinGroup_->getChildByName(tagName);
    }
    else if ((skinFallback_1_ != nullptr) &&
             (skinFallback_1_->getChildByName(tagName) != nullptr))
    {
        xmlComponent = skinFallback_1_->getChildByName(tagName);
    }
    else if ((skinFallback_2_ != nullptr) &&
             (skinFallback_2_->getChildByName(tagName) != nullptr))
    {
        xmlComponent = skinFallback_2_->getChildByName(tagName);
    }
    else
    {
        Logger::outputDebugString(
            String("[Skin] XML element \"") +
            tagName +
            "\" not found");

        xmlComponent = nullptr;
    }

    return xmlComponent;
}


void GenericSkin::placeAndSkinButton(
    ImageButton *button,
    const String &tagName)

{
    jassert(button != nullptr);

    XmlElement *xmlButton = getComponentFromXml(tagName);

    if (xmlButton != nullptr)
    {
        int x = xmlButton->getIntAttribute("x", -1);
        int y = xmlButton->getIntAttribute("y", -1);

        String strImageOn = xmlButton->getStringAttribute("image_on");
        File fileImageOn = resourcePath_.getChildFile(strImageOn);
        Image imageOn;

        if (!fileImageOn.existsAsFile())
        {
            Logger::outputDebugString(
                String("[Skin] image file \"") +
                fileImageOn.getFullPathName() +
                "\" not found");

            imageOn = Image();
        }
        else
        {
            imageOn = ImageFileFormat::loadFrom(fileImageOn);
        }

        String strImageOff = xmlButton->getStringAttribute("image_off");
        File fileImageOff = resourcePath_.getChildFile(strImageOff);
        Image imageOff;

        if (!fileImageOff.existsAsFile())
        {
            Logger::outputDebugString(
                String("[Skin] image file \"") +
                fileImageOff.getFullPathName() +
                "\" not found");

            imageOff = Image();
        }
        else
        {
            imageOff = ImageFileFormat::loadFrom(fileImageOff);
        }

        button->setImages(true, true, true,
                          imageOff, 1.0f, Colour(),
                          imageOn, 0.5f, Colour(),
                          imageOn, 1.0f, Colour(),
                          0.3f);
        button->setTopLeftPosition(x, y);
    }
}


void GenericSkin::placeAndSkinHorizontalMeter(
    GenericHorizontalMeter *meter,
    const String &tagName)

{
    jassert(meter != nullptr);

    XmlElement *xmlMeter = getComponentFromXml(tagName);

    if (xmlMeter != nullptr)
    {
        int x = xmlMeter->getIntAttribute("x", -1);
        int y = xmlMeter->getIntAttribute("y", -1);

        int spacing_left = xmlMeter->getIntAttribute("spacing_left", 0);
        int spacing_top = xmlMeter->getIntAttribute("spacing_top", 0);

        String strImageBackground = xmlMeter->getStringAttribute("image");
        File fileImageBackground = resourcePath_.getChildFile(
                                       strImageBackground);
        Image imageBackground;

        if (!fileImageBackground.existsAsFile())
        {
            Logger::outputDebugString(
                String("[Skin] image file \"") +
                fileImageBackground.getFullPathName() +
                "\" not found");

            imageBackground = Image();
        }
        else
        {
            imageBackground = ImageFileFormat::loadFrom(fileImageBackground);
        }

        String strImageNeedle = xmlMeter->getStringAttribute("image_needle");
        File fileImageNeedle = resourcePath_.getChildFile(strImageNeedle);
        Image imageNeedle;

        if (!fileImageNeedle.existsAsFile())
        {
            Logger::outputDebugString(
                String("[Skin] image file \"") +
                fileImageNeedle.getFullPathName() +
                "\" not found");

            imageNeedle = Image();
        }
        else
        {
            imageNeedle = ImageFileFormat::loadFrom(fileImageNeedle);
        }

        int width = imageBackground.getWidth();
        int height = imageBackground.getHeight();

        meter->setImages(imageBackground,
                         imageNeedle,
                         spacing_left,
                         spacing_top);

        meter->setBounds(x, y, width, height);
    }
}


void GenericSkin::placeAndSkinLabel(
    ImageComponent *label,
    const String &tagName)

{
    jassert(label != nullptr);

    XmlElement *xmlLabel = getComponentFromXml(tagName);

    if (xmlLabel != nullptr)
    {
        int x = xmlLabel->getIntAttribute("x", -1);
        int y = xmlLabel->getIntAttribute("y", -1);

        String strImage = xmlLabel->getStringAttribute("image");
        File fileImage = resourcePath_.getChildFile(strImage);
        Image imageLabel;

        if (!fileImage.existsAsFile())
        {
            Logger::outputDebugString(
                String("[Skin] image file \"") +
                fileImage.getFullPathName() +
                "\" not found");

            imageLabel = Image();
        }
        else
        {
            imageLabel = ImageFileFormat::loadFrom(fileImage);
        }

        int width = imageLabel.getWidth();
        int height = imageLabel.getHeight();

        label->setImage(imageLabel);
        label->setBounds(x, y, width, height);
    }
}


void GenericSkin::placeAndSkinSignalLed(
    GenericSignalLed *label,
    const String &tagName)

{
    jassert(label != nullptr);

    XmlElement *xmlLabel = getComponentFromXml(tagName);

    if (xmlLabel != nullptr)
    {
        int x = xmlLabel->getIntAttribute("x", -1);
        int y = xmlLabel->getIntAttribute("y", -1);

        String strImageOff = xmlLabel->getStringAttribute("image_off");
        File fileImageOff = resourcePath_.getChildFile(strImageOff);
        Image imageOff;

        if (!fileImageOff.existsAsFile())
        {
            Logger::outputDebugString(
                String("[Skin] image file \"") +
                fileImageOff.getFullPathName() +
                "\" not found");

            imageOff = Image();
        }
        else
        {
            imageOff = ImageFileFormat::loadFrom(fileImageOff);
        }

        String strImageLow = xmlLabel->getStringAttribute("image_low");
        File fileImageLow = resourcePath_.getChildFile(strImageLow);
        Image imageLow;

        if (!fileImageLow.existsAsFile())
        {
            Logger::outputDebugString(
                String("[Skin] image file \"") +
                fileImageLow.getFullPathName() +
                "\" not found");

            imageLow = Image();
        }
        else
        {
            imageLow = ImageFileFormat::loadFrom(fileImageLow);
        }

        String strImageHigh = xmlLabel->getStringAttribute("image_high");
        File fileImageHigh = resourcePath_.getChildFile(strImageHigh);
        Image imageHigh;

        if (!fileImageHigh.existsAsFile())
        {
            Logger::outputDebugString(
                String("[Skin] image file \"") +
                fileImageHigh.getFullPathName() +
                "\" not found");

            imageHigh = Image();
        }
        else
        {
            imageHigh = ImageFileFormat::loadFrom(fileImageHigh);
        }

        int width = imageOff.getWidth();

        if ((width != imageLow.getWidth()) ||
                (width != imageHigh.getWidth()))
        {
            Logger::outputDebugString(
                String("[Skin] width of image files for \"") +
                tagName +
                "\" differs");
        }

        int height = imageOff.getHeight();

        if ((height != imageLow.getHeight()) ||
                (height != imageHigh.getHeight()))
        {
            Logger::outputDebugString(
                String("[Skin] height of image files for \"") +
                tagName +
                "\" differs");
        }

        label->setImages(imageOff, imageLow, imageHigh);
        label->setBounds(x, y, width, height);
    }
}


void GenericSkin::placeAndSkinStateLabel(
    GenericStateLabel *label,
    const String &tagName)

{
    jassert(label != nullptr);

    XmlElement *xmlLabel = getComponentFromXml(tagName);

    if (xmlLabel != nullptr)
    {
        int x = xmlLabel->getIntAttribute("x", -1);
        int y = xmlLabel->getIntAttribute("y", -1);

        int spacing_left = xmlLabel->getIntAttribute("spacing_left", 0);
        int spacing_top = xmlLabel->getIntAttribute("spacing_top", 0);
        int font_size = xmlLabel->getIntAttribute("font_size", 12);

        String strColourOn = xmlLabel->getStringAttribute(
                                 "colour_on",
                                 "ffffff");

        String strColourActive = xmlLabel->getStringAttribute(
                                     "colour_active",
                                     "ffffff");

        String strImageOff = xmlLabel->getStringAttribute("image_off");
        File fileImageOff = resourcePath_.getChildFile(strImageOff);
        Image imageOff;

        if (!fileImageOff.existsAsFile())
        {
            Logger::outputDebugString(
                String("[Skin] image file \"") +
                fileImageOff.getFullPathName() +
                "\" not found");

            imageOff = Image();
        }
        else
        {
            imageOff = ImageFileFormat::loadFrom(fileImageOff);
        }

        String strImageOn = xmlLabel->getStringAttribute("image_on");
        File fileImageOn = resourcePath_.getChildFile(strImageOn);
        Image imageOn;

        if (!fileImageOn.existsAsFile())
        {
            Logger::outputDebugString(
                String("[Skin] image file \"") +
                fileImageOn.getFullPathName() +
                "\" not found");

            imageOn = Image();
        }
        else
        {
            imageOn = ImageFileFormat::loadFrom(fileImageOn);
        }

        // will use "image_on" if "image_active" does not exist
        String strImageActive = xmlLabel->getStringAttribute(
                                    "image_active",
                                    strImageOn);

        File fileImageActive = resourcePath_.getChildFile(strImageActive);
        Image imageActive;

        if (!fileImageActive.existsAsFile())
        {
            Logger::outputDebugString(
                String("[Skin] image file \"") +
                fileImageActive.getFullPathName() +
                "\" not found");

            imageActive = Image();
        }
        else
        {
            imageActive = ImageFileFormat::loadFrom(fileImageActive);
        }

        int width = imageOff.getWidth();

        if (width != imageActive.getWidth())
        {
            Logger::outputDebugString(
                String("[Skin] width of image files for \"") +
                tagName +
                "\" differs");
        }

        int height = imageOff.getHeight();

        if (height != imageActive.getHeight())
        {
            Logger::outputDebugString(
                String("[Skin] height of image files for \"") +
                tagName +
                "\" differs");
        }

        label->setImages(imageOff,
                         imageOn,
                         imageActive,
                         strColourOn,
                         strColourActive,
                         spacing_left,
                         spacing_top,
                         (float) font_size);

        label->setBounds(x, y, width, height);
    }
}


void GenericSkin::placeComponent(
    Component *component,
    const String &tagName)

{
    jassert(component != nullptr);

    XmlElement *xmlComponent = getComponentFromXml(tagName);

    if (xmlComponent != nullptr)
    {
        int x = xmlComponent->getIntAttribute("x", -1);
        int y = xmlComponent->getIntAttribute("y", -1);
        int width = xmlComponent->getIntAttribute("width", -1);
        int height = xmlComponent->getIntAttribute("height", -1);

        component->setBounds(x, y, width, height);
    }
}


void GenericSkin::placeMeterBar(
    GenericMeterBar *meterBar,
    const String &tagName)

{
    jassert(meterBar != nullptr);

    XmlElement *xmlComponent = getComponentFromXml(tagName);

    if (xmlComponent != nullptr)
    {
        int x = xmlComponent->getIntAttribute("x", -1);
        int y = xmlComponent->getIntAttribute("y", -1);

        meterBar->setTopLeftPosition(x, y);

        int segment_width = xmlComponent->getIntAttribute("segment_width", -1);
        bool isVertical = xmlComponent->getBoolAttribute("vertical", true);

        if (segment_width < 4)
        {
            Logger::outputDebugString(
                String("[Skin] segment width for \"") +
                tagName +
                "\" not set");

            segment_width = 8;
        }

        meterBar->setSegmentWidth(segment_width);

        // vertical bar
        if (isVertical)
        {
            meterBar->setOrientation(GenericMeterOrientation::vertical);
        }
        // horizontal bar
        else
        {
            meterBar->setOrientation(GenericMeterOrientation::horizontal);
        }
    }
}


void GenericSkin::setBackgroundImage(
    ImageComponent *background,
    AudioProcessorEditor *editor)

{
    if (skinGroup_ != nullptr)
    {
        Image imageBackground;

        XmlElement *xmlBackground = skinGroup_->getChildByName("background");

        if (xmlBackground == nullptr)
        {
            Logger::outputDebugString(
                String("[Skin] XML element \"") +
                currentGroupName_ +
                "\" specifies no background image");

            imageBackground = Image();
        }
        else
        {
            String strImage = xmlBackground->getStringAttribute(
                                  currentBackgroundName_);
            File fileImage = resourcePath_.getChildFile(strImage);

            if (!fileImage.existsAsFile())
            {
                Logger::outputDebugString(
                    String("[Skin] image file \"") +
                    fileImage.getFullPathName() +
                    "\" not found");

                imageBackground = Image();
            }
            else
            {
                imageBackground = ImageFileFormat::loadFrom(fileImage);
            }
        }

        XmlElement *xmlMeterGraduation = nullptr;

        // get rid of the "unused variable" warning
        (void) xmlMeterGraduation;

        forEachXmlChildElementWithTagName(*skinGroup_,
                                          xmlMeterGraduation,
                                          "meter_graduation")
        {
            String strImage = xmlMeterGraduation->getStringAttribute(
                                  currentBackgroundName_);
            File fileImage = resourcePath_.getChildFile(strImage);

            if (!fileImage.existsAsFile())
            {
                Logger::outputDebugString(
                    String("[Skin] image file \"") +
                    fileImage.getFullPathName() +
                    "\" not found");
            }
            else
            {
                Image imageMeterGraduation = ImageFileFormat::loadFrom(
                                                 fileImage);

                int x = xmlMeterGraduation->getIntAttribute("x", -1);
                int y = xmlMeterGraduation->getIntAttribute("y", -1);

                Graphics g(imageBackground);
                g.drawImageAt(imageMeterGraduation, x, y, false);
            }
        }

        int nBackgroundWidth = imageBackground.getWidth();
        int nBackgroundHeight = imageBackground.getHeight();

        background->setImage(imageBackground);
        background->setBounds(0, 0, nBackgroundWidth, nBackgroundHeight);

        // moves background image to the back of the editor's z-plane
        // so that it doesn't overlay (and thus block) any other
        // components
        background->toBack();

        editor->setSize(nBackgroundWidth, nBackgroundHeight);
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
