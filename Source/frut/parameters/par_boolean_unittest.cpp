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

#include "unittest.h"


class ParBoolean :
   public ::testing::Test
{
protected:
   void SetUp() override
   {
      p1 = std::make_unique<frut::parameters::ParBoolean>();
      p1->init( "state_true", "state_off" );
      p1->setName( "p1" );
      p1->setDefaultBoolean( false, true );
   }

   // void TearDown() override
   // {
   // }

   std::unique_ptr<frut::parameters::ParBoolean> p1;
};


TEST_F( ParBoolean, Basics )
{
   ASSERT_NE( p1, nullptr );

   // parameters are initialised to have their change flag set
   EXPECT_EQ( p1->hasChanged(), true );
   p1->clearChangeFlag();
   EXPECT_EQ( p1->hasChanged(), false );

   EXPECT_STREQ( p1->getName().getCharPointer(), "p1" );
   EXPECT_STREQ( p1->getTagName().getCharPointer(), "p1" );

   p1->setName( "RenaMing lower UPPER" );
   EXPECT_STREQ( p1->getName().getCharPointer(), "RenaMing lower UPPER" );
   EXPECT_STREQ( p1->getTagName().getCharPointer(), "renaming_lower_upper" );
}


TEST_F( ParBoolean, GetDefaultValues )
{
   EXPECT_FLOAT_EQ( p1->getDefaultFloat(), 0.0f );
   EXPECT_FLOAT_EQ( p1->getDefaultRealFloat(), 0.0f );
   EXPECT_EQ( p1->getDefaultRealInteger(), 0 );
   EXPECT_EQ( p1->getDefaultBoolean(), false );
}


TEST_F( ParBoolean, GetValues )
{
   p1->clearChangeFlag();

   EXPECT_FLOAT_EQ( p1->getFloat(), 0.0f );
   EXPECT_FLOAT_EQ( p1->getRealFloat(), 0.0f );
   EXPECT_FLOAT_EQ( p1->getRealInteger(), 0 );
   EXPECT_EQ( p1->getBoolean(), false );
   EXPECT_STREQ( p1->getText().getCharPointer(), "state_off" );

   EXPECT_EQ( p1->hasChanged(), false );
   p1->toggleState();
   EXPECT_EQ( p1->hasChanged(), true );

   EXPECT_FLOAT_EQ( p1->getFloat(), 1.0f );
   EXPECT_FLOAT_EQ( p1->getRealFloat(), 1.0f );
   EXPECT_FLOAT_EQ( p1->getRealInteger(), 1 );
   EXPECT_EQ( p1->getBoolean(), true );
   EXPECT_STREQ( p1->getText().getCharPointer(), "state_true" );

   EXPECT_EQ( p1->hasChanged(), true );
   p1->toggleState();
   EXPECT_EQ( p1->hasChanged(), true );

   EXPECT_FLOAT_EQ( p1->getFloat(), 0.0f );
   EXPECT_FLOAT_EQ( p1->getRealFloat(), 0.0f );
   EXPECT_FLOAT_EQ( p1->getRealInteger(), 0 );
   EXPECT_EQ( p1->getBoolean(), false );
   EXPECT_STREQ( p1->getText().getCharPointer(), "state_off" );

   p1->clearChangeFlag();
   EXPECT_EQ( p1->hasChanged(), false );
   p1->toggleState();
   EXPECT_EQ( p1->hasChanged(), true );
   p1->toggleState();
}


TEST_F( ParBoolean, SetDefaultValues1 )
{
   p1->clearChangeFlag();

   // set new default without updating parameter
   p1->setDefaultBoolean( true, false );

   EXPECT_FLOAT_EQ( p1->getDefaultFloat(), 1.0f );
   EXPECT_FLOAT_EQ( p1->getDefaultRealFloat(), 1.0f );
   EXPECT_EQ( p1->getDefaultRealInteger(), 1 );
   EXPECT_EQ( p1->getDefaultBoolean(), true );

   EXPECT_EQ( p1->hasChanged(), false );
   EXPECT_FLOAT_EQ( p1->getRealFloat(), 0.0f );
}


