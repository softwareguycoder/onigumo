from prompters.prompter import Prompter
from common.inuyasha_symbols import IDS_PRESS_ENTER_TO_RETURN_TO_MAIN_MENU
from common.exit_handler import ExitHandler


class PressEnterToReturnToMainMenu(object):

    @staticmethod
    def Print():
        Prompter.PressAnyKeyToContinue(
            strPrompt=IDS_PRESS_ENTER_TO_RETURN_TO_MAIN_MENU,
            keyboardInterruptHandler=ExitHandler.ExitApp)
        pass
