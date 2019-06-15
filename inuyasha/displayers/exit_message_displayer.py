from console.console_class import Console
from common.banner import Banner
from common.thank_you_message_printer import ThankYouMessagePrinter
from common.footer import Footer


class ExitMessageDisplayer(object):

    @staticmethod
    def Display():
        print("no yay")
        Console.Clear()
        Banner.Print()
        ThankYouMessagePrinter.Print()
        Footer.Print()
        pass
