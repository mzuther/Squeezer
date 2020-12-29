/* ----------------------------------------------------------------------------

   Squeezer
   ========
   Flexible general-purpose audio compressor with a touch of citrus

   Copyright (c) 2013-2020 Martin Zuther (http://www.mzuther.de/)

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

#include "plugin_processor.h"
#include "plugin_editor.h"


/*==============================================================================

Flow of parameter processing:

  Editor:      buttonClicked(button) / sliderValueChanged(slider)
  Processor:   changeParameter(nIndex, fValue)
  Processor:   setParameter(nIndex, fValue)
  Parameters:  setFloat(nIndex, fValue)
  Editor:      actionListenerCallback(strMessage)
  Editor:      updateParameter(nIndex)

==============================================================================*/

SqueezerAudioProcessor::SqueezerAudioProcessor()

#ifndef JucePlugin_PreferredChannelConfigurations

   : AudioProcessor( getBusesProperties() )

#endif // JucePlugin_PreferredChannelConfigurations

{
   frut::Frut::printVersionNumbers();

#ifdef SQUEEZER_MONO
   numberOfChannels_ = 1;
#else // SQUEEZER_MONO
   numberOfChannels_ = 2;
#endif // SQUEEZER_MONO

   sampleRateIsValid_ = false;
   hasSideChain_ = false;
   reloadEditor_ = false;

   setLatencySamples( 0 );
}


SqueezerAudioProcessor::~SqueezerAudioProcessor()
{
   removeAllActionListeners();
}


#ifndef JucePlugin_PreferredChannelConfigurations

AudioProcessor::BusesProperties SqueezerAudioProcessor::getBusesProperties()
{
#ifdef SQUEEZER_MONO

#if SQUEEZER_EXTERNAL_SIDECHAIN == 1

#if JucePlugin_Build_VST

   return BusesProperties()
          .withInput( "Main + S-C",
                      AudioChannelSet::discreteChannels( 2 ) )
          .withOutput( "Main",
                       AudioChannelSet::mono() )
          .withInput( "(Disabled)",
                      AudioChannelSet::disabled() );

#else // JucePlugin_Build_VST

   return BusesProperties()
          .withInput( "Main",
                      AudioChannelSet::mono() )
          .withOutput( "Main",
                       AudioChannelSet::mono() )
          .withInput( "Side-Chain",
                      AudioChannelSet::mono() );

#endif // JucePlugin_Build_VST

#else // SQUEEZER_EXTERNAL_SIDECHAIN == 1

   return BusesProperties()
          .withInput( "Main",
                      AudioChannelSet::mono() )
          .withOutput( "Main",
                       AudioChannelSet::mono() )
          .withInput( "(Disabled)",
                      AudioChannelSet::disabled() );

#endif // SQUEEZER_EXTERNAL_SIDECHAIN == 1

#else // SQUEEZER_MONO

#if SQUEEZER_EXTERNAL_SIDECHAIN == 1

#if JucePlugin_Build_VST

   return BusesProperties()
          .withInput( "Main + S-C",
                      AudioChannelSet::discreteChannels( 4 ) )
          .withOutput( "Main",
                       AudioChannelSet::stereo() )
          .withInput( "(Disabled)",
                      AudioChannelSet::disabled() );

#else // JucePlugin_Build_VST

   return BusesProperties()
          .withInput( "Main",
                      AudioChannelSet::stereo() )
          .withOutput( "Main",
                       AudioChannelSet::stereo() )
          .withInput( "Side-Chain",
                      AudioChannelSet::stereo() );

#endif // JucePlugin_Build_VST

#else // SQUEEZER_EXTERNAL_SIDECHAIN == 1

   return BusesProperties()
          .withInput( "Main",
                      AudioChannelSet::stereo() )
          .withOutput( "Main",
                       AudioChannelSet::stereo() )
          .withInput( "(Disabled)",
                      AudioChannelSet::disabled() );

#endif // SQUEEZER_EXTERNAL_SIDECHAIN == 1

#endif // SQUEEZER_MONO
}

#endif // JucePlugin_PreferredChannelConfigurations


#ifndef JucePlugin_PreferredChannelConfigurations

