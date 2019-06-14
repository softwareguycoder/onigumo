from common.string_utils import StringUtilities
class ListUtilities(object):
    @staticmethod
    def FormatEltsSeparatedBy(char, theList):
        if StringUtilities.IsNullOrWhiteSpace(char):
            return ''
        return str(char).join(map(str, theList))
    
    @staticmethod
    def PrintEltsSeparatedBy(char, theList):
        print(str(char).join(map(str,theList)))
        pass
