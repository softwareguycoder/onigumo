from common.inuyasha_symbols import ERROR_FAILED_CONNECT_TO_SERVER_FORMAT, \
    EXIT_FAILURE, ASCII_ENCODING, ERROR_FAILED_SEND_MESSAGE_FORMAT, \
    LF, MULTILINE_DATA_TERMINATOR,\
    ERROR_PORT_INVALID_VALUE
import socket


class SocketWrapper(object):
    '''
    This class wraps the commonly-used functionality of a socket.
    A context manager is also used so this object can be used in a 
    with statement.
    '''

    def __DoReceive(self):
        strReply = ''
        if not self.__clientSocket:
            return strReply
        while True:
            c = ''
            try:
                c = self.__clientSocket.recv(1).decode(ASCII_ENCODING)
                strReply += c
            except:
                pass
            if c == LF or c == '':
                break
            
        return strReply.strip()
       
    def __DoSend(self, bytesToSend):
        result = 0  # bytes sent
        if (len(bytesToSend) == 0):
            return result
        if self.__clientSocket is None:
            return result
        try:
            self.__clientSocket.sendall(bytesToSend)
            result = len(bytesToSend)
        except Exception as e:
            print(e)
            result = 0
    
        return result

    def __DoConnect(self, hostname, port):
        result = False
        if self.__clientSocket is None:
            return result
        try:
            self.__clientSocket.connect((hostname, port))
        except Exception as e:
            print(ERROR_FAILED_CONNECT_TO_SERVER_FORMAT.format(
                hostname, port, e.message()))
            result = False
        else:
            result = True
            
        return result
    
    def ReceiveAllLines(self):
        reecivedLines = []
        if not self.__clientSocket:
            return reecivedLines
        strCurLine = self.__DoReceive()
        while strCurLine != MULTILINE_DATA_TERMINATOR:
            if len(strCurLine.strip()) == 0:
                continue    # skip blank line
            reecivedLines.append(strCurLine.strip())
            strCurLine = self.__DoReceive()
                
        return reecivedLines
    
    def Receive(self):
        strReply = self.__DoReceive()
        if len(strReply.strip()) == 0:
            self.Close()
            exit(EXIT_FAILURE)
            
        return strReply
    
    def Send(self, strMessage):
        bytesSent = 0
        if len(strMessage.strip()) == 0:
            return bytesSent
        bytesSent = self.__DoSend(strMessage.encode(ASCII_ENCODING))
        if bytesSent <= 0:
            self.Close()
            print(ERROR_FAILED_SEND_MESSAGE_FORMAT.
                  format(strMessage.strip()))
            exit(EXIT_FAILURE)
            
        return bytesSent
    
    def Close(self):
        if self.__clientSocket is None:
            return
        self.__clientSocket.close()
        self.__clientSocket = None
        pass

    def Connect(self, hostname, port):
        self.__hostname = hostname
        self.__port = port
        if not len(self.__hostname.strip()):
            return False
        if self.__port < 1024 or self.__port > 49151:
            print(ERROR_PORT_INVALID_VALUE)
            return False
        result = self.__DoConnect(self.__hostname, self.__port)
        if not result:
            self.Close()
            exit(EXIT_FAILURE)
        return result

    def __init__(self):
        self.__clientSocket = \
            socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        
    def __enter__(self):
        self.Connect(self.__hostname, self.__port)
        return self
    
    def __exit__(self, exc_type, exc_value, tb):
        self.__exc_type = exc_type
        self.__exc_value = exc_value
        self.__tb = tb
        self.Close()
        return True
