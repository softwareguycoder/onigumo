from common.inuyasha_symbols import TCP_IP, TCP_PORT, EXIT_FAILURE, \
    APP_SUBTITLE, APP_TITLE, \
    IDM_SERVER_LIST_PROCESSES, EXIT_SUCCESS, IDS_PICK_PROCESS_TO_KILL, \
    IDS_PRESS_ENTER_TO_CONTINUE, ERROR_NO_RESPONSE_LINES,\
    ERROR_PROCESS_INFO_NOT_VALID, ERROR_FAILED_CONNECT_TO_SERVER
from managers.session_manager import SessionManager
from consolemenu.console_menu import ConsoleMenu
from consolemenu.items.function_item import FunctionItem
from factories.process_info_factory import ProcessInfoFactory
from validators.process_info_validator import ProcessInfoValidator
from common.error_handler import ErrorHandler
from translators.ps_exec_output_line_to_function_item_translator \
    import PsExecOutputLineToFunctionItemTranslator


class CommandTarget(object):
    session = None

    @staticmethod
    def __CreateProcessPickerMenu():
        menu = ConsoleMenu(APP_TITLE, IDS_PICK_PROCESS_TO_KILL,
            show_exit_option=True)
        response_lines = CommandTarget.session.GetResponseLines()[1:]
        if len(response_lines) == 0:
            ErrorHandler.ShowErrorThenExit(
                ERROR_NO_RESPONSE_LINES)
                
        try:
            for line in response_lines:
                procInfo = ProcessInfoFactory.Make(line)
                new_menu_item = \
                    PsExecOutputLineToFunctionItemTranslator \
                        .ToFunctionItem(CommandTarget.OnKillProcess, procInfo)
                if new_menu_item is None:
                    continue
                menu.append_item(new_menu_item)
        except Exception as e:
            ErrorHandler.ShowErrorThenExit(e)
        menu.show()
        pass
    
    @staticmethod
    def __CreateServerFunctionsMenu():
        menu = ConsoleMenu(APP_TITLE, APP_SUBTITLE, show_exit_option=True)
        menu.append_item(FunctionItem(IDM_SERVER_LIST_PROCESSES,
            CommandTarget.OnListProcessesOnRemoteMachine))
        menu.append_item(FunctionItem("End Session",
            CommandTarget.OnEndSession))
        menu.show()

    @staticmethod
    def OnConnectRemoteMachine():
        CommandTarget.session = SessionManager(TCP_IP, TCP_PORT)
        CommandTarget.session.Connect()
        if not CommandTarget.session.Open():
            ErrorHandler.ShowErrorThenExit(ERROR_FAILED_CONNECT_TO_SERVER)           
        CommandTarget.__CreateServerFunctionsMenu()
        pass
    
    @staticmethod
    def OnEndSession():
        try:
            if CommandTarget.session is None:
                return
            CommandTarget.session.Close()
        except:
            pass  # at this point, ignore exceptions
        
    @staticmethod
    def OnKillProcess(procInfo):
        if not ProcessInfoValidator.IsValid(procInfo):
            ErrorHandler.ShowErrorThenExit(ERROR_PROCESS_INFO_NOT_VALID)
        print("Killing process", procInfo.get_CMD(), "...")
        input(IDS_PRESS_ENTER_TO_CONTINUE)
        # TODO: Add lines here to compile shellcode and send to the server
        # in order to kill the specified process
        pass    
    
    @staticmethod
    def OnListProcessesOnRemoteMachine():
        if not CommandTarget.session:
            exit(EXIT_FAILURE)
        if not CommandTarget.session.ListRemoteProcesses():
            exit(EXIT_FAILURE)
        CommandTarget.__CreateProcessPickerMenu()
        pass
    
