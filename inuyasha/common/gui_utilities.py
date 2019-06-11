from common.inuyasha_symbols import IDS_WELCOME, \
    IDS_PRESS_ENTER_TO_CONTINUE, IDS_PRESS_ENTER_TO_RETURN_TO_MAIN_MENU, \
    IDS_FOOTER, IDS_STAR_ROW, IDS_SOFTWARE_TITLE, IDS_THANK_YOU_MESSAGE, \
    IDS_PRESS_ENTER_TO_PICK_PROC_TO_KILL


class Banner(object):

    @staticmethod
    def Print():
        print(IDS_STAR_ROW)
        print(IDS_SOFTWARE_TITLE)
        print(IDS_STAR_ROW)
        print('')
        pass


class BlankLinePrinter(object):
    
    @staticmethod
    def Print():
        print('')
        pass
    
    
class WelcomePrinter(object):

    @staticmethod
    def Print():
        print(IDS_WELCOME)
        print('')
        pass


class Footer(object):

    @staticmethod
    def Print():
        print()
        print(IDS_FOOTER)
        
        
class WaitForEnterToContinue(object):

    @staticmethod
    def Print():
        try:
            input(IDS_PRESS_ENTER_TO_CONTINUE)
        except KeyboardInterrupt as e:
            raise e
        pass


class PressEnterToReturnToMainMenu(object):

    @staticmethod
    def Print():
        try:
            input(IDS_PRESS_ENTER_TO_RETURN_TO_MAIN_MENU)
        except KeyboardInterrupt as e:
            raise e
        pass
    

class ThankYouMessagePrinter(object):
    
    @staticmethod
    def Print():
        print(IDS_THANK_YOU_MESSAGE)
        pass
    

class PressEnterToPickProcToKill(object):
    
    @staticmethod
    def Print():
        input(IDS_PRESS_ENTER_TO_PICK_PROC_TO_KILL)
        pass
