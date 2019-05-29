from managers.connection_manager import ConnectionManager


class CommandTarget(object):
    '''
    Serves as a "bag" of functions that execute client commands when called,
    e.g., from a menu system.  For ease-of-use, all the methods of this class
    should be declared static
    '''
        
    @staticmethod
    def ConnectRemoteMachine():
        return ConnectionManager.GetRemoteMachineInfo()
    
