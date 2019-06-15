import os
from common.inuyasha_symbols import EXIT_SUCCESS
from displayers.exit_message_displayer import ExitMessageDisplayer


class ExitHandler(object):

    @staticmethod
    def ExitApp(nExitCode=EXIT_SUCCESS, sessionObject=None, 
                exitMessageDisplayer=None):
        if sessionObject is not None:
            sessionObject.End(nExitCode)
            return
        else:       
            if exitMessageDisplayer is not None:
                exitMessageDisplayer()
            else:
                ExitMessageDisplayer.Display()
            os._exit(nExitCode)
        pass
