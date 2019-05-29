from factories.machine_info_factory import MachineInfoFactory
from validators.machine_info_validator import MachineInfoValidator


class ConnectionManager(object):

    def GetRemoteMachineInfo(self):
        return MachineInfoFactory.Make()
    
    def ConnectRemote(self):
        mi = self.GetRemoteMachineInfo()
        while(not MachineInfoValidator.IsValid(mi, True)):
            mi = self.GetRemoteMachineInfo()
        pass
    
