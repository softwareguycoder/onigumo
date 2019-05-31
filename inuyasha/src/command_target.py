from managers.connection_manager import ConnectionManager
from inuyasha_symbols import EXIT_FAILURE, IDS_EXITING_PROGRAM, APP_TITLE, \
    APP_SUBTITLE, IDM_CONNECT_REMOTE_MACHINE, \
    IDS_LISTING_PROCESSES_RUNNING_ON_SERVER, IDS_PID_PROMPT, \
    IDS_PRESS_ENTER_TO_CONTINUE, ERROR_NO_PROC_ENTRIES, ERROR_FAILED_CONNECT


class CommandTarget(object):
    connmgr = None
    
    '''
    Serves as a "bag" of functions that execute client commands when called,
    e.g., from a menu system.  For ease-of-use, all the methods of this class
    should be declared static
    '''
        
    @staticmethod
    def ConnectRemoteMachine():
        print(APP_TITLE)
        print(APP_SUBTITLE)
        print()
        print(IDM_CONNECT_REMOTE_MACHINE)
        print()

        CommandTarget.connmgr = ConnectionManager()
        if CommandTarget.connmgr.ConnectRemote():
            CommandTarget.connmgr.HandshakeWithServer()
        else:
            print(ERROR_FAILED_CONNECT)
            print(IDS_EXITING_PROGRAM)
            exit(EXIT_FAILURE)
        pass
    
    @staticmethod
    def ListProcessesOnRemoteMachine():
        print(APP_TITLE)
        print(APP_SUBTITLE)
        print()
        print(IDS_LISTING_PROCESSES_RUNNING_ON_SERVER)
        print()
        returnedLines = []
        returnedLines.append(
            CommandTarget.connmgr.GetProcessesOnServer()
            )  
        if (len(returnedLines) == 0):
            print(ERROR_NO_PROC_ENTRIES)
            input(IDS_PRESS_ENTER_TO_CONTINUE)
        else:
            print(returnedLines)
            print()
            input(IDS_PID_PROMPT)
        CommandTarget.ShowMainMenu()           
        pass
            
