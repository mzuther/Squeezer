#!/bin/bash

#  ----------------------------------------------------------------------------
#  
#  Squeezer
#  ========
#  Flexible general-purpose audio compressor with a touch of citrus
#  
#  Copyright (c) 2013-2020 Martin Zuther (http://www.mzuther.de/)
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
#  WARNING: this file is auto-generated, please do not edit!
#
###############################################################################

binary_dir="final"


function finalise_binary
{
    filepath=$1
    filename=$(basename "$1")

    if [ -f "./$filepath" ]; then
        if [ ! -f "$binary_dir/$filename" ] || [ "./$filepath" -nt "$binary_dir/$filename" ]; then
            printf "  Finalising binary:   %s -->\n" "$filepath"
            printf "                       %s\n"     "$binary_dir/$filename"

            cp "./$filepath" "./$binary_dir/$filename"

            printf "\n"
        fi
    fi
}


function finalise_symbols
{
    filepath=$1
    filename=$(basename "$1")

    if [ -f "./$filepath" ]; then
        if [ ! -f "$binary_dir/debug_symbols/$filepath" ] || [ "./$filepath" -nt "$binary_dir/debug_symbols/$filepath" ]; then
            printf "  Finalising symbols:  %s -->\n" "$filepath"
            printf "                       %s\n"     "$binary_dir/debug_symbols/$filepath"

            mkdir -p "$(dirname "./$binary_dir/debug_symbols/$filepath")"
            cp "./$filepath" "./$binary_dir/debug_symbols/$filepath"

            printf "\n"
        fi
    fi
}


printf "\n  === Finalising binaries ===\n\n"

mkdir -p "$binary_dir/debug_symbols"

finalise_binary  "standalone/squeezer_stereo"
finalise_binary  "standalone/squeezer_mono"

finalise_binary  "vst2/squeezer_stereo_vst2.so"
finalise_binary  "vst2/squeezer_mono_vst2.so"
finalise_binary  "vst2/squeezer_stereo_no_sidechain_vst2.so"
finalise_binary  "vst2/squeezer_mono_no_sidechain_vst2.so"

finalise_binary  "standalone/squeezer_stereo_x64"
finalise_binary  "standalone/squeezer_mono_x64"

finalise_binary  "vst2/squeezer_stereo_vst2_x64.so"
finalise_binary  "vst2/squeezer_mono_vst2_x64.so"
finalise_binary  "vst2/squeezer_stereo_no_sidechain_vst2_x64.so"
finalise_binary  "vst2/squeezer_mono_no_sidechain_vst2_x64.so"

finalise_binary  "standalone/Squeezer (Stereo).exe"
finalise_symbols "standalone/Squeezer (Stereo).pdb"
finalise_binary  "standalone/Squeezer (Mono).exe"
finalise_symbols "standalone/Squeezer (Mono).pdb"

finalise_binary  "vst2/Squeezer (Stereo).dll"
finalise_symbols "vst2/Squeezer (Stereo).pdb"
finalise_binary  "vst2/Squeezer (Mono).dll"
finalise_symbols "vst2/Squeezer (Mono).pdb"
finalise_binary  "vst2/Squeezer (Stereo, no side-chain).dll"
finalise_symbols "vst2/Squeezer (Stereo, no side-chain).pdb"
finalise_binary  "vst2/Squeezer (Mono, no side-chain).dll"
finalise_symbols "vst2/Squeezer (Mono, no side-chain).pdb"

finalise_binary  "vst3/Squeezer (Stereo).vst3"
finalise_symbols "vst3/Squeezer (Stereo).pdb"
finalise_binary  "vst3/Squeezer (Mono).vst3"
finalise_symbols "vst3/Squeezer (Mono).pdb"

finalise_binary  "standalone/Squeezer (Stereo x64).exe"
finalise_symbols "standalone/Squeezer (Stereo x64).pdb"
finalise_binary  "standalone/Squeezer (Mono x64).exe"
finalise_symbols "standalone/Squeezer (Mono x64).pdb"

finalise_binary  "vst2/Squeezer (Stereo x64).dll"
finalise_symbols "vst2/Squeezer (Stereo x64).pdb"
finalise_binary  "vst2/Squeezer (Mono x64).dll"
finalise_symbols "vst2/Squeezer (Mono x64).pdb"
finalise_binary  "vst2/Squeezer (Stereo, no side-chain x64).dll"
finalise_symbols "vst2/Squeezer (Stereo, no side-chain x64).pdb"
finalise_binary  "vst2/Squeezer (Mono, no side-chain x64).dll"
finalise_symbols "vst2/Squeezer (Mono, no side-chain x64).pdb"

finalise_binary  "vst3/Squeezer (Stereo x64).vst3"
finalise_symbols "vst3/Squeezer (Stereo x64).pdb"
finalise_binary  "vst3/Squeezer (Mono x64).vst3"
finalise_symbols "vst3/Squeezer (Mono x64).pdb"

printf "  Done.\n\n\n"
