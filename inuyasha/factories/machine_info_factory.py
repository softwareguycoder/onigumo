from machines.machine_info import MachineInfo
from common.gui_utilities import WelcomePrinter, Banner, Footer, \
    WaitForEnterToContinue
from console.console_class import Console
from common.inuyasha_symbols import DEFAULT_HOSTNAME, DEFAULT_PORT, \
    SERVER_ADDRESS_PROMPT_FORMAT, SERVER_PORT_PROMPT_FORMAT


class MachineInfoFactory(object):
        
    @staticmethod
    def __PromptHostNameAndPort():
        hostname = input(SERVER_ADDRESS_PROMPT_FORMAT \
            .format(DEFAULT_HOSTNAME))
        if not len(hostname.strip()):
            hostname = DEFAULT_HOSTNAME
        portString = input(SERVER_PORT_PROMPT_FORMAT \
            .format(DEFAULT_PORT))
        port = 0
        if not len(portString.strip()):
            port = DEFAULT_PORT
        else:
            port = int(portString)
        return (hostname, port)
    
    @staticmethod
    def Make():
        Console.Clear()
        Banner.Print()
        WelcomePrinter.Print()
        hostname, port = MachineInfoFactory.__PromptHostNameAndPort()
        return MachineInfo(hostname, port)
