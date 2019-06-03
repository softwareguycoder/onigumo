from common.string_utils import StringUtilities
from common.inuyasha_symbols import EXIT_FAILURE, IDS_PRESS_ENTER_TO_CONTINUE
class ErrorHandler(object):
    
    @staticmethod
    def ShowErrorThenExit(strError):
        if StringUtilities.IsNullOrWhiteSpace(strError):
            exit(EXIT_FAILURE)
        print(strError)
        input(IDS_PRESS_ENTER_TO_CONTINUE)
        exit(EXIT_FAILURE)