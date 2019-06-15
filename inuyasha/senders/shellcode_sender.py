from console.console_class import Console
from common.inuyasha_symbols import ERROR_FAILED_ESTABLISH_SESSION, \
    PROTOCOL_CODE_COMMAND_FORMAT
from announcers.announcer import Announcer
from extractors.shellcode_extractor import ShellcodeExtractor
from serializers.shellcode_serializer import ShellcodeSerializer
from validators.shellcode_tuple_validator import ShellcodeTupleValidator
from common.banner import Banner
from common.footer import Footer
from common.press_enter_to_return_to_main_menu import PressEnterToReturnToMainMenu


class ShellcodeSender(object):
        
    @staticmethod
    def __DoSend(theSocket, shellcode_encoded):
        if not theSocket:
            return False
        if not len(shellcode_encoded.strip()):
            return False
        
        if theSocket.Send(
            PROTOCOL_CODE_COMMAND_FORMAT.format(len(shellcode_encoded))) <= 0 \
            or not theSocket.Receive().startswith('3'):
            return False
        
        theSocket.Send("{}\n.\n".format(shellcode_encoded))
        
        return theSocket.Receive().startswith('2')
         
    @staticmethod
    def Send(theSocket):
        Console.Clear()
        Banner.Print()

        if not theSocket:
            print(ERROR_FAILED_ESTABLISH_SESSION)
            Footer.Print()
            PressEnterToReturnToMainMenu.Print()
            return False
            
        shellcode = ShellcodeExtractor.Extract()
        
        if not ShellcodeTupleValidator.IsValid(shellcode):
            return False
        
        shellcode_encoded = ShellcodeSerializer.Serialize(shellcode)
        if not len(shellcode_encoded.strip()):
            Announcer.AnnounceFailedSerializeShellcode()
            Footer.Print()
            PressEnterToReturnToMainMenu.Print()
            return False

        result = ShellcodeSender.__DoSend(theSocket,
            shellcode_encoded)
        if not result:
            Announcer.AnnounceFailedSendShellcode()
            Footer.Print()
            PressEnterToReturnToMainMenu.Print()
       
        return result
        
        