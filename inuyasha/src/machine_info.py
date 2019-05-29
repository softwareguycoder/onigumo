#!/usr/bin/python3
from _ctypes import ArgumentError


class MachineInfo:

    def isPortValid(self, nPort):
        '''
        Returns TRUE if the port number passed in nPort is a valid TCP
        port.  Valid port numbers lie in the range [1024, 49151) and are
        integers.
        '''
        return nPort >= 1024 and nPort < 49151;
    
    def __str__(self):
        return "MachineInfo: szHostName = '{}', nPort = {}" \
            .format(self.szHostName, self.nPort)
    
    def __init__(self, pszHostName, nPort):
        '''
        Constructs a new instance of MachineInfo
        '''
        self.szHostName = pszHostName
        self.nPort = nPort
        
        if (not self.isPortValid(nPort)):
            raise ArgumentError("A valid port number is 1024 - 49150.  Try again.")
