import os
from common.inuyasha_symbols import EXIT_SUCCESS
from displayers.exit_message_displayer import ExitMessageDisplayer
from sessions.session_global import SessionGlobal


class ExitHandler(object):

    @staticmethod
    def ExitApp(nExitCode=EXIT_SUCCESS, sessionObject=None, 
                exitMessageDisplayer=None):
        if  sessionObject is not None:
            sessionObject.End(nExitCode)
        elif SessionGlobal.GetSession() is not None:
            SessionGlobal.GetSession().End(nExitCode)
   
        if exitMessageDisplayer is not None:
            exitMessageDisplayer()
        else:
            ExitMessageDisplayer.Display()
        os._exit(nExitCode)
        exit()
        quit()
