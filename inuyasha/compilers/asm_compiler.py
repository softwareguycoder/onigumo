import subprocess

from common.string_utils import StringUtilities
from common.file_utilities import FileUtilities
from common.inuyasha_symbols import \
    NASM_SHELL_COMMAND_FORMAT, OK, ERROR_FAILED_DETERMINE_TEMP_DIR_PATH,\
    ERROR_FAILED_FIND_FILE_FORMAT
import os
from common.path_utilities import PathUtilities
from common.directory_utilities import DirectoryUtilities
from common.error_handler import ErrorHandler


class AsmCompiler(object):
    
    @staticmethod
    def __NASMCompile(asmpath):
        if StringUtilities.IsNullOrWhiteSpace(asmpath):
            ErrorHandler.ShowErrorThenExit(
                ERROR_FAILED_FIND_FILE_FORMAT.format(asmpath))
        expandedPath = os.path.expanduser(asmpath)
        if not FileUtilities.Exists(expandedPath):
            ErrorHandler.ShowErrorThenExit(
                ERROR_FAILED_FIND_FILE_FORMAT.format(expandedPath))
        filename_only = PathUtilities.GetFileNameWithoutExtension(expandedPath)
        args = NASM_SHELL_COMMAND_FORMAT.format(filename_only,
            filename_only).split()
        code_dir = PathUtilities.GetDirectoryName(expandedPath)
        if StringUtilities.IsNullOrWhiteSpace(code_dir):
            ErrorHandler.ShowErrorThenExit(
                ERROR_FAILED_DETERMINE_TEMP_DIR_PATH)       
        DirectoryUtilities.SetCurrentDirectory(code_dir)
        completedProc = subprocess.run(args)
        return OK == completedProc.returncode
    

    @staticmethod
    def CompileWitNasm(asmpath):
        return AsmCompiler.__NASMCompile(asmpath)                      