bool SqueezerAudioProcessor::isBusesLayoutSupported( const BusesLayout& layouts ) const
{
#ifdef SQUEEZER_MONO

   // main output must be mono
   if ( layouts.getMainOutputChannelSet() != AudioChannelSet::mono() ) {
      return false;
   }

#else // SQUEEZER_MONO

#ifdef SQUEEZER_STEREO

   // main output must be stereo
   if ( layouts.getMainOutputChannelSet() != AudioChannelSet::stereo() ) {
      return false;
   }

#else // SQUEEZER_STEREO

   // main output can be either mono or stereo
   if ( ( layouts.getMainOutputChannelSet() != AudioChannelSet::mono() ) &&
        ( layouts.getMainOutputChannelSet() != AudioChannelSet::stereo() ) ) {
      return false;
   }

#endif // SQUEEZER_STEREO

#endif // SQUEEZER_MONO

   // main input must be enabled
   if ( layouts.getMainInputChannelSet().isDisabled() ) {
      return false;
   }

   // optional side chain input must correspond to main input
   if ( ( layouts.getChannelSet( true, 1 ) != AudioChannelSet::disabled() ) &&
        ( layouts.getChannelSet( true, 1 ) != layouts.getMainInputChannelSet() ) ) {
      return false;
   }

#if JucePlugin_Build_VST && SQUEEZER_EXTERNAL_SIDECHAIN == 1

   // VST2 with side chain: add additional channels to main input
   if ( layouts.getMainInputChannelSet().size() != 2 * layouts.getMainOutputChannelSet().size() ) {
      return false;
   }

#else // JucePlugin_Build_VST && SQUEEZER_EXTERNAL_SIDECHAIN == 1

   // main input and main output must be identical
   if ( layouts.getMainInputChannelSet() != layouts.getMainOutputChannelSet() ) {
      return false;
   }

#endif // JucePlugin_Build_VST && SQUEEZER_EXTERNAL_SIDECHAIN == 1

   // valid channel layout
   return true;
}

#endif // JucePlugin_PreferredChannelConfigurations


#ifndef JucePlugin_PreferredChannelConfigurations

void SqueezerAudioProcessor::processorLayoutsChanged()
{
   reloadEditor_ = true;
   numberOfChannels_ = getMainBusNumOutputChannels();
}

#endif // JucePlugin_PreferredChannelConfigurations


const String SqueezerAudioProcessor::getName() const
{
   return JucePlugin_Name;
}


int SqueezerAudioProcessor::getNumParameters()
{
   return pluginParameters_.getNumParameters( false );
}


const String SqueezerAudioProcessor::getParameterName( int nIndex )
{
   return pluginParameters_.getName( nIndex );
}


const String SqueezerAudioProcessor::getParameterText( int nIndex )
{
   return pluginParameters_.getText( nIndex );
}


String SqueezerAudioProcessor::getParameters()
{
   return pluginParameters_.toString();
}


float SqueezerAudioProcessor::getParameter( int nIndex )
{
   // This method will be called by the host, probably on the audio
   // thread, so it's absolutely time-critical. Don't use critical
   // sections or anything GUI-related, or anything at all that may
   // block in any way!

   return pluginParameters_.getFloat( nIndex );
}


void SqueezerAudioProcessor::changeParameter( int nIndex,
                                              float fValue )
{
   // This method will be called by the host, probably on the audio
   // thread, so it's absolutely time-critical. Don't use critical
   // sections or anything GUI-related, or anything at all that may
   // block in any way!

   // notify host of parameter change (this will automatically call
   // "setParameter"!)
   beginParameterChangeGesture( nIndex );
   setParameterNotifyingHost( nIndex, fValue );
   endParameterChangeGesture( nIndex );
}


