from console.console_class import Console
from common.gui_utilities import Banner, Footer, PressEnterToReturnToMainMenu
from common.inuyasha_symbols import PROTOCOL_LDIR_COMMAND, \
    IDS_SERVER_DIR_PATH_PROMPT, ERROR_FAILED_GET_DIR_LISTING, \
    ERROR_FAILED_ESTABLISH_SESSION, ERROR_FAILED_CONNECT_TO_SERVER
from announcers.announcer import Announcer


class DirListing(object):

    @staticmethod
    def GetServerDirectoryToList():
        Console.Clear()
        Banner.Print()
        Announcer.AnnounceListServerDirectory()
        dirChosen = input(IDS_SERVER_DIR_PATH_PROMPT)
        if not dirChosen.strip():
            dirChosen = "~"
        print()
        return dirChosen

    @staticmethod
    def PrintDirListing(theSocket):
        if not theSocket:
            return
        status = theSocket.Receive()
        if status.startswith('2'):
            lines = theSocket.ReceiveAllLines()
            for line in lines:
                if not line.strip():
                    continue
                print(line.strip())
            print()
        else:
            print(ERROR_FAILED_GET_DIR_LISTING)
        pass

    @staticmethod
    def RequestServerDirectoryListing(theSocket, dirChosen):
        if not len(dirChosen.strip()):
            return
        if not theSocket:
            return
        Console.Clear()
        Banner.Print()
        Announcer.AnnounceListServerDirectory()
        Announcer.AnnounceServerDirListed(dirChosen)
        theSocket.Send(PROTOCOL_LDIR_COMMAND + " " + dirChosen.strip() + "\n")

    @staticmethod
    def Get(theSocket, isConnected):
        Console.Clear()
        Banner.Print()
        Announcer.AnnounceListServerDirectory()
        if not theSocket:
            print(ERROR_FAILED_ESTABLISH_SESSION)
            Footer.Print()
            PressEnterToReturnToMainMenu.Print()
            return
        if not isConnected:
            print(ERROR_FAILED_CONNECT_TO_SERVER)
            Footer.Print()
            PressEnterToReturnToMainMenu.Print()          
            return

        dirChosen = DirListing.GetServerDirectoryToList()
        DirListing.RequestServerDirectoryListing(theSocket, dirChosen)
        DirListing.PrintDirListing(theSocket)
        Footer.Print()
        PressEnterToReturnToMainMenu.Print()
        
