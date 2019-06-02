from consolemenu import ConsoleMenu
from abc import abstractmethod, ABC


class MenuBase(ABC):
    def GetMenu(self):
        return self.__menu
    
    @abstractmethod
    def InitMenu(self):
        pass

    def LoadMenu(self, title, subtitle, bShowExit=True):
        self.__menu = ConsoleMenu(title, subtitle, show_exit_option=bShowExit)

    def Show(self):
        self.__menu.show()