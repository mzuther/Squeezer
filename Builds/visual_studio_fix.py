#! /usr/bin/env python3

# ----------------------------------------------------------------------------
#
#  K-Meter
#  =======
#  Implementation of a K-System meter according to Bob Katz' specifications
#
#  Copyright (c) 2010-2016 Martin Zuther (http://www.mzuther.de/)
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

import os
import sys


assert sys.version_info.major == 3, ('Python {0} is not supported, '
                                     'please use Python 3.'.format(
                                     sys.version_info.major))


def change_vcxproj(filename):
    print('Processing {filename}...'.format(**locals()))

    with open(filename, 'rt') as infile:
        contents = infile.read()

    start = contents.find('<PropertyGroup Label="Globals">')
    end = contents.find('</PropertyGroup>', start)

    insert = '\t<PlatformToolset>{0}</PlatformToolset>\n\t'.format(toolset)
    contents = insert.join((contents[:end], contents[end:]))

    with open(filename, 'wt') as outfile:
        outfile.write(contents)


assert len(sys.argv) == 2, 'Usage: visual_studio.py toolset'
toolset = sys.argv[1]

print('\nChanging project toolsets to "{0}"...'.format(toolset))
for path in ('windows/vs2010', ):
    for filename in os.listdir(path):
        (root, ext) = os.path.splitext(filename)
        if ext == '.vcxproj':
            change_vcxproj(os.path.join(path, filename))
print('Done.')
