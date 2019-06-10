from translators.bytes_to_base64_translator import BytesToBase64Translator
from validators.shellcode_tuple_validator import ShellcodeTupleValidator
from announcers.announcer import Announcer
class ShellcodeSerializer(object):
    
    @staticmethod
    def __DoEncode(shellcodeTuple):
        if not ShellcodeTupleValidator.IsValid(shellcodeTuple):
            return ''
        return BytesToBase64Translator.Translate(
            shellcodeTuple[0])
    
    @staticmethod
    def Serialize(shellcodeTuple):
        Announcer.AnnounceAttemptingToSerializeShellcode()
        shellcode_encoded = ShellcodeSerializer.__DoEncode(shellcodeTuple)
        if not len(shellcode_encoded.strip()):
            return ''
        return shellcode_encoded