from menus.menu_base import MenuBase
from common.inuyasha_symbols import APP_SUBTITLE, APP_TITLE,\
    IDM_CONNECT_REMOTE_MACHINE
from commands.command_target import CommandTarget
from factories.function_item_factory import FunctionItemFactory

class MainMenu(MenuBase):
    def InitMenu(self):
        connect_item = FunctionItemFactory.Make(IDM_CONNECT_REMOTE_MACHINE,
                                CommandTarget.OnConnectRemoteMachine)
        self.GetMenu().append_item(connect_item)
        pass
    
    def __init__(self):
        self.LoadMenu(APP_TITLE, APP_SUBTITLE)
        self.InitMenu()