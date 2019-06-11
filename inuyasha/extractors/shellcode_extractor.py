from common.inuyasha_symbols import IDS_ASSEMBLY_CODE_PATH_PROMPT_FORMAT, \
    ASM_CODE_PATH, ERROR_FAILED_FIND_SHELLCODE_SOURCE_FORMAT,\
    IDS_COMPILING_FILE_FORMAT, ERROR_OBJECT_CODE_NOT_EXISTS_FORMAT,\
    ERROR_FAILED_COMPILE_FILE
from common.gui_utilities import BlankLinePrinter, Footer,\
    PressEnterToReturnToMainMenu
import os
from common.file_utilities import FileUtilities
from compilers.asm_compiler import AsmCompiler
from parsers.object_code_parser import ObjectCodeParser
from announcers.announcer import Announcer


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
        result = input(IDS_ASSEMBLY_CODE_PATH_PROMPT_FORMAT\
            .format(ASM_CODE_PATH)).strip()
        if len(result.strip()) == 0:
            result = ASM_CODE_PATH  # use default
        result = os.path.expanduser(result)
        BlankLinePrinter.Print()
        return result
    
    @staticmethod
    def Extract():
        Announcer.AnnounceAttemptingExtractShellcodeBytes()
        
        strCodePath = ShellcodeExtractor.__DoGetAsmCodePath()
        
        if not ShellcodeExtractor.__DoCheckSourceCodeExists(strCodePath):
            Footer.Print()
            PressEnterToReturnToMainMenu.Print()
            return ()
        
        if not ShellcodeExtractor.__DoCompileSourceCode(strCodePath):
            print(ERROR_FAILED_COMPILE_FILE.format(
                strCodePath))
            Footer.Print()
            PressEnterToReturnToMainMenu.Print()
            return ()
        
        strObjectCodePath = \
            ShellcodeExtractor.__DoFormulateObjectCodePath(strCodePath)    
    
        shellcode = \
            ShellcodeExtractor\
            .__DoConvertObjectCodeToShellCode(strObjectCodePath)
    
        return shellcode