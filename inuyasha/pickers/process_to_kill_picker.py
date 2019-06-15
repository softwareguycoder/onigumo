from console.console_class import Console
from common.inuyasha_symbols import PROTOCOL_LIST_COMMAND, \
    ERROR_FAILED_ESTABLISH_SESSION, ERROR_FAILED_CONNECT_TO_SERVER, \
    ERROR_MUST_PICK_PID, SHOULD_PROC_LIST_BE_PAGINATED_PROMPT,\
    DEFAULT_PAGE_LINE_COUNT
from announcers.announcer import Announcer
from validators.yes_no_input_validator import YesNoInputValidator
from paginators.paginator import Paginator
from common.banner import Banner
from common.blank_line_printer import BlankLinePrinter
from common.press_enter_to_pick_proc_to_kill import PressEnterToPickProcToKill
from common.footer import Footer
from common.press_enter_to_return_to_main_menu import PressEnterToReturnToMainMenu


class ProcessToKillPicker(object):

    @staticmethod
    def __OnDisplayNextPage(nPageNum, userState):  # @UnusedVariable
        Console.Clear()
        Banner.Print()
        Announcer.AnnounceRemoteProcessList()

    @staticmethod
    def __DoListProcessesOnRemoteMachine(theSocket):
        if not theSocket:
            print("wah-wah-WAAAAHHHHH-7")
            print(ERROR_FAILED_ESTABLISH_SESSION)
            return
        BlankLinePrinter.Print()
        strChoice = ''
        while not len(strChoice.strip()):
            strChoice = input(SHOULD_PROC_LIST_BE_PAGINATED_PROMPT)
            if not len(strChoice.strip()):
                strChoice = 'Y'
            if not YesNoInputValidator.IsValid(strChoice):
                strChoice = ''
                continue
        Console.Clear()
        Banner.Print()
        Announcer.AnnounceRemoteProcessList()
        pid = 0
        theSocket.Send(PROTOCOL_LIST_COMMAND)
        status = theSocket.Receive()
        if status.startswith('2'):
            lines = theSocket.ReceiveAllLines()
            
            if strChoice.strip().lower() == 'n':
                for line in lines:
                    if not line.strip():
                        continue
                    print(line.strip())
            else:
                Paginator.Paginate(iter(lines),\
                     ProcessToKillPicker.__OnDisplayNextPage, 
                     userState=None, nLineCount=DEFAULT_PAGE_LINE_COUNT - 1)
            print()
            while pid == 0:
                try:
                    pid = int(input("> PID of the process to kill: > "))
                except KeyboardInterrupt as e:
                    raise e # let the user press Ctrl+C to escape
                except:
                    print(">>> ERROR: Please type a number from the PID column above.")
                    pid = 0
        else:
            print(">>> ERROR: Unable to obtain a list of running processes.")
                       
        return pid
    
    @staticmethod
    def __DoShowIntroScreen():
        Console.Clear()
        Banner.Print()
        Announcer.AnnouncePickProcessToKillPicker()
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
            print("wah-wah-WAAAAHHHHH-8")
            print(ERROR_FAILED_ESTABLISH_SESSION)
            return -1
        if not isConnected:
            print(ERROR_FAILED_CONNECT_TO_SERVER)
            return -1
        
        ProcessToKillPicker.__DoShowIntroScreen()
        pid = ProcessToKillPicker.__DoListProcessesOnRemoteMachine(theSocket)
        if not ProcessToKillPicker.__IsPidValid(pid):
            return -1
        return pid
    
        
