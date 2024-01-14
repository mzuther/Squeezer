#! /usr/bin/env bash

#  ----------------------------------------------------------------------------
#  
#  Squeezer
#  ========
#  Flexible general-purpose audio compressor with a touch of citrus
#  
#  Copyright (c) 2013-2024 Martin Zuther (http://www.mzuther.de/)
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

version="$(git describe --first-parent)"

binary_dir="./final"
release_dir="./releases"

resource_dir_standalone="./standalone/squeezer"
resource_dir_lv2="./lv2/squeezer"
resource_dir_vst2="./vst2/squeezer"
resource_dir_vst3="./vst3/Squeezer.vst3/Contents/Resources"


function archive_is_missing
{
    archive_type=$1
    destination_dir=$2

    if [ "$archive_type" = "bzip2" ]; then
        archive_name="$archive_dir.tar.bz2"
    elif [ "$archive_type" = "gzip" ]; then
        archive_name="$archive_dir.tar.gz"
    elif [ "$archive_type" = "zip" ]; then
        archive_name="$archive_dir.zip"
    fi

    if [ -f "$destination_dir/$archive_name" ]; then
        return 1
    else
        return 0
    fi
}


function archive_create
{
    rm -rf "/tmp/$archive_dir"

    printf "  Creating archive in \"%s\" ...\n" "/tmp/$archive_dir"
    mkdir -p "/tmp/$archive_dir"
    printf "\n"
}


function archive_add
{
    source_file="$1"
    source_file_display=$(realpath --relative-base . --no-symlinks "$source_file")
    target_dir="/tmp/$archive_dir/$2"

    if [ ! -d "$target_dir" ]; then
        mkdir -p "$target_dir"
    fi

    if [ -f "$source_file" ]; then
        printf "  [+] %s\n" "$source_file_display"
        cp --dereference "$source_file" "$target_dir"
    elif [ -d "$source_file" ]; then
        printf "  [+] %s/*\n" "$source_file_display"
        cp --dereference --recursive "$source_file/" "$target_dir"
    else
        printf "  [ ] %s  --> not added\n" "$source_file_display"
    fi
}


function archive_del
{
    filename="$1"

    if [ -f "/tmp/$archive_dir/$filename" ]; then
        printf "  [-] %s\n" "$filename"
        rm "/tmp/$archive_dir/$filename"
    elif [ -d "/tmp/$archive_dir/$filename" ]; then
        printf "  [-] %s/*\n" "$filename"
        rm -rf "/tmp/$archive_dir/$filename/"
    else
        printf "  [ ] %s  --> not deleted\n" "$filename"
    fi
}


function archive_compress
{
    archive_type=$1
    old_dir=$(pwd)

    printf "\n  Compressing archive ...\n"
    cd /tmp || exit

    if [ "$archive_type" = "bzip2" ]; then
        archive_name="$archive_dir.tar.bz2"
        rm -f "$archive_name"
        tar --create --bzip2 --verbose --file "$archive_name" "$archive_dir" > /dev/null
    elif [ "$archive_type" = "gzip" ]; then
        archive_name="$archive_dir.tar.gz"
        rm -f "$archive_name"
        tar --create --gzip --verbose --file "$archive_name" "$archive_dir" > /dev/null
    elif [ "$archive_type" = "zip" ]; then
        archive_name="$archive_dir.zip"
        rm -f "$archive_name"
        zip --recurse-paths "$archive_name" "$archive_dir" > /dev/null
    fi

    cd "$old_dir" || exit
}


function archive_store
{
    archive_type=$1
    destination_dir=$2

    if [ ! -e "$destination_dir" ]; then
        printf "  Creating folder \"%s\" ...\n" "$destination_dir"
        mkdir -p "$destination_dir"
    fi

    if [ "$archive_type" = "bzip2" ]; then
        archive_name="$archive_dir.tar.bz2"
    elif [ "$archive_type" = "gzip" ]; then
        archive_name="$archive_dir.tar.gz"
    elif [ "$archive_type" = "zip" ]; then
        archive_name="$archive_dir.zip"
    fi

    rm -rf "/tmp/$archive_dir/"

    if [ -f "$destination_dir/$archive_name" ]; then
        printf "  Overwriting"
    else
        printf "  Storing at"
    fi

    printf " \"%s\" ...\n" "$destination_dir/$archive_name"

    mv "/tmp/$archive_name" "$destination_dir/$archive_name"

    printf "  Done.\n\n\n"
}


