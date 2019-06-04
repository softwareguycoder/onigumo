from common.string_utils import StringUtilities
from common.inuyasha_symbols import EXIT_FAILURE, IDS_PRESS_ENTER_TO_CONTINUE
import consolemenu
class ErrorHandler(object):
    
    @staticmethod
    def ShowErrorThenExit(strError):
        if StringUtilities.IsNullOrWhiteSpace(strError):
            exit(EXIT_FAILURE)
        consolemenu.clear_terminal()
        print(strError)
        input(IDS_PRESS_ENTER_TO_CONTINUE)
        exit(EXIT_FAILURE)