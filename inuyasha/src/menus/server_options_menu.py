from menus.menu_base import MenuBase
from inuyasha_symbols import APP_TITLE, IDS_SERVER_OPTIONS,\
    IDM_SERVER_LIST_PROCESSES
from command_target import CommandTarget
from consolemenu.items.function_item import FunctionItem

class ServerOptionsMenu(MenuBase):
    
    def init_menu(self):
        list_item = FunctionItem(IDM_SERVER_LIST_PROCESSES,
                                CommandTarget.ListProcesses)
        self.menu.append_item(list_item)
        list_item = 
        pass
    
    def __init__(self):
        self.loadMenu(APP_TITLE, IDS_SERVER_OPTIONS, bShowExit=False)
        self.init_menu()
    