void SqueezerAudioProcessor::setParameter(
   int nIndex,
   float fValue )
{
   // This method will be called by the host, probably on the audio
   // thread, so it's absolutely time-critical. Don't use critical
   // sections or anything GUI-related, or anything at all that may
   // block in any way!

   // Please only call this method directly for non-automatable
   // values!

   // update signal processing unit
   switch ( nIndex ) {
      case SqueezerPluginParameters::selBypass:

         pluginParameters_.setFloat( nIndex, fValue );

         if ( compressor_ ) {
            bool bBypassCompressor = pluginParameters_.getBoolean( nIndex );
            compressor_->setBypass( bBypassCompressor );
         }

         break;

      case SqueezerPluginParameters::selRmsWindowSize:

         pluginParameters_.setFloat( nIndex, fValue );

         if ( compressor_ ) {
            float fRmsWindowSizeMilliSeconds = pluginParameters_.getRealFloat( nIndex );
            compressor_->setRmsWindowSize( fRmsWindowSizeMilliSeconds );
         }

         break;

      case SqueezerPluginParameters::selDesign:

         pluginParameters_.setFloat( nIndex, fValue );

         if ( compressor_ ) {
            int nDesign = pluginParameters_.getRealInteger( nIndex );
            compressor_->setDesign( nDesign );
         }

         break;

      case SqueezerPluginParameters::selCurveType:

         pluginParameters_.setFloat( nIndex, fValue );

         if ( compressor_ ) {
            int nCurveType = pluginParameters_.getRealInteger( nIndex );
            compressor_->setCurve( nCurveType );
         }

         break;

      case SqueezerPluginParameters::selGainStage:

         pluginParameters_.setFloat( nIndex, fValue );

         if ( compressor_ ) {
            int nGainStage = pluginParameters_.getRealInteger( nIndex );
            compressor_->setGainStage( nGainStage );
         }

         break;

      case SqueezerPluginParameters::selThreshold:

         pluginParameters_.setFloat( nIndex, fValue );

         if ( compressor_ ) {
            float fThreshold = pluginParameters_.getRealFloat( nIndex );
            compressor_->setThreshold( fThreshold );
         }

         break;

      case SqueezerPluginParameters::selRatio:

         pluginParameters_.setFloat( nIndex, fValue );

         if ( compressor_ ) {
            float fRatio = pluginParameters_.getRealFloat( nIndex );
            compressor_->setRatio( fRatio );
         }

         break;

      case SqueezerPluginParameters::selKneeWidth:

         pluginParameters_.setFloat( nIndex, fValue );

         if ( compressor_ ) {
            float fKneeWidth = pluginParameters_.getRealFloat( nIndex );
            compressor_->setKneeWidth( fKneeWidth );
         }

         break;

      case SqueezerPluginParameters::selAttackRate:

         pluginParameters_.setFloat( nIndex, fValue );

         if ( compressor_ ) {
            float fAttackRate = pluginParameters_.getRealFloat( nIndex );
            compressor_->setAttackRate( fAttackRate );
         }

         break;

      case SqueezerPluginParameters::selReleaseRate:

         pluginParameters_.setFloat( nIndex, fValue );

         if ( compressor_ ) {
            int nReleaseRate = pluginParameters_.getRealInteger( nIndex );
            compressor_->setReleaseRate( nReleaseRate );
         }

         break;

      case SqueezerPluginParameters::selInputTrim:

         pluginParameters_.setFloat( nIndex, fValue );

         if ( compressor_ ) {
            float fInputTrim = pluginParameters_.getRealFloat( nIndex );
            compressor_->setInputTrim( fInputTrim );
         }

         break;

      case SqueezerPluginParameters::selAutoMakeupGain:

         pluginParameters_.setFloat( nIndex, fValue );

         if ( compressor_ ) {
            bool bAutoMakeupGain = pluginParameters_.getBoolean( nIndex );
            compressor_->setAutoMakeupGain( bAutoMakeupGain );
         }

         break;

      case SqueezerPluginParameters::selMakeupGain:

         pluginParameters_.setFloat( nIndex, fValue );

         if ( compressor_ ) {
            float fMakeupGain = pluginParameters_.getRealFloat( nIndex );
            compressor_->setMakeupGain( fMakeupGain );
         }

         break;

      case SqueezerPluginParameters::selStereoLink:

         pluginParameters_.setFloat( nIndex, fValue );

         if ( compressor_ ) {
            int nStereoLink = pluginParameters_.getRealInteger( nIndex );
            compressor_->setStereoLink( nStereoLink );
         }

         break;

      case SqueezerPluginParameters::selWetMix:

         pluginParameters_.setFloat( nIndex, fValue );

         if ( compressor_ ) {
            int nWetMix = pluginParameters_.getRealInteger( nIndex );
            compressor_->setWetMix( nWetMix );
         }

         break;

      case SqueezerPluginParameters::selSidechainInput:

         pluginParameters_.setFloat( nIndex, fValue );

         if ( compressor_ ) {
            bool bSidechainInput = pluginParameters_.getBoolean( nIndex );
            compressor_->setSidechainInput( bSidechainInput );
         }

         break;

      case SqueezerPluginParameters::selSidechainHPFCutoff:

         pluginParameters_.setFloat( nIndex, fValue );

         if ( compressor_ ) {
            int nSidechainHPFCutoff = pluginParameters_.getRealInteger( nIndex );
            compressor_->setSidechainHPFCutoff( nSidechainHPFCutoff );
         }

         break;

      case SqueezerPluginParameters::selSidechainLPFCutoff:

         pluginParameters_.setFloat( nIndex, fValue );

         if ( compressor_ ) {
            int nSidechainLPFCutoff = pluginParameters_.getRealInteger( nIndex );
            compressor_->setSidechainLPFCutoff( nSidechainLPFCutoff );
         }

         break;

      case SqueezerPluginParameters::selSidechainListen:

         pluginParameters_.setFloat( nIndex, fValue );

         if ( compressor_ ) {
            bool bSidechainListen = pluginParameters_.getBoolean( nIndex );
            compressor_->setSidechainListen( bSidechainListen );
         }

         break;

      default: {
         frut::parameters::ParCombined* pCombined = dynamic_cast<frut::parameters::ParCombined*>(
                                                       pluginParameters_.getPluginParameter( nIndex + 1 ) );

         if ( pCombined ) {
            switch ( nIndex ) {
               case SqueezerPluginParameters::selThresholdSwitch:
               case SqueezerPluginParameters::selRatioSwitch:

               case SqueezerPluginParameters::selAttackRateSwitch:
               case SqueezerPluginParameters::selReleaseRateSwitch:

               case SqueezerPluginParameters::selInputTrimSwitch:
               case SqueezerPluginParameters::selMakeupGainSwitch:
               case SqueezerPluginParameters::selStereoLinkSwitch:
               case SqueezerPluginParameters::selWetMixSwitch:

               case SqueezerPluginParameters::selSidechainHPFCutoffSwitch:
               case SqueezerPluginParameters::selSidechainLPFCutoffSwitch:

                  pCombined->setMode( fValue != 0.0f );
                  break;

               default:
                  DBG( "[Squeezer] processor::setParameter ==> invalid combined index" );
                  break;
            }
         } else {
            DBG( "[Squeezer] processor::setParameter ==> invalid index" );
         }
      }
      break;
   }

   // notify plug-in editor of parameter change
   if ( pluginParameters_.hasChanged( nIndex ) ) {
      // for visible parameters, notify the editor of changes (this
      // will also clear the change flag)
      if ( nIndex < pluginParameters_.getNumParameters( false ) ) {
         // "PC" ==> parameter changed, followed by a hash and the
         // parameter's ID
         sendActionMessage( "PC#" + String( nIndex ) );
         // for hidden parameters, we only have to clear the change
         // flag
      } else {
         pluginParameters_.clearChangeFlag( nIndex );
      }
   }
}


