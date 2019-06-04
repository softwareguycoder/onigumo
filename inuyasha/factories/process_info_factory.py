from entities.process_info import ProcessInfo

class ProcessInfoFactory(object):
    @staticmethod
    def Make(line):
        return ProcessInfo.fromPsExecOutputLine(line)