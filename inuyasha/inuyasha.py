#!/usr/bin/python3

from factories.session_factory import SessionFactory
from common.inuyasha_symbols import EXIT_FAILURE, \
    ERROR_FAILED_START_USER_SESSION, ERROR_FAILED_CONNECT_TO_SERVER, \
    EXIT_SUCCESS
from menus.main_menu import MainMenu


class InuyashaApp(object):

    def prompt_user_for_server_and_connect(self):
        self.__session = SessionFactory.Make()
        pass
        
    def main(self):
        self.prompt_user_for_server_and_connect()
        if not self.__session:
            print(ERROR_FAILED_START_USER_SESSION)
            exit(EXIT_FAILURE)
        if not self.__session.IsConnected():
            print(ERROR_FAILED_CONNECT_TO_SERVER)
            exit(EXIT_FAILURE)
            
        while(True):
            choice = MainMenu.Show()
            print()
            if choice == 1:
                self.__session.GetCpuInfo()
                continue
            
            if choice == 2:
                self.__session.GetDirListing()
                continue
            
            if choice == 3:
                self.__session.KillSpecifiedProcess()
                continue
            
            if choice == 4:
                self.__session.End(EXIT_SUCCESS)
                    
        pass


__app = None

if __name__ == "__main__":
    __app = InuyashaApp()
    __app.main()
