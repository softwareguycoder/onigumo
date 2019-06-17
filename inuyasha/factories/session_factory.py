from sessions.session import Session
from sessions.session_global import SessionGlobal

class SessionFactory(object):

    @staticmethod
    def Make():
        theSession = Session.Start()
        SessionGlobal.SetSession(theSession)
        return theSession
