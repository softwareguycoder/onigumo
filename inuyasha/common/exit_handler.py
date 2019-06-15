import os
from common.inuyasha_symbols import EXIT_SUCCESS
from displayers.exit_message_displayer import ExitMessageDisplayer


class ExitHandler(object):

    @staticmethod
    def ExitApp(nExitCode=EXIT_SUCCESS, sessionObject=None, 
                exitMessageDisplayer=None):
        if sessionObject is not None:
            print("Session object detected.")
            sessionObject.End(nExitCode)
            return
        else:
            print("Session object NOT detected.")
        
        if exitMessageDisplayer is not None:
            exitMessageDisplayer()
        else:
            ExitMessageDisplayer.Display()
        os._exit(nExitCode)
