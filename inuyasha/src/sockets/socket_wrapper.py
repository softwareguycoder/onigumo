import socket
from validators.machine_info_validator import MachineInfoValidator
from inuyasha_symbols import MULTILINE_DATA_TERMINATOR, LF,\
    ERROR_FAILED_CREATE_SOCKET, EXIT_FAILURE


class SocketWrapper:
      
    def Close(self):
        try:
            if (not self.__theSocket._closed):
                self.__theSocket.close()
                self.connected = False
        except:
            pass

    def Send(self, pszMessage):
        print("In SocketWrapper.Send")
        print("SocketWrapper.Send: pszMessage = '{}'".format(pszMessage.strip()))
        bytesSent = 0
        print("SocketWrapper.Send: bytesSent = {}".format(bytesSent))
        print("SocketWrapper.Send: Checking whether the socket is closed...")
        if (self.__theSocket._closed):
            print("ERROR: Socket is closed.")
            exit(EXIT_FAILURE)
            return bytesSent
        print("SocketWrapper.Send: Checking whether the message is blank...")
        if (len(pszMessage) == 0):
            print("ERROR: Message is blank.")
            exit(EXIT_FAILURE)
            return bytesSent
        print("SocketWrapper.Send: Trying to send the message...")
        try:
            print("SocketWrapper.Send: Calling sendall...")
            self.__theSocket.sendall(pszMessage.encode())
            bytesSent = len(pszMessage)
            print("SocketWrapper.Send: {} bytes sent.".format(bytesSent))
        except Exception as e:
            print("SocketWrapper.Send: {}".format(e))
            bytesSent = 0
        print("SocketWrapper.Send: Done.")
        return bytesSent
        
    def ReceiveLine(self):
        print("In SocketWrapper.ReceiveLine")
        result = ''
        print("SocketWrapper.ReceiveLine: Checking whether the socket is closed...") 
        if (self.__theSocket._closed):
            print("SocketWrapper.ReceiveLine: ERROR: The socket is closed.")
            return result
        print("SocketWrapper.ReceiveLine: The socket is not closed.")
        while True:
            c = ''
            try:
                print("SocketWrapper.ReceiveLine: trying to receive a char...")
                c = self.__theSocket.recv(1)
                result += c
            except:
                pass
            if c == LF or c == '':
                break
            
        print("SocketWrapper.ReceiveLine: result '{}'".format(result.strip()))
        return result

    def ReceiveAllLines(self):
        result = []
        if (self.__theSocket._closed):
            return result
        curline = self.ReceiveLine()
        
        while (curline != MULTILINE_DATA_TERMINATOR):
                result.append(curline)
                curline = self.ReceiveLine()
                
        return result
    
    def __Connect(self):
        self.connected = False
        if (not MachineInfoValidator.IsValid(self.__machineInfo,
            show_errors=False)):
            return
        if (self.__theSocket is None):
            return
                     
        try:
            self.__theSocket.connect((self.__machineInfo.szHostName,
                self.__machineInfo.nPort))
            self.connected = True
        except:
            print("ERROR: Failed to connect to server '{}' on port {}."\
                  .format(self.__machineInfo.szHostName,
                          self.__machineInfo.nPort))
            self.connected = False
            self.Close()           
        pass
    
    def __CreateSocket(self):
        try:
            self.__theSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except:
            print(ERROR_FAILED_CREATE_SOCKET)
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
