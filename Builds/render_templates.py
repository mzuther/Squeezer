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

import jinja2


def cache_templates(searchpath):
    templateLoader = jinja2.FileSystemLoader(searchpath=searchpath)
    return jinja2.Environment(loader=templateLoader, trim_blocks=True)


def render_template(templates, template_file, output_file):
    print('{:30s} --> {}'.format(template_file, output_file))

    with open(output_file, 'w') as f:
        template = templates.get_template(template_file)
        output = template.render()

        f.write(output)


if __name__ == '__main__':
    templates = cache_templates('../templates/')

    render_template(
        templates,
        'premake5.template',
        './premake5.lua')

    render_template(
        templates,
        'finalise_executables.template',
        '../bin/finalise_executables.sh')
