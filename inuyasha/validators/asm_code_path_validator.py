from common.file_utilities import FileUtilities
from announcers.announcer import Announcer


class AsmCodePathValidator(object):
    
    @staticmethod
    def HandleInvalidInput(strCodePath):
        Announcer.AnnounceFailedFindShellcodeSource(strCodePath)
        pass
    
    @staticmethod
    def IsValid(strCodePath, choiceValueSet=[]):  # @UnusedVariable
        return FileUtilities.Exists(strCodePath)
