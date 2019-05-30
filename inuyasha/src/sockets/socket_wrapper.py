from socket import *  # @UnusedWildImport
from validators.machine_info_validator import MachineInfoValidator
from inuyasha_symbols import MULTILINE_DATA_TERMINATOR, LF
from common.string_utils import StringUtils

class SocketWrapper:

    def Send(self, pszMessage):
        result = 0
        if (len(pszMessage) == 0):
            return result
        if (self.__theSocket.sendall(pszMessage) is None):
            result = len(pszMessage)
        return result
        
    def __ReceiveLine(self):
        result = ''
        while True:
            c = self.__theSocket.recv(1)
            result += c
            if c == LF or c == '':
                break

        return result

    def __ReceiveAllLines(self):
        result = []
        curline = self.__ReceiveLine()
        
        while (curline != MULTILINE_DATA_TERMINATOR):
                result.append(curline)
                curline = self.__ReceiveLine()
                
        return result
    
    def __Connect(self):
        if (not MachineInfoValidator.IsValid(self.__machineInfo,
            show_errors=False)):
            return
        if (self.__theSocket is None):
            return
        
        self.__theSocket.connect((self.__machineInfo.szHostName,
            self.__machineInfo.nPort))
    
    def __CreateSocket(self):
        try:
            self.__theSocket = socket.socket(socket.AF_INET,
                                             socket.SOCK_STREAM)
        except:
            print("ERROR: Failed to create TCP endpoint.")
        finally:
            self.__theSocket = None
    
    def __init__(self, machineInfo):
        if (not MachineInfoValidator.IsValid(machineInfo, show_errors=False)):
            return
        
        self.__machineInfo = machineInfo
        self.__CreateSocket()
        
        self.__Connect()
        
