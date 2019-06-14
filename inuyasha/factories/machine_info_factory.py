from machines.machine_info import MachineInfo
from common.gui_utilities import WelcomePrinter, Banner
from console.console_class import Console
from common.inuyasha_symbols import DEFAULT_HOSTNAME, DEFAULT_PORT, \
    ERROR_PORT_INVALID_VALUE, SERVER_ADDRESS_PROMPT, EXIT_SUCCESS,\
    SERVER_PORT_PROMPT
from validators.port_number_validator import PortNumberValidator
from prompters.prompter import Prompter
from common.exit_handler import ExitHandler


class MachineInfoFactory(object):
            
    @staticmethod
    def __PromptHostname():
        strHostName = Prompter.PromptForString(strPrompt=SERVER_ADDRESS_PROMPT,
            strDefault=DEFAULT_HOSTNAME, 
            keyboardInterruptHandler=lambda: ExitHandler\
                .ExitApp(EXIT_SUCCESS, None))
        return strHostName
    
    @staticmethod
    def __PromptPort():
        nPort = Prompter.PromptForInt(SERVER_PORT_PROMPT,
            DEFAULT_PORT, 
            inputValidator=lambda x, y:PortNumberValidator.IsValid(int(x)),
            invalidInputHandler=lambda x:print(ERROR_PORT_INVALID_VALUE),
            keyboardInterruptHandler=lambda: ExitHandler\
                .ExitApp(EXIT_SUCCESS, None))
        return nPort
        
    @staticmethod
    def __PromptHostNameAndPort():
        return (MachineInfoFactory.__PromptHostname(),
            MachineInfoFactory.__PromptPort())
    
    @staticmethod
    def Make():
        Console.Clear()
        Banner.Print()
        WelcomePrinter.Print()
        strHostName, nPort = MachineInfoFactory.__PromptHostNameAndPort()
        return MachineInfo(strHostName, nPort)
