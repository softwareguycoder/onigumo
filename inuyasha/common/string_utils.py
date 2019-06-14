class StringUtilities(object):

        @staticmethod
        def IsNullOrWhiteSpace(value):
            if not isinstance(value, str):
                return False
            # assume value is a string
            return value == '' or len(value.strip()) == 0
                
        @staticmethod
        def ReplaceFromRight(strValue, strFindWhat, 
            strReplaceWith, nOccurrences):
            if not strValue or not len(strValue.strip()):
                return strValue
            if not strFindWhat or not isinstance(strFindWhat, str):
                return strValue
            if not strReplaceWith or not isinstance(strReplaceWith, str):
                return strValue
            if not nOccurrences or not isinstance(nOccurrences, int):
                return strValue
            if nOccurrences <= 0:
                return strValue
            partsList = strValue.rsplit(strFindWhat, nOccurrences)
            return strReplaceWith.join(partsList)
