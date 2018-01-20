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


// The following class members are initialized in the derived class:
//
// * currentBackgroundName_
// * currentFallbackName_
// * currentGroupName_
// * skinGroup_
// * skinFallback_1_
// * skinFallback_2_
//
bool Skin::loadFromXml(
    const File &skinFile,
    const String &rootName,
    const String &assumedVersionNumber)
{
    settingsGroup_ = nullptr;
    skinGroup_ = nullptr;
    skinFallback_1_ = nullptr;
    skinFallback_2_ = nullptr;

    backgroundWidth_ = 0;
    backgroundHeight_ = 0;

    Logger::outputDebugString(
        String("[Skin] loading file \"") +
        skinFile.getFileName() + "\"");

    document_ = XmlDocument::parse(skinFile);

    if (document_ == nullptr)
    {
        Logger::outputDebugString(
            String("[Skin] file \"") +
            skinFile.getFullPathName() +
            "\" not found");

        return false;
    }

    String skinVersion = getString(document_, "version");

    if (skinVersion.compare(assumedVersionNumber) != 0)
    {
        Logger::outputDebugString(
            String("[Skin] file \"") +
            skinFile.getFileName() +
            "\" has incompatible version number \"" +
            skinVersion +
            "\"");

        // try to keep running
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

        String resourcePathName = getString(document_, "path");
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


XmlElement *Skin::getComponentFromXml(
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


XmlElement *Skin::getSetting(
    const String &tagName)
{
    if (settingsGroup_ == nullptr)
    {
        Logger::outputDebugString(
            String("[Skin] settings group not found"));

        return nullptr;
    }

    XmlElement *xmlSetting = settingsGroup_->getChildByName(tagName);

    if (xmlSetting == nullptr)
    {
        Logger::outputDebugString(
            String("[Skin] XML element \"") +
            tagName +
            "\" not found in settings");

        return nullptr;
    }

    return xmlSetting;
}


bool Skin::getBoolean(
    const XmlElement *xmlComponent,
    const String &attributeName,
    const bool defaultValue)
{
    if (xmlComponent == nullptr)
    {
        return defaultValue;
    }
    else
    {
        return xmlComponent->getBoolAttribute(attributeName, defaultValue);
    }
}


bool Skin::getBooleanSetting(
    const String &tagName,
    const String &attributeName,
    const bool defaultValue)
{
    XmlElement *xmlSetting = getSetting(tagName);
    return getBoolean(xmlSetting, attributeName, defaultValue);
}


int Skin::getInteger(
    const XmlElement *xmlComponent,
    const String &attributeName,
    const int defaultValue)
{
    if (xmlComponent == nullptr)
    {
        return defaultValue;
    }
    else
    {
        return xmlComponent->getIntAttribute(attributeName, defaultValue);
    }
}


int Skin::getIntegerSetting(
    const String &tagName,
    const String &attributeName,
    const int defaultValue)
{
    XmlElement *xmlSetting = getSetting(tagName);
    return getInteger(xmlSetting, attributeName, defaultValue);
}


float Skin::getFloat(
    const XmlElement *xmlComponent,
    const String &attributeName,
    const float defaultValue)
{
    if (xmlComponent == nullptr)
    {
        return defaultValue;
    }
    else
    {
        double result = xmlComponent->getDoubleAttribute(
                            attributeName, defaultValue);

        return static_cast<float>(result);
    }
}


float Skin::getFloatSetting(
    const String &tagName,
    const String &attributeName,
    const float defaultValue)
{
    XmlElement *xmlSetting = getSetting(tagName);
    return getFloat(xmlSetting, attributeName, defaultValue);
}


const String Skin::getString(
    const XmlElement *xmlComponent,
    const String &attributeName,
    const String &defaultValue)
{
    if (xmlComponent == nullptr)
    {
        return defaultValue;
    }
    else
    {
        return xmlComponent->getStringAttribute(attributeName, defaultValue);
    }
}


const String Skin::getStringSetting(
    const String &tagName,
    const String &attributeName,
    const String &defaultValue)
{
    XmlElement *xmlSetting = getSetting(tagName);
    return getString(xmlSetting, attributeName, defaultValue);
}


const Colour Skin::getColour(
    const XmlElement *xmlComponent,
    const float defaultHue)
{
    float hue = getFloat(xmlComponent,
                         "hue",
                         defaultHue);

    float saturation = getFloat(xmlComponent,
                                "saturation",
                                1.00f);

    float brightness = getFloat(xmlComponent,
                                "brightness",
                                1.00f);

    float alpha = getFloat(xmlComponent,
                           "alpha",
                           1.00f);

    // initialise HSBA colour
    return Colour(hue, saturation, brightness, alpha);
}


const Colour Skin::getColourSetting(
    const String &tagName,
    const float defaultHue)
{
    XmlElement *xmlSetting = getSetting(tagName);
    return getColour(xmlSetting, defaultHue);
}


void Skin::setBackgroundImage(
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
            String strImage = getString(xmlBackground,
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
            String strImage = getString(xmlMeterGraduation,
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

                int x = getInteger(xmlMeterGraduation, "x", -1);
                int y = getInteger(xmlMeterGraduation, "y", -1);

                Graphics g(imageBackground);
                g.drawImageAt(imageMeterGraduation, x, y, false);
            }
        }

        backgroundWidth_ = imageBackground.getWidth();
        backgroundHeight_ = imageBackground.getHeight();

        background->setImage(imageBackground);
        background->setBounds(0, 0, backgroundWidth_, backgroundHeight_);

        // moves background image to the back of the editor's z-plane
        // so that it doesn't overlay (and thus block) any other
        // components
        background->toBack();

        editor->setSize(backgroundWidth_, backgroundHeight_);
    }
}


void Skin::placeComponent(
    const XmlElement *xmlComponent,
    Component *component)
{
    jassert(component != nullptr);
    jassert(backgroundHeight_ != 0);

    int width = getInteger(xmlComponent, "width", 0);
    int height = getInteger(xmlComponent, "height", 0);

    int x = getInteger(xmlComponent, "x", -1);
    int y = backgroundHeight_ - getInteger(xmlComponent, "y", -1) - height;

    component->setBounds(x, y, width, height);
}


void Skin::placeMeterBar(
    const String &tagName,
    widget::MeterBar *meterBar)
{
    jassert(meterBar != nullptr);

    XmlElement *xmlComponent = getComponentFromXml(tagName);

    if (xmlComponent != nullptr)
    {
        int x = getInteger(xmlComponent, "x", -1);
        int y = getInteger(xmlComponent, "y", -1);

        meterBar->setTopLeftPosition(x, y);

        int segment_width = getInteger(xmlComponent, "segment_width", 0);
        bool isVertical = getBoolean(xmlComponent, "vertical", true);

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
            meterBar->setOrientation(widget::Orientation::vertical);
        }
        // horizontal bar
        else
        {
            meterBar->setOrientation(widget::Orientation::horizontal);
        }
    }
}


void Skin::placeAndSkinButton(
    const String &tagName,
    ImageButton *button)
{
    jassert(button != nullptr);

    XmlElement *xmlButton = getComponentFromXml(tagName);

    if (xmlButton != nullptr)
    {
        int x = getInteger(xmlButton, "x", -1);
        int y = getInteger(xmlButton, "y", -1);

        String strImageOn = getString(xmlButton, "image_on");
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

        String strImageOff = getString(xmlButton, "image_off");
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

        Image imageOver;
        String strImageOver = getString(xmlButton, "image_over");

        if (strImageOver.isEmpty())
        {
            File fileImageOver = resourcePath_.getChildFile(strImageOver);

            if (!fileImageOver.existsAsFile())
            {
                Logger::outputDebugString(
                    String("[Skin] image file \"") +
                    fileImageOver.getFullPathName() +
                    "\" not found");

                imageOver = Image();
            }
            else
            {
                imageOver = ImageFileFormat::loadFrom(fileImageOver);
            }
        }
        else
        {
            imageOver = imageOn.createCopy();
            imageOver.multiplyAllAlphas(0.5f);
        }

        button->setImages(true, true, true,
                          imageOff, 1.0f, Colour(),
                          imageOver, 1.0f, Colour(),
                          imageOn, 1.0f, Colour(),
                          0.3f);
        button->setTopLeftPosition(x, backgroundHeight_ - y - imageOn.getHeight());
    }
}


void Skin::placeAndSkinSlider(
    const String &tagName,
    widget::SliderCombined *slider)
{
    XmlElement *xmlComponent = getComponentFromXml(tagName);
    Colour sliderColour = getColour(xmlComponent, 0.0f);

    placeComponent(xmlComponent, slider);
    slider->setSliderColour(sliderColour);
}


void Skin::placeAndSkinNeedleMeter(
    const String &tagName,
    widget::NeedleMeter *meter)
{
    jassert(meter != nullptr);

    XmlElement *xmlMeter = getComponentFromXml(tagName);

    if (xmlMeter != nullptr)
    {
        int x = getInteger(xmlMeter, "x", -1);
        int y = getInteger(xmlMeter, "y", -1);

        int spacing_left = getInteger(xmlMeter, "spacing_left", 0);
        int spacing_top = getInteger(xmlMeter, "spacing_top", 0);

        String strImageBackground = getString(xmlMeter, "image");
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

        String strImageNeedle = getString(xmlMeter, "image_needle");
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


void Skin::placeAndSkinLabel(
    const String &tagName,
    ImageComponent *label)
{
    jassert(label != nullptr);

    XmlElement *xmlLabel = getComponentFromXml(tagName);

    if (xmlLabel != nullptr)
    {
        int x = getInteger(xmlLabel, "x", -1);
        int y = getInteger(xmlLabel, "y", -1);

        String strImage = getString(xmlLabel, "image");
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
        label->setBounds(x, backgroundHeight_ - y - height, width, height);
    }
}


void Skin::placeAndSkinSignalLed(
    const String &tagName,
    widget::SignalLed *label)
{
    jassert(label != nullptr);

    XmlElement *xmlLabel = getComponentFromXml(tagName);

    if (xmlLabel != nullptr)
    {
        int x = getInteger(xmlLabel, "x", -1);
        int y = getInteger(xmlLabel, "y", -1);

        String strImageOff = getString(xmlLabel, "image_off");
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

        String strImageLow = getString(xmlLabel, "image_low");
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

        String strImageHigh = getString(xmlLabel, "image_high");
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


void Skin::placeAndSkinStateLabel(
    const String &tagName,
    widget::StateLabel *label)
{
    jassert(label != nullptr);

    XmlElement *xmlLabel = getComponentFromXml(tagName);

    if (xmlLabel != nullptr)
    {
        int x = getInteger(xmlLabel, "x", -1);
        int y = getInteger(xmlLabel, "y", -1);

        int spacing_left = getInteger(xmlLabel, "spacing_left", 0);
        int spacing_top = getInteger(xmlLabel, "spacing_top", 0);
        int font_size = getInteger(xmlLabel, "font_size", 12);

        String strColourOff = getString(xmlLabel, "colour_off", "ffffff");
        String strColourOn = getString(xmlLabel, "colour_on", "ffffff");
        String strColourActive = getString(xmlLabel, "colour_active", "ffffff");

        String strImageOff = getString(xmlLabel, "image_off");
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

        String strImageOn = getString(xmlLabel, "image_on");
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
        String strImageActive = getString(xmlLabel, "image_active", strImageOn);
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
                         strColourOff,
                         strColourOn,
                         strColourActive,
                         spacing_left,
                         spacing_top,
                         static_cast<float>(font_size));

        label->setBounds(x, y, width, height);
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
