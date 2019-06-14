from machines.machine_info import MachineInfo
from common.gui_utilities import WelcomePrinter, Banner
from console.console_class import Console
from common.inuyasha_symbols import DEFAULT_HOSTNAME, DEFAULT_PORT, \
    SERVER_ADDRESS_PROMPT_FORMAT, SERVER_PORT_PROMPT_FORMAT, INVALID_PORT_NUMBER, \
    ERROR_PORT_INVALID_VALUE
from validators.port_number_validator import PortNumberValidator


class MachineInfoFactory(object):
            
    @staticmethod
    def __PromptHostname():
        strHostName = ''
        try:
            strHostName = input(SERVER_ADDRESS_PROMPT_FORMAT \
                .format(DEFAULT_HOSTNAME))
            if not len(strHostName.strip()):
                strHostName = DEFAULT_HOSTNAME
        except KeyboardInterrupt as e:
            raise e
        except:
            strHostName = DEFAULT_HOSTNAME
        return strHostName
    
    @staticmethod
    def __PromptPort():
        nPort = INVALID_PORT_NUMBER
        while nPort == INVALID_PORT_NUMBER:
            strPort = input(SERVER_PORT_PROMPT_FORMAT \
                .format(DEFAULT_PORT))
            if not len(strPort.strip()):
                nPort = DEFAULT_PORT
            else:
                try:
                    nPort = int(strPort)
                    if not PortNumberValidator.IsValid(nPort):
                        nPort = INVALID_PORT_NUMBER
                        print(ERROR_PORT_INVALID_VALUE)
                        continue
                except KeyboardInterrupt as e:
                    raise e
                except:
                    nPort = INVALID_PORT_NUMBER
                    print(ERROR_PORT_INVALID_VALUE)
                    
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
