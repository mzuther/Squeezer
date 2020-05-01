#! /usr/bin/python3

# ----------------------------------------------------------------------------
#
#  FrutJUCE
#  ========
#  Common classes for use with the JUCE library
#
#  Copyright (c) 2010-2020 Martin Zuther (http://www.mzuther.de/)
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
import pathlib
import stat

import jinja2


script_path = os.path.dirname(os.path.abspath(__file__))
os.chdir(script_path)


def cache_templates(searchpath):
    templateLoader = jinja2.FileSystemLoader(searchpath=searchpath)
    return jinja2.Environment(loader=templateLoader, trim_blocks=True)


def render_template(templates, template_file, output_file):
    if output_file.endswith('.bat'):
        newline = '\r\n'
    else:
        newline = None

    print('{:30s} --> {}'.format(template_file, output_file))

    with open(output_file, mode='w', newline=newline) as f:
        template = templates.get_template(template_file)
        output = template.render()

        f.write(output)

    # shell files are executable by users
    if output_file.endswith('.sh'):
        with pathlib.Path(output_file) as f:
            mode = f.stat().st_mode
            f.chmod(mode | stat.S_IEXEC)


if __name__ == '__main__':
    templates = cache_templates('.')

    render_template(
        templates,
        'premake5.template',
        '../../../Builds/premake5.lua')

    render_template(
        templates,
        'build_sh.template',
        '../../../Builds/build.sh')

    render_template(
        templates,
        'copy_vst.template',
        '../../../bin/copy_vst.bat')

    render_template(
        templates,
        'finalise_binaries.template',
        '../../../bin/finalise_binaries.sh')

    render_template(
        templates,
        'package_releases.template',
        '../../../bin/package_releases.sh')

    render_template(
        templates,
        'link_vst.template',
        '../../../bin/link_vst.sh')
