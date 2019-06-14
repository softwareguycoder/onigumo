from console.console_class import Console
from common.gui_utilities import Banner, ThankYouMessagePrinter, Footer
import os


class ExitHandler(object):

    @staticmethod
    def ExitApp(nExitCode, sessionObject):
        if sessionObject is not None:
            sessionObject.End(nExitCode)
            return
        
        Console.Clear()
        Banner.Print()
        ThankYouMessagePrinter.Print()
        Footer.Print()
        os._exit(nExitCode)
