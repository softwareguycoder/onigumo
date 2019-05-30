from socket import socket
from validators.machine_info_validator import MachineInfoValidator
from inuyasha_symbols import MULTILINE_DATA_TERMINATOR, LF


class SocketWrapper:
      
    def Close(self):
        try:
            if (not self.__theSocket._closed):
                self.__theSocket.close()
        except:
            pass

    def Send(self, pszMessage):
        bytesSent = 0
        if (self.__theSocket._closed):
            return bytesSent
        if (len(pszMessage) == 0):
            return bytesSent
        try:
            if (self.__theSocket.sendall(pszMessage) is None):
                bytesSent = len(pszMessage)
        except:
            bytesSent = 0
        return bytesSent
        
    def ReceiveLine(self):
        result = ''
        if (self.__theSocket._closed):
            return result
        while True:
            c = ''
            try:
                c = self.__theSocket.recv(1)
                result += c
            except:
                pass
            if c == LF or c == '':
                break
        return result

    def ReceiveAllLines(self):
        result = []
        if (self.__theSocket._closed):
            return result
        curline = self.__ReceiveLine()
        
        while (curline != MULTILINE_DATA_TERMINATOR):
                result.append(curline)
                curline = self.__ReceiveLine()
                
        return result
    
    def __Connect(self):
        self.Close()
        if (not MachineInfoValidator.IsValid(self.__machineInfo,
            show_errors=False)):
            return
        if (self.__theSocket is None):
            return
        
        try:
            self.__theSocket.connect((self.__machineInfo.szHostName,
                self.__machineInfo.nPort))
        except:
            print("ERROR: Failed to connect to server '{}' on port {}."\
                  .format(self.__machineInfo.szHostName,
                          self.__machineInfo.nPort))
        finally:
            self.Close()
        pass
    
    def __CreateSocket(self):
        try:
            self.__theSocket = socket.socket(socket.AF_INET,
                                             socket.SOCK_STREAM)
        except:
            print("ERROR: Failed to create TCP endpoint.")
        finally:
            self.__theSocket = None
        pass
    
    def __init__(self, machineInfo):
        if (not MachineInfoValidator.IsValid(machineInfo, show_errors=False)):
            return
        
        self.__machineInfo = machineInfo
        self.__CreateSocket()
        
        self.__Connect()
        pass

    def __del__(self):
        self.Close()
        pass
