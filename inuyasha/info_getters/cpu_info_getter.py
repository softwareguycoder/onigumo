from console.console_class import Console
from common.gui_utilities import Banner, Footer, PressEnterToReturnToMainMenu
from common.inuyasha_symbols import PROTOCOL_INFO_COMMAND,\
    ERROR_FAILED_ESTABLISH_SESSION, EXIT_FAILURE, \
    ERROR_FAILED_CONNECT_TO_SERVER, ERROR_FAILED_GET_CPU_INFO
from announcers.announcer import Announcer
import pager
from paginators.paginator import Paginator

class CpuInfo(object):

    @staticmethod
    def Get(theSocket, isConnected):
        if not theSocket:
            print(ERROR_FAILED_ESTABLISH_SESSION)
            exit(EXIT_FAILURE)
        if not isConnected:
            print(ERROR_FAILED_CONNECT_TO_SERVER)
            exit(EXIT_FAILURE)
            
        Console.Clear()
        Banner.Print()

        theSocket.Send(PROTOCOL_INFO_COMMAND)
        status = theSocket.Receive()
        if status.startswith('2'):
            Announcer.AnnounceServerCPUInfo()
            lines = theSocket.ReceiveAllLines()
            Paginator.Paginate(iter(lines))
#             for line in lines:
#                 if not line.strip():
#                     continue
#                 print(line.strip())
            print()
        else:
            print(ERROR_FAILED_GET_CPU_INFO)
        Footer.Print()
        PressEnterToReturnToMainMenu.Print()
