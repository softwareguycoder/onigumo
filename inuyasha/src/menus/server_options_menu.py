from menus.menu_base import MenuBase
from inuyasha_symbols import APP_TITLE, IDS_SERVER_OPTIONS,\
    IDM_SERVER_LIST_PROCESSES, IDM_BACK
from command_target import CommandTarget
from consolemenu.items.function_item import FunctionItem

class ServerOptionsMenu(MenuBase):
    
    def init_menu(self):
        list_item = FunctionItem(IDM_SERVER_LIST_PROCESSES,
                                CommandTarget.ListProcessesOnRemoteMachine)
        self.menu.append_item(list_item)
        back_item = FunctionItem(IDM_BACK,
                                 CommandTarget.ShowMainMenu)
        self.menu.append_item(back_item) 
        pass
    
    def __init__(self):
        self.loadMenu(APP_TITLE, IDS_SERVER_OPTIONS, bShowExit=False)
        self.init_menu()
    