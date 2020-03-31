#!/bin/bash

# ----------------------------------------------------------------------------
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
# ----------------------------------------------------------------------------

executable_dir="final"


function finalise_executable
{
	filepath=$1
	filename=$(basename "$1")

	if [ -f "./$filepath" ]; then
		echo "  Finalising:  $filepath -->"
		echo "               $executable_dir/$filename"

		mv "./$filepath" "./$executable_dir/$filename"

        echo
	fi
}


function finalise_symbols
{
	filepath=$1
	filename=$(basename "$1")

	if [ -f "./$filepath" ]; then
		echo "  Finalising:  $filepath -->"
		echo "               $executable_dir/debug_symbols/$filepath"

        mkdir -p "$(dirname "./$executable_dir/debug_symbols/$filepath")"
		mv "./$filepath" "./$executable_dir/debug_symbols/$filepath"

        echo
	fi
}


echo
echo "  === Finalising executables ==="
echo

finalise_executable "standalone/squeezer_stereo"
finalise_executable "standalone/squeezer_mono"

finalise_executable "lv2/squeezer_stereo_lv2.so"
finalise_executable "lv2/squeezer_mono_lv2.so"

finalise_executable "vst/squeezer_stereo_vst.so"
finalise_executable "vst/squeezer_mono_vst.so"

finalise_executable "standalone/squeezer_stereo_x64"
finalise_executable "standalone/squeezer_mono_x64"

finalise_executable "lv2/squeezer_stereo_lv2_x64.so"
finalise_executable "lv2/squeezer_mono_lv2_x64.so"

finalise_executable "vst/squeezer_stereo_vst_x64.so"
finalise_executable "vst/squeezer_mono_vst_x64.so"

finalise_executable "standalone/Squeezer (Stereo).exe"
finalise_symbols    "standalone/Squeezer (Stereo).pdb"
finalise_executable "standalone/Squeezer (Mono).exe"
finalise_symbols    "standalone/Squeezer (Mono).pdb"

finalise_executable "vst/Squeezer (Stereo).dll"
finalise_symbols    "vst/Squeezer (Stereo).pdb"
finalise_executable "vst/Squeezer (Mono).dll"
finalise_symbols    "vst/Squeezer (Mono).pdb"

finalise_executable "vst3/Squeezer (Stereo).vst3"
finalise_symbols    "vst3/Squeezer (Stereo).pdb"
finalise_executable "vst3/Squeezer (Mono).vst3"
finalise_symbols    "vst3/Squeezer (Mono).pdb"

finalise_executable "standalone/Squeezer (Stereo x64).exe"
finalise_symbols    "standalone/Squeezer (Stereo x64).pdb"
finalise_executable "standalone/Squeezer (Mono x64).exe"
finalise_symbols    "standalone/Squeezer (Mono x64).pdb"

finalise_executable "vst/Squeezer (Stereo x64).dll"
finalise_symbols    "vst/Squeezer (Stereo x64).pdb"
finalise_executable "vst/Squeezer (Mono x64).dll"
finalise_symbols    "vst/Squeezer (Mono x64).pdb"

finalise_executable "vst3/Squeezer (Stereo x64).vst3"
finalise_symbols    "vst3/Squeezer (Stereo x64).pdb"
finalise_executable "vst3/Squeezer (Mono x64).vst3"
finalise_symbols    "vst3/Squeezer (Mono x64).pdb"

echo "  Done."
echo
echo
