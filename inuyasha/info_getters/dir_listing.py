from console.console_class import Console
from common.inuyasha_symbols import PROTOCOL_LDIR_COMMAND, \
    IDS_SERVER_DIR_PATH_PROMPT, \
    ERROR_FAILED_ESTABLISH_SESSION, ERROR_FAILED_CONNECT_TO_SERVER, LF,\
    DEFAULT_PAGE_LINE_COUNT, ERROR_FAILED_GET_DIR_LISTING_FORMAT,\
    ERROR_DIR_NAME_TOO_LONG, MAX_DIR_LENGTH
from announcers.announcer import Announcer
from paginators.paginator import Paginator
from common.banner import Banner
from common.footer import Footer
from common.press_enter_to_return_to_main_menu import PressEnterToReturnToMainMenu
from common.string_utils import StringUtilities


class DirListing(object):
    
    @staticmethod
    def __OnShowNextPage(nPageNum, strDirChosen):  # @UnusedVariable
        DirListing.__PrintDirListingHeader(strDirChosen)
    
    @staticmethod
    def __PrintDirListingHeader(strDirChosen):
        if not len(strDirChosen.strip()):
            return
        Console.Clear()
        Banner.Print()
        Announcer.AnnounceListServerDirectory()
        Announcer.AnnounceServerDirListed(strDirChosen)
    

    @staticmethod
    def GetServerDirectoryToList():
        Console.Clear()
        Banner.Print()
        Announcer.AnnounceListServerDirectory()
        strDirChosen = ''
        while StringUtilities.IsNullOrWhiteSpace(strDirChosen)\
            or len(strDirChosen.strip()) > MAX_DIR_LENGTH:
            strDirChosen = input(IDS_SERVER_DIR_PATH_PROMPT)
            if not strDirChosen.strip():
                strDirChosen = "~"
            if len(strDirChosen.strip()) > MAX_DIR_LENGTH:
                print(ERROR_DIR_NAME_TOO_LONG.format(
                    MAX_DIR_LENGTH))
                continue
        print()
        return strDirChosen

    @staticmethod
    def PrintDirListing(theSocket, strDirChosen):
        if not theSocket:
            return
        if not len(strDirChosen.strip()):
            return
        status = theSocket.Receive()
        if status.startswith('2'):
            lines = theSocket.ReceiveAllLines()
            Paginator.Paginate(iter(lines),
                promptCallable=DirListing.__OnShowNextPage,
                userState=strDirChosen, nLineCount=DEFAULT_PAGE_LINE_COUNT - 1)
#             for line in lines:
#                 if not line.strip():
#                     continue
#                 print(line.strip())
            print()
        else:
            print(ERROR_FAILED_GET_DIR_LISTING_FORMAT.format(strDirChosen))
        pass

    @staticmethod
    def RequestServerDirectoryListing(theSocket, strDirChosen):
        if not len(strDirChosen.strip()):
            return
        if not theSocket:
            return
        DirListing.__PrintDirListingHeader(strDirChosen)
        theSocket.Send(PROTOCOL_LDIR_COMMAND + " " + strDirChosen.strip() + LF)

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

        strDirChosen = DirListing.GetServerDirectoryToList()
        DirListing.RequestServerDirectoryListing(theSocket, strDirChosen)
        DirListing.PrintDirListing(theSocket, strDirChosen)
        Footer.Print()
        PressEnterToReturnToMainMenu.Print()
        
