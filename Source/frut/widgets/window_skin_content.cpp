/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2021 Martin Zuther (http://www.mzuther.de/)

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

namespace frut::widgets
{

/// Create content component for dialog window that allows selecting a
/// new GUI skin.
///
/// ### Dialog window exit values
///
/// | %Value | %Result                           |
/// | :----: | --------------------------------- |
/// | 0      | window has been closed "by force" |
/// | 1      | user has selected a skin          |
///
WindowSkinContent::WindowSkinContent()
{
}


/// Static helper function to create a dialog window for selecting a
/// new GUI skin.
///
/// @param pluginEditor audio plug-in editor
///
/// @param skin currently used skin
///
/// @return created dialog window
///
DialogWindow* WindowSkinContent::createDialogWindow(
   AudioProcessorEditor* pluginEditor,
   int scale )

{
   DialogWindow::LaunchOptions windowSkinLauncher;

   WindowSkinContent* contentComponent =
      new WindowSkinContent();
   contentComponent->initialise( scale );

   // initialise dialog window settings
   windowSkinLauncher.dialogTitle = "Select UI scale";
   windowSkinLauncher.dialogBackgroundColour = Colours::black.brighter( 0.2f );
   windowSkinLauncher.content.setOwned( contentComponent );
   windowSkinLauncher.componentToCentreAround = pluginEditor;

   windowSkinLauncher.escapeKeyTriggersCloseButton = true;
   windowSkinLauncher.useNativeTitleBar = false;
   windowSkinLauncher.resizable = false;
   windowSkinLauncher.useBottomRightCornerResizer = false;

   // launch dialog window
   auto windowSkin = windowSkinLauncher.launchAsync();
   windowSkin->setAlwaysOnTop( true );

   return windowSkin;
}


/// Initialise dialog window components.
///
/// @param skin currently used skin
///
void WindowSkinContent::initialise(
   int scale )

{
   button100_.setRadioGroupId( 1 );
   button100_.setButtonText( "100%" );
   addAndMakeVisible( button100_ );
   button100_.addListener( this );

   button125_.setRadioGroupId( 1 );
   button125_.setButtonText( "125%" );
   addAndMakeVisible( button125_ );
   button125_.addListener( this );

   button150_.setRadioGroupId( 1 );
   button150_.setButtonText( "150%" );
   addAndMakeVisible( button150_ );
   button150_.addListener( this );

   button200_.setRadioGroupId( 1 );
   button200_.setButtonText( "200%" );
   addAndMakeVisible( button200_ );
   button200_.addListener( this );

   button250_.setRadioGroupId( 1 );
   button250_.setButtonText( "250%" );
   addAndMakeVisible( button250_ );
   button250_.addListener( this );

   button300_.setRadioGroupId( 1 );
   button300_.setButtonText( "300%" );
   addAndMakeVisible( button300_ );
   button300_.addListener( this );

   if ( scale == 125 ) {
      button125_.setToggleState( true, dontSendNotification );
   } else if ( scale == 150 ) {
      button150_.setToggleState( true, dontSendNotification );
   } else if ( scale == 200 ) {
      button200_.setToggleState( true, dontSendNotification );
   } else if ( scale == 250 ) {
      button250_.setToggleState( true, dontSendNotification );
   } else if ( scale == 300 ) {
      button300_.setToggleState( true, dontSendNotification );
   } else {
      button100_.setToggleState( true, dontSendNotification );
   }

   // style and place the dialog window's components
   applySkin();
}


/// Style and place the dialog window's components.
///
void WindowSkinContent::applySkin()
{
   button100_.setColour(
      TextButton::buttonOnColourId,
      Colours::yellow );

   button100_.setColour(
      TextButton::buttonColourId,
      Colours::darkgrey );

   button100_.setColour(
      TextButton::textColourOnId,
      Colours::black );

   button125_.setColour(
      TextButton::buttonOnColourId,
      Colours::yellow );

   button125_.setColour(
      TextButton::buttonColourId,
      Colours::darkgrey );

   button125_.setColour(
      TextButton::textColourOnId,
      Colours::black );

   button150_.setColour(
      TextButton::buttonOnColourId,
      Colours::yellow );

   button150_.setColour(
      TextButton::buttonColourId,
      Colours::darkgrey );

   button150_.setColour(
      TextButton::textColourOnId,
      Colours::black );

   button200_.setColour(
      TextButton::buttonOnColourId,
      Colours::yellow );

   button200_.setColour(
      TextButton::buttonColourId,
      Colours::darkgrey );

   button200_.setColour(
      TextButton::textColourOnId,
      Colours::black );

   button250_.setColour(
      TextButton::buttonOnColourId,
      Colours::yellow );

   button250_.setColour(
      TextButton::buttonColourId,
      Colours::darkgrey );

   button250_.setColour(
      TextButton::textColourOnId,
      Colours::black );

   button300_.setColour(
      TextButton::buttonOnColourId,
      Colours::yellow );

   button300_.setColour(
      TextButton::buttonColourId,
      Colours::darkgrey );

   button300_.setColour(
      TextButton::textColourOnId,
      Colours::black );

   setSize( 250, 110 );

   grid_.templateRows = {
      Grid::TrackInfo ( Grid::Fr ( 1 ) ),
      Grid::TrackInfo ( Grid::Fr ( 1 ) ),
   };

   grid_.templateColumns = {
      Grid::TrackInfo ( Grid::Fr ( 1 ) ),
      Grid::TrackInfo ( Grid::Fr ( 1 ) ),
      Grid::TrackInfo ( Grid::Fr ( 1 ) )
   };

   grid_.items = {
      juce::GridItem ( button100_ ),
      juce::GridItem ( button125_ ),
      juce::GridItem ( button150_ ),
      juce::GridItem ( button200_ ),
      juce::GridItem ( button250_ ),
      juce::GridItem ( button300_ )
   };

   grid_.autoFlow = Grid::AutoFlow::row;
   grid_.rowGap = Grid::Px ( 10 );
   grid_.columnGap = Grid::Px ( 10 );

   grid_.performLayout ( getLocalBounds().reduced( 10 ) );
}


/// Called when a button is clicked.
///
/// @param button clicked button
///
void WindowSkinContent::buttonClicked(
   Button* button )

{
   int scale = 100;

   if ( button == &button125_ ) {
      scale = 125;
   } else if ( button == &button150_ ) {
      scale = 150;
   } else if ( button == &button200_ ) {
      scale = 200;
   } else if ( button == &button250_ ) {
      scale = 250;
   } else if ( button == &button300_ ) {
      scale = 300;
   }

   auto dialogWindow = findParentComponentOfClass<DialogWindow>();

   if ( dialogWindow != nullptr ) {
      // close dialog window (exit value is positive and corresponds
      // to new UI scaling)
      dialogWindow->exitModalState( scale );
   }
}


} // namespace
