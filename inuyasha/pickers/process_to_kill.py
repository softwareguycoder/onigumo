from console.console_class import Console
from common.gui_utilities import Banner, PressEnterToPickProcToKill, \
    PressEnterToReturnToMainMenu, Footer
from common.inuyasha_symbols import PROTOCOL_LIST_COMMAND, \
    ERROR_FAILED_ESTABLISH_SESSION, ERROR_FAILED_CONNECT_TO_SERVER, \
    ERROR_MUST_PICK_PID
from announcers.announcer import Announcer


class ProcessToKill(object):

    @staticmethod
    def __DoListProcessesOnRemoteMachine(theSocket):
        if not theSocket:
            print(ERROR_FAILED_ESTABLISH_SESSION)
            return
        Console.Clear()
        Banner.Print()
        Announcer.AnnounceRemoteProcessList()
        pid = 0
        theSocket.Send(PROTOCOL_LIST_COMMAND)
        status = theSocket.Receive()
        if status.startswith('2'):
            lines = theSocket.ReceiveAllLines()
            for line in lines:
                if not line.strip():
                    continue
                print(line.strip())
            print()
            pid = int(input("> PID of the process to kill: > "))
        return pid
    
    @staticmethod
    def __DoShowIntroScreen():
        Console.Clear()
        Banner.Print()
        Announcer.AnnouncePickProcessToKill()
        PressEnterToPickProcToKill.Print()
        pass
    
    @staticmethod
    def __IsPidValid(pid):
        if pid < 1:
            print(ERROR_MUST_PICK_PID)
            Footer.Print()
            PressEnterToReturnToMainMenu.Print()
            return False
        return True
    
    @staticmethod
    def PickAndKillProc(theSocket, isConnected):
        if not theSocket:
            print(ERROR_FAILED_ESTABLISH_SESSION)
            return -1
        if not isConnected:
            print(ERROR_FAILED_CONNECT_TO_SERVER)
            return -1
        
        ProcessToKill.__DoShowIntroScreen()
        pid = ProcessToKill.__DoListProcessesOnRemoteMachine(theSocket)
        if not ProcessToKill.__IsPidValid(pid):
            return -1
        return pid
    
        
