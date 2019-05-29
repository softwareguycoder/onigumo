class PortNumberValidator:
    
    @staticmethod
    def isPortNumberValid(nPortNum):
        return nPortNum >= 1024 and nPortNum < 49151
        