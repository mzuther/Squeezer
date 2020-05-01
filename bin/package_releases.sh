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

version="2.5.4"

binary_dir="./final"
release_dir="releases"


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
    filename="$1"
    source_dir="$2"
    target_dir=$(dirname "/tmp/$archive_dir/$1")

    if [ ! -d "$target_dir" ]; then
        mkdir -p "$target_dir"
    fi

    if [ -f "$source_dir/$filename" ]; then
        printf "  [+] %s\n" "$filename"
        cp --dereference "$source_dir/$filename" "/tmp/$archive_dir/$1"
    elif [ -d "$source_dir/$filename" ]; then
        printf "  [+] %s/*\n" "$filename"
        cp --dereference --recursive "$source_dir/$filename/" "/tmp/$archive_dir/$1"
    else
        printf "  [ ] %s  --> not added\n" "$filename"
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

mkdir -p "./$release_dir/linux/i386"
mkdir -p "./$release_dir/linux/amd64"

mkdir -p "./$release_dir/windows/x32"
mkdir -p "./$release_dir/windows/x64"
mkdir -p "./$release_dir/windows/debug_symbols"

printf "  Done.\n\n\n"


# ----- GNU/Linux Standalone (32 bit) -----

archive_dir="squeezer-linux32-standalone_$version"

if archive_is_missing "gzip" "$release_dir/linux" ; then
    printf "  --- GNU/Linux Standalone %s (32 bit) ---\n\n" "$version"

    archive_create

    archive_add "squeezer_stereo" "$binary_dir"
    archive_add "squeezer_mono" "$binary_dir"

    archive_add "squeezer.pdf" "$binary_dir"
    archive_add "squeezer/doc" "$binary_dir"
    archive_add "squeezer/skins/Default" "$binary_dir"
    archive_add "squeezer/skins/Default.skin" "$binary_dir"

    archive_compress "gzip"
    archive_store "gzip" "$release_dir/linux"
fi


# ----- GNU/Linux VST2 (32 bit) -----

archive_dir="squeezer-linux32-vst2_$version"

if archive_is_missing "gzip" "$release_dir/linux" ; then
    printf "  --- GNU/Linux VST2 %s (32 bit) ---\n\n" "$version"

    archive_create

    archive_add "squeezer_stereo_vst2.so" "$binary_dir"
    archive_add "squeezer_mono_vst2.so" "$binary_dir"
    archive_add "squeezer_stereo_no_sidechain_vst2.so" "$binary_dir"
    archive_add "squeezer_mono_no_sidechain_vst2.so" "$binary_dir"

    archive_add "squeezer.pdf" "$binary_dir"
    archive_add "squeezer/doc" "$binary_dir"
    archive_add "squeezer/skins/Default" "$binary_dir"
    archive_add "squeezer/skins/Default.skin" "$binary_dir"

    archive_compress "gzip"
    archive_store "gzip" "$release_dir/linux"
fi


# ----- GNU/Linux Standalone (64 bit) -----

archive_dir="squeezer-linux64-standalone_$version"

if archive_is_missing "gzip" "$release_dir/linux" ; then
    printf "  --- GNU/Linux Standalone %s (64 bit) ---\n\n" "$version"

    archive_create

    archive_add "squeezer_stereo_x64" "$binary_dir"
    archive_add "squeezer_mono_x64" "$binary_dir"

    archive_add "squeezer.pdf" "$binary_dir"
    archive_add "squeezer/doc" "$binary_dir"
    archive_add "squeezer/skins/Default" "$binary_dir"
    archive_add "squeezer/skins/Default.skin" "$binary_dir"

    archive_compress "gzip"
    archive_store "gzip" "$release_dir/linux"
fi


# ----- GNU/Linux VST2 (64 bit) -----

archive_dir="squeezer-linux64-vst2_$version"

if archive_is_missing "gzip" "$release_dir/linux" ; then
    printf "  --- GNU/Linux VST2 %s (64 bit) ---\n\n" "$version"

    archive_create

    archive_add "squeezer_stereo_vst2_x64.so" "$binary_dir"
    archive_add "squeezer_mono_vst2_x64.so" "$binary_dir"
    archive_add "squeezer_stereo_no_sidechain_vst2_x64.so" "$binary_dir"
    archive_add "squeezer_mono_no_sidechain_vst2_x64.so" "$binary_dir"

    archive_add "squeezer.pdf" "$binary_dir"
    archive_add "squeezer/doc" "$binary_dir"
    archive_add "squeezer/skins/Default" "$binary_dir"
    archive_add "squeezer/skins/Default.skin" "$binary_dir"

    archive_compress "gzip"
    archive_store "gzip" "$release_dir/linux"
fi


# ----- Windows Standalone (32 bit) -----

archive_dir="squeezer-w32-standalone_$version"

