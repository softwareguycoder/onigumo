from common.inuyasha_symbols import TCP_IP, TCP_PORT, EXIT_FAILURE,\
    APP_SUBTITLE, APP_TITLE, \
    IDM_SERVER_LIST_PROCESSES, EXIT_SUCCESS
from managers.session_manager import SessionManager
from consolemenu.console_menu import ConsoleMenu
from consolemenu.items.function_item import FunctionItem
from translators.ps_exec_output_line_to_function_item_translator \
    import PsExecOutputLineToFunctionItemTranslator

class CommandTarget(object):
    session = None

    @staticmethod
    def __CreateProcessPickerMenu():
        menu = ConsoleMenu(APP_TITLE, "Pick a process to kill", 
            show_exit_option=True)
        if len(CommandTarget.session.GetResponseLines()) > 0:
            for line in CommandTarget.session.GetResponseLines()[1:].strip():
                new_menu_item = \
                    PsExecOutputLineToFunctionItemTranslator \
                        .ToFunctionItem(line)
                if new_menu_item is None:
                    continue
                menu.append_item(new_menu_item)
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
            print("ERROR: Failed to connect to remote server.")
            exit(EXIT_FAILURE)
        CommandTarget.__CreateServerFunctionsMenu()
        pass
    
    @staticmethod
    def OnEndSession():
        CommandTarget.session.Close()
        exit(EXIT_SUCCESS)
        
    @staticmethod
    def OnListProcessesOnRemoteMachine():
        if not CommandTarget.session:
            exit(EXIT_FAILURE)
        CommandTarget.session.ListRemoteProcesses()
        CommandTarget.__CreateProcessPickerMenu()
        pass
    
    @staticmethod
    def DoNothing():
        pass
    