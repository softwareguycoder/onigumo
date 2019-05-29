from managers.connection_manager import ConnectionManager
from inuyasha_symbols import *  # @UnusedWildImport


class CommandTarget(object):
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

        mgr = ConnectionManager()
        mgr.ConnectRemote()
            