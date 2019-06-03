import os
from common.inuyasha_symbols import EMPTY_STRING

class PathUtilities(object):
    
    @staticmethod
    def GetDirectoryName(path):
        if len(path.strip()) == 0:
            return EMPTY_STRING
        expandedPath = os.path.expanduser(path.strip())      
        if not os.path.isfile(expandedPath):
            return EMPTY_STRING
        return os.path.dirname(expandedPath)
    
    @staticmethod
    def GetFileNameWithoutExtension(path):
        if len(path.strip()) == 0:
            return EMPTY_STRING
        expandedPath = os.path.expanduser(path.strip())      
        if not os.path.isfile(expandedPath):
            return EMPTY_STRING
        return os.path.splitext(os.path.basename(expandedPath))[0]
    
    