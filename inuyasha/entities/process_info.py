from common.string_utils import StringUtilities


class ProcessInfo(object):

    def get_PID(self):
        return self.__pid
    
    def get_TTY(self):
        return self.__tty
    
    def get_CPUTIME(self):
        return self.__cputime
    
    def get_CMD(self):
        return self.__cmd
    
    def set_PID(self, value):
        self.__pid = value
        
    def set_TTY(self, value):
        self.__tty = value
        
    def set_CPUTIME(self, value):
        self.__cputime = value
        
    def set_CMD(self, value):
        self.__cmd = value
        
    @classmethod
    def __makeInvalid(cls):
        '''
        Makes an instance of this class whose properties are all set
        to invalid values, on purpose.  This allows us to return a non-None
        variable from classmethods without initializing them to anything
        '''
        return cls(-1, '', '', '')   
    
    @classmethod
    def fromPsExecOutputLine(cls, line):
        if StringUtilities.IsNullOrWhiteSpace(line):
            return ProcessInfo.__makeInvalid(cls)
        elts = line.split()
        if len(elts) != 4:
            return ProcessInfo.__makeInvalid(cls)
        return cls(int(elts[0]), elts[1], elts[2], elts[3])
        
    # line: 1015 tty1     00:00:00 gnome-session-b
    def __init__(self, pid, tty, cputime, cmd):
        self.__pid = pid
        self.__tty = tty
        self.__cputime = cputime
        self.__cmd = cmd
