from console.console_class import Console
from common.gui_utilities import Banner


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
        value = int(input("Choice: >> "))
        return value