void SqueezerAudioProcessor::clearChangeFlag(
   int nIndex )
{
   pluginParameters_.clearChangeFlag( nIndex );
}


bool SqueezerAudioProcessor::hasChanged(
   int nIndex )
{
   return pluginParameters_.hasChanged( nIndex );
}


void SqueezerAudioProcessor::updateParameters(
   bool bIncludeHiddenParameters )
{
   int nNumParameters = pluginParameters_.getNumParameters( false );

   for ( int nIndex = 0; nIndex < nNumParameters; ++nIndex ) {
      if ( pluginParameters_.hasChanged( nIndex ) ) {
         float fValue = pluginParameters_.getFloat( nIndex );
         changeParameter( nIndex, fValue );
      }
   }

   if ( bIncludeHiddenParameters ) {
      // handle hidden parameters here!

      // the following parameters need no updating:
      //
   }
}


void SqueezerAudioProcessor::resetMeters()
{
   if ( compressor_ ) {
      compressor_->resetMeters();
   }

   // "UM" ==> update meters
   sendActionMessage( "UM" );
}


float SqueezerAudioProcessor::getGainReduction(
   int nChannel )
{
   if ( compressor_ ) {
      return ( float ) compressor_->getGainReduction( nChannel );
   } else {
      return -1.0f;
   }
}


float SqueezerAudioProcessor::getPeakMeterInputLevel(
   int nChannel )
{
   if ( compressor_ ) {
      return ( float ) compressor_->getPeakMeterInputLevel( nChannel );
   } else {
      return -1.0f;
   }
}


float SqueezerAudioProcessor::getPeakMeterOutputLevel(
   int nChannel )
{
   if ( compressor_ ) {
      return ( float ) compressor_->getPeakMeterOutputLevel( nChannel );
   } else {
      return -1.0f;
   }
}