# ----- General -----

./finalise_binaries.sh

printf "  === Creating release directories ===\n\n"

mkdir -p "$release_dir/linux/i386"
mkdir -p "$release_dir/linux/amd64"

mkdir -p "$release_dir/windows/x32"
mkdir -p "$release_dir/windows/x64"
mkdir -p "$release_dir/windows/debug_symbols"

printf "  Done.\n\n\n"


# ----- GNU/Linux Standalone (64 bit) -----

archive_dir="squeezer-linux64-standalone_$version"

if archive_is_missing "gzip" "$release_dir/linux" ; then
    printf "  --- GNU/Linux Standalone %s (64 bit) ---\n\n" "$version"

    archive_create

    archive_add "$binary_dir/standalone/squeezer_x64" ""
    archive_add "$binary_dir/standalone/squeezer_mono_x64" ""

    archive_add "$binary_dir/Documentation" ""

    archive_compress "gzip"
    archive_store "gzip" "$release_dir/$version/linux"
fi


# ----- GNU/Linux LV2 (64 bit) -----

archive_dir="squeezer-linux64-lv2_$version"

if archive_is_missing "gzip" "$release_dir/linux" ; then
    printf "  --- GNU/Linux LV2 %s (64 bit) ---\n\n" "$version"

    lv2_dir="./lv2/squeezer_lv2_x64"

    archive_create


    archive_add "$binary_dir/Documentation" ""

    archive_add "manifest.ttl" "$lv2_dir"
    archive_add "squeezer_stereo.ttl" "$lv2_dir"
    archive_add "squeezer_mono.ttl" "$lv2_dir"

    archive_compress "gzip"
    archive_store "gzip" "$release_dir/$version/linux"
fi


# ----- GNU/Linux VST2 (64 bit) -----

archive_dir="squeezer-linux64-vst2_$version"

if archive_is_missing "gzip" "$release_dir/linux" ; then
    printf "  --- GNU/Linux VST2 %s (64 bit) ---\n\n" "$version"

    archive_create

    archive_add "$binary_dir/vst2/squeezer_vst2_x64.so" ""
    archive_add "$binary_dir/vst2/squeezer_vst2_mono_x64.so" ""
    archive_add "$binary_dir/vst2/squeezer_vst2_sidechain_x64.so" ""
    archive_add "$binary_dir/vst2/squeezer_vst2_sidechain_mono_x64.so" ""

    archive_add "$binary_dir/Documentation" ""

    archive_compress "gzip"
    archive_store "gzip" "$release_dir/$version/linux"
fi


# ----- GNU/Linux VST3 (64 bit) -----

archive_dir="squeezer-linux64-vst3_$version"

if archive_is_missing "gzip" "$release_dir/linux" ; then
    printf "  --- GNU/Linux VST3 %s (64 bit) ---\n\n" "$version"

    archive_create

    archive_add "$binary_dir/vst3/Squeezer.vst3/Contents/x86_64-linux/Squeezer.so" "Squeezer.vst3/Contents/x86_64-linux"

    archive_add "$binary_dir/Documentation" ""
    archive_add "$resource_dir_vst3/Documentation" "Squeezer.vst3/Contents/Resources"

    archive_compress "gzip"
    archive_store "gzip" "$release_dir/$version/linux"
fi


# ----- Windows Standalone (32 bit) -----

archive_dir="squeezer-w32-standalone_$version"

if archive_is_missing "zip" "$release_dir/windows" ; then
    printf "  --- Windows Standalone %s (32 bit) ---\n\n" "$version"

    archive_create

    archive_add "$binary_dir/standalone/Squeezer.exe" ""
    archive_add "$binary_dir/standalone/Squeezer (mono).exe" ""

    archive_add "$binary_dir/Documentation" ""

    archive_compress "zip"
    archive_store "zip" "$release_dir/$version/windows"