TEST_F( ParBoolean, SetDefaultValues2 )
{
   p1->clearChangeFlag();

   // set new default and update parameter
   p1->setDefaultBoolean( true, true );

   EXPECT_FLOAT_EQ( p1->getDefaultFloat(), 1.0f );
   EXPECT_FLOAT_EQ( p1->getDefaultRealFloat(), 1.0f );
   EXPECT_EQ( p1->getDefaultRealInteger(), 1 );
   EXPECT_EQ( p1->getDefaultBoolean(), true );

   EXPECT_EQ( p1->hasChanged(), true );
   EXPECT_FLOAT_EQ( p1->getRealFloat(), 1.0f );
}


TEST_F( ParBoolean, SetDefaultValues3 )
{
   p1->clearChangeFlag();

   // set new default without updating parameter
   p1->setDefaultRealFloat( 0.01f, false );

   EXPECT_FLOAT_EQ( p1->getDefaultFloat(), 1.0f );
   EXPECT_FLOAT_EQ( p1->getDefaultRealFloat(), 1.0f );
   EXPECT_EQ( p1->getDefaultRealInteger(), 1 );
   EXPECT_EQ( p1->getDefaultBoolean(), true );

   EXPECT_EQ( p1->hasChanged(), false );
   EXPECT_FLOAT_EQ( p1->getRealFloat(), 0.0f );
}


TEST_F( ParBoolean, SetDefaultValues4 )
{
   p1->clearChangeFlag();

   // set new default and update parameter
   p1->setDefaultRealFloat( 0.01f, true );

   EXPECT_FLOAT_EQ( p1->getDefaultFloat(), 1.0f );
   EXPECT_FLOAT_EQ( p1->getDefaultRealFloat(), 1.0f );
   EXPECT_EQ( p1->getDefaultRealInteger(), 1 );
   EXPECT_EQ( p1->getDefaultBoolean(), true );

   EXPECT_EQ( p1->hasChanged(), true );
   EXPECT_FLOAT_EQ( p1->getRealFloat(), 1.0f );
}


TEST_F( ParBoolean, SetValuesFloat )
{
   ASSERT_FLOAT_EQ( p1->getFloat(), 0.0f );

   p1->clearChangeFlag();
   p1->setFloat( 0.01f );
   EXPECT_FLOAT_EQ( p1->getFloat(), 1.0f );
   EXPECT_EQ( p1->hasChanged(), true );

   p1->clearChangeFlag();
   p1->setFloat( 10.0f );
   EXPECT_FLOAT_EQ( p1->getFloat(), 1.0f );
   EXPECT_EQ( p1->hasChanged(), false );

   p1->clearChangeFlag();
   p1->setFloat( 0.0f );
   EXPECT_FLOAT_EQ( p1->getFloat(), 0.0f );
   EXPECT_EQ( p1->hasChanged(), true );

   p1->clearChangeFlag();
   p1->setFloat( -0.01f );
   EXPECT_FLOAT_EQ( p1->getFloat(), 1.0f );
   EXPECT_EQ( p1->hasChanged(), true );
}


TEST_F( ParBoolean, SetValuesRealFloat )
{
   ASSERT_FLOAT_EQ( p1->getRealFloat(), 0.0f );

   p1->clearChangeFlag();
   p1->setRealFloat( 0.01f );
   EXPECT_FLOAT_EQ( p1->getRealFloat(), 1.0f );
   EXPECT_EQ( p1->hasChanged(), true );

   p1->clearChangeFlag();
   p1->setRealFloat( 10.0f );
   EXPECT_FLOAT_EQ( p1->getRealFloat(), 1.0f );
   EXPECT_EQ( p1->hasChanged(), false );

   p1->clearChangeFlag();
   p1->setRealFloat( 0.0f );
   EXPECT_FLOAT_EQ( p1->getRealFloat(), 0.0f );
   EXPECT_EQ( p1->hasChanged(), true );

   p1->clearChangeFlag();
   p1->setRealFloat( -0.01f );
   EXPECT_FLOAT_EQ( p1->getRealFloat(), 1.0f );
   EXPECT_EQ( p1->hasChanged(), true );
}


