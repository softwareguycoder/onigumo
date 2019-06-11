from common.inuyasha_symbols import CONTINUATION_PROMPT, LF,\
    DEFAULT_PAGE_LINE_COUNT
from console.console_class import Console
from common.gui_utilities import Banner


class Paginator(object):
    
    @staticmethod
    def __PrintLine(strLine):
        if not len(strLine.strip()):
            return
        print(strLine.strip())
    
    @staticmethod
    def __ShouldStop():
        print()
        strValue=input(CONTINUATION_PROMPT)
        return strValue.strip().lower() == 'q'
    
    @staticmethod
    def Paginate(textLineIterator, promptCallable=None, 
        userState=None, nLineCount=DEFAULT_PAGE_LINE_COUNT):
        nPageNumber = 1
        for nIndex , strLine in enumerate(textLineIterator):
            if nIndex % nLineCount == 1 and nIndex != 1:
                nPageNumber += 1
                Console.Clear()
                Banner.Print()
                if promptCallable:
                    promptCallable(nPageNumber, userState)
                Paginator.__PrintLine(strLine)
            elif nIndex % nLineCount == 0 and nIndex:
                if Paginator.__ShouldStop():
                    break
            else:
                Paginator.__PrintLine(strLine)