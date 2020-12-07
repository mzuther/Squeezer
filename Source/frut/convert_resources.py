#!/usr/bin/env python3

#  ----------------------------------------------------------------------------
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
#  ----------------------------------------------------------------------------

import os
import subprocess
import sys


def process_directory(resource_dir, output_dir, namespace):
    header_entries = []
    source_entries = []
    ns_filename = namespace.replace('::', '_')

    # change working directory to enable addressing via relative file paths
    os.chdir(resource_dir)

    for root_directory, directories, files in os.walk(resource_dir):
        # ensure that identical files always yield the same output
        directories.sort()
        files.sort()

        for filename in files:
            # calculate path relative to resource directory
            filepath = os.path.join(root_directory, filename)
            filepath = os.path.relpath(filepath, resource_dir)

            header, source = process_file(filepath, namespace)
            header_entries.append(header)
            source_entries.append(source)

    # write binary header file
    with open(os.path.join(output_dir, ns_filename + '.h'), mode='w') as f:
        f.write('''// WARNING: this binary file was auto-generated, please do not edit!

#pragma once

namespace {}
{{
{}
}}'''.format(namespace,
             '\n'.join(header_entries).rstrip()))

    # write binary source file
    with open(os.path.join(output_dir, ns_filename + '.cpp'), mode='w') as f:
        f.write('''// WARNING: this binary file was auto-generated, please do not edit!

#include "{}.h"

{}'''.format(ns_filename,
             '\n'.join(source_entries).rstrip()))


def process_file(filepath, namespace):
    print(filepath)

    # convert binary file using "xxd"
    cmd = 'xxd -i {0}'.format(filepath)
    proc = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE, universal_newlines=True)

    [stdout, stderr] = proc.communicate()

    if stderr:
        print(stderr)
        exit(2)

    resource, resource_length, ignore_this = stdout.split(';')

    header_extern = resource.strip()
    header_extern = header_extern.split('[] =')[0]
    header_extern = header_extern.replace('unsigned char',
                                          '    extern const char* ')

    header_length = resource_length.strip()
    header_length = header_length.replace('unsigned int',
                                          '    const int          ')

    header = ';\n'.join([header_extern, header_length, ''])

    source_variable_name = resource.split('[]')[0]
    source_variable_name = source_variable_name.split(' ')[-1]

    source_temp = resource.strip()
    source_temp = 'static const ' + source_temp.replace('[] =', '_temp[] =')

    source_extern = 'const char* {}::{} = (const char*) {}_temp'.format(
        namespace, source_variable_name, source_variable_name)

    source = ';\n'.join([source_temp, source_extern, ''])

    return [header, source]


def print_usage(error_message):
    print('')
    print('Error:', error_message)
    print('')
    print('Usage: convert_resources.py SOURCE_DIR TARGET_DIR NAMESPACE')
    print('')

    exit(1)


if __name__ == '__main__':
    if len(sys.argv) != 4:
        print_usage('wrong number of arguments.')

    # enable running script from other directories; strip trailing file
    # separator (if any)
    current_dir = os.getcwd()
    resource_dir = os.path.join(current_dir, sys.argv[1].rstrip(os.path.sep))
    output_dir = os.path.join(current_dir, sys.argv[2].rstrip(os.path.sep))
    namespace = sys.argv[3]

    if not os.path.isdir(resource_dir):
        print_usage('"{}" is not a directory.'.format(resource_dir))

    if not os.path.isdir(output_dir):
        print_usage('"{}" is not a directory.'.format(output_dir))

    print('')
    print('Converting resources...')
    print('')

    print('Resource path: ', resource_dir)
    print('Output path:   ', output_dir)
    print('Namespace:     ', namespace)
    print('')

    process_directory(resource_dir, output_dir, namespace)

    print('')
    print('Done.')
    print('')
