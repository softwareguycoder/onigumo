from prompters.prompter import Prompter
from common.inuyasha_symbols import IDS_PRESS_ENTER_TO_RETURN_TO_MAIN_MENU,\
    EXIT_SUCCESS
from common.exit_handler import ExitHandler
from displayers.exit_message_displayer import ExitMessageDisplayer


class PressEnterToReturnToMainMenu(object):

    @staticmethod
    def Print():        
        Prompter.PressAnyKeyToContinue(
            strPrompt=IDS_PRESS_ENTER_TO_RETURN_TO_MAIN_MENU,
            keyboardInterruptHandler=lambda: \
            ExitHandler.ExitApp(EXIT_SUCCESS, \
            exitMessageDisplayer=ExitMessageDisplayer.Display()))
        pass
