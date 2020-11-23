#!/usr/bin/python3

import os
import pyinotify
import subprocess


class OnWriteHandler(pyinotify.ProcessEvent):
    def __init__(self, payload):
        self.payload = payload

        # run payload directly on startup
        self.run_payload('initial run')


    def process_IN_CLOSE_WRITE(self, event):
        # ignore temporary files
        if event.name.endswith('#'):
            return

        if event.name:
            self.run_payload(os.path.join(event.path, event.name))
        else:
            self.run_payload(event.path)


    def run_payload(self, message):
        print()
        print('==>', message)
        print()

        self.payload()


def exclude_directories(path):
    return False


def payload():
    command = 'make'
    proc = subprocess.Popen(command, shell=True, universal_newlines=True)
    proc.wait()


if __name__ == '__main__':
    watchmanager = pyinotify.WatchManager()

    watchmanager.add_watch(['Original/'],
                           pyinotify.IN_CLOSE_WRITE,
                           rec=True,
                           auto_add=True,
                           exclude_filter=exclude_directories)

    watchhandler = OnWriteHandler(payload)
    watchnotifier = pyinotify.Notifier(watchmanager,
                                       default_proc_fun=watchhandler)
    watchnotifier.loop()
