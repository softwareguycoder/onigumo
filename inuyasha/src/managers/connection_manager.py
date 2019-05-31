from factories.machine_info_factory import MachineInfoFactory
from validators.machine_info_validator import MachineInfoValidator
from sockets.socket_wrapper import SocketWrapper
from common.string_utils import StringUtils
from inuyasha_symbols import HELO_OK_STATUS_CODE, IDS_SESSION_STARTED_OK,\
    LIST_OK_STATUS_CODE, PROTOCOL_HELO_COMMAND, PROTOCOL_LIST_COMMAND,\
    IDS_SERVER_RETURNED_ERROR_RESPONSE, EXIT_FAILURE, PROTOCOL_QUIT_COMMAND


class ConnectionManager(object):
    get_Socket = None
    ReceivingProcList = False
    
    def __Send(self, pszCommand):
        print("In ConnectionManager.__Send")
        print("ConnectionManager.__Send: Checking whether the socket is valid...")
        if (ConnectionManager.get_Socket is None):
            print("ConnectionManager.__Send: ERROR: Invalid socket.")
            exit(EXIT_FAILURE)
            return
        print("ConnectionManager.__Send: Socket is valid.")
        print("ConnectionManager.__Send: pszCommand = '{}'".format(pszCommand.strip()))
        print("ConnectionManager.__Send: Sending through the socket...")
        ConnectionManager.get_Socket.Send(pszCommand)
        print("ConnectionManager.__Send: Command sent.")        
    
    def EndSession(self):
        if (not ConnectionManager.get_Socket.connected):
            return
        self.__Send(PROTOCOL_QUIT_COMMAND)
        self.HandleServerRespone()
        ConnectionManager.get_Socket.Close()
        ConnectionManager.get_Socket = None
        pass
    
    def HandshakeWithServer(self):
        print("In ConnectionManager.HandshakeWithServer")
        print("ConnectionManager.HandshakeWithServer: ConnectionManager.get_Socket.connected = {}".format(ConnectionManager.get_Socket.connected))
        print("ConnectionManager.HandshakeWithServer: Checking whether the socket is connected...")
        if (not ConnectionManager.get_Socket.connected):
            print("ConnectionManager.HandshakeWithServer: The socket is not connected.")
            return
        print("ConnectionManager.HandshakeWithServer: Sending the HELO command...")
        self.__Send(PROTOCOL_HELO_COMMAND)
        print("ConnectionManager.HandshakeWithServer: HELO command sent.")
        print("ConnectionManager.HandshakeWithServer: Handling the server's response...")
        self.HandleServerRespone()
        print("ConnectionManager.HandshakeWithServer: Server responded.")
        pass
    
    def GetProcessesOnServer(self):
        returnedLines = []
        self.__Send(PROTOCOL_LIST_COMMAND)
        if (not self.HandleServerRespone()):
            return returnedLines
        returnedLines = \
            ConnectionManager.get_Socket.ReceiveAllLines()
        return returnedLines

    def HandleServerRespone(self):
        print("In ConnectionManager.HandleServerResponse")
        handled = False
        
        response = ConnectionManager.get_Socket.ReceiveLine()
        if (StringUtils.IsNullOrWhiteSpace(response)):
            return handled
        elif (response.startswith(HELO_OK_STATUS_CODE)):
            print(IDS_SESSION_STARTED_OK)
            handled = True
        elif (response.startswith(LIST_OK_STATUS_CODE)):
            handled = True
        else:
            print(IDS_SERVER_RETURNED_ERROR_RESPONSE)
            handled = False
            exit(EXIT_FAILURE)
        return handled

    def GetRemoteMachineInfo(self):
        return MachineInfoFactory.Make()
    
    def ConnectRemote(self):
        succeeded = False
        mi = self.GetRemoteMachineInfo()
        while(not MachineInfoValidator.IsValid(mi, True)):
            mi = self.GetRemoteMachineInfo()
        
        ConnectionManager.get_Socket = SocketWrapper(mi)
        succeeded = ConnectionManager.get_Socket.connected
        return succeeded        
    
