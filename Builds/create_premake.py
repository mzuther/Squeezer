#! /usr/bin/python3

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

import sys
import jinja2


windows_sdk = sys.argv[1]

templateLoader = jinja2.FileSystemLoader(searchpath='.')
templateEnv = jinja2.Environment(loader=templateLoader, trim_blocks=True)
template = templateEnv.get_template('layout.template')

filename = 'premake5.lua'
output = template.render(windows_sdk=windows_sdk)

print()
print('=== Generating premake script ===')
print('Rendering file \'{}\'...'.format(filename))

with open(filename, 'w') as f:
    f.write(output)

print('Done.')
