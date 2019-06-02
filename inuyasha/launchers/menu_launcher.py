from menus.main_menu import MainMenu
from menus.server_options_menu import ServerOptionsMenu

class MenuLauncher(object):
    @staticmethod
    def LaunchMainMenu():
        theMenu = MainMenu()
        theMenu.Show()
    
    @staticmethod
    def LaunchServerOptionsMenu():
        theMenu = ServerOptionsMenu()
        theMenu.Show()
    
