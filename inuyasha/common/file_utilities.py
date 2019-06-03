from common.inuyasha_symbols import FILE_MODE_READ, FILE_MDOE_WRITE,\
    EMPTY_STRING
import os

class FileUtilities(object):
    @staticmethod
    def GetLength(path):
        if len(path.strip()) == 0:
            return 0
        expandedPath = os.path.expanduser(path.strip())
        if not os.path.isfile(expandedPath):
            return 0
        return int(os.stat(expandedPath).st_size)
    
    @staticmethod
    def Exists(path):
        if len(path.strip()) == 0:
            return False
        expandedPath = os.path.expanduser(path.strip())
        if os.path.isdir(expandedPath):
            return False
        return os.path.isfile(expandedPath)
    
    @staticmethod
    def ReadAllText(path):
        if len(path.strip()):
            return EMPTY_STRING       
        expandedPath = os.path.expanduser(path.strip())
        with open(expandedPath, FILE_MODE_READ) as f:
            if not f.mode == FILE_MODE_READ:
                return EMPTY_STRING
            result = str(f.read())
        return result
    
    @staticmethod
    def WriteAllText(path, text):
        if len(path.strip()) == 0:
            return False
        expandedPath = os.path.expanduser(path.strip())
        if os.path.isfile(expandedPath) or len(text.strip()) == 0:
            os.remove(path)
        with open(expandedPath, FILE_MDOE_WRITE) as f:
            if not f.mode == FILE_MDOE_WRITE:
                return False
            f.write(text)
        return os.path.isfile(expandedPath)
    
    