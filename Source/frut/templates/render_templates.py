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

script_dir = os.path.dirname(os.path.abspath(__file__))
project_dir = os.path.join(script_dir, '..', '..', '..')
settings_dir = 'settings'

os.chdir(script_dir)


# note: use relative paths to access templates in sub-directories
# (https://stackoverflow.com/a/9644828)
def cache_templates(searchpath):
    searchpath = [os.path.join(script_dir, settings_dir), searchpath]
    templateLoader = jinja2.FileSystemLoader(searchpath)
    return jinja2.Environment(loader=templateLoader, trim_blocks=True)


def render_template(templates, input_file):
    output_file = os.path.abspath(os.path.join(project_dir, input_file))
    output_file = os.path.splitext(output_file)[0]

    template_file = os.path.split(input_file)[1]

    print(os.path.relpath(output_file, project_dir))

    if output_file.endswith('.bat'):
        newline = '\r\n'
    else:
        newline = None

    with open(output_file, mode='w', encoding='utf-8', newline=newline) as f:
        template = templates.get_template(template_file)
        output = template.render()

        f.write(output)

    # shell files are executable by users
    if output_file.endswith('.sh'):
        with pathlib.Path(output_file) as f:
            mode = f.stat().st_mode
            f.chmod(mode | stat.S_IEXEC)


if __name__ == '__main__':
    for root_dir, directories, files in os.walk('.'):
        if root_dir == os.path.join('.', settings_dir):
            continue

        templates = cache_templates(root_dir)

        for filename in sorted(files):
            if filename.endswith('.jinja'):
                template_file = os.path.join(root_dir, filename)
                render_template(templates, template_file)
