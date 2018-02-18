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

namespace frut
{
namespace skin
{

// adapted from juce::LookAndFeel_V2
void LookAndFeel_Frut_V3::drawRotarySlider(
    Graphics &g, int x, int y, int width, int height, float sliderPos,
    const float rotaryStartAngle, const float rotaryEndAngle, Slider &slider)
{
    const float radius = jmin(width / 2, height / 2) - 2.0f;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();

    if (radius > 12.0f)
    {
        if (slider.isEnabled())
        {
            g.setColour(slider.findColour(Slider::rotarySliderFillColourId).withAlpha(isMouseOver ? 1.0f : 0.7f));
        }
        else
        {
            g.setColour(Colour(0x80808080));
        }

        const float thickness = 0.7f;

        {
            Path filledArc;
            filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, angle, thickness);
            g.fillPath(filledArc);
        }

        {
            const float innerRadius = radius * 0.2f;
            Path p;
            p.addTriangle(-innerRadius, 0.0f,
                          0.0f, -radius * thickness * 1.1f,
                          innerRadius, 0.0f);

            p.addEllipse(-innerRadius, -innerRadius, innerRadius * 2.0f, innerRadius * 2.0f);

            g.fillPath(p, AffineTransform::rotation(angle).translated(centreX, centreY));
        }

        if (slider.isEnabled())
        {
            g.setColour(slider.findColour(Slider::rotarySliderOutlineColourId));
        }
        else
        {
            g.setColour(Colour(0x80808080));
        }

        Path outlineArc;
        outlineArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, thickness);
        outlineArc.closeSubPath();

        g.strokePath(outlineArc, PathStrokeType(slider.isEnabled() ? (isMouseOver ? 2.0f : 1.2f) : 0.3f));
    }
    else
    {
        if (slider.isEnabled())
        {
            g.setColour(slider.findColour(Slider::rotarySliderFillColourId).withAlpha(isMouseOver ? 1.0f : 0.7f));
        }
        else
        {
            g.setColour(Colour(0x80808080));
        }

        Path p;
        p.addEllipse(-0.4f * rw, -0.4f * rw, rw * 0.8f, rw * 0.8f);
        PathStrokeType(rw * 0.1f).createStrokedPath(p, p);

        p.addLineSegment(Line<float> (0.0f, 0.0f, 0.0f, -radius), rw * 0.2f);

        g.fillPath(p, AffineTransform::rotation(angle).translated(centreX, centreY));
    }
}

}
}
