from prompters.prompter import Prompter
from common.inuyasha_symbols import IDS_PRESS_ENTER_TO_PICK_PROC_TO_KILL
from common.exit_handler import ExitHandler


class PressEnterToPickProcToKill(object):
    
    @staticmethod
    def Print():
        Prompter.PressAnyKeyToContinue(
            strPrompt=IDS_PRESS_ENTER_TO_PICK_PROC_TO_KILL,
            keyboardInterruptHandler=ExitHandler.ExitApp)
        pass
