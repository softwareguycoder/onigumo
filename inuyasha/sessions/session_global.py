class SessionGlobal(object):
    
    @staticmethod
    def GetSession():
        return SessionGlobal.__theSession
    
    @staticmethod
    def SetSession(value):
        SessionGlobal.__theSession = value