if archive_is_missing "zip" "$release_dir/windows" ; then
    printf "  --- Windows Standalone %s (32 bit) ---\n\n" "$version"

    archive_create

    archive_add "Squeezer (Stereo).exe" "$binary_dir"
    archive_add "Squeezer (Mono).exe" "$binary_dir"

    archive_add "squeezer.pdf" "$binary_dir"
    archive_add "squeezer/doc" "$binary_dir"
    archive_add "squeezer/skins/Default" "$binary_dir"
    archive_add "squeezer/skins/Default.skin" "$binary_dir"

    archive_compress "zip"
    archive_store "zip" "$release_dir/windows"
fi


# ----- Windows VST2 (32 bit) -----

archive_dir="squeezer-w32-vst2_$version"

if archive_is_missing "zip" "$release_dir/windows" ; then
    printf "  --- Windows VST2 %s (32 bit) ---\n\n" "$version"

    archive_create

    archive_add "Squeezer (Stereo).dll" "$binary_dir"
    archive_add "Squeezer (Mono).dll" "$binary_dir"
    archive_add "Squeezer (Stereo, no side-chain).dll" "$binary_dir"
    archive_add "Squeezer (Mono, no side-chain).dll" "$binary_dir"

    archive_add "squeezer.pdf" "$binary_dir"
    archive_add "squeezer/doc" "$binary_dir"
    archive_add "squeezer/skins/Default" "$binary_dir"
    archive_add "squeezer/skins/Default.skin" "$binary_dir"

    archive_compress "zip"
    archive_store "zip" "$release_dir/windows"
fi


# ----- Windows VST3 (32 bit) -----

archive_dir="squeezer-w32-vst3_$version"

if archive_is_missing "zip" "$release_dir/windows" ; then
    printf "  --- Windows VST3 %s (32 bit) ---\n" "$version"
    printf "\n"

    archive_create

    archive_add "Squeezer (Stereo).vst3" "$binary_dir"
    archive_add "Squeezer (Mono).vst3" "$binary_dir"

    archive_add "squeezer.pdf" "$binary_dir"
    archive_add "squeezer/doc" "$binary_dir"
    archive_add "squeezer/skins/Default" "$binary_dir"
    archive_add "squeezer/skins/Default.skin" "$binary_dir"

    archive_compress "zip"
    archive_store "zip" "$release_dir/windows"
fi


# ----- Windows Standalone (64 bit) -----

archive_dir="squeezer-w64-standalone_$version"

if archive_is_missing "zip" "$release_dir/windows" ; then
    printf "  --- Windows Standalone %s (64 bit) ---\n\n" "$version"

    archive_create

    archive_add "Squeezer (Stereo x64).exe" "$binary_dir"
    archive_add "Squeezer (Mono x64).exe" "$binary_dir"

    archive_add "squeezer.pdf" "$binary_dir"
    archive_add "squeezer/doc" "$binary_dir"
    archive_add "squeezer/skins/Default" "$binary_dir"
    archive_add "squeezer/skins/Default.skin" "$binary_dir"

    archive_compress "zip"
    archive_store "zip" "$release_dir/windows"
fi


# ----- Windows VST2 (64 bit) -----

archive_dir="squeezer-w64-vst2_$version"

if archive_is_missing "zip" "$release_dir/windows" ; then
    printf "  --- Windows VST2 %s (64 bit) ---\n\n" "$version"

    archive_create

    archive_add "Squeezer (Stereo x64).dll" "$binary_dir"
    archive_add "Squeezer (Mono x64).dll" "$binary_dir"
    archive_add "Squeezer (Stereo, no side-chain x64).dll" "$binary_dir"
    archive_add "Squeezer (Mono, no side-chain x64).dll" "$binary_dir"

    archive_add "squeezer.pdf" "$binary_dir"
    archive_add "squeezer/doc" "$binary_dir"
    archive_add "squeezer/skins/Default" "$binary_dir"
    archive_add "squeezer/skins/Default.skin" "$binary_dir"

    archive_compress "zip"
    archive_store "zip" "$release_dir/windows"
fi


# ----- Windows VST3 (64 bit) -----

archive_dir="squeezer-w64-vst3_$version"

if archive_is_missing "zip" "$release_dir/windows" ; then
    printf "  --- Windows VST3 %s (64 bit) ---\n" "$version"
    printf "\n"

    archive_create

    archive_add "Squeezer (Stereo x64).vst3" "$binary_dir"
    archive_add "Squeezer (Mono x64).vst3" "$binary_dir"

    archive_add "squeezer.pdf" "$binary_dir"
    archive_add "squeezer/doc" "$binary_dir"
    archive_add "squeezer/skins/Default" "$binary_dir"
    archive_add "squeezer/skins/Default.skin" "$binary_dir"

    archive_compress "zip"
    archive_store "zip" "$release_dir/windows"
fi


# ----- Windows debug symbols -----

archive_dir="debug-symbols_$version"

if archive_is_missing "zip" "$release_dir/windows" ; then
    printf "  --- Windows debug symbols ---\n\n"

    archive_create

    archive_add "standalone" "$binary_dir/debug_symbols"
    archive_add "vst2" "$binary_dir/debug_symbols"
    archive_add "vst3" "$binary_dir/debug_symbols"

    archive_compress "zip"
    archive_store "zip" "$release_dir/windows"
fi

printf "  === Finishing up ===\n\n"
printf "  Done.\n\n\n"
