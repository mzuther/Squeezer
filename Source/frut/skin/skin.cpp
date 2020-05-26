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

namespace frut
{
namespace skin
{

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

    document_ = juce::parseXML(skinFile);

    if (document_ == nullptr)
    {
        Logger::outputDebugString(
            String("[Skin] file \"") +
            skinFile.getFullPathName() +
            "\" not found");

        return false;
    }

    String skinVersion = getString(document_.get(), "version");

    if (skinVersion.compare(assumedVersionNumber) != 0)
    {
        Logger::outputDebugString(
            String("[Skin] file \"") +
            skinFile.getFileName() +
            "\" has incompatible version number \"" +
            skinVersion +
            "\"");

        document_ = nullptr;
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

        if (skinGroup_ == nullptr)
        {
            Logger::outputDebugString(
                String("[Skin] XML element \"") +
                currentGroupName_ +
                "\" not found in settings");
        }

        skinFallback_1_ = document_->getChildByName(currentFallbackName_);

        String resourcePathName = getString(document_.get(), "path");
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

    String originOfY = getString(document_.get(), "origin_of_y", "top");
    originOfYIsBottom_ = originOfY.compare("bottom") == 0;

    return true;
}


File Skin::getDefaultSkinFile(
    const File &skinDirectory)
{
    // locate file containing the default skin's name
    File defaultSkinFile = skinDirectory.getChildFile("default_skin.ini");

    // make sure the file exists
    if (!defaultSkinFile.existsAsFile())
    {
        // create file
        defaultSkinFile.create();

        // set "Default" as default skin, as it comes with the plug-in
        // (uses Unicode encoding)
        defaultSkinFile.replaceWithText("Default", true, true);
    }

    return defaultSkinFile;
}


String Skin::getDefaultSkin(
    const File &skinDirectory)
{
    File defaultSkinFile = getDefaultSkinFile(skinDirectory);

    return defaultSkinFile.loadFileAsString();
}


void Skin::setDefaultSkin(
    const String &defaultSkinName,
    const File &skinDirectory)
{
    File defaultSkinFile = getDefaultSkinFile(skinDirectory);

    // uses Unicode encoding
    defaultSkinFile.replaceWithText(defaultSkinName, true, true);
}


XmlElement *Skin::getSetting(
    const String &tagName)
{
    if (document_ == nullptr)
    {
        return nullptr;
    }

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


XmlElement *Skin::getComponent(
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


const Colour Skin::getColour(
    const XmlElement *xmlComponent,
    const Colour defaultColour,
    const String valuePrefix)
{
    if (xmlComponent == nullptr)
    {
        return Colours::white;
    }

    float hue = getFloat(xmlComponent,
                         valuePrefix + "hue",
                         defaultColour.getHue());

    float saturation = getFloat(xmlComponent,
                                valuePrefix + "saturation",
                                defaultColour.getSaturation());

    float brightness = getFloat(xmlComponent,
                                valuePrefix + "brightness",
                                defaultColour.getBrightness());

    float alpha = getFloat(xmlComponent,
                           valuePrefix + "alpha",
                           defaultColour.getFloatAlpha());

    // initialise HSBA colour
    return Colour(hue, saturation, brightness, alpha);
}


std::unique_ptr<Drawable> Skin::createBogusImage(
    const String &warningText,
    int width,
    int height)
{
    auto drawable = new DrawableComposite();
    auto boundingBox = Rectangle<float>(
                           0,
                           0,
                           static_cast<float>(width),
                           static_cast<float>(height));

    auto rectangle = new DrawablePath();
    rectangle->setFill(Colours::red);
    drawable->addAndMakeVisible(rectangle);

    Path path;
    path.addRectangle(boundingBox);
    rectangle->setPath(path);

    auto text = new DrawableText();
    drawable->addAndMakeVisible(text);

    text->setText(warningText);
    text->setColour(Colours::white);
    text->setFontHeight(20);
    text->setJustification(Justification::Flags::topLeft);

    float shrinkFactor = 10.0f;
    text->setBoundingBox(boundingBox.reduced(shrinkFactor).withPosition(shrinkFactor, shrinkFactor));

    return std::unique_ptr<Drawable>(drawable);
}


std::unique_ptr<Drawable> Skin::loadImage(
    const String &strFilename)
{
    File fileImage = resourcePath_.getChildFile(strFilename);
    std::unique_ptr<Drawable> component;

    if (fileImage.existsAsFile())
    {
        component = Drawable::createFromImageFile(fileImage);

        if (strFilename.endsWith(".svg"))
        {
            DBG(String("[Skin] loading SVG file \"") + strFilename + "\" (" +
                String(component->getWidth()) + " x " + String(component->getHeight()) + " pixels)");
        }

        if (!component)
        {
            Logger::outputDebugString(
                String("[Skin] image file \"") +
                fileImage.getFullPathName() +
                "\" not loaded");

            component = createBogusImage("Image file not loaded", 200, 200);
        }
    }
    else
    {
        Logger::outputDebugString(
            String("[Skin] image file \"") +
            fileImage.getFullPathName() +
            "\" not found");

        component = createBogusImage("Image file not found", 200, 200);
    }

    return component;
}


void Skin::loadImage(
    const String &strFilename,
    Image &image)
{
    auto drawable = loadImage(strFilename);

    image = Image(Image::PixelFormat::ARGB,
                  drawable->getWidth(),
                  drawable->getHeight(),
                  true);

    Graphics g(image);
    drawable->draw(g, 1.0f);
}


void Skin::setBackground(
    std::unique_ptr<Drawable> &background,
    AudioProcessorEditor *editor)
{
    if (skinGroup_ != nullptr)
    {
        XmlElement *xmlBackground = skinGroup_->getChildByName("background");

        if (xmlBackground != nullptr)
        {
            String imageFilename = getString(xmlBackground,
                                             currentBackgroundName_);
            background = loadImage(imageFilename);

            if (!background)
            {
                auto message = "No background specified";

                Logger::outputDebugString(String("[Skin] ") + message);
                background = createBogusImage(message, 200, 200);
            }
        }
        else
        {
            auto message = String("XML element \"") +
                           currentGroupName_ +
                           String("\" specifies no background image");

            Logger::outputDebugString(String("[Skin] ") + message);
            background = createBogusImage(message, 200, 200);
        }
    }
    else
    {
        auto message = "No skin groups found";

        Logger::outputDebugString(String("[Skin] ") + message);
        background = createBogusImage(message, 200, 200);
    }

    if (skinGroup_ != nullptr)
    {
        forEachXmlChildElementWithTagName(*skinGroup_,
                                          xmlMeterGraduation,
                                          "meter_graduation")
        {
            String imageFilename = getString(xmlMeterGraduation,
                                             currentBackgroundName_);

            auto drawableGraduation = loadImage(imageFilename);
            Point<int> position = getPositionInteger(
                                      xmlMeterGraduation,
                                      drawableGraduation->getHeight());

            background->addAndMakeVisible(drawableGraduation.get());
            drawableGraduation->setTopLeftPosition(position.getX(), position.getY());
        }
    }

    backgroundWidth_ = background->getWidth();
    backgroundHeight_ = background->getHeight();

    background->setBounds(0, 0, backgroundWidth_, backgroundHeight_);

    // moves background image to the back of the editor's z-plane so
    // that it doesn't overlay (and thus block) any other components
    editor->addAndMakeVisible(background.get(), 0);

    editor->setSize(backgroundWidth_, backgroundHeight_);
}


Point<float> Skin::getPositionFloat(
    const XmlElement *xmlComponent,
    const float componentHeight)
{
    jassert(componentHeight >= 0.0f);

    float x = getFloat(xmlComponent, "x", 0.0f);
    float y = getFloat(xmlComponent, "y", 0.0f);

    if (originOfYIsBottom_)
    {
        y = backgroundHeight_ - (y + componentHeight);
    }

    return Point<float>(x, y);
}


Point<int> Skin::getPositionInteger(
    const XmlElement *xmlComponent,
    const int componentHeight)
{
    auto position = getPositionFloat(xmlComponent, static_cast<float>(componentHeight));

    return Point<int>(math::SimpleMath::round(position.getX()),
                      math::SimpleMath::round(position.getY()));
}


Rectangle<float> Skin::getBoundsFloat(
    const XmlElement *xmlComponent,
    float width,
    float height)
{
    if (document_ == nullptr)
    {
        return Rectangle<float>(0.0f, 0.0f, 1.0f, 1.0f);
    }

    if (width < 0.0f)
    {
        width = getFloat(xmlComponent, "width", 0.0f);
    }

    if (height < 0.0f)
    {
        height = getFloat(xmlComponent, "height", 0.0f);
    }

    auto position = getPositionFloat(xmlComponent, height);
    auto extends = position.translated(width, height);

    return Rectangle<float>(position, extends);
}


Rectangle<int> Skin::getBoundsInteger(
    const XmlElement *xmlComponent,
    int width,
    int height)
{
    auto bounds = getBoundsFloat(xmlComponent,
                                 static_cast<float>(width),
                                 static_cast<float>(height));

    return Rectangle<int>(math::SimpleMath::round(bounds.getX()),
                          math::SimpleMath::round(bounds.getY()),
                          math::SimpleMath::round(bounds.getWidth()),
                          math::SimpleMath::round(bounds.getHeight()));
}


void Skin::placeComponent(
    const XmlElement *xmlComponent,
    Component *component)
{
    jassert(component != nullptr);

    if (document_ == nullptr)
    {
        return;
    }

    Rectangle<int> bounds = getBoundsInteger(xmlComponent);
    component->setBounds(bounds);
}


void Skin::placeMeterBar(
    const String &tagName,
    widgets::MeterBar *meterBar)
{
    jassert(meterBar != nullptr);

    if (document_ == nullptr)
    {
        return;
    }

    XmlElement *xmlComponent = getComponent(tagName);

    if (xmlComponent != nullptr)
    {
        int height = meterBar->getHeight();
        Point<int> position = getPositionInteger(xmlComponent, height);

        meterBar->setTopLeftPosition(position);

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
            meterBar->setOrientation(widgets::Orientation::vertical);
        }
        // horizontal bar
        else
        {
            meterBar->setOrientation(widgets::Orientation::horizontal);
        }
    }
}


void Skin::placeAndSkinButton(
    const String &tagName,
    DrawableButton *button)
{
    jassert(button != nullptr);

    if (document_ == nullptr)
    {
        return;
    }

    XmlElement *xmlComponent = getComponent(tagName);

    if (xmlComponent != nullptr)
    {
        Image imageOn;
        String strImageFilenameOn = getString(xmlComponent, "image_on");

        std::unique_ptr<Drawable> componentOn = loadImage(strImageFilenameOn);

        Image imageOff;
        String strImageFilenameOff = getString(xmlComponent, "image_off");

        std::unique_ptr<Drawable> componentOff = loadImage(strImageFilenameOff);

        String strImageFilenameOver = getString(xmlComponent, "image_over");

        // a missing "image_over" is handled gracefully by "setImages"
        std::unique_ptr<Drawable> componentOver = loadImage(strImageFilenameOver);

        button->setImages(componentOff.get(),
                          componentOver.get(),
                          componentOn.get(),
                          nullptr,
                          componentOn.get(),
                          componentOn.get(),
                          componentOff.get(),
                          nullptr);

        button->setColour(DrawableButton::backgroundOnColourId, Colours::transparentBlack);

        int width = componentOn->getWidth();
        int height = componentOn->getHeight();

        Point<int> position = getPositionInteger(xmlComponent, height);
        button->setTopLeftPosition(position);
        button->setSize(width, height);
    }
}


void Skin::placeAndSkinSlider(
    const String &tagName,
    widgets::FrutSlider *slider)
{
    if (document_ == nullptr)
    {
        return;
    }

    XmlElement *xmlComponent = getComponent(tagName);
    Colour sliderColour = getColour(xmlComponent);

    placeComponent(xmlComponent, slider);
    slider->setSliderColour(sliderColour);
}


void Skin::placeAndSkinSlider(
    const String &tagName,
    widgets::SliderCombined *slider)
{
    if (document_ == nullptr)
    {
        return;
    }

    XmlElement *xmlComponent = getComponent(tagName);
    Colour sliderColour = getColour(xmlComponent);
    Colour toggleSwitchColour = getColour(xmlComponent, sliderColour, "toggle_");

    placeComponent(xmlComponent, slider);
    slider->setSliderColour(sliderColour);
    slider->setToggleSwitchColour(toggleSwitchColour);
}


void Skin::placeAndSkinNeedleMeter(
    const String &tagName,
    widgets::NeedleMeter *meter)
{
    jassert(meter != nullptr);

    if (document_ == nullptr)
    {
        return;
    }

    XmlElement *xmlComponent = getComponent(tagName);

    if (xmlComponent != nullptr)
    {
        Image imageBackground;
        String strImageFilenameBackground = getString(xmlComponent, "image");

        loadImage(strImageFilenameBackground, imageBackground);

        Image imageNeedle;
        String strImageFilenameNeedle = getString(xmlComponent, "image_needle");

        loadImage(strImageFilenameNeedle, imageNeedle);

        int spacing_left = getInteger(xmlComponent, "spacing_left", 0);
        int spacing_top = getInteger(xmlComponent, "spacing_top", 0);

        meter->setImages(imageBackground,
                         imageNeedle,
                         spacing_left,
                         spacing_top);

        int width = imageBackground.getWidth();
        int height = imageBackground.getHeight();

        Rectangle<int> bounds = getBoundsInteger(xmlComponent, width, height);
        meter->setBounds(bounds);
    }
}


void Skin::placeAndSkinLabel(
    const String &tagName,
    std::unique_ptr<Drawable> &label,
    AudioProcessorEditor *editor)
{
    XmlElement *xmlComponent = getComponent(tagName);

    if (xmlComponent == nullptr)
    {
        return;
    }

    auto fileName = getString(xmlComponent, "image");
    label = loadImage(fileName);

    auto position = getPositionFloat(
                        xmlComponent,
                        static_cast<float>(label->getHeight()));

    label->setOriginWithOriginalSize(position);
    editor->addAndMakeVisible(label.get());
}


void Skin::placeAndSkinSignalLed(
    const String &tagName,
    widgets::SignalLed *label)
{
    jassert(label != nullptr);

    if (document_ == nullptr)
    {
        return;
    }

    XmlElement *xmlComponent = getComponent(tagName);

    if (xmlComponent != nullptr)
    {
        Image imageOff;
        String strImageFilenameOff = getString(xmlComponent, "image_off");

        loadImage(strImageFilenameOff, imageOff);

        Image imageLow;
        String strImageFilenameLow = getString(xmlComponent, "image_low");

        loadImage(strImageFilenameLow, imageLow);

        Image imageHigh;
        String strImageFilenameHigh = getString(xmlComponent, "image_high");

        loadImage(strImageFilenameHigh, imageHigh);

        label->setImages(imageOff, imageLow, imageHigh);

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

        Rectangle<int> bounds = getBoundsInteger(xmlComponent, width, height);
        label->setBounds(bounds);
    }
}


void Skin::placeAndSkinStateLabel(
    const String &tagName,
    widgets::StateLabel *label)
{
    jassert(label != nullptr);

    if (document_ == nullptr)
    {
        return;
    }

    XmlElement *xmlComponent = getComponent(tagName);

    if (xmlComponent != nullptr)
    {
        Image imageOff;
        String strImageFilenameOff = getString(xmlComponent, "image_off");

        loadImage(strImageFilenameOff, imageOff);

        Image imageOn;
        String strImageFilenameOn = getString(xmlComponent, "image_on");

        loadImage(strImageFilenameOn, imageOn);

        Image imageActive;
        String strImageFilenameActive = getString(xmlComponent, "image_active");

        // use "image_on" if "image_active" does not exist
        if (!strImageFilenameActive.isEmpty())
        {
            loadImage(strImageFilenameActive, imageActive);
        }
        else
        {
            imageActive = imageOn.createCopy();
        }

        int spacing_left = getInteger(xmlComponent, "spacing_left", 0);
        int spacing_top = getInteger(xmlComponent, "spacing_top", 0);
        int font_size = getInteger(xmlComponent, "font_size", 12);

        String strColourOff = getString(xmlComponent, "colour_off", "ffffff");
        String strColourOn = getString(xmlComponent, "colour_on", "ffffff");
        String strColourActive = getString(xmlComponent, "colour_active", "ffffff");

        label->setImages(imageOff,
                         imageOn,
                         imageActive,
                         strColourOff,
                         strColourOn,
                         strColourActive,
                         spacing_left,
                         spacing_top,
                         static_cast<float>(font_size));

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

        Rectangle<int> bounds = getBoundsInteger(xmlComponent, width, height);
        label->setBounds(bounds);
    }
}

}
}
