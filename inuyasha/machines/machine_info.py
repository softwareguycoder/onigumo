class MachineInfo(object):
    
    def GetHostName(self):
        return self.__hostname
    
    def GetPort(self):
        return self.__port
    
    def __init__(self, hostname, port):
        self.__hostname = hostname
        self.__port = port
