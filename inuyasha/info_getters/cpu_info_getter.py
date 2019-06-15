from console.console_class import Console
from common.inuyasha_symbols import PROTOCOL_INFO_COMMAND,\
    ERROR_FAILED_ESTABLISH_SESSION, EXIT_FAILURE, \
    ERROR_FAILED_CONNECT_TO_SERVER, ERROR_FAILED_GET_CPU_INFO
from announcers.announcer import Announcer
from paginators.paginator import Paginator
from common.banner import Banner
from common.footer import Footer
from common.press_enter_to_return_to_main_menu import PressEnterToReturnToMainMenu

class CpuInfo(object):

    @staticmethod
    def Get(theSocket, isConnected):
        if not theSocket:
            print("wah-wah-WAAAAHHHHH-5")
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
