import os


class DirectoryUtilities(object):
    
    @staticmethod
    def RemoveDirectory(path):
        if len(os.path.expanduser(path).strip()) == 0:
            return False  # Failed
        expandedPath = os.path.expanduser(path.strip())
        if not os.path.isdir(expandedPath):
            return False  # Failed
        os.rmdir(expandedPath)
        return not os.path.isdir(expandedPath)
    
    @staticmethod
    def CreateDirectory(path):
        if len(path.strip()) == 0:
            return False
        expandedPath = os.path.expanduser(path.strip())
        if os.path.isdir(expandedPath):
            return False
        os.mkdir(expandedPath)
        return os.path.isdir(expandedPath)
    
    @staticmethod
    def SetCurrentDirectory(path):
        if len(path.strip()) == 0:
            return
        expandedPath = os.path.expanduser(path.strip())
        os.chdir(expandedPath)
        
    @staticmethod
    def Exists(path):
        if len(path.strip()) == 0:
            return False
        expandedPath = os.path.expanduser(path.strip())
        return os.path.isdir(expandedPath)
