--------------------------------------------------------------------------------
--
--  Squeezer
--  ========
--  Flexible general-purpose audio compressor with a touch of lemon.
--
--  Copyright (c) 2013-2015 Martin Zuther (http://www.mzuther.de/)
--
--  This program is free software: you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation, either version 3 of the License, or
--  (at your option) any later version.
--
--  This program is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--
--  You should have received a copy of the GNU General Public License
--  along with this program.  If not, see <http://www.gnu.org/licenses/>.
--
--  Thank you for using free software!
--
--------------------------------------------------------------------------------

if not _ACTION then
	-- prevent "attempt to ... (a nil value)" errors
elseif _ACTION == "gmake" then
	print ("=== Generating project files (GNU g++, " .. os.get():upper() .. ") ===")
elseif string.startswith(_ACTION, "vs") then
	print "=== Generating project files (Visual C++, Windows) ==="
elseif string.startswith(_ACTION, "xcode") then
	print "=== Generating project files (Xcode, Mac OS X) ==="
else
	print "Action not specified\n"
end

solution "squeezer"
	language "C++"

	platforms { "x32", "x64" }

	configurations { "Debug", "Release" }

	files {
		"../Source/**.h",
		"../Source/**.cpp",

		"../libraries/juce/modules/juce_audio_basics/juce_audio_basics.cpp",
		"../libraries/juce/modules/juce_audio_devices/juce_audio_devices.cpp",
		"../libraries/juce/modules/juce_audio_formats/juce_audio_formats.cpp",
		"../libraries/juce/modules/juce_audio_processors/juce_audio_processors.cpp",
		"../libraries/juce/modules/juce_audio_utils/juce_audio_utils.cpp",
		"../libraries/juce/modules/juce_core/juce_core.cpp",
		"../libraries/juce/modules/juce_cryptography/juce_cryptography.cpp",
		"../libraries/juce/modules/juce_data_structures/juce_data_structures.cpp",
		"../libraries/juce/modules/juce_events/juce_events.cpp",
		"../libraries/juce/modules/juce_graphics/juce_graphics.cpp",
		"../libraries/juce/modules/juce_gui_basics/juce_gui_basics.cpp",
		"../libraries/juce/modules/juce_gui_extra/juce_gui_extra.cpp",
		"../libraries/juce/modules/juce_video/juce_video.cpp"
	}

	includedirs {
		"../JuceLibraryCode/",
		"../libraries/"
	}

	targetdir "../bin/"

	configuration { "Debug*" }
		defines { "_DEBUG=1", "DEBUG=1", "JUCE_CHECK_MEMORY_LEAKS=1" }
		flags { "Symbols", "ExtraWarnings" }
		buildoptions { "-fno-inline", "-ggdb", "-std=c++11" }

	configuration { "Release*" }
		defines { "NDEBUG=1", "JUCE_CHECK_MEMORY_LEAKS=0" }
		flags { "OptimizeSpeed", "NoFramePointer", "ExtraWarnings" }
		buildoptions { "-fvisibility=hidden", "-pipe", "-std=c++11" }

	configuration { "Debug", "x32" }
		targetsuffix "_debug"

	configuration { "Debug", "x64" }
		targetsuffix "_debug_x64"

	configuration { "Release", "x32" }
		targetsuffix ""

	configuration { "Release", "x64" }
		targetsuffix "_x64"

--------------------------------------------------------------------------------

	project ("squeezer_standalone_mono")
		kind "WindowedApp"
		location (os.get() .. "/standalone_mono")
		targetname "squeezer_mono"
		targetprefix ""

		defines {
			"SQUEEZER_STAND_ALONE=1",
			"SQUEEZER_MONO=1",
			"JUCE_USE_VSTSDK_2_4=0"
		}

		configuration {"linux"}
			defines {
				"LINUX=1",
				"JUCE_USE_XSHM=1",
				"JUCE_ALSA=1",
				"JUCE_JACK=1",
				"JUCE_ASIO=0",
				"JUCE_WASAPI=0",
				"JUCE_DIRECTSOUND=0"
			}

			links {
				"dl",
				"freetype",
				"pthread",
				"rt",
				"X11",
				"Xext",
				"asound"
			}

			includedirs {
				"/usr/include",
				"/usr/include/freetype2"
			}

		configuration "Debug"
			objdir ("../bin/intermediate_" .. os.get() .. "/standalone_mono_debug")

		configuration "Release"
			objdir ("../bin/intermediate_" .. os.get() .. "/standalone_mono_release")

--------------------------------------------------------------------------------

	project ("squeezer_standalone_stereo")
		kind "WindowedApp"
		location (os.get() .. "/standalone_stereo")
		targetname "squeezer_stereo"
		targetprefix ""

		defines {
			"SQUEEZER_STAND_ALONE=1",
			"SQUEEZER_STEREO=1",
			"JUCE_USE_VSTSDK_2_4=0"
		}

		configuration {"linux"}
			defines {
				"LINUX=1",
				"JUCE_USE_XSHM=1",
				"JUCE_ALSA=1",
				"JUCE_JACK=1",
				"JUCE_ASIO=0",
				"JUCE_WASAPI=0",
				"JUCE_DIRECTSOUND=0"
			}

			links {
				"dl",
				"freetype",
				"pthread",
				"rt",
				"X11",
				"Xext",
				"asound"
			}

			includedirs {
				"/usr/include",
				"/usr/include/freetype2"
			}

		configuration "Debug"
			objdir ("../bin/intermediate_" .. os.get() .. "/standalone_stereo_debug")

		configuration "Release"
			objdir ("../bin/intermediate_" .. os.get() .. "/standalone_stereo_release")

--------------------------------------------------------------------------------

	project ("squeezer_lv2_mono")
		kind "SharedLib"
		location (os.get() .. "/lv2_mono")
		targetname "squeezer_mono_lv2"
		targetprefix ""

		defines {
			"SQUEEZER_LV2_PLUGIN=1",
			"SQUEEZER_MONO=1",
			"JUCE_USE_VSTSDK_2_4=0"
		}

		files {
			  "../libraries/juce/modules/juce_audio_plugin_client/utility/juce_PluginUtilities.cpp",
			  "../libraries/juce/modules/juce_audio_plugin_client/LV2/juce_LV2_Wrapper.cpp"
		}

		excludes {
			"../Source/standalone_application.h",
			"../Source/standalone_application.cpp"
		}

		configuration {"linux"}
			defines {
				"LINUX=1",
				"JUCE_USE_XSHM=1",
				"JUCE_ALSA=0",
				"JUCE_JACK=0",
				"JUCE_ASIO=0",
				"JUCE_WASAPI=0",
				"JUCE_DIRECTSOUND=0"
			}

			includedirs {
				"/usr/include",
				"/usr/include/freetype2"
			}

			links {
				"dl",
				"freetype",
				"pthread",
				"rt",
				"X11",
				"Xext"
			}

		configuration "Debug"
			objdir ("../bin/intermediate_" .. os.get() .. "/lv2_mono_debug")

		configuration "Release"
			objdir ("../bin/intermediate_" .. os.get() .. "/lv2_mono_release")

--------------------------------------------------------------------------------

	project ("squeezer_lv2_stereo")
		kind "SharedLib"
		location (os.get() .. "/lv2_stereo")
		targetname "squeezer_stereo_lv2"
		targetprefix ""

		defines {
			"SQUEEZER_LV2_PLUGIN=1",
			"SQUEEZER_STEREO=1",
			"JUCE_USE_VSTSDK_2_4=0"
		}

		files {
			  "../libraries/juce/modules/juce_audio_plugin_client/utility/juce_PluginUtilities.cpp",
			  "../libraries/juce/modules/juce_audio_plugin_client/LV2/juce_LV2_Wrapper.cpp"
		}

		excludes {
			"../Source/standalone_application.h",
			"../Source/standalone_application.cpp"
		}

		configuration {"linux"}
			defines {
				"LINUX=1",
				"JUCE_USE_XSHM=1",
				"JUCE_ALSA=0",
				"JUCE_JACK=0",
				"JUCE_ASIO=0",
				"JUCE_WASAPI=0",
				"JUCE_DIRECTSOUND=0"
			}

			includedirs {
				"/usr/include",
				"/usr/include/freetype2"
			}

			links {
				"dl",
				"freetype",
				"pthread",
				"rt",
				"X11",
				"Xext"
			}

		configuration "Debug"
			objdir ("../bin/intermediate_" .. os.get() .. "/lv2_stereo_debug")

		configuration "Release"
			objdir ("../bin/intermediate_" .. os.get() .. "/lv2_stereo_release")

--------------------------------------------------------------------------------

	project ("squeezer_vst_mono")
		kind "SharedLib"
		location (os.get() .. "/vst_mono")
		targetname "squeezer_mono_vst"
		targetprefix ""

		defines {
			"SQUEEZER_VST_PLUGIN=1",
			"SQUEEZER_MONO=1",
			"JUCE_USE_VSTSDK_2_4=1"
		}

		includedirs {
			"../libraries/vstsdk2.4"
		}

		files {
			  "../libraries/juce/modules/juce_audio_plugin_client/utility/juce_PluginUtilities.cpp",
			  "../libraries/juce/modules/juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp"
		}

		excludes {
			"../Source/standalone_application.h",
			"../Source/standalone_application.cpp"
		}

		configuration {"linux"}
			defines {
				"LINUX=1",
				"JUCE_USE_XSHM=1",
				"JUCE_ALSA=0",
				"JUCE_JACK=0",
				"JUCE_ASIO=0",
				"JUCE_WASAPI=0",
				"JUCE_DIRECTSOUND=0"
			}

			includedirs {
				"/usr/include",
				"/usr/include/freetype2"
			}

			links {
				"dl",
				"freetype",
				"pthread",
				"rt",
				"X11",
				"Xext"
			}

		configuration "Debug"
			objdir ("../bin/intermediate_" .. os.get() .. "/vst_mono_debug")

		configuration "Release"
			objdir ("../bin/intermediate_" .. os.get() .. "/vst_mono_release")

--------------------------------------------------------------------------------

	project ("squeezer_vst_stereo")
		kind "SharedLib"
		location (os.get() .. "/vst_stereo")
		targetname "squeezer_stereo_vst"
		targetprefix ""

		defines {
			"SQUEEZER_VST_PLUGIN=1",
			"SQUEEZER_STEREO=1",
			"JUCE_USE_VSTSDK_2_4=1"
		}

		includedirs {
			"../libraries/vstsdk2.4"
		}

		files {
			  "../libraries/juce/modules/juce_audio_plugin_client/utility/juce_PluginUtilities.cpp",
			  "../libraries/juce/modules/juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp"
		}

		excludes {
			"../Source/standalone_application.h",
			"../Source/standalone_application.cpp"
		}

		configuration {"linux"}
			defines {
				"LINUX=1",
				"JUCE_USE_XSHM=1",
				"JUCE_ALSA=0",
				"JUCE_JACK=0",
				"JUCE_ASIO=0",
				"JUCE_WASAPI=0",
				"JUCE_DIRECTSOUND=0"
			}

			includedirs {
				"/usr/include",
				"/usr/include/freetype2"
			}

			links {
				"dl",
				"freetype",
				"pthread",
				"rt",
				"X11",
				"Xext"
			}

		configuration "Debug"
			objdir ("../bin/intermediate_" .. os.get() .. "/vst_stereo_debug")

		configuration "Release"
			objdir ("../bin/intermediate_" .. os.get() .. "/vst_stereo_release")

--------------------------------------------------------------------------------
