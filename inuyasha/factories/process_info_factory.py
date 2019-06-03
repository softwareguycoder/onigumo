from entities.process_info import ProcessInfo

class ProcessInfoFactory(object):
    @staticmethod
    def Make(self, line):
        return ProcessInfo.fromPsExecOutputLine(line)