#!/usr/bin/python3

import os
import pyinotify
import subprocess


input_file = 'Default.svg'


class OnWriteHandler(pyinotify.ProcessEvent):
    def __init__(self, payload):
        print()
        print('==> initial run')
        print()

        # initialise payload
        self.payload = payload

        # run payload once on startup
        self.run_payload()


    # is called on completed writes
    def process_IN_CLOSE_WRITE(self, event):
        # ignore temporary files
        if event.name.endswith('#'):
            return
        # ignore compiled Python files
        elif event.name.endswith('.pyc'):
            return

        # get name of changed file
        if event.name:
            filename = os.path.join(event.path, event.name)
        else:
            filename = event.path

        # print name of changed file
        print('==> ' + filename)
        print()

        # run payload
        self.run_payload()


    def run_payload(self):
        # run payload
        self.payload()


# define directories to be ignored
def exclude_directories(path):
    if path.startswith('../doc'):
        return True
    else:
        return False


def payload():
    # get component IDs for export
    command = 'inkscape --query-all "{}" | grep -E "^export-"'.format(
        input_file)

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

        output_file = component_id + '.svg'
        output_file = output_file.replace('export-', '', 1)
        output_file = output_file.replace('-', '_')
        output_file = output_file.replace('_', os.path.sep, 1)

        print('  component:  {}'.format(
            component_id))
        print('  file name:  {} '.format(
            output_file))
        print('  position:   {} | {}'.format(
            x, y))
        print('  size:       {} x {}'.format(
            width, height))
        print()

        inkscape_action = [
            'file-open:{}'.format(input_file),
            'export-filename:{}'.format(output_file),
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
    # create an instance of "pyinotify"
    watchmanager = pyinotify.WatchManager()

    # add a watch (will trigger on completed writes)
    watchmanager.add_watch(input_file,
                           pyinotify.IN_CLOSE_WRITE,
                           rec=True,
                           auto_add=True,
                           exclude_filter=exclude_directories)

    # create an instance of our file processor
    watchhandler = OnWriteHandler(payload)

    # connect to our file processor to "pyinotify"
    watchnotifier = pyinotify.Notifier(watchmanager,
                                       default_proc_fun=watchhandler)

    # keep monitoring
    watchnotifier.loop()
