class ExecResponseParser(object):
    
    @staticmethod
    def GetExecReturnCode(status):
        if not len(status.strip()) \
            or not status.strip().startswith('2'):
            return -32767  # return some nonsense value
        partsOfStatus = list(status.strip().split())
        retcode = int((''.join(partsOfStatus[-1]))\
                      .replace('.', ''))
        return retcode
    
    @staticmethod
    def GetExecReturnCodeAndErrno(status):
        if not len(status.strip()) \
            or not status.strip().startswith('2') \
            and not status.strip().startswith('4'):
            return -32767  # return some nonsense value
        partsOfStatus = list(status.strip().split())
        retcode = int((''.join(partsOfStatus[-3]))\
                      .replace(',', ''))
        errno = int((''.join(partsOfStatus[-1]))\
                    .replace('.', ''))
        return (retcode, errno)
