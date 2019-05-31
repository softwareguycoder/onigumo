#!/usr/bin/python3
from command_target import CommandTarget
from menus.main_menu import MainMenu

class InuyashaApp:
    @staticmethod
    def ShowMainMenu():
        if (CommandTarget.connmgr is not None
            and CommandTarget.connmgr.get_Socket is not None):
            CommandTarget.connmgr.EndClientSession()
            CommandTarget.connmgr.get_Socket.Close()
        theMenu = MainMenu()
        theMenu.show()
        pass


def main():
    InuyashaApp.ShowMainMenu()
    pass


if __name__ == "__main__":
    main()
        
