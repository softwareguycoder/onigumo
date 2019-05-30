class StringUtils:
       
    @staticmethod
    def IsNullOrWhiteSpace(pszTest):
        result = True
        if (pszTest is None):
            return result
        if (len(pszTest.strip()) == 0):
            return result
        return False
    
    @staticmethod
    def GetLineCharCount(pszTest):
        result = 0
        if (StringUtils.IsNullOrWhiteSpace(pszTest)):
            return result
        if pszTest[len(pszTest)-1] == '\n':
            for i in range(len(pszTest)):
                if (pszTest[i] != '\n'):
                    result++
        return result
                