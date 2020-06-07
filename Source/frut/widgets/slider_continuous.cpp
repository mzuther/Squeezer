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
namespace widgets
{

SliderContinuous::SliderContinuous(
   parameters::Juggler& parameters,
   int parameterIndex )

{
   parameter_ = dynamic_cast<parameters::ParContinuous*>(
                   parameters.getPluginParameter( parameterIndex ) );

   jassert( parameter_ != nullptr );

   setRange( 0.0f, 1.0f, parameter_->getStepSize() );
   setVelocityModeParameters( 1.0, 1, 0.0, true );
   setSliderStyle( Slider::RotaryHorizontalVerticalDrag );
   colourRotary_ = Colours::white;

   setDoubleClickReturnValue( true, parameter_->getDefaultFloat() );
}


void SliderContinuous::visibilityChanged()
{
   Slider::visibilityChanged();

   setColour( Slider::rotarySliderFillColourId, colourRotary_ );
   setColour( Slider::textBoxTextColourId, Colours::white );
   setColour( Slider::textBoxBackgroundColourId, Colours::darkgrey.darker( 0.7f ) );
   setColour( Slider::textBoxOutlineColourId, Colours::darkgrey.darker( 0.4f ) );
}


void SliderContinuous::resized()
{
   Slider::resized();

   int nWidth = getBounds().getWidth();
   setTextBoxStyle( Slider::TextBoxBelow, true, nWidth, 18 );
}


void SliderContinuous::setSliderColour(
   const Colour& colour )

{
   colourRotary_ = colour;
   setColour( Slider::thumbColourId, colourRotary_ );
   setColour( Slider::rotarySliderFillColourId, colourRotary_ );
}


float SliderContinuous::getRealFloat()
{
   return parameter_->getRealFloat();
}


bool SliderContinuous::getBoolean()
{
   return parameter_->getBoolean();
}


int SliderContinuous::getRealInteger()
{
   return parameter_->getRealInteger();
}


double SliderContinuous::getValueFromText(
   const String& strText )

{
   return parameter_->getFloatFromText( strText );
}


String SliderContinuous::getTextFromValue(
   double dValue )

{
   return parameter_->getTextFromFloat( static_cast<float>( dValue ) );
}

}
}
