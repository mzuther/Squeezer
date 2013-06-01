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