float SqueezerAudioProcessor::getAverageMeterInputLevel(
   int nChannel )
{
   if ( compressor_ ) {
      return ( float ) compressor_->getAverageMeterInputLevel( nChannel );
   } else {
      return -1.0f;
   }
}


float SqueezerAudioProcessor::getAverageMeterOutputLevel(
   int nChannel )
{
   if ( compressor_ ) {
      return ( float ) compressor_->getAverageMeterOutputLevel( nChannel );
   } else {
      return -1.0f;
   }
}


bool SqueezerAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
   return true;
#else // JucePlugin_WantsMidiInput
   return false;
#endif // JucePlugin_WantsMidiInput
}


bool SqueezerAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
   return true;
#else // JucePlugin_ProducesMidiOutput
   return false;
#endif // JucePlugin_ProducesMidiOutput
}


bool SqueezerAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
   return true;
#else // JucePlugin_IsMidiEffect
   return false;
#endif // JucePlugin_IsMidiEffect
}


double SqueezerAudioProcessor::getTailLengthSeconds() const
{
   return 0.0;
}


StringArray SqueezerAudioProcessor::getAlternateDisplayNames() const
{
   StringArray displayNames( JucePlugin_Name );
   displayNames.add( "Squeezer" );
   displayNames.add( "squz" );

   return displayNames;
}


int SqueezerAudioProcessor::getNumPrograms()
{
   // some hosts don't cope very well if you tell them there are no
   // programs, so this should be at least 1, even if you're not
   // really implementing programs.
   return 1;
}


int SqueezerAudioProcessor::getCurrentProgram()
{
   return 0;
}


void SqueezerAudioProcessor::setCurrentProgram(
   int nIndex )
{
   ignoreUnused( nIndex );
}


const String SqueezerAudioProcessor::getProgramName(
   int nIndex )
{
   ignoreUnused( nIndex );

   return String();
}


void SqueezerAudioProcessor::changeProgramName(
   int nIndex,
   const String& newName )
{
   ignoreUnused( nIndex, newName );
}


