--  ----------------------------------------------------------------------------
--  
--  Squeezer
--  ========
--  Flexible general-purpose audio compressor with a touch of citrus
--  
--  Copyright (c) 2013-2021 Martin Zuther (http://www.mzuther.de/)
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
--  ----------------------------------------------------------------------------


-- #############################################################################
--
-- WARNING: this file was auto-generated, please do not edit!
--
-- #############################################################################


if not _ACTION then
    -- prevent "attempt to concatenate a nil value" error
    _ACTION = ""
elseif _ACTION == "gmake" then
    if _OPTIONS["cc"] == "clang" then
        print ("=== Generating project files (Clang, " .. os.target():upper() .. ") ===")
    else
        print ("=== Generating project files (GNU gcc, " .. os.target():upper() .. ") ===")
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
        "../JuceLibraryCode/include_juce_dsp.cpp",
        "../JuceLibraryCode/include_juce_events.cpp",
        "../JuceLibraryCode/include_juce_graphics.cpp",
        "../JuceLibraryCode/include_juce_gui_basics.cpp",
        "../JuceLibraryCode/include_juce_gui_extra.cpp",
        "../JuceLibraryCode/include_juce_video.cpp"
    }

    removefiles {
        "../Source/*-COPY.*",
    }

    defines {
       "JUCE_MODAL_LOOPS_PERMITTED=1"
    }

    includedirs {
        "../JuceLibraryCode/",
        "../libraries/",
        "../libraries/juce/modules/",
        "../Source/frut/",
    }

    cppdialect "C++17"

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
        systemversion "latest"

        defines {
            "_WINDOWS=1",
            "_USE_MATH_DEFINES=1",
        }

        flags {
           "MultiProcessorCompile",
           "NoMinimalRebuild"
        }

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
        buildoptions { "-DHAVE_LROUND", "-fmessage-length=78", "-fcolor-diagnostics" }

    filter { "system:linux", "configurations:Debug" }
        warnings "Extra"
        buildoptions { "-fno-inline", "-ggdb" }

    filter { "system:linux", "configurations:Debug", "platforms:x32" }
        targetsuffix "_debug"

    filter { "system:linux", "configurations:Debug", "platforms:x64" }
        targetsuffix "_x64_debug"

    filter { "system:windows", "configurations:Debug" }
       symbols "Full"
       -- increase the number of sections that an object file can contain
       buildoptions { "/bigobj" }

    filter { "system:windows", "configurations:Debug", "platforms:x32" }
        targetsuffix " Debug"

    filter { "system:windows", "configurations:Debug", "platforms:x64" }
        targetsuffix " x64 Debug"

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
        targetsuffix ""

    filter { "system:windows", "configurations:Release", "platforms:x64" }
        targetsuffix " x64"

