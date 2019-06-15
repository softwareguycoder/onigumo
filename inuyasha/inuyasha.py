#!/usr/bin/python3

from factories.session_factory import SessionFactory
from common.inuyasha_symbols import EXIT_FAILURE, \
    ERROR_FAILED_START_USER_SESSION, ERROR_FAILED_CONNECT_TO_SERVER, \
    EXIT_SUCCESS, CHOICE_GET_CPU_INFO, CHOICE_LIST_DIR, CHOICE_KILL_PROC, \
    CHOICE_EXIT, INVALID_CHOICE_VALUE, ERROR_INVALID_MAIN_MENU_CHOICE
from menus.main_menu import MainMenu
from common.exit_handler import ExitHandler
from displayers.exit_message_displayer import ExitMessageDisplayer


class InuyashaApp(object):

    def InitSession(self):
        try:
            self.__session = SessionFactory.Make()
            if not self.__session:
                print(ERROR_FAILED_START_USER_SESSION)
                self.ExitInstance(EXIT_FAILURE, self.__session)
            if not self.__session.IsConnected():
                print(ERROR_FAILED_CONNECT_TO_SERVER)
                self.ExitInstance(EXIT_FAILURE, self.__session)
        except KeyboardInterrupt:
            ExitHandler.ExitApp(EXIT_FAILURE, None,
                exitMessageDisplayer=ExitMessageDisplayer.Display)
        pass
   
    def PickMainMenuChoice(self):
        nChoice = INVALID_CHOICE_VALUE
        while nChoice == INVALID_CHOICE_VALUE:
            nChoice = MainMenu.Show()
            if nChoice == INVALID_CHOICE_VALUE:
                print(ERROR_INVALID_MAIN_MENU_CHOICE)
                continue
        return nChoice
        
    def main(self):
        self.InitSession()
        
        try:
            while(True):
                nChoice = self.PickMainMenuChoice()
                
                print()
                if nChoice == CHOICE_GET_CPU_INFO:
                    self.__session.GetCpuInfo()
                    continue
                
                if nChoice == CHOICE_LIST_DIR:
                    self.__session.GetDirListing()
                    continue
                
                if nChoice == CHOICE_KILL_PROC:
                    self.__session.KillSpecifiedProcess()
                    continue
                
                if nChoice == CHOICE_EXIT:
                    print("I am going to cry")
                    self.__session.End(EXIT_SUCCESS)
        except KeyboardInterrupt:
            print("I hate Python!")
            self.__session.End(EXIT_FAILURE)    
        pass


__app = None

if __name__ == "__main__":
    __app = InuyashaApp()
    __app.main()