void SqueezerAudioProcessor::prepareToPlay(
   double sampleRate,
   int samplesPerBlock )
{
   ignoreUnused( samplesPerBlock );

   // Use this method as the place to do any pre-playback
   // initialisation that you need..

   Logger::outputDebugString( "[Squeezer] preparing to play" );

   if ( ( sampleRate < 44100 ) || ( sampleRate > 192000 ) ) {
      Logger::outputDebugString( "[Squeezer] WARNING: sample rate of " +
                                 String( sampleRate ) + " Hz not supported" );
      sampleRateIsValid_ = false;
      return;
   } else {
      sampleRateIsValid_ = true;
   }

   Logger::outputDebugString( "[Squeezer] number of main/aux input channels:  " +
                              String( getMainBusNumInputChannels() ) + "/" +
                              String( getTotalNumInputChannels() - getMainBusNumInputChannels() ) );
   Logger::outputDebugString( "[Squeezer] number of main/aux output channels: " +
                              String( getMainBusNumOutputChannels() ) + "/" +
                              String( getTotalNumOutputChannels() - getMainBusNumOutputChannels() ) );

   if ( reloadEditor_ ) {
      reloadEditor_ = false;
      auto editor = dynamic_cast<SqueezerAudioProcessorEditor*>( getActiveEditor() );

      if ( editor != nullptr ) {
         editor->setNumberOfChannels( numberOfChannels_, true );
      }
   }

   bool bBypassCompressor = pluginParameters_.getBoolean(
                               SqueezerPluginParameters::selBypass );
   float fRmsWindowSizeMilliSeconds = pluginParameters_.getRealFloat(
                                         SqueezerPluginParameters::selRmsWindowSize );
   int nDesign = pluginParameters_.getRealInteger(
                    SqueezerPluginParameters::selDesign );
   int nCurveType = pluginParameters_.getRealInteger(
                       SqueezerPluginParameters::selCurveType );
   int nGainStage = pluginParameters_.getRealInteger(
                       SqueezerPluginParameters::selGainStage );

   float fThreshold = pluginParameters_.getRealFloat(
                         SqueezerPluginParameters::selThreshold );
   float fRatio = pluginParameters_.getRealFloat(
                     SqueezerPluginParameters::selRatio );
   float fKneeWidth = pluginParameters_.getRealFloat(
                         SqueezerPluginParameters::selKneeWidth );

   float fAttackRate = pluginParameters_.getRealFloat(
                          SqueezerPluginParameters::selAttackRate );
   int nReleaseRate = pluginParameters_.getRealInteger(
                         SqueezerPluginParameters::selReleaseRate );

   float fInputTrim = pluginParameters_.getRealFloat(
                         SqueezerPluginParameters::selInputTrim );
   bool bAutoMakeupGain = pluginParameters_.getBoolean(
                             SqueezerPluginParameters::selAutoMakeupGain );
   float fMakeupGain = pluginParameters_.getRealFloat(
                          SqueezerPluginParameters::selMakeupGain );
   int nStereoLink = pluginParameters_.getRealInteger(
                        SqueezerPluginParameters::selStereoLink );
   int nWetMix = pluginParameters_.getRealInteger(
                    SqueezerPluginParameters::selWetMix );

   bool bSidechainInput = pluginParameters_.getBoolean(
                             SqueezerPluginParameters::selSidechainInput );
   int nSidechainHPFCutoff = pluginParameters_.getRealInteger(
                                SqueezerPluginParameters::selSidechainHPFCutoff );
   int nSidechainLPFCutoff = pluginParameters_.getRealInteger(
                                SqueezerPluginParameters::selSidechainLPFCutoff );
   bool bSidechainListen = pluginParameters_.getBoolean(
                              SqueezerPluginParameters::selSidechainListen );

   dither_.initialise( jmax( getMainBusNumInputChannels(),
                             getMainBusNumOutputChannels() ),
                       24 );

   compressor_ = std::make_unique<Compressor>( numberOfChannels_, ( int ) sampleRate );

   compressor_->setBypass( bBypassCompressor );
   compressor_->setRmsWindowSize( fRmsWindowSizeMilliSeconds );
   compressor_->setDesign( nDesign );
   compressor_->setCurve( nCurveType );
   compressor_->setGainStage( nGainStage );

   compressor_->setThreshold( fThreshold );
   compressor_->setRatio( fRatio );
   compressor_->setKneeWidth( fKneeWidth );

   compressor_->setAttackRate( fAttackRate );
   compressor_->setReleaseRate( nReleaseRate );

   compressor_->setInputTrim( fInputTrim );
   compressor_->setAutoMakeupGain( bAutoMakeupGain );
   compressor_->setMakeupGain( fMakeupGain );
   compressor_->setStereoLink( nStereoLink );
   compressor_->setWetMix( nWetMix );

   compressor_->setSidechainInput( bSidechainInput );
   compressor_->setSidechainHPFCutoff( nSidechainHPFCutoff );
   compressor_->setSidechainLPFCutoff( nSidechainLPFCutoff );
   compressor_->setSidechainListen( bSidechainListen );
}


void SqueezerAudioProcessor::releaseResources()
{
   // When playback stops, you can use this as an opportunity to free
   // up any spare memory, etc.

   Logger::outputDebugString( "[Squeezer] releasing resources" );
   Logger::outputDebugString( "" );
}


void SqueezerAudioProcessor::reset()
{
   // Use this method as the place to clear any delay lines, buffers,
   // etc, as it means there's been a break in the audio's
   // continuity.
}


void SqueezerAudioProcessor::processBlock(
   AudioBuffer<float>& buffer,
   MidiBuffer& midiMessages )
{
   jassert( ! isUsingDoublePrecision() );
   ignoreUnused( midiMessages );

   // temporarily disable denormals
   ScopedNoDenormals noDenormals;

   // create temporary buffer
   AudioBuffer<double> processBuffer( buffer.getNumChannels(), buffer.getNumSamples() );

   // copy input to temporary buffer and convert to double;
   // de-normalize samples
   dither_.convertToDouble( buffer, processBuffer );

   // process input samples
   process( processBuffer );

   // copy temporary buffer to output and dither to float
   dither_.ditherToFloat( processBuffer, buffer );
}


void SqueezerAudioProcessor::processBlock(
   AudioBuffer<double>& buffer,
   MidiBuffer& midiMessages )
{
   jassert( isUsingDoublePrecision() );
   ignoreUnused( midiMessages );

   // process input samples
   process( buffer );
}


