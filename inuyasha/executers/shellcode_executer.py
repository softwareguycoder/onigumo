from common.inuyasha_symbols import PROTOCOL_EXEC_COMMAND_FORMAT, \
    ERROR_FAILED_ESTABLISH_SESSION, ERROR_PID_INVALID_VALUE, \
    ERRNO_REMOTE_PROCESS_ACCESS_DENIED, \
    ERRNO_REMOTE_PROCESS_NOT_EXISTS, SHELLCODE_EXECUTION_RESPONSE_ERROR_STATUS, \
    OK, SHELLCODE_EXECUTION_RESPONSE_SUCCESS_STATUS, INVALID_PID_VALUE
from common.gui_utilities import Footer, PressEnterToReturnToMainMenu
from announcers.announcer import Announcer
from parsers.exec_response_parser import ExecResponseParser


class ShellcodeExecuter(object):
        
    @staticmethod
    def __DoDisplayResults(status):
        if not len(status.strip()):
            return False
        
        nReturnCode = nErrno = 0
        if status.startswith(SHELLCODE_EXECUTION_RESPONSE_ERROR_STATUS):
            nReturnCode, nErrno = ExecResponseParser\
                .GetExecReturnCodeAndErrno(status)
            if nReturnCode == OK:
                Announcer.AnnounceShellcodeExecutionSucceeded()
                return False
            elif nErrno == ERRNO_REMOTE_PROCESS_ACCESS_DENIED:
                Announcer.AnnounceRemoteProcessAccesDenied()
                return False
            elif nErrno == ERRNO_REMOTE_PROCESS_NOT_EXISTS:
                Announcer.AnnounceRemoteProcessNotFound()
                return False
        elif status.startswith(SHELLCODE_EXECUTION_RESPONSE_SUCCESS_STATUS): 
            nReturnCode = ExecResponseParser.GetExecReturnCode(status)
            if nReturnCode == OK:
                Announcer.AnnounceShellcodeExecutionSucceeded()
            else:
                Announcer.AnnounceShellcodeExecutionFailed()
                return False

        return True
        
    @staticmethod
    def Execute(theSocket, pid):
        if not theSocket:
            print(ERROR_FAILED_ESTABLISH_SESSION)
            Footer.Print()
            PressEnterToReturnToMainMenu.Print()
            return False
            
        if pid <= INVALID_PID_VALUE:
            print(ERROR_PID_INVALID_VALUE)        
            Footer.Print()
            PressEnterToReturnToMainMenu.Print()
            return False
            
        theSocket.Send(PROTOCOL_EXEC_COMMAND_FORMAT.format(pid))
        status = theSocket.Receive()
        if not status.startswith('2') and not status.startswith('4'):
            Announcer.AnnounceFailedSendShellcode()
            Footer.Print()
            PressEnterToReturnToMainMenu.Print()
            return False
        
        return ShellcodeExecuter.__DoDisplayResults(status)
    
