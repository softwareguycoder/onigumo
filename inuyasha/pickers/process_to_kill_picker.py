from console.console_class import Console
from common.inuyasha_symbols import PROTOCOL_LIST_COMMAND, \
    ERROR_FAILED_ESTABLISH_SESSION, ERROR_FAILED_CONNECT_TO_SERVER, \
    ERROR_MUST_PICK_PID, SHOULD_PROC_LIST_BE_PAGINATED_PROMPT,\
    DEFAULT_PAGE_LINE_COUNT, EXIT_SUCCESS,\
    IDS_PICK_PROCESS_TO_KILL_PROMPT, INVALID_PID_VALUE
from announcers.announcer import Announcer
from paginators.paginator import Paginator
from common.banner import Banner
from common.blank_line_printer import BlankLinePrinter
from common.press_enter_to_pick_proc_to_kill import PressEnterToPickProcToKill
from prompters.prompter import Prompter
from common.exit_handler import ExitHandler
from displayers.exit_message_displayer import ExitMessageDisplayer
from validators.process_id_validator import ProcessIdValidator


class ProcessToKillPicker(object):

    @staticmethod
    def __OnDisplayNextPage(nPageNum, userState):  # @UnusedVariable
        Console.Clear()
        Banner.Print()
        Announcer.AnnounceRemoteProcessList()

    @staticmethod
    def __DoListProcessesOnRemoteMachine(theSocket):
        try:
            if not theSocket:
                print(ERROR_FAILED_ESTABLISH_SESSION)
                return
            BlankLinePrinter.Print()
            bShouldPaginate = Prompter.YesNoPrompt(
                strPrompt=SHOULD_PROC_LIST_BE_PAGINATED_PROMPT,
                bYesIsDefault=True,
                keyboardInterruptHandler=lambda: ExitHandler.\
                    ExitApp(EXIT_SUCCESS,
                    exitMessageDisplayer=ExitMessageDisplayer.Display())
                    )
            Console.Clear()
            Banner.Print()
            Announcer.AnnounceRemoteProcessList()
            nPID = INVALID_PID_VALUE
            theSocket.Send(PROTOCOL_LIST_COMMAND)
            status = theSocket.Receive()
            if status.startswith('2'):
                lines = theSocket.ReceiveAllLines()
                if len(lines) == 0:
                    Announcer.AnnounceNoResponseLines()
                    return nPID
                if bShouldPaginate:
                    Paginator.Paginate(iter(lines),\
                         ProcessToKillPicker.__OnDisplayNextPage, 
                         userState=None, nLineCount=9)
                else:
                    for line in lines:
                        if not line.strip():
                            continue
                        print(line.strip())
                print()
                nPID = Prompter.PromptForInt(
                    strPrompt=IDS_PICK_PROCESS_TO_KILL_PROMPT,
                    nDefault=None,
                    keyboardInterruptHandler=lambda: \
                        ExitHandler.ExitApp(EXIT_SUCCESS, \
                        exitMessageDisplayer=ExitMessageDisplayer.Display()),
                    inputValidator=ProcessIdValidator.IsValid,
                    invalidInputHandler=ProcessIdValidator.HandleInvalidChoice,
                    nInvalidValue=INVALID_PID_VALUE)
        except KeyboardInterrupt:
            return INVALID_PID_VALUE
        else:
            Announcer.AnnounceFailedListRemoteProcesses()
            return INVALID_PID_VALUE
                       
        return nPID
    
    @staticmethod
    def __DoShowIntroScreen():
        Console.Clear()
        Banner.Print()
        Announcer.AnnouncePickProcessToKillPicker()
        PressEnterToPickProcToKill.Print()
        pass
    
    @staticmethod
    def __IsPidValid(nPID):
        return nPID > INVALID_PID_VALUE
    
    @staticmethod
    def PickAndKillProc(theSocket, isConnected):
        if not theSocket:
            print(ERROR_FAILED_ESTABLISH_SESSION)
            return INVALID_PID_VALUE
        if not isConnected:
            print(ERROR_FAILED_CONNECT_TO_SERVER)
            return INVALID_PID_VALUE
        
        ProcessToKillPicker.__DoShowIntroScreen()
        nPID = ProcessToKillPicker.\
            __DoListProcessesOnRemoteMachine(theSocket)
        if not ProcessToKillPicker.__IsPidValid(nPID):
            return INVALID_PID_VALUE
        return nPID
    
        