fi


# ----- Windows VST2 (32 bit) -----

archive_dir="squeezer-w32-vst2_$version"

if archive_is_missing "zip" "$release_dir/windows" ; then
    printf "  --- Windows VST2 %s (32 bit) ---\n\n" "$version"

    archive_create

    archive_add "$binary_dir/vst2/Squeezer.dll" ""
    archive_add "$binary_dir/vst2/Squeezer (mono).dll" ""
    archive_add "$binary_dir/vst2/Squeezer (side-chain).dll" ""
    archive_add "$binary_dir/vst2/Squeezer (side-chain, mono).dll" ""

    archive_add "$binary_dir/Documentation" ""

    archive_compress "zip"
    archive_store "zip" "$release_dir/$version/windows"
fi


# ----- Windows VST3 (32 bit) -----

archive_dir="squeezer-w32-vst3_$version"

if archive_is_missing "zip" "$release_dir/windows" ; then
    printf "  --- Windows VST3 %s (32 bit) ---\n\n" "$version"

    archive_create

    archive_add "$binary_dir/vst3/Squeezer.vst3/Contents/x86-win/Squeezer.vst3" "Squeezer.vst3/Contents/x86-win"

    archive_add "$binary_dir/Documentation" ""
    archive_add "$resource_dir_vst3/Documentation" "Squeezer.vst3/Contents/Resources"

    archive_compress "zip"
    archive_store "zip" "$release_dir/$version/windows"
fi


# ----- Windows Standalone (64 bit) -----

archive_dir="squeezer-w64-standalone_$version"

if archive_is_missing "zip" "$release_dir/windows" ; then
    printf "  --- Windows Standalone %s (64 bit) ---\n\n" "$version"

    archive_create

    archive_add "$binary_dir/standalone/Squeezer x64.exe" ""
    archive_add "$binary_dir/standalone/Squeezer (mono) x64.exe" ""

    archive_add "$binary_dir/Documentation" ""

    archive_compress "zip"
    archive_store "zip" "$release_dir/$version/windows"
fi


# ----- Windows VST2 (64 bit) -----

archive_dir="squeezer-w64-vst2_$version"

if archive_is_missing "zip" "$release_dir/windows" ; then
    printf "  --- Windows VST2 %s (64 bit) ---\n\n" "$version"

    archive_create

    archive_add "$binary_dir/vst2/Squeezer x64.dll" ""
    archive_add "$binary_dir/vst2/Squeezer (mono) x64.dll" ""
    archive_add "$binary_dir/vst2/Squeezer (side-chain) x64.dll" ""
    archive_add "$binary_dir/vst2/Squeezer (side-chain, mono) x64.dll" ""

    archive_add "$binary_dir/Documentation" ""

    archive_compress "zip"
    archive_store "zip" "$release_dir/$version/windows"
fi


# ----- Windows VST3 (64 bit) -----

archive_dir="squeezer-w64-vst3_$version"

if archive_is_missing "zip" "$release_dir/windows" ; then
    printf "  --- Windows VST3 %s (64 bit) ---\n\n" "$version"

    archive_create

    archive_add "$binary_dir/vst3/Squeezer.vst3/Contents/x86_64-win/Squeezer.vst3" "Squeezer.vst3/Contents/x86_64-win"

    archive_add "$binary_dir/Documentation" ""
    archive_add "$resource_dir_vst3/Documentation" "Squeezer.vst3/Contents/Resources"

    archive_compress "zip"
    archive_store "zip" "$release_dir/$version/windows"
fi


# ----- Windows debug symbols -----

archive_dir="debug-symbols_$version"

if archive_is_missing "zip" "$release_dir/windows" ; then
    printf "  --- Windows debug symbols ---\n\n"

    archive_create

    archive_add "$binary_dir/debug_symbols/standalone" ""
    archive_add "$binary_dir/debug_symbols/vst2" ""
    archive_add "$binary_dir/debug_symbols/vst3" ""

    archive_compress "zip"
    archive_store "zip" "$release_dir/$version/windows/debug_symbols"
fi

printf "  === Finishing up ===\n\n"
printf "  Done.\n\n\n"
