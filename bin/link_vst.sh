#! /usr/bin/env bash

#  ----------------------------------------------------------------------------
#  
#  Squeezer
#  ========
#  Flexible general-purpose audio compressor with a touch of citrus
#  
#  Copyright (c) 2013-2021 Martin Zuther (http://www.mzuther.de/)
#  
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#  
#  Thank you for using free software!
#  
#  ----------------------------------------------------------------------------


###############################################################################
#
#  WARNING: this file was auto-generated, please do not edit!
#
###############################################################################

lv2_dir="$HOME/.lv2"
vst2_dir="$HOME/.vst/mzuther"
vst3_dir="$HOME/.vst3/mzuther"
use_final_binaries=0

resource_dir="squeezer"
source_dir_current=$(realpath "$(dirname "$0")")
source_dir_final=$(realpath "$(dirname "$0")/final")


printf "\nCreating directories"
printf "\n====================\n"

printf "%s\n" "$lv2_dir"
mkdir -p "$lv2_dir"

printf "%s\n" "$vst2_dir"
mkdir -p "$vst2_dir"

printf "%s\n" "$vst3_dir"
mkdir -p "$vst3_dir"


printf "\nLinking LV2 directory"
printf "\n=====================\n"

if [ $use_final_binaries -eq 0 ]; then
    printf "%s\n" "$source_dir_current/lv2/squeezer_lv2"

    ln -sf "$source_dir_current/lv2/squeezer_lv2" "$lv2_dir/"
else
    printf "%s\n" "$source_dir_final/lv2/squeezer_lv2"

    ln -sf "$source_dir_final/lv2/squeezer_lv2" "$lv2_dir/"
fi


printf "\nUnlinking VST2 binaries"
printf "\n=======================\n"

find  "$vst2_dir" -iname "squeezer_vst2_*.so" -print -execdir rm {} \; | \
    sort


if [ $use_final_binaries -eq 0 ]; then
    if [ -d "$source_dir_current/vst2/$resource_dir" ]; then
        printf "\nLinking resource directory"
        printf "\n==========================\n"
        printf "%s\n" "$resource_dir"

        ln -sf "$source_dir_current/vst2/$resource_dir" "$vst2_dir"
    fi
else
    if [ -d "$source_dir_final/vst2/$resource_dir" ]; then
        printf "\nLinking resource directory"
        printf "\n==========================\n"
        printf "%s\n" "$resource_dir"

        ln -sf "$source_dir_final/vst2/$resource_dir" "$vst2_dir"
    fi
fi


printf "\nLinking VST2 debug binaries"
printf "\n===========================\n"

find "$source_dir_current/vst2" -iname "squeezer_vst2*_x64_debug.so" -printf "%f\n" | \
    tee /dev/tty | \
    xargs "-d\n" -I{} ln -sf "$source_dir_current/vst2"/{} "$vst2_dir"/{} | \
    sort


printf "\nLinking VST2 release binaries"
printf "\n=============================\n"

if [ $use_final_binaries -eq 0 ]; then
    find "$source_dir_current/vst2" -iname "squeezer_vst2*_x64.so" -printf "%f\n" | \
        tee /dev/tty | \
        xargs "-d\n" -I{} ln -sf "$source_dir_current/vst2"/{} "$vst2_dir"/{} | \
        sort
else
    find "$source_dir_final/vst2" -iname "squeezer_vst2*_x64.so" -printf "%f\n" | \
        tee /dev/tty | \
        xargs "-d\n" -I{} ln -sf "$source_dir_final/vst2"/{} "$vst2_dir"/{} | \
        sort
fi


printf "\nLinking VST3 directory"
printf "\n======================\n"

if [ $use_final_binaries -eq 0 ]; then
    printf "%s\n" "$source_dir_current/vst3/Squeezer.vst3"

    ln -sf "$source_dir_current/vst3/Squeezer.vst3" "$vst3_dir/"
else
    printf "%s\n" "$source_dir_final/vst3/Squeezer.vst3"

    ln -sf "$source_dir_final/vst3/Squeezer.vst3" "$vst3_dir/"
fi


printf "\n"
