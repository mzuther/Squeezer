// ----------------------------------------------------------------------------
//
//  WARNING: this file was auto-generated, please do not edit!
//
// ----------------------------------------------------------------------------

#pragma once

#include "FrutHeader.h"

namespace frut::widgets::resources
{

extern const char*  button_gpl_off_svg;
const String        button_gpl_off_svg_name( "button_gpl_off.svg" );
const int           button_gpl_off_svg_len = 36213;

extern const char*  button_gpl_on_svg;
const String        button_gpl_on_svg_name( "button_gpl_on.svg" );
const int           button_gpl_on_svg_len = 37393;


bool resourceExists( const String& resourceName );
const char* getResource( const String& resourceName, int& numberOfBytes );

std::unique_ptr<Drawable> getDrawable( const String& resourceName );
String getStringUTF8( const String& resourceName );
}
