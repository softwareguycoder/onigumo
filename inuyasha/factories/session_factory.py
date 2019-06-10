from sessions.session import Session

class SessionFactory(object):

    @staticmethod
    def Make():
        return Session.Start()
