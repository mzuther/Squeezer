#!/usr/bin/env python3

# ----------------------------------------------------------------------------
#
#  DirWalk
#  =======
#  Custom directory walker
#
#  Copyright (c) 2022-2024 Martin Zuther (https://www.mzuther.de/)
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#
#  2. Redistributions in binary form must reproduce the above
#     copyright notice, this list of conditions and the following
#     disclaimer in the documentation and/or other materials provided
#     with the distribution.
#
#  3. Neither the name of the copyright holder nor the names of its
#     contributors may be used to endorse or promote products derived
#     from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
#  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
#  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
#  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
#  OF THE POSSIBILITY OF SUCH DAMAGE.
#
#  Thank you for using free software!
#
# ----------------------------------------------------------------------------

import math
import os
import pathlib
import sys


def is_directory_included(current_path, dir_entry, follow_symlinks,
                          included, modified_after):
    if not dir_entry.is_dir(follow_symlinks=follow_symlinks):
        return False

    # exclude directories
    if current_path.name in included.get('excluded_directory_names', []):
        return False

    return True


def is_file_included(current_path, dir_entry, follow_symlinks,
                     included, modified_after):
    if not dir_entry.is_file(follow_symlinks=follow_symlinks):
        return False

    # exclude files
    if current_path.name in included.get('excluded_file_names', []):
        return False

    # only include some file suffixes
    for suffix in included.get('included_suffixes', []):
        if current_path.match(suffix):
            break
    else:
        return False

    # "stat" is costly
    if not modified_after:
        return True

    # only include files modified after a given date; get timestamp of linked
    # file, not of symlink
    stat_result = dir_entry.stat(follow_symlinks=True)

    # "st_mtime_ns" gets the exact timestamp, although nanoseconds may be
    # missing or inexact
    modification_time_in_seconds = stat_result.st_mtime_ns / 1e9

    # round up to ensure that files with inaccurate timestamps and other edge
    # cases are included
    modification_time_in_seconds = math.ceil(modification_time_in_seconds)

    return modification_time_in_seconds >= modified_after


def dirwalk_prepare(root_directory, included, modified_after):
    root_directory = pathlib.Path(root_directory)

    if not included:
        included = {}

    # UNIX timestamp, remove digital places after period
    if modified_after:
        modified_after = int(modified_after)

    return (root_directory, included, modified_after)


def dirwalk(root_directory, directories_first=True, include_directories=False,
            follow_symlinks=False, included=None, modified_after=None):
    root_directory, included, modified_after = dirwalk_prepare(
        root_directory, included, modified_after)

    directories, files = dirwalk_process(root_directory, directories_first,
                                         include_directories, follow_symlinks,
                                         included, modified_after)

    # sort results
    directories.sort()
    files.sort()

    # collect results
    found_items = []

    if not directories_first:
        found_items.extend(files)

    # recurse
    for current_directory in directories:
        deep_found_items = dirwalk(current_directory, directories_first,
                                   include_directories, follow_symlinks,
                                   included, modified_after)

        if include_directories:
            found_items.append(current_directory + os.sep)

        found_items.extend(deep_found_items)

    if directories_first:
        found_items.extend(files)

    return found_items


def dirwalk_process(root_directory, directories_first,
                    include_directories, follow_symlinks,
                    included, modified_after):
    directories = []
    files = []

    # "os.scandir" minimizes system calls (including the retrieval of
    # timestamps)
    for dir_entry in os.scandir(root_directory):
        current_path = root_directory / dir_entry.name

        # process directories
        if is_directory_included(current_path, dir_entry, follow_symlinks,
                                 included, modified_after):
            directories.append(current_path)
        # process files
        elif is_file_included(current_path, dir_entry, follow_symlinks,
                              included, modified_after):
            files.append(current_path)

    return directories, files


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print()
        print('MONSTER:   ME WANT EAT DIRECTORIES.  PLEASE SHOW PLACE.  '
              'THEN ME START EAT.')
        print()
        print('engineer:  please provide the root directory as first '
              'parameter.')
        print()

        exit(1)

    SOURCE_DIR = sys.argv[1]

    INCLUSIONS = {
        'excluded_directory_names': [
        ],
        'excluded_file_names': [
        ],
        'included_suffixes': [
        ],
    }

    MODIFIED_AFTER = None

    # import datetime
    # MODIFIED_AFTER = datetime.datetime(2022, 12, 1).timestamp()

    for current_path_name in dirwalk(
            SOURCE_DIR,
            included=INCLUSIONS,
            modified_after=MODIFIED_AFTER):
        print(current_path_name)