void SqueezerAudioProcessor::process(
   AudioBuffer<double>& buffer )
{
   if ( ! sampleRateIsValid_ ) {
      buffer.clear();
      return;
   }

   // In case we have more main outputs than inputs, we'll clear any
   // output channels that didn't contain input data, because these
   // aren't guaranteed to be empty -- they may contain garbage.
   for ( int nChannel = getMainBusNumInputChannels(); nChannel < getMainBusNumOutputChannels(); ++nChannel ) {
      buffer.clear( nChannel, 0, buffer.getNumSamples() );
   }

   if ( getMainBusNumInputChannels() < 1 ) {
      Logger::outputDebugString( "[Squeezer] no input channels!" );
      return;
   }

   AudioBuffer<double> mainInput_( numberOfChannels_, buffer.getNumSamples() );
   AudioBuffer<double> sideChainInput_( numberOfChannels_, buffer.getNumSamples() );

#if JucePlugin_Build_VST && SQUEEZER_EXTERNAL_SIDECHAIN == 1

   hasSideChain_ = true;

#ifdef SQUEEZER_MONO

   mainInput_.copyFrom( 0, 0, buffer,
                        0, 0, buffer.getNumSamples() );

   sideChainInput_.copyFrom( 0, 0, buffer,
                             1, 0, buffer.getNumSamples() );

#else // SQUEEZER_MONO

   mainInput_.copyFrom( 0, 0, buffer,
                        0, 0, buffer.getNumSamples() );
   mainInput_.copyFrom( 1, 0, buffer,
                        1, 0, buffer.getNumSamples() );

   sideChainInput_.copyFrom( 0, 0, buffer,
                             2, 0, buffer.getNumSamples() );
   sideChainInput_.copyFrom( 1, 0, buffer,
                             3, 0, buffer.getNumSamples() );

#endif // SQUEEZER_MONO

#else // JucePlugin_Build_VST && SQUEEZER_EXTERNAL_SIDECHAIN == 1

   if ( getChannelLayoutOfBus( true, 0 ).size() == numberOfChannels_ ) {
      mainInput_ = getBusBuffer( buffer, true, 0 );

      if ( getChannelLayoutOfBus( true, 1 ).size() == numberOfChannels_ ) {
         hasSideChain_ = true;
         sideChainInput_ = getBusBuffer( buffer, true, 1 );
      } else {
         hasSideChain_ = false;
         sideChainInput_ = getBusBuffer( buffer, true, 0 );
      }
   } else {
      Logger::outputDebugString( "clearing main input and side chain" );

      hasSideChain_ = false;
      mainInput_.clear();
      sideChainInput_.clear();
   }

#endif // JucePlugin_Build_VST && SQUEEZER_EXTERNAL_SIDECHAIN == 1

   compressor_->process( mainInput_, sideChainInput_ );

   for ( int channel = 0; channel < numberOfChannels_; ++channel ) {
      buffer.copyFrom( channel, 0, mainInput_,
                       channel, 0, buffer.getNumSamples() );
   }

   // "UM" ==> update meters
   sendActionMessage( "UM" );
}


AudioProcessorEditor* SqueezerAudioProcessor::createEditor()
{
   return new SqueezerAudioProcessorEditor( *this, pluginParameters_, numberOfChannels_ );
}


bool SqueezerAudioProcessor::hasEditor() const
{
   return true;
}


void SqueezerAudioProcessor::getStateInformation(
   MemoryBlock& destData )
{
   XmlElement xmlParameters = pluginParameters_.storeAsXml();

   DBG( "[Squeezer]" );
   DBG( "[Squeezer] storing plug-in parameters:" );
   DBG( "[Squeezer]" );
   DBG( String( "[Squeezer]   " ) + xmlParameters.toString().replace(
           "\n", "\n[Squeezer]   " ) );

   copyXmlToBinary( xmlParameters, destData );
}


void SqueezerAudioProcessor::setStateInformation(
   const void* data,
   int sizeInBytes )
{
   std::unique_ptr<XmlElement> xmlParameters( getXmlFromBinary( data, sizeInBytes ) );

   DBG( "[Squeezer] loading plug-in parameters:" );
   DBG( "[Squeezer]" );
   DBG( String( "[Squeezer]   " ) + xmlParameters->toString().replace(
           "\n", "\n[Squeezer]   " ) );

   pluginParameters_.loadFromXml( xmlParameters.get() );
   updateParameters( true );
}


// This creates new instances of the plug-in.
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
   return new SqueezerAudioProcessor();
}
