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
    source_dump_entries = []
    source_getter_entries = []
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

            header, source_dump, source_getter = process_file(
                filepath, namespace)
            header_entries.append(header)
            source_dump_entries.append(source_dump)
            source_getter_entries.append(source_getter)

    # write binary header file
    with open(os.path.join(output_dir, ns_filename + '.h'), mode='w') as f:
        f.write('''// ----------------------------------------------------------------------------
//
//  WARNING: this file was auto-generated, please do not edit!
//
// ----------------------------------------------------------------------------

#pragma once

#include "FrutHeader.h"

namespace {0}
{{

{1}


bool resourceExists( const String& resourceName );
const char* getResource( const String& resourceName, int& numberOfBytes );

std::unique_ptr<Drawable> getDrawable( const String& resourceName );
String getStringUTF8( const String& resourceName );
}}
'''.format(namespace,
           '\n'.join(header_entries).rstrip()))

    # write binary source file
    with open(os.path.join(output_dir, ns_filename + '.cpp'), mode='w') as f:
        f.write('''// ----------------------------------------------------------------------------
//
//  WARNING: this file was auto-generated, please do not edit!
//
// ----------------------------------------------------------------------------

#include "{1}.h"

{2}


const char* {0}::getResource(
   const String& resourceName,
   int& numberOfBytes )
{{
{3}

   numberOfBytes = -1;
   return nullptr;
}}


bool {0}::resourceExists(
   const String& resourceName )
{{
   int numberOfBytes;
   // cppcheck-suppress unreadVariable
   auto ignore_this = getResource( resourceName, numberOfBytes );
   return numberOfBytes > 0;
}}


std::unique_ptr<Drawable> {0}::getDrawable(
   const String& resourceName )
{{
   int numberOfBytes;
   auto data = getResource( resourceName, numberOfBytes );
   return Drawable::createFromImageData( data, numberOfBytes );
}}


String {0}::getStringUTF8(
   const String& resourceName )
{{
   int numberOfBytes;
   auto data = getResource( resourceName, numberOfBytes );

   return String ( CharPointer_UTF8 ( data ), numberOfBytes );
}}
'''.format(namespace,
           ns_filename,
           '\n'.join(source_dump_entries).rstrip(),
           '\n'.join(source_getter_entries).rstrip()))


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

    resource, resource_size, ignore_this = stdout.split(';')

    # extract variable name
    variable_name = resource.split('[]')[0]
    variable_name = variable_name.split(' ')[-1]

    # extract resource size in bytes
    resource_size = resource_size.split(' ')[-1]
    resource_size = resource_size.strip(' ;')

    # re-format dumped resource
    resource_dump = resource.strip()
    resource_dump = 'static const ' + resource_dump.replace('[]', '_temp[]')
    resource_dump = '\n '.join(resource_dump.split('\n'))
    resource_dump = resource_dump.replace(' }', '}')

    # prepare entry for header file
    header = '''extern const char*  {0};
const String        {0}_name( "{1}" );
const int           {0}_len = {2};
'''.format(
        variable_name, filepath, resource_size)

    # prepare entry for source file (dump)
    source_dump = '''{0};
const char* {1}::{2} = ( const char* ) {2}_temp;
'''.format(
        resource_dump, namespace, variable_name)

    # prepare entry for source file (resource getter)
    source_getter = '''   if ( resourceName == {0}_name ) {{
      numberOfBytes = {0}_len;
      return {0};
   }}
'''.format(variable_name)

    return [header, source_dump, source_getter]


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
        print_usage('"{0}" is not a directory.'.format(resource_dir))

    if not os.path.isdir(output_dir):
        print_usage('"{0}" is not a directory.'.format(output_dir))

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
