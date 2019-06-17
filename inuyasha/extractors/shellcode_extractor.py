from common.inuyasha_symbols import ERROR_FAILED_FIND_SHELLCODE_SOURCE_FORMAT,\
    IDS_COMPILING_FILE_FORMAT, ERROR_OBJECT_CODE_NOT_EXISTS_FORMAT,\
    ERROR_FAILED_COMPILE_FILE, IDS_ASSEMBLY_CODE_PATH_PROMPT,\
    DEFAULT_ASM_CODE_PATH, EXIT_SUCCESS
import os
from common.file_utilities import FileUtilities
from compilers.asm_compiler import AsmCompiler
from parsers.object_code_parser import ObjectCodeParser
from announcers.announcer import Announcer
from common.blank_line_printer import BlankLinePrinter
from common.footer import Footer
from common.press_enter_to_return_to_main_menu import PressEnterToReturnToMainMenu
from prompters.prompter import Prompter
from common.exit_handler import ExitHandler
from displayers.exit_message_displayer import ExitMessageDisplayer
from validators.asm_code_path_validator import AsmCodePathValidator
from common.string_utils import StringUtilities


class ShellcodeExtractor(object):
    
    @staticmethod
    def __DoCheckSourceCodeExists(strCodePath):
        if not len(strCodePath.strip()):
            return False
        result = FileUtilities.Exists(strCodePath)
        if not result:
            print(ERROR_FAILED_FIND_SHELLCODE_SOURCE_FORMAT.format(
                strCodePath))
            BlankLinePrinter.Print()
        return result

    @staticmethod
    def __DoCompileSourceCode(strCodePath):
        Announcer.AnnounceAttemptingCompileShellcode(strCodePath)
        if not len(strCodePath.strip()):
            return False
        if not FileUtilities.Exists(strCodePath):
            return False
        print(IDS_COMPILING_FILE_FORMAT.format(strCodePath))
        return AsmCompiler.CompileWitNasm(strCodePath)
    
    @staticmethod
    def __DoConvertObjectCodeToShellCode(strObjectCodePath):     
        if not len(strObjectCodePath.strip()):
            return ()
        if not FileUtilities.Exists(strObjectCodePath):
            print(ERROR_OBJECT_CODE_NOT_EXISTS_FORMAT.format(
                strObjectCodePath))
            return ()
        shellcode = \
            ObjectCodeParser.ObjectCodeToShellCode(strObjectCodePath)
        return shellcode
        
    @staticmethod
    def __DoFormulateObjectCodePath(strCodePath):
        if not len(strCodePath.strip()):
            return ''
        return strCodePath.replace('.asm', '.o')
        
    @staticmethod
    def __DoGetAsmCodePath():
        result = Prompter.PromptForString(
            strPrompt=IDS_ASSEMBLY_CODE_PATH_PROMPT,
            strDefault=DEFAULT_ASM_CODE_PATH,
            inputValidator=AsmCodePathValidator.IsValid,
            invalidInputHandler=AsmCodePathValidator.HandleInvalidInput,
            keyboardInterruptHandler=lambda: ExitHandler.ExitApp(EXIT_SUCCESS,
                exitMessageDisplayer=ExitMessageDisplayer.Display()))
        if StringUtilities.IsNullOrWhiteSpace(result):
            return result
        result = os.path.expanduser(result)
        BlankLinePrinter.Print()
        return result
    
    @staticmethod
    def Extract():
        Announcer.AnnounceAttemptingExtractShellcodeBytes()
        
        strCodePath = ShellcodeExtractor.__DoGetAsmCodePath()
        
        if StringUtilities.IsNullOrWhiteSpace(strCodePath):
            return ()
        
        if not ShellcodeExtractor.__DoCompileSourceCode(strCodePath):
            print(ERROR_FAILED_COMPILE_FILE.format(
                strCodePath))
            return ()
        
        strObjectCodePath = \
            ShellcodeExtractor.__DoFormulateObjectCodePath(strCodePath)    
    
        shellcode = \
            ShellcodeExtractor\
            .__DoConvertObjectCodeToShellCode(strObjectCodePath)
    
        return shellcode