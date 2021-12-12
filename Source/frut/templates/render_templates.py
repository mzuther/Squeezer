#! /usr/bin/python3

# ----------------------------------------------------------------------------
#
#  FrutJUCE
#  ========
#  Common classes for use with the JUCE library
#
#  Copyright (c) 2010-2021 Martin Zuther (http://www.mzuther.de/)
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
import platform
import stat

import jinja2

# ensure that this script can be called from anywhere
script_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(script_dir)

# root directory where the result of templates will be written to;
# joining paths one by one adds the correct path separator
output_dir = os.path.join(script_dir, '..', '..', '..')

# directory with common template settings
settings_dir = 'settings'

# template directory (must be *relative* to script directory!)
template_dir = '.'


# note: use relative paths to access templates in sub-directories
# (https://stackoverflow.com/a/9644828)
def cache_templates(template_dir, list_templates=False):
    # directories containing templates (no need to add sub-directories)
    template_dir = [template_dir]
    templateLoader = jinja2.FileSystemLoader(template_dir)
    cached_templates = jinja2.Environment(
        loader=templateLoader, trim_blocks=True)

    # list all templates in cache
    if list_templates:
        print('Templates:')
        for template_filename in cached_templates.list_templates():
            print('*', template_filename)
        print()

    return cached_templates


def render_template(cached_templates, template_filename, output_dir):
    template = cached_templates.get_template(template_filename)
    content_of_multiple_files = template.render()
    file_separator = '### File: '

    for content_of_single_file in content_of_multiple_files.split(
            file_separator):
        # content starts with file_separator, so first string is empty
        # (or contains whitespace when a template is not well written)
        if not content_of_single_file.strip():
            continue

        # extract and normalize file name
        output_filename, content = content_of_single_file.split('\n', 1)
        output_filename = os.path.abspath(
            os.path.join(output_dir, output_filename.strip()))

        print('*', os.path.relpath(output_filename, output_dir))

        # ensure Batch files use Windows line endings, otherwise
        # seemingly random lines will be executed
        if output_filename.endswith('.bat'):
            newline = '\r\n'
        # otherwise, use default line ending of system
        else:
            newline = None

        # Jinja2 encodes all strings in UTF-8
        with open(
                output_filename, mode='w', encoding='utf-8',
                newline=newline) as f:
            f.write(content)

        # make Linux shell files executable by owner
        if output_filename.endswith('.sh') and platform.system() == 'Linux':
            with pathlib.Path(output_filename) as f:
                mode = f.stat().st_mode
                f.chmod(mode | stat.S_IXUSR)


if __name__ == '__main__':
    cached_templates = cache_templates(template_dir, list_templates=False)
    template_filenames = []

    # find all Jinja2 files in template directory
    for root_dir, _, files in os.walk(template_dir):
        # do not render settings
        if root_dir == os.path.join(template_dir, settings_dir):
            continue

        for filename in files:
            if filename.endswith('.jinja'):
                template_filenames.append([root_dir, filename])

    # sort templates by location and render each one
    for root_dir, filename in sorted(template_filenames):
        template_filename = os.path.join(root_dir, filename)
        render_template(cached_templates, template_filename, output_dir)
