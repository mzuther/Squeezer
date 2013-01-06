--------------------------------------------------------------------------------
--
--  Squeezer
--  ========
--  Flexible general-purpose compressor with a touch of lemon
--
--  Copyright (c) 2013 Martin Zuther (http://www.mzuther.de/)
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
	print "=== Generating project files (Visual C++, WINDOWS) ==="
elseif string.startswith(_ACTION, "xcode") then
	print "=== Generating project files (Xcode, Mac OS X) ==="
else
	print "Action not specified\n"
end

solution "squeezer"
	location ("windows/" .. _ACTION .. "/")
	language "C++"

	platforms { "x32" }

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

	flags {
			"EnableSSE",
			"EnableSSE2",
			"NoMinimalRebuild",
			"StaticRuntime",
			"Unicode",
			"WinMain"
	}

	configuration { "Debug*" }
		defines { "_DEBUG=1", "DEBUG=1", "JUCE_CHECK_MEMORY_LEAKS=1" }
		flags { "Symbols" }
		buildoptions { "" }

	configuration { "Release*" }
		defines { "NDEBUG=1", "JUCE_CHECK_MEMORY_LEAKS=0" }
		flags { "OptimizeSpeed", "NoFramePointer", "NoManifest" }
		buildoptions { "/Zi" }

	configuration { "Debug", "x32" }
		targetsuffix ", Debug)"

	configuration { "Release", "x32" }
		targetsuffix ")"

--------------------------------------------------------------------------------

	project ("Stand-alone (Stereo)")
		kind "WindowedApp"
		targetname "Squeezer (Stereo"
		targetprefix ""

		defines {
			"SQUEEZER_STAND_ALONE=1",
			"SQUEEZER_STEREO=1",
			"JUCETICE_USE_AMALGAMA=1",
			"JUCE_USE_VSTSDK_2_4=0"
		}

		includedirs {
			"../libraries/asiosdk2.2/common"
		}

		configuration {"windows"}
			defines {
				"_WINDOWS=1",
				"_USE_MATH_DEFINES=1",
				"WIN32=1",
				"JUCE_USE_XSHM=0",
				"JUCE_ALSA=0",
				"JUCE_JACK=0",
				"JUCE_ASIO=1",
				"JUCE_DIRECTSOUND=1"
			}

			links {
				"kernel32",
				"user32",
				"gdi32",
				"winspool",
				"comdlg32",
				"advapi32",
				"shell32",
				"ole32",
				"oleaut32",
				"uuid",
				"odbc32",
				"odbccp32"
			 }

		configuration "Debug"
			objdir ("../bin/intermediate_" .. os.get() .. "/standalone_stereo_debug")

		configuration "Release"
			objdir ("../bin/intermediate_" .. os.get() .. "/standalone_stereo_release")

--------------------------------------------------------------------------------

	project ("VST Plug-in (Stereo)")
		kind "SharedLib"
		targetname "Squeezer (Stereo"
		targetprefix ""

		defines {
			"SQUEEZER_VST_PLUGIN=1",
			"SQUEEZER_STEREO=1",
			"JUCETICE_USE_AMALGAMA=1",
			"JUCE_USE_VSTSDK_2_4=1"
		}

		includedirs {
			"../libraries/vstsdk2.4"
		}

		files {
			  "../libraries/juce/modules/juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp"
		}

		excludes {
			"../Source/standalone_application.h",
			"../Source/standalone_application.cpp"
		}

		configuration {"windows"}
			defines {
				"_WINDOWS=1",
				"_USE_MATH_DEFINES=1",
				"WIN32=1",
				"JUCE_USE_XSHM=0",
				"JUCE_ALSA=0",
				"JUCE_JACK=0",
				"JUCE_ASIO=0",
				"JUCE_DIRECTSOUND=0"
			}

			links {
				"kernel32",
				"user32",
				"gdi32",
				"winspool",
				"comdlg32",
				"advapi32",
				"shell32",
				"ole32",
				"oleaut32",
				"uuid",
				"odbc32",
				"odbccp32"
			 }

		configuration "Debug"
			objdir ("../bin/intermediate_" .. os.get() .. "/vst_stereo_debug")

		configuration "Release"
			objdir ("../bin/intermediate_" .. os.get() .. "/vst_stereo_release")

--------------------------------------------------------------------------------
