@echo off
#!/bin/bash

rem  ----------------------------------------------------------------------------
rem  
rem  Squeezer
rem  ========
rem  Flexible general-purpose audio compressor with a touch of citrus
rem  
rem  Copyright (c) 2013-2020 Martin Zuther (http://www.mzuther.de/)
rem  
rem  This program is free software: you can redistribute it and/or modify
rem  it under the terms of the GNU General Public License as published by
rem  the Free Software Foundation, either version 3 of the License, or
rem  (at your option) any later version.
rem  
rem  This program is distributed in the hope that it will be useful,
rem  but WITHOUT ANY WARRANTY; without even the implied warranty of
rem  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
rem  GNU General Public License for more details.
rem  
rem  You should have received a copy of the GNU General Public License
rem  along with this program.  If not, see <http://www.gnu.org/licenses/>.
rem  
rem  Thank you for using free software!
rem  
rem  ----------------------------------------------------------------------------


rem ############################################################################
rem
rem  WARNING: this file is auto-generated, please do not edit!
rem
rem ############################################################################

setlocal

set rsync_path="E:\Documents\System\Tools\rsync\bin"
set rsync_cmd="%rsync_path%\rsync.exe" --archive

set vst2_32=/cygdrive/c/Program Files (x86)/Steinberg/VSTPlugins/radix/
set vst3_32=/cygdrive/c/Program Files (x86)/Common Files/VST3/radix/
set vst2_64=/cygdrive/c/Program Files/Steinberg/VSTPlugins/radix/
set vst3_64=/cygdrive/c/Program Files/Common Files/VST3/radix/

set vst2_32_categories=/cygdrive/d/Plugins/32-bit/Categories/Dynamics/Compressor
set vst2_64_categories=/cygdrive/d/Plugins/64-bit/Categories/Dynamics/Compressor


echo.
echo "VST2 (32 bit)"
echo.

%rsync_cmd% --delete "vst2/squeezer" "%vst2_32%"
%rsync_cmd%          "vst2/Squeezer (Stereo).dll" "%vst2_32%"
%rsync_cmd%          "vst2/Squeezer (Mono).dll" "%vst2_32%"
%rsync_cmd%          "vst2/Squeezer (Stereo, no side-chain).dll" "%vst2_32%"
%rsync_cmd%          "vst2/Squeezer (Mono, no side-chain).dll" "%vst2_32%"

echo.
echo "VST2 (32 bit, Categories)"
echo.

%rsync_cmd% --delete "vst2/squeezer" "%vst2_32_categories%"
%rsync_cmd%          "vst2/Squeezer (Stereo).dll" "%vst2_32_categories%"
%rsync_cmd%          "vst2/Squeezer (Mono).dll" "%vst2_32_categories%"
%rsync_cmd%          "vst2/Squeezer (Stereo, no side-chain).dll" "%vst2_32_categories%"
%rsync_cmd%          "vst2/Squeezer (Mono, no side-chain).dll" "%vst2_32_categories%"


echo.
echo "VST3 (32 bit)"
echo.

%rsync_cmd% --delete "vst3/squeezer" "%vst3_32%"
%rsync_cmd%          "vst3/Squeezer (Stereo).vst3" "%vst3_32%"
%rsync_cmd%          "vst3/Squeezer (Mono).vst3" "%vst3_32%"

%rsync_cmd% --delete "vst3/squeezer" "%vst3_32%_categories"
%rsync_cmd%          "vst3/Squeezer (Stereo).vst3" "%vst3_32_categories%"
%rsync_cmd%          "vst3/Squeezer (Mono).vst3" "%vst3_32_categories%"


echo.
echo "VST2 (64 bit)"
echo.

%rsync_cmd% --delete "vst2/squeezer" "%vst2_64%"
%rsync_cmd%          "vst2/Squeezer (Stereo x64).dll" "%vst2_64%"
%rsync_cmd%          "vst2/Squeezer (Mono x64).dll" "%vst2_64%"
%rsync_cmd%          "vst2/Squeezer (Stereo, no side-chain x64).dll" "%vst2_64%"
%rsync_cmd%          "vst2/Squeezer (Mono, no side-chain x64).dll" "%vst2_64%"

echo.
echo "VST2 (64 bit, Categories)"
echo.

%rsync_cmd% --delete "vst2/squeezer" "%vst2_64_categories%"
%rsync_cmd%          "vst2/Squeezer (Stereo x64).dll" "%vst2_64_categories%"
%rsync_cmd%          "vst2/Squeezer (Mono x64).dll" "%vst2_64_categories%"
%rsync_cmd%          "vst2/Squeezer (Stereo, no side-chain x64).dll" "%vst2_64_categories%"
%rsync_cmd%          "vst2/Squeezer (Mono, no side-chain x64).dll" "%vst2_64_categories%"


echo.
echo "VST3 (64 bit)"
echo.

%rsync_cmd% --delete "vst3/squeezer" "%vst3_64%"
%rsync_cmd%          "vst3/Squeezer (Stereo x64).vst3" "%vst3_64%"
%rsync_cmd%          "vst3/Squeezer (Mono x64).vst3" "%vst3_64%"

%rsync_cmd% --delete "vst3/squeezer" "%vst3_64%_categories"
%rsync_cmd%          "vst3/Squeezer (Stereo x64).vst3" "%vst3_64_categories%"
%rsync_cmd%          "vst3/Squeezer (Mono x64).vst3" "%vst3_64_categories%"


