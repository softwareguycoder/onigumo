from menus.menu_base import MenuBase
from factories.function_item_factory import FunctionItemFactory
from commands.command_target import CommandTarget
from common.inuyasha_symbols import IDM_SERVER_LIST_PROCESSES, APP_TITLE,\
    IDS_SERVER_OPTIONS


class ServerOptionsMenu(MenuBase):
    
    def init_menu(self):
        list_item = FunctionItemFactory.Make(IDM_SERVER_LIST_PROCESSES,
                                CommandTarget.OnListProcessesOnRemoteMachine)
        self.menu.append_item(list_item)
        pass
    
    def __init__(self):
        self.loadMenu(APP_TITLE, IDS_SERVER_OPTIONS, bShowExit=True)
        self.init_menu()
