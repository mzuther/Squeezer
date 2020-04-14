--------------------------------------------------------------------------------
--
--  Squeezer
--  ========
--  Flexible general-purpose audio compressor with a touch of citrus
--
--  Copyright (c) 2013-2020 Martin Zuther (http://www.mzuther.de/)
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
    -- prevent "attempt to concatenate a nil value" error
    _ACTION = ""
elseif _ACTION == "gmake" then
    if _OPTIONS["cc"] == "clang" then
        print ("=== Generating project files (Clang++, " .. os.target():upper() .. ") ===")
    else
        print ("=== Generating project files (GNU g++, " .. os.target():upper() .. ") ===")
    end
elseif string.startswith(_ACTION, "codeblocks") then
    print "=== Generating project files (Code::Blocks, Windows) ==="
elseif string.startswith(_ACTION, "vs") then
    print "=== Generating project files (Visual C++, Windows) ==="
elseif string.startswith(_ACTION, "xcode") then
    print "=== Generating project files (Xcode, Mac OS X) ==="
else
    print "Action not specified\n"
end


workspace "squeezer"
    language "C++"
    platforms { "x32", "x64" }
    configurations { "Debug", "Release" }

    location (os.target() .. "/" .. _ACTION .. "/")
    targetprefix ""

    files {
        "../Source/frut/FrutHeader.h",
        "../Source/frut/amalgamated/*.h",
        "../Source/frut/amalgamated/*.cpp",

        "../Source/*.h",
        "../Source/*.cpp",

        "../JuceLibraryCode/include_juce_audio_basics.cpp",
        "../JuceLibraryCode/include_juce_audio_devices.cpp",
        "../JuceLibraryCode/include_juce_audio_formats.cpp",
        "../JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp",
        "../JuceLibraryCode/include_juce_audio_processors.cpp",
        "../JuceLibraryCode/include_juce_audio_utils.cpp",
        "../JuceLibraryCode/include_juce_core.cpp",
        "../JuceLibraryCode/include_juce_cryptography.cpp",
        "../JuceLibraryCode/include_juce_data_structures.cpp",
        "../JuceLibraryCode/include_juce_events.cpp",
        "../JuceLibraryCode/include_juce_graphics.cpp",
        "../JuceLibraryCode/include_juce_gui_basics.cpp",
        "../JuceLibraryCode/include_juce_gui_extra.cpp",
        "../JuceLibraryCode/include_juce_video.cpp"
    }

    includedirs {
        "../JuceLibraryCode/",
        "../libraries/juce/modules/",
        "../Source/frut/",
        "../libraries/"
    }

    cppdialect "C++14"

    filter { "system:linux" }
        defines {
            "LINUX=1"
        }

        includedirs {
            "/usr/include",
            "/usr/include/freetype2"
        }

        linkoptions {
            -- check for unresolved symbols in shared libraries
            "-Wl,--no-undefined"
        }

        links {
            "dl",
            "freetype",
            "pthread",
            "rt",
            "X11",
            "Xext"
        }

    filter { "system:windows" }
        defines {
            "_WINDOWS=1",
            "_USE_MATH_DEFINES=1",
        }

        systemversion "10.0.17763.0"

        vectorextensions "SSE2"

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

    filter { "platforms:x32" }
        architecture "x86"

    filter { "platforms:x64" }
        architecture "x86_64"

    filter { "system:windows", "platforms:x32" }
        defines {
            "WIN32=1",
        }

    filter { "system:windows", "platforms:x64" }
        defines {
            "WIN64=1",
        }

    filter { "configurations:Debug" }
        defines { "_DEBUG=1", "DEBUG=1", "JUCE_CHECK_MEMORY_LEAKS=1" }
        optimize "Off"
        symbols "On"

    filter { "system:linux" }
        buildoptions { "-DHAVE_LROUND", "-fmessage-length=78" }

    filter { "system:linux", "configurations:Debug" }
        warnings "Extra"
        buildoptions { "-fno-inline", "-ggdb" }

    filter { "system:linux", "configurations:Debug", "platforms:x32" }
        targetsuffix "_debug"

    filter { "system:linux", "configurations:Debug", "platforms:x64" }
        targetsuffix "_debug_x64"

    filter { "system:windows", "configurations:Debug" }
       symbols "Full"
       -- increase the number of sections that an object file can contain
       buildoptions { "/bigobj" }

    filter { "system:windows", "configurations:Debug", "platforms:x32" }
        targetsuffix ", Debug)"

    filter { "system:windows", "configurations:Debug", "platforms:x64" }
        targetsuffix " x64, Debug)"

    filter { "configurations:Release" }
        defines { "NDEBUG=1", "JUCE_CHECK_MEMORY_LEAKS=0" }
        omitframepointer "On"
        optimize "Speed"

    filter { "system:linux", "configurations:Release" }
        warnings "Extra"
        buildoptions { "-fvisibility=hidden", "-pipe", "-Wno-deprecated"}

    filter { "system:linux", "configurations:Release", "platforms:x32" }
        targetsuffix ""

    filter { "system:linux", "configurations:Release", "platforms:x64" }
        targetsuffix "_x64"

    filter { "system:windows", "configurations:Release" }
       symbols "Full"
       -- globally disable deprecation warnings
       buildoptions { "/wd4996" }

    filter { "system:windows", "configurations:Release", "platforms:x32" }
        targetsuffix ")"

    filter { "system:windows", "configurations:Release", "platforms:x64" }
        targetsuffix " x64)"

--------------------------------------------------------------------------------

    project ("squeezer_standalone_mono")
        kind "WindowedApp"
        targetdir "../bin/standalone/"

        defines {
            "SQUEEZER_MONO=1",
            "SQUEEZER_EXTERNAL_SIDECHAIN=1",
            "JucePlugin_Build_Standalone=1",
            "JucePlugin_Build_VST=0",
            "JucePlugin_Build_VST3=0"
        }

        entrypoint "WinMainCRTStartup"

        files {
              "../JuceLibraryCode/include_juce_audio_plugin_client_Standalone.cpp"
        }

        filter { "system:linux" }
            targetname "squeezer_mono"

            defines {
                "JUCE_ALSA=1",
                "JUCE_JACK=1",
                "JUCE_WASAPI=0",
                "JUCE_DIRECTSOUND=0"
            }

            links {
                "asound"
            }

        filter { "system:windows" }
            targetname "Squeezer (Mono"
            targetextension (".exe")

            defines {
                "JUCE_ALSA=0",
                "JUCE_JACK=0",
                "JUCE_WASAPI=1",
                "JUCE_DIRECTSOUND=1"
            }

        filter { "configurations:Debug" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/standalone_mono_debug")

        filter { "configurations:Release" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/standalone_mono_release")

--------------------------------------------------------------------------------

    project ("squeezer_standalone_stereo")
        kind "WindowedApp"
        targetdir "../bin/standalone/"

        defines {
            "SQUEEZER_STEREO=1",
            "SQUEEZER_EXTERNAL_SIDECHAIN=1",
            "JucePlugin_Build_Standalone=1",
            "JucePlugin_Build_VST=0",
            "JucePlugin_Build_VST3=0"
        }

        entrypoint "WinMainCRTStartup"

        files {
              "../JuceLibraryCode/include_juce_audio_plugin_client_Standalone.cpp"
        }

        filter { "system:linux" }
            targetname "squeezer_stereo"

            defines {
                "JUCE_ALSA=1",
                "JUCE_JACK=1",
                "JUCE_WASAPI=0",
                "JUCE_DIRECTSOUND=0"
            }

            links {
                "asound"
            }

        filter { "system:windows" }
            targetname "Squeezer (Stereo"
            targetextension (".exe")

            defines {
                "JUCE_ALSA=0",
                "JUCE_JACK=0",
                "JUCE_WASAPI=1",
                "JUCE_DIRECTSOUND=1"
            }

        filter { "configurations:Debug" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/standalone_stereo_debug")

        filter { "configurations:Release" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/standalone_stereo_release")

--------------------------------------------------------------------------------

    project ("squeezer_vst2_mono")
        kind "SharedLib"
        targetdir "../bin/vst2/"

        defines {
            "SQUEEZER_MONO=1",
            "SQUEEZER_EXTERNAL_SIDECHAIN=1",
            "JucePlugin_Build_Standalone=0",
            "JucePlugin_Build_VST=1",
            "JucePlugin_Build_VST3=0"
        }

        files {
              "../JuceLibraryCode/include_juce_audio_plugin_client_VST2.cpp"
        }

        defines {
            "JUCE_ALSA=0",
            "JUCE_JACK=0",
            "JUCE_WASAPI=0",
            "JUCE_DIRECTSOUND=0"
        }

        includedirs {
            "../libraries/vst2/VST2_SDK"
        }

        filter { "system:linux" }
            targetname "squeezer_mono_vst2"

        filter { "system:windows" }
            targetname "Squeezer (Mono"
            targetextension (".dll")

        filter { "configurations:Debug" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/vst2_mono_debug")

        filter { "configurations:Release" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/vst2_mono_release")

--------------------------------------------------------------------------------

    project ("squeezer_vst2_mono_no_sidechain")
        kind "SharedLib"
        targetdir "../bin/vst2/"

        defines {
            "SQUEEZER_MONO=1",
            "SQUEEZER_EXTERNAL_SIDECHAIN=0",
            "JucePlugin_Build_Standalone=0",
            "JucePlugin_Build_VST=1",
            "JucePlugin_Build_VST3=0"
        }

        files {
              "../JuceLibraryCode/include_juce_audio_plugin_client_VST2.cpp"
        }

        defines {
            "JUCE_ALSA=0",
            "JUCE_JACK=0",
            "JUCE_WASAPI=0",
            "JUCE_DIRECTSOUND=0"
        }

        includedirs {
            "../libraries/vst2/VST2_SDK"
        }

        filter { "system:linux" }
            targetname "squeezer_mono_no_sidechain_vst2"

        filter { "system:windows" }
            targetname "Squeezer (Mono, no side-chain"
            targetextension (".dll")

        filter { "configurations:Debug" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/vst2_mono_no_sidechain_debug")

        filter { "configurations:Release" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/vst2_mono_no_sidechain_release")

--------------------------------------------------------------------------------

    project ("squeezer_vst2_stereo")
        kind "SharedLib"
        targetdir "../bin/vst2/"

        defines {
            "SQUEEZER_STEREO=1",
            "SQUEEZER_EXTERNAL_SIDECHAIN=1",
            "JucePlugin_Build_Standalone=0",
            "JucePlugin_Build_VST=1",
            "JucePlugin_Build_VST3=0"
        }

        files {
              "../JuceLibraryCode/include_juce_audio_plugin_client_VST2.cpp"
        }

        defines {
            "JUCE_ALSA=0",
            "JUCE_JACK=0",
            "JUCE_WASAPI=0",
            "JUCE_DIRECTSOUND=0"
        }

        includedirs {
            "../libraries/vst2/VST2_SDK"
        }

        filter { "system:linux" }
            targetname "squeezer_stereo_vst2"

        filter { "system:windows" }
            targetname "Squeezer (Stereo"
            targetextension (".dll")

        filter { "configurations:Debug" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/vst2_stereo_debug")

        filter { "configurations:Release" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/vst2_stereo_release")

--------------------------------------------------------------------------------

    project ("squeezer_vst2_stereo_no_sidechain")
        kind "SharedLib"
        targetdir "../bin/vst2/"

        defines {
            "SQUEEZER_STEREO=1",
            "SQUEEZER_EXTERNAL_SIDECHAIN=0",
            "JucePlugin_Build_Standalone=0",
            "JucePlugin_Build_VST=1",
            "JucePlugin_Build_VST3=0"
        }

        files {
              "../JuceLibraryCode/include_juce_audio_plugin_client_VST2.cpp"
        }

        defines {
            "JUCE_ALSA=0",
            "JUCE_JACK=0",
            "JUCE_WASAPI=0",
            "JUCE_DIRECTSOUND=0"
        }

        includedirs {
            "../libraries/vst2/VST2_SDK"
        }

        filter { "system:linux" }
            targetname "squeezer_stereo_no_sidechain_vst2"

        filter { "system:windows" }
            targetname "Squeezer (Stereo, no side-chain"
            targetextension (".dll")

        filter { "configurations:Debug" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/vst2_stereo_no_sidechain_debug")

        filter { "configurations:Release" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/vst2_stereo_no_sidechain_release")

--------------------------------------------------------------------------------

-- create VST3 projects on Windows only
if os.target() == "windows" then

    project ("squeezer_vst3_mono")
        kind "SharedLib"
        targetdir "../bin/vst3/"

        defines {
            "SQUEEZER_MONO=1",
            "SQUEEZER_EXTERNAL_SIDECHAIN=1",
            "JucePlugin_Build_Standalone=0",
            "JucePlugin_Build_VST=0",
            "JucePlugin_Build_VST3=1"
        }

        files {
              "../JuceLibraryCode/include_juce_audio_plugin_client_VST3.cpp"
        }

        defines {
            "JUCE_ALSA=0",
            "JUCE_JACK=0",
            "JUCE_WASAPI=0",
            "JUCE_DIRECTSOUND=0"
        }

        includedirs {
            "../libraries/vst2/VST2_SDK",
            "../libraries/vst3"
        }

        filter { "system:windows" }
            targetname "Squeezer (Mono"
            targetextension (".vst3")

        filter { "configurations:Debug" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/vst3_mono_debug")

        filter { "configurations:Release" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/vst3_mono_release")

-- create VST3 projects on Windows only
end

--------------------------------------------------------------------------------

-- create VST3 projects on Windows only
if os.target() == "windows" then

    project ("squeezer_vst3_stereo")
        kind "SharedLib"
        targetdir "../bin/vst3/"

        defines {
            "SQUEEZER_STEREO=1",
            "SQUEEZER_EXTERNAL_SIDECHAIN=1",
            "JucePlugin_Build_Standalone=0",
            "JucePlugin_Build_VST=0",
            "JucePlugin_Build_VST3=1"
        }

        files {
              "../JuceLibraryCode/include_juce_audio_plugin_client_VST3.cpp"
        }

        defines {
            "JUCE_ALSA=0",
            "JUCE_JACK=0",
            "JUCE_WASAPI=0",
            "JUCE_DIRECTSOUND=0"
        }

        includedirs {
            "../libraries/vst2/VST2_SDK",
            "../libraries/vst3"
        }

        filter { "system:windows" }
            targetname "Squeezer (Stereo"
            targetextension (".vst3")

        filter { "configurations:Debug" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/vst3_stereo_debug")

        filter { "configurations:Release" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/vst3_stereo_release")

-- create VST3 projects on Windows only
end
