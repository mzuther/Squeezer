#!/usr/bin/python3

import os
import pyinotify
import subprocess


class OnWriteHandler(pyinotify.ProcessEvent):
    def __init__(self, payload_command):
        print()
        print('==> initial run')

        # initialise payload command
        self.payload_command = payload_command

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

        # run payload
        self.run_payload()


    def run_payload(self):
        # run payload
        proc = subprocess.Popen(self.payload_command,
                                shell=True,
                                stdin=subprocess.PIPE,
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE,
                                universal_newlines=True)

        # display output of "stdout" and "stderr" (if any)
        for pipe_output in proc.communicate():
            if pipe_output:
                print()
                print(pipe_output.strip())

        print()
        print('Done.')
        print()


# define directories to be ignored
def exclude_directories(path):
    if path.startswith('../Source/api'):
        return True
    else:
        return False


if __name__ == '__main__':
    # directory that should be monitored
    directory_to_watch = '../Source'

    # command to be run on payload.  "unbuffer" pretends a TTY, thus
    # keeping escape sequences
    payload_command = 'unbuffer doxygen Doxyfile'

    # create an instance of "pyinotify"
    watchmanager = pyinotify.WatchManager()

    # add a watch (will trigger on completed writes)
    watchmanager.add_watch(directory_to_watch,
                           pyinotify.IN_CLOSE_WRITE,
                           rec=True,
                           auto_add=True,
                           exclude_filter=exclude_directories)

    # add watch for "Doxyfile" (will trigger on completed writes)
    watchmanager.add_watch('Doxyfile',
                           pyinotify.IN_CLOSE_WRITE)

    # create an instance of our file processor
    watchhandler = OnWriteHandler(payload_command)

    # connect to our file processor to "pyinotify"
    watchnotifier = pyinotify.Notifier(watchmanager,
                                       default_proc_fun=watchhandler)

    # keep monitoring
    watchnotifier.loop()