--------------------------------------------------------------------------------

    project ("squeezer_standalone_stereo")
        kind "WindowedApp"
        targetdir "../bin/standalone/"

        defines {
            "SQUEEZER_STEREO=1",
            "SQUEEZER_EXTERNAL_SIDECHAIN=1",
            "JucePlugin_Build_Standalone=1",
            "JucePlugin_Build_LV2=0",
            "JucePlugin_Build_VST=0",
            "JucePlugin_Build_VST3=0"
        }

        entrypoint "WinMainCRTStartup"

        files {
              "../JuceLibraryCode/include_juce_audio_plugin_client_Standalone.cpp"
        }

        filter { "system:linux" }
            targetname "squeezer"

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
            targetname "Squeezer"
            targetextension (".exe")

            defines {
                "JUCE_ALSA=0",
                "JUCE_JACK=0",
                "JUCE_WASAPI=1",
                "JUCE_DIRECTSOUND=1"
            }

        filter { "configurations:Debug" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/squeezer_standalone_stereo_debug")

        filter { "configurations:Release" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/squeezer_standalone_stereo_release")

--------------------------------------------------------------------------------

    project ("squeezer_standalone_mono")
        kind "WindowedApp"
        targetdir "../bin/standalone/"

        defines {
            "SQUEEZER_MONO=1",
            "SQUEEZER_EXTERNAL_SIDECHAIN=1",
            "JucePlugin_Build_Standalone=1",
            "JucePlugin_Build_LV2=0",
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
            targetname "Squeezer (mono)"
            targetextension (".exe")

            defines {
                "JUCE_ALSA=0",
                "JUCE_JACK=0",
                "JUCE_WASAPI=1",
                "JUCE_DIRECTSOUND=1"
            }

        filter { "configurations:Debug" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/squeezer_standalone_mono_debug")

        filter { "configurations:Release" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/squeezer_standalone_mono_release")

--------------------------------------------------------------------------------

    project ("squeezer_vst2_stereo")
        kind "SharedLib"
        targetdir "../bin/vst2/"

        defines {
            "SQUEEZER_STEREO=1",
            "SQUEEZER_EXTERNAL_SIDECHAIN=0",
            "JucePlugin_Build_Standalone=0",
            "JucePlugin_Build_LV2=0",
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
            targetname "squeezer_vst2"

        filter { "system:windows" }
            targetname "Squeezer"
            targetextension (".dll")

        filter { "configurations:Debug" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/squeezer_vst2_stereo_debug")

        filter { "system:windows", "configurations:Debug", "platforms:x32" }
            targetdir "D:/Plugins/32-bit/Categories/Dynamics/Compressor"
            debugcommand "C:/Program Files (x86)/REAPER/reaper.exe"

        filter { "system:windows", "configurations:Debug", "platforms:x64" }
            targetdir "D:/Plugins/64-bit/Categories/Dynamics/Compressor"
            debugcommand "C:/Program Files/REAPER (x64)/reaper.exe"

        filter { "configurations:Release" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/squeezer_vst2_stereo_release")

--------------------------------------------------------------------------------

    project ("squeezer_vst2_mono")
        kind "SharedLib"
        targetdir "../bin/vst2/"

        defines {
            "SQUEEZER_MONO=1",
            "SQUEEZER_EXTERNAL_SIDECHAIN=0",
            "JucePlugin_Build_Standalone=0",
            "JucePlugin_Build_LV2=0",
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
            targetname "squeezer_vst2_mono"

        filter { "system:windows" }
            targetname "Squeezer (mono)"
            targetextension (".dll")

        filter { "configurations:Debug" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/squeezer_vst2_mono_debug")

        filter { "system:windows", "configurations:Debug", "platforms:x32" }
            targetdir "D:/Plugins/32-bit/Categories/Dynamics/Compressor"
            debugcommand "C:/Program Files (x86)/REAPER/reaper.exe"

        filter { "system:windows", "configurations:Debug", "platforms:x64" }
            targetdir "D:/Plugins/64-bit/Categories/Dynamics/Compressor"
            debugcommand "C:/Program Files/REAPER (x64)/reaper.exe"

        filter { "configurations:Release" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/squeezer_vst2_mono_release")

--------------------------------------------------------------------------------

    project ("squeezer_vst2_sidechain_stereo")
        kind "SharedLib"
        targetdir "../bin/vst2/"

        defines {
            "SQUEEZER_STEREO=1",
            "SQUEEZER_EXTERNAL_SIDECHAIN=1",
            "JucePlugin_Build_Standalone=0",
            "JucePlugin_Build_LV2=0",
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
            targetname "squeezer_vst2_sidechain"

        filter { "system:windows" }
            targetname "Squeezer (side-chain)"
            targetextension (".dll")

        filter { "configurations:Debug" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/squeezer_vst2_sidechain_stereo_debug")

        filter { "system:windows", "configurations:Debug", "platforms:x32" }
            targetdir "D:/Plugins/32-bit/Categories/Dynamics/Compressor"
            debugcommand "C:/Program Files (x86)/REAPER/reaper.exe"

        filter { "system:windows", "configurations:Debug", "platforms:x64" }
            targetdir "D:/Plugins/64-bit/Categories/Dynamics/Compressor"
            debugcommand "C:/Program Files/REAPER (x64)/reaper.exe"

        filter { "configurations:Release" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/squeezer_vst2_sidechain_stereo_release")

--------------------------------------------------------------------------------

    project ("squeezer_vst2_sidechain_mono")
        kind "SharedLib"
        targetdir "../bin/vst2/"

        defines {
            "SQUEEZER_MONO=1",
            "SQUEEZER_EXTERNAL_SIDECHAIN=1",
            "JucePlugin_Build_Standalone=0",
            "JucePlugin_Build_LV2=0",
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
            targetname "squeezer_vst2_sidechain_mono"

        filter { "system:windows" }
            targetname "Squeezer (side-chain, mono)"
            targetextension (".dll")

        filter { "configurations:Debug" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/squeezer_vst2_sidechain_mono_debug")

        filter { "system:windows", "configurations:Debug", "platforms:x32" }
            targetdir "D:/Plugins/32-bit/Categories/Dynamics/Compressor"
            debugcommand "C:/Program Files (x86)/REAPER/reaper.exe"

        filter { "system:windows", "configurations:Debug", "platforms:x64" }
            targetdir "D:/Plugins/64-bit/Categories/Dynamics/Compressor"
            debugcommand "C:/Program Files/REAPER (x64)/reaper.exe"

        filter { "configurations:Release" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/squeezer_vst2_sidechain_mono_release")

--------------------------------------------------------------------------------

    project ("squeezer_vst3")
        kind "SharedLib"

        defines {
            "SQUEEZER_VARIBUS=1",
            "SQUEEZER_EXTERNAL_SIDECHAIN=1",
            "JucePlugin_Build_Standalone=0",
            "JucePlugin_Build_LV2=0",
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
            "../libraries/juce/modules/juce_audio_processors/format_types/VST3_SDK"
        }

        filter { "system:linux" }
            targetname "Squeezer"

        filter { "system:linux", "platforms:x32" }
            targetdir "../bin/vst3/Squeezer.vst3/Contents/i386-linux/"

        filter { "system:linux", "platforms:x64" }
            targetdir "../bin/vst3/Squeezer.vst3/Contents/x86_64-linux/"

        filter { "system:windows" }
            targetname "Squeezer"
            targetextension (".vst3")

        filter { "system:windows", "platforms:x32" }
            targetdir "../bin/vst3/Squeezer.vst3/Contents/x86-win/"

        filter { "system:windows", "platforms:x64" }
            targetdir "../bin/vst3/Squeezer.vst3/Contents/x86_64-win/"

        filter { "configurations:Debug" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/squeezer_vst3_debug")

        filter { "system:linux", "configurations:Debug" }
            targetsuffix "\\ Debug"

        filter { "system:windows", "configurations:Debug" }
            targetsuffix " Debug"

        filter { "system:windows", "configurations:Debug", "platforms:x32" }
            targetdir "C:/Program Files (x86)/Common Files/VST3/mzuther/Squeezer.vst3/Contents/x86-win/"
            debugcommand "C:/Program Files (x86)/REAPER/reaper.exe"

        filter { "system:windows", "configurations:Debug", "platforms:x64" }
            targetdir "C:/Program Files/Common Files/VST3/mzuther/Squeezer.vst3/Contents/x86_64-win/"
            debugcommand "C:/Program Files/REAPER (x64)/reaper.exe"

        filter { "configurations:Release" }
            targetsuffix ""
            objdir ("../bin/.intermediate_" .. os.target() .. "/squeezer_vst3_release")

--------------------------------------------------------------------------------

-- create unit tests on Linux only
if os.target() == "linux" then

    project ("squeezer_unittest")
        kind "ConsoleApp"
        targetdir "../bin/unittest/"

        defines {
            "SQUEEZER_STEREO=1",
            "SQUEEZER_EXTERNAL_SIDECHAIN=1",
            "JucePlugin_Build_Standalone=1",
            "JucePlugin_Build_LV2=0",
            "JucePlugin_Build_VST=0",
            "JucePlugin_Build_VST3=0"
        }

        includedirs {
            "../libraries/googletest/googletest/include/",
        }

        files {
              "../Source/frut/unittest.h",
              "../Source/frut/**_unittest.h",
              "../Source/frut/**_unittest.cpp"
        }

        filter { "system:linux" }
            targetname "squeezer_unittest"

            defines {
                "JUCE_ALSA=0",
                "JUCE_JACK=0",
                "JUCE_WASAPI=0",
                "JUCE_DIRECTSOUND=0"
            }

        filter { "system:linux", "platforms:x32" }
            linkoptions {
                -- force static linking to googletest
                "../../../libraries/googletest/googletest/build/lib/linux/i386/libgtest.a",
                "../../../libraries/googletest/googletest/build/lib/linux/i386/libgtest_main.a"
        }

        filter { "system:linux", "platforms:x64" }
            linkoptions {
                -- force static linking to googletest
                "../../../libraries/googletest/googletest/build/lib/linux/amd64/libgtest.a",
                "../../../libraries/googletest/googletest/build/lib/linux/amd64/libgtest_main.a"
        }

        filter { "configurations:Debug" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/squeezer_unittest_debug")

        filter { "configurations:Release" }
            objdir ("../bin/.intermediate_" .. os.target() .. "/squeezer_unittest_release")

-- create unit tests on Linux only
end
