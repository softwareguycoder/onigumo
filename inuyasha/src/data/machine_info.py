#!/usr/bin/python3

class MachineInfo:

    def __str__(self):
        return "MachineInfo: szHostName = '{}', nPort = {}" \
            .format(self.szHostName, self.nPort)
    
    def __init__(self, pszHostName, nPort):
        '''
        Constructs a new instance of MachineInfo
        '''
        self.szHostName = pszHostName
        self.nPort = nPort