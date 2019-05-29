from factories.machine_info_factory import MachineInfoFactory


class ConnectionManager(object):
    '''
    classdocs
    '''
    
    @staticmethod
    def GetRemoteMachineInfo():
        return MachineInfoFactory.make()
