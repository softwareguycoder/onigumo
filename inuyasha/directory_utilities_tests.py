import os
from test_symbols import TMP_MY_DIR
from asserts import Assert
from common.directory_utilities import DirectoryUtilities


class DirectoryUtilitiesTests(object):
    
    @staticmethod
    def TestRemoveDirectory():
        test_dir_path = os.path.expanduser(TMP_MY_DIR)
        os.mkdir(os.path.expanduser(test_dir_path))
        Assert.IsTrue(os.path.isdir(test_dir_path))
        
        DirectoryUtilities.RemoveDirectory(test_dir_path)
        
        Assert.IsFalse(os.path.isdir(test_dir_path))

    
    @staticmethod    
    def TestCreateDirectory():
        pass
    
    @staticmethod
    def TestSetCurrentDirectory():
        pass
    
    
    @staticmethod
    def TestExists():
        pass
    
