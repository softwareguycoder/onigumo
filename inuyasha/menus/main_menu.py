from console.console_class import Console
from common.gui_utilities import Banner
from common.inuyasha_symbols import INVALID_CHOICE_VALUE,\
    ERROR_INVALID_MAIN_MENU_CHOICE


class MainMenu(object):

    @staticmethod
    def Show():
        Console.Clear()
        Banner.Print()
        print("                            ***  Main Menu  ***            ")
        print()
        print("Pick an option:")
        print()
        print("   1) List CPU info")
        print("   2) List Directory Contents")
        print("   3) Kill a Process")
        print("   4) Exit")
        print()
        print("*************************************************************************")
        nValue = INVALID_CHOICE_VALUE
        while nValue == INVALID_CHOICE_VALUE:
            try:
                nValue = int(input("> Choice (1/2/3/4): > "))
                if nValue < 1 or nValue > 4:
                    nValue = INVALID_CHOICE_VALUE
                    print(ERROR_INVALID_MAIN_MENU_CHOICE)
                    continue
            except KeyboardInterrupt as e:
                raise e
            except:
                nValue = INVALID_CHOICE_VALUE
                print(ERROR_INVALID_MAIN_MENU_CHOICE)
        return nValue
