#!/usr/bin/python3

import sys
import os
import subprocess


def inkscape2skin(input_filename, output_directory):
    print('-' * 80)
    print('')
    print('  Creating SVG files from "{}"'.format(input_filename))
    print('')
    print('-' * 80)
    print('')

    # get component IDs for export
    command = 'inkscape --query-all "{}" | grep -E "^export-"'.format(
        input_filename)

    proc = subprocess.Popen(command,
                            shell=True,
                            stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE,
                            universal_newlines=True)

    stdout = proc.communicate()[0]
    components = stdout.strip().split()

    inkscape_actions = []

    for component in sorted(components):
        component_id, x, y, width, height = component.split(',')

        x = float(x)
        y = float(y)
        width = float(width)
        height = float(height)

        output_filename = component_id + '.svg'
        output_filename = output_filename.replace('export-', '', 1)
        output_filename = output_filename.replace('-', '_')
        output_filename = output_filename.replace('_', os.path.sep, 1)
        output_filename = os.path.join(output_directory, output_filename)

        print('  {} --> {}  ({} | {}, {} x {})'.format(
            component_id, output_filename, x, y, width, height))

        if int(x) != x:
            print('  *** x is not an integer')

        if int(y) != y:
            print('  *** y is not an integer')

        if int(width) != width:
            print('  *** width is not an integer')

        if int(height) != height:
            print('  *** height is not an integer')

        inkscape_action = [
            'file-open:{}'.format(input_filename),
            'export-filename:{}'.format(output_filename),
            'export-id:{}'.format(component_id),
            'export-id-only',
            'export-area-page',
            'export-plain-svg',
            'export-text-to-path',
            'vacuum-defs',
            'export-do'
        ]

        inkscape_action = '; '.join(inkscape_action)
        inkscape_actions.append(inkscape_action)

    print()
    print('  Exporting SVG documents ...')

    # assemble actions
    inkscape_actions = '; '.join(inkscape_actions)

    command = 'inkscape --actions="{}"'.format(
        inkscape_actions)

    proc = subprocess.Popen(command, shell=True, universal_newlines=True)
    proc.wait()

    print('  Done.')
    print('')


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print('')
        print('  Usage: export_svg infile outdir')
        print('')

        exit(1)

    input_filename = sys.argv[1]
    output_directory = sys.argv[2]

    if not os.path.isfile(input_filename):
        print('')
        print('  Error: file "{}" not found'.format(input_filename))
        print('')

        exit(2)

    if not os.path.isdir(output_directory):
        print('')
        print('  Error: directory "{}" not found'.format(output_directory))
        print('')

        exit(2)

    inkscape2skin(input_filename, output_directory)
