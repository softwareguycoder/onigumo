class StringUtils:
    
    @staticmethod
    def IsNullOrWhiteSpace(pszTest):
        result = True
        if (pszTest is None):
            return result
        if (len(pszTest.strip()) == 0):
            return result
        return False
    
