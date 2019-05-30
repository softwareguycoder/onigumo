from factories.machine_info_factory import MachineInfoFactory
from validators.machine_info_validator import MachineInfoValidator
from sockets.socket_wrapper import SocketWrapper


class ConnectionManager(object):

    def GetRemoteMachineInfo(self):
        return MachineInfoFactory.Make()
    
    def ConnectRemote(self):
        mi = self.GetRemoteMachineInfo()
        while(not MachineInfoValidator.IsValid(mi, True)):
            mi = self.GetRemoteMachineInfo()
            
        self.__clientSocket = SocketWrapper(mi)
        
        
        pass
    
