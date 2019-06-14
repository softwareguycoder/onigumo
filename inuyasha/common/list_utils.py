class ListUtilities(object):
    @staticmethod
    def PrintEltsSeparatedBy(char, theList):
        print(str(char).join(map(str,theList)))
        pass
