from prompters.prompter import Prompter
from common.inuyasha_symbols import IDS_PRESS_ENTER_TO_CONTINUE
from common.exit_handler import ExitHandler


class WaitForEnterToContinue(object):

    @staticmethod
    def Print():
        Prompter.PressAnyKeyToContinue(
            strPrompt=IDS_PRESS_ENTER_TO_CONTINUE,
            keyboardInterruptHandler=ExitHandler.ExitApp)
        pass
