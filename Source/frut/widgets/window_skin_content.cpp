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
WindowSkinContent::WindowSkinContent() :
    skin_(nullptr)
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
   frut::skin::Skin* skin )

{
   DialogWindow::LaunchOptions windowSkinLauncher;

   WindowSkinContent* contentComponent =
      new WindowSkinContent();

   contentComponent->initialise( skin );

   // initialise dialog window settings
   windowSkinLauncher.dialogTitle = "Select skin";
   windowSkinLauncher.dialogBackgroundColour = Colours::white;
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
   frut::skin::Skin* skin )

{
   skin_ = skin;
   listModel_.fill( skin );

   skinList_.setModel( &listModel_ );
   addAndMakeVisible( skinList_ );

   skinList_.setMultipleSelectionEnabled( false );

   // select current skin in list box
   int row = listModel_.getRow( skin->getDefaultSkin() );
   skinList_.selectRow( row );

   buttonSelect_.setButtonText( "Select" );
   addAndMakeVisible( buttonSelect_ );
   buttonSelect_.addListener( this );

   // style and place the dialog window's components
   applySkin();
}


/// Style and place the dialog window's components.
///
void WindowSkinContent::applySkin()
{
   skinList_.setOutlineThickness( 1 );

   skinList_.setColour(
      ListBox::backgroundColourId,
      Colours::white );

   skinList_.setColour(
      ListBox::outlineColourId,
      Colours::grey );


   buttonSelect_.setColour(
      TextButton::buttonColourId,
      Colours::yellow );

   buttonSelect_.setColour(
      TextButton::textColourOffId,
      Colours::black );


   int width = 150;
   int height = 50;

   int listBoxHeight = listModel_.getNumRows() * skinList_.getRowHeight() + 2;
   height += listBoxHeight;

   setSize( width, height );

   skinList_.setBounds( 10, 10, width - 20, listBoxHeight );
   buttonSelect_.setBounds( ( width / 2 ) - 30, height - 30, 60, 20 );
}


/// Called when a button is clicked.
///
/// @param button clicked button
///
void WindowSkinContent::buttonClicked(
   Button* button )

{
   if ( button == &buttonSelect_ ) {
      auto selectedRow = skinList_.getSelectedRow( 0 );
      auto newSkinName = listModel_.getSkinName( selectedRow );

      auto dialogWindow = findParentComponentOfClass<DialogWindow>();

      if ( dialogWindow != nullptr ) {
         // store selected skin
         skin_->setDefaultSkin( newSkinName );

         // close dialog window (exit code 1)
         dialogWindow->exitModalState( 1 );
      }
   }
}


/// Create a list box model for a WindowSkinContent.
///
SkinListBoxModel::SkinListBoxModel() :
   skin_( nullptr ),
   skinWildcard_( "*.skin", String(), "Skin files" ),
   directoryThread_( "Skin directory scanner" )
{
}


/// Fill list box model with all skins in a given directory.
///
/// @param skin currently used skin
///
void SkinListBoxModel::fill(
   frut::skin::Skin* skin )

{
   skin_ = skin;

   DirectoryContentsList skinFiles( &skinWildcard_, directoryThread_ );
   skinFiles.setDirectory( skin_->getSkinDirectory(), false, true );
   directoryThread_.startThread();

   // wait for directory scan
   while ( skinFiles.isStillLoading() ) {
      // keep Visual C++ from optimising (and thus breaking) the loop
      Thread::sleep( 20 );
   }

   for ( auto n = 0; n < skinFiles.getNumFiles(); ++n ) {
      auto skinFile = skinFiles.getFile( n );

      // strip file extension
      auto skinName = skinFile.getFileNameWithoutExtension();
      skinNames_.add( skinName );
   }
}


/// Get the number of skin names in the list box.
///
/// @return number of skin names
///
int SkinListBoxModel::getNumRows()
{
   return skinNames_.size();
}


/// Get the index of the first row that matches the given skin name.
///
/// @param skinNameToLookFor skin name to look for
///
/// @return index of the first matching element
///
int SkinListBoxModel::getRow(
   const String& skinNameToLookFor )

{
   return skinNames_.indexOf( skinNameToLookFor );
}


/// Return one of the skin names in the list box.
///
/// @param rowNumber zero-based row number to retrieve
///
/// @return skin name
///
const String SkinListBoxModel::getSkinName(
   int rowNumber )

{
   // valid index
   if ( rowNumber < skinNames_.size() ) {
      return skinNames_[rowNumber];
      // invalid index
   } else {
      return String();
   }
}


/// Draws a row of the list.
///
/// @param rowNumber index of row to draw
///
/// @param g graphics context
///
/// @param rowWidth row width
///
/// @param rowHeight row height
///
/// @param isRowSelected indicates whether row is currently selected
///
void SkinListBoxModel::paintListBoxItem(
   int rowNumber,
   Graphics& g,
   int rowWidth,
   int rowHeight,
   bool isRowSelected )

{
   if ( isRowSelected ) {
      g.fillAll( Colours::lightblue );
   }

   auto skinName = getSkinName( rowNumber );

   // draw default skin name in bold font
   if ( skinName == skin_->getDefaultSkin() ) {
      g.setFont( Font( 14.0f, Font::bold ) );
   } else {
      g.setFont( Font( 14.0f, Font::plain ) );
   }

   g.setColour( Colours::black );
   g.drawText( skinName,
               2, 0, rowWidth - 4, rowHeight,
               Justification::centredLeft,
               true );
}

}
}
