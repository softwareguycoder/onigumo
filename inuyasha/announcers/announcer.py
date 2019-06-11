from common.inuyasha_symbols import IDS_SERVER_CPU_INFO_HEADER, \
    IDS_NOW_CONNECTED_TO_SERVER, IDS_LIST_SERVER_DIR_HEADER, \
    IDS_SERVER_DIR_LIST_HEADER, IDS_PICK_PROCESS_TO_KILL_HEADER, \
    IDS_REMOTE_PROCESS_LIST_HEADER, IDS_TRYING_TO_KILL_PROC_WITH_PID_HEADER, \
    ERROR_FAILED_SERIALIZE_SHELLCODE, ERROR_FAILED_SEND_SHELLCODE, \
    ERROR_FAILED_KILL_REMOTE_PROCESS, SHELLCODE_EXECUTION_SUCCEEDED, \
    ERROR_REMOTE_PROCESS_ACCESS_DENIED, ERROR_REMOTE_PROCESS_NOT_EXISTS, \
    ERROR_FAILED_EXECUTE_SHELLCODE, IDS_ATTEMPTING_SERIALIZE_SHELLCODE, \
    IDS_ATTEMPTING_EXTRACT_SHELLCODE_BYTES, IDS_ATTEMPTING_COMPILE_SHELLCODE


class Announcer(object):

    @staticmethod
    def AnnounceAttemptingCompileShellcode(strCodePath):
        print(IDS_ATTEMPTING_COMPILE_SHELLCODE\
              .format(strCodePath.strip()))
        print()

    @staticmethod
    def AnnounceAttemptingExtractShellcodeBytes():
        print(IDS_ATTEMPTING_EXTRACT_SHELLCODE_BYTES)
        print()
        pass

    @staticmethod
    def AnnounceAttemptingToSerializeShellcode():
        print(IDS_ATTEMPTING_SERIALIZE_SHELLCODE)
        print()
        pass

    @staticmethod
    def AnnounceShellcodeExecutionFailed():
        print(ERROR_FAILED_EXECUTE_SHELLCODE)
        print()
        pass
    
    @staticmethod
    def AnnounceRemoteProcessNotFound():
        print(ERROR_REMOTE_PROCESS_NOT_EXISTS)
        print()

    @staticmethod
    def AnnounceRemoteProcessAccesDenied():
        print(ERROR_REMOTE_PROCESS_ACCESS_DENIED)
        print()

    @staticmethod
    def AnnounceShellcodeExecutionSucceeded():
        print(SHELLCODE_EXECUTION_SUCCEEDED)
        print()

    @staticmethod
    def AnnounceFailedKillProcess():
        print(ERROR_FAILED_KILL_REMOTE_PROCESS)
        pass

    @staticmethod
    def AnnounceConnectedToServer():
        print()
        print(IDS_NOW_CONNECTED_TO_SERVER)
        pass
    
    @staticmethod
    def AnnounceServerCPUInfo():
        print(IDS_SERVER_CPU_INFO_HEADER)
        print()
        pass

    @staticmethod
    def AnnounceListServerDirectory():
        print(IDS_LIST_SERVER_DIR_HEADER)
        print()
        pass
        
    @staticmethod
    def AnnounceServerDirListed(dirChosen):
        if not len(dirChosen.strip()):
            return
        print(IDS_SERVER_DIR_LIST_HEADER.format(dirChosen.strip()))
        print()
        pass
    
    @staticmethod
    def AnnouncePickProcessToKillPicker():
        print(IDS_PICK_PROCESS_TO_KILL_HEADER)
        print()
        pass
    
    @staticmethod
    def AnnounceRemoteProcessList():
        print(IDS_REMOTE_PROCESS_LIST_HEADER)
        print()
        pass
    
    @staticmethod
    def AnnounceTryingToKillProcWithPid(pid):
        print(
            IDS_TRYING_TO_KILL_PROC_WITH_PID_HEADER.format(pid))
        print()
        pass
    
    @staticmethod
    def AnnounceFailedSerializeShellcode():
        print(ERROR_FAILED_SERIALIZE_SHELLCODE)
        print()
        pass
    
    @staticmethod
    def AnnounceFailedSendShellcode():
        print(ERROR_FAILED_SEND_SHELLCODE)
        print()
        pass
    