TEST_F( ParBoolean, SetValuesRealInteger )
{
   ASSERT_EQ( p1->getRealInteger(), 0 );

   p1->clearChangeFlag();
   p1->setRealInteger( 1 );
   EXPECT_EQ( p1->getRealInteger(), 1 );
   EXPECT_EQ( p1->hasChanged(), true );

   p1->clearChangeFlag();
   p1->setRealInteger( 10 );
   EXPECT_EQ( p1->getRealInteger(), 1 );
   EXPECT_EQ( p1->hasChanged(), false );

   p1->clearChangeFlag();
   p1->setRealInteger( 0 );
   EXPECT_EQ( p1->getRealInteger(), 0 );
   EXPECT_EQ( p1->hasChanged(), true );

   p1->clearChangeFlag();
   p1->setRealInteger( -1 );
   EXPECT_EQ( p1->getRealInteger(), 1 );
   EXPECT_EQ( p1->hasChanged(), true );
}


TEST_F( ParBoolean, SetValuesBoolean )
{
   ASSERT_EQ( p1->getBoolean(), false );

   p1->clearChangeFlag();
   p1->setBoolean( true );
   EXPECT_EQ( p1->getBoolean(), true );
   EXPECT_EQ( p1->hasChanged(), true );

   p1->clearChangeFlag();
   p1->setBoolean( true );
   EXPECT_EQ( p1->getBoolean(), true );
   EXPECT_EQ( p1->hasChanged(), false );

   p1->clearChangeFlag();
   p1->setBoolean( false );
   EXPECT_EQ( p1->getBoolean(), false );
   EXPECT_EQ( p1->hasChanged(), true );

   p1->clearChangeFlag();
   p1->setBoolean( false );
   EXPECT_EQ( p1->getBoolean(), false );
   EXPECT_EQ( p1->hasChanged(), false );
}


TEST_F( ParBoolean, SetValuesText )
{
   ASSERT_STREQ( p1->getText().getCharPointer(), "state_off" );

   p1->clearChangeFlag();
   p1->setText( "state_true" );
   EXPECT_STREQ( p1->getText().getCharPointer(), "state_true" );
   EXPECT_EQ( p1->hasChanged(), true );

   p1->clearChangeFlag();
   p1->setText( "state_on" );
   EXPECT_STREQ( p1->getText().getCharPointer(), "state_true" );
   EXPECT_EQ( p1->hasChanged(), false );

   p1->clearChangeFlag();
   p1->setText( "state_off" );
   EXPECT_STREQ( p1->getText().getCharPointer(), "state_off" );
   EXPECT_EQ( p1->hasChanged(), true );

   p1->clearChangeFlag();
   p1->setText( "state_off" );
   EXPECT_STREQ( p1->getText().getCharPointer(), "state_off" );
   EXPECT_EQ( p1->hasChanged(), false );

   p1->clearChangeFlag();
   p1->setText( "ojaSd~~ioah" );
   EXPECT_STREQ( p1->getText().getCharPointer(), "state_true" );
   EXPECT_EQ( p1->hasChanged(), true );
}


TEST_F( ParBoolean, XmlStoreAndRetrieve )
{
   // store state #1
   XmlElement xml1( "test" );
   p1->storeAsXml( &xml1 );

   XmlElement* xmlTest1 = xml1.getChildByName( "p1" );
   ASSERT_NE( xmlTest1, nullptr );
   EXPECT_EQ( xmlTest1->getDoubleAttribute( "value" ), 0.0 );

   p1->setName( "RenaMing lower UPPER" );
   p1->setBoolean( true );

   // store state #2
   XmlElement xml2( "test" );
   p1->storeAsXml( &xml2 );

   XmlElement* xmlTest2 = xml2.getChildByName( "renaming_lower_upper" );
   ASSERT_NE( xmlTest2, nullptr );
   EXPECT_EQ( xmlTest2->getDoubleAttribute( "value" ), 1.0 );

   // load state #1
   p1->setName( "p1" );
   EXPECT_FLOAT_EQ( p1->getRealFloat(), 1.0f );
   p1->loadFromXml( &xml1 );
   EXPECT_FLOAT_EQ( p1->getRealFloat(), 0.0f );

   // load state #2
   p1->setName( "renaming_lower_upper" );
   EXPECT_FLOAT_EQ( p1->getRealFloat(), 0.0f );
   p1->loadFromXml( &xml2 );
   EXPECT_FLOAT_EQ( p1->getRealFloat(), 1.0f );
}
