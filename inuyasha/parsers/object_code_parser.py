
import os

from common.string_utils import StringUtilities
from common.inuyasha_symbols import EMPTY_BYTES, ERROR_FAILED_FIND_FILE_FORMAT,\
    HEXADECIMAL_BASE, ERROR_FAILED_EXTRACT_SHELLCODE, OBJDUMP_SYSTEM_COMMAND
from common.file_utilities import FileUtilities
from common.error_handler import ErrorHandler

class ObjectCodeParser(object):
    
    @staticmethod
    def __ChangeListToShellCodeBytes(theList):
        if (len(theList) == 0):
            ErrorHandler.ShowErrorThenExit(            
                ERROR_FAILED_EXTRACT_SHELLCODE)
            return ([], 0)
    
        output = []
        for i in range(len(theList)):
            items = list (theList[i]
            .strip()
            .replace(" ", "\t")
            .split("\t"))
            
            for j in range(0, len(items)):
                output.append(int(items[j], HEXADECIMAL_BASE))

        if len(output) == 0:
            ErrorHandler.ShowErrorThenExit(            
                ERROR_FAILED_EXTRACT_SHELLCODE)
        output = bytes(output)    
        output_len = len(output)
        return (output, output_len)

    @staticmethod
    def __ProduceShellCode(path):
        if StringUtilities.IsNullOrWhiteSpace(path):
            return ([], 0)
        if not FileUtilities.Exists(path):
            return ([], 0)
        with os.popen(OBJDUMP_SYSTEM_COMMAND.format(path)) as f:
            if f is None:
                ErrorHandler.ShowErrorThenExit(            
                    ERROR_FAILED_EXTRACT_SHELLCODE)
                return
            theList = list(filter(None, f))    
            return ObjectCodeParser.__ChangeListToShellCodeBytes(theList)
        
    
    @staticmethod
    def __IsShellCodeEmpty(shellcodeOutput):
        return shellcodeOutput is None or \
            len(shellcodeOutput[0]) == 0 and shellcodeOutput[1] == 0
            
    
    @staticmethod
    def ObjectCodeToShellCode(objfilepath):
        if StringUtilities.IsNullOrWhiteSpace(objfilepath):
            return EMPTY_BYTES
        expandedPath = os.path.expanduser(objfilepath.strip())
        if not FileUtilities.Exists(expandedPath):
            ErrorHandler.ShowErrorThenExit(
                ERROR_FAILED_FIND_FILE_FORMAT.format(expandedPath))
        shellcodeOutput = ObjectCodeParser.__ProduceShellCode(expandedPath)
        if ObjectCodeParser.__IsShellCodeEmpty(shellcodeOutput):
            ErrorHandler.ShowErrorThenExit(
                ERROR_FAILED_EXTRACT_SHELLCODE)
        return shellcodeOutput  # is a tuple (shellcode_bytes, length)

