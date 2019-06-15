from sockets.socket_wrapper import SocketWrapper
from console.console_class import Console
from factories.machine_info_factory import MachineInfoFactory
from common.inuyasha_symbols import \
    EXIT_FAILURE, PROTOCOL_HELO_COMMAND, ERROR_FAILED_ESTABLISH_SESSION, \
    PROTOCOL_QUIT_COMMAND, ERROR_FAILED_CONNECT_TO_SERVER
from announcers.announcer import Announcer
from info_getters.cpu_info_getter import CpuInfo
from info_getters.dir_listing import DirListing
from pickers.process_to_kill_picker import ProcessToKillPicker
from senders.shellcode_sender import ShellcodeSender
from executers.shellcode_executer import ShellcodeExecuter
from common.footer import Footer
from common.press_enter_to_return_to_main_menu import PressEnterToReturnToMainMenu
from common.thank_you_message_printer import ThankYouMessagePrinter
from common.blank_line_printer import BlankLinePrinter
from common.banner import Banner
from common.welcome_printer import WelcomePrinter
from common.wait_for_enter_to_continue import WaitForEnterToContinue


class Session(object):
       
    def IsConnected(self):
        return self.__connected
    
    def IsValid(self):
        return self.__socket and self.__connected
   
    def GetCpuInfo(self):
        if not self.IsValid():
            print("wah-wah-WAAAAHHHHH-10")
            print(ERROR_FAILED_ESTABLISH_SESSION)
            Footer.Print()
            PressEnterToReturnToMainMenu.Print()

        CpuInfo.Get(self.GetSocket(), self.IsConnected())
        pass
    
    def GetDirListing(self):
        if not self.IsValid():
            print("Calling Session.End in GetDirListing")
            self.End(EXIT_FAILURE)

        DirListing.Get(self.GetSocket(), self.IsConnected())
        
    def GetSocket(self):
        return self.__socket
    
    def KillSpecifiedProcess(self):
        pid = ProcessToKillPicker.PickAndKillProc(
            self.GetSocket(), self.IsConnected())
        if pid <= 0:
            Announcer.AnnounceFailedKillProcess()
            Footer.Print()
            PressEnterToReturnToMainMenu.Print()
            return
        
        Announcer.AnnounceTryingToKillProcWithPid(pid)

        if not ShellcodeSender.Send(self.GetSocket()):
            Footer.Print()
            PressEnterToReturnToMainMenu.Print()
            return
        
        if not ShellcodeExecuter.Execute(self.GetSocket(), pid):
            Footer.Print()
            PressEnterToReturnToMainMenu.Print()
            return
        
        Footer.Print()
        PressEnterToReturnToMainMenu.Print()
    
    @classmethod
    def Start(cls):
        return cls()
        pass
    
    def End(self, nExitCode):
        if not self.__socket:
            print("wah-wah-WAAAAHHHHH-11")
            print(ERROR_FAILED_ESTABLISH_SESSION)
            exit(EXIT_FAILURE)
        if not self.__connected:
            print(ERROR_FAILED_CONNECT_TO_SERVER)
            BlankLinePrinter.Print()
            ThankYouMessagePrinter.Print()
            Footer.Print()
            exit(EXIT_FAILURE)

        self.GetSocket().Send(PROTOCOL_QUIT_COMMAND)
        _ = self.GetSocket().Receive()  # ignore response
        self.__connected = False
        Console.Clear()
        Banner.Print()
        ThankYouMessagePrinter.Print()
        Footer.Print()
        exit(nExitCode)
        pass
    
    def __init__(self):
        try:
            self.__connected = False
            self.__socket = None
            
            Console.Clear()
            Banner.Print()
            WelcomePrinter.Print()
            self.__machineInfo = MachineInfoFactory.Make()
            self.__socket = SocketWrapper()
            if self.__socket is None:
                print("wah-wah-WAAAAHHHHH-1")
                print(ERROR_FAILED_ESTABLISH_SESSION)
                self.End(EXIT_FAILURE)
                
            if not self.__socket.Connect(self.__machineInfo.GetHostName(),
                self.__machineInfo.GetPort()):
                self.End(EXIT_FAILURE)
                
            if 0 == self.__socket.Send(PROTOCOL_HELO_COMMAND):
                print("wah-wah-WAAAAHHHHH-2")
                print(ERROR_FAILED_ESTABLISH_SESSION)
                self.End(EXIT_FAILURE)
                
            status = self.__socket.Receive()
            if not status.startswith('2'):
                print("wah-wah-WAAAAHHHHH-3")
                print(ERROR_FAILED_ESTABLISH_SESSION)
                self.End(EXIT_FAILURE)
        
            self.__connected = True
            Announcer.AnnounceConnectedToServer()
            Footer.Print()
            WaitForEnterToContinue.Print()
        except:
            print("Poo poo")
            self.End(EXIT_FAILURE)
        pass
    
