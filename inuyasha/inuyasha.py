#!/usr/bin/python3
from os import system, name
from sockets.socket_wrapper import SocketWrapper
from common.inuyasha_symbols import EXIT_FAILURE, IDS_PRESS_ENTER_TO_CONTINUE, \
    PROTOCOL_HELO_COMMAND, PROTOCOL_QUIT_COMMAND, \
    PROTOCOL_INFO_COMMAND, PROTOCOL_LDIR_COMMAND, PROTOCOL_LIST_COMMAND, \
    OBJECT_CODE_PATH, PROTOCOL_CODE_COMMAND_FORMAT, \
    PROTOCOL_EXEC_COMMAND_FORMAT, EXIT_SUCCESS, ASM_CODE_PATH
from pkg_resources._vendor.pyparsing import line
from compilers.asm_compiler import AsmCompiler
from parsers.object_code_parser import ObjectCodeParser
from translators.bytes_to_base64_translator import BytesToBase64Translator
import os


# define our clear function 
def clear(): 
  
    # for windows 
    if name == 'nt': 
        _ = system('cls') 
  
    # for mac and linux(here, os.name is 'posix') 
    else: 
        _ = system('clear') 
        
        
def prompt_user_for_server_and_connect():
    clear()
    print("*************************************************************************")
    print("***                         INUYASHA CLIENT                           ***")
    print("*************************************************************************")
    print()
    print("             ***   Welcome to the Inuyasha Client!   ***")
    print()
    hostname = input("> Server address [localhost]: > ")
    if len(hostname.strip()) <= 0:
        hostname = 'localhost'
    portString = input("> Server port [9000]: > ")
    port = 0
    if len(portString.strip()) == 0:
        port = 9000
    else:
        port = int(portString)
    global SOCKET
    SOCKET = SocketWrapper(hostname, port)
    if SOCKET is None:
        print("ERROR: Failed to connect.")
        exit(EXIT_FAILURE)
        
    SOCKET.Connect()
    SOCKET.Send(PROTOCOL_HELO_COMMAND)
    _ = SOCKET.Receive()
    global CONNECTED
    CONNECTED = True
    print()
    print("               *** Now connected to the server!!! ***")
    print()
    print("*************************************************************************")
    input(IDS_PRESS_ENTER_TO_CONTINUE)


def print_menu():
    clear()
    print("*************************************************************************")
    print("***                         INUYASHA CLIENT                           ***")
    print("*************************************************************************")
    print()
    print("                            ***  Main Menu  ***            ")
    print()
    print("Pick an option:")
    print()
    print("   1) List CPU info")
    print("   2) List Directory Contents")
    print("   3) Kill a Process")
    print("   4) Exit")
    print()
    print("*************************************************************************")
    value = int(input("Choice: >> "))
    return value
    
    
def get_cpu_info():
    clear()
    print("*************************************************************************")
    print("***                         INUYASHA CLIENT                           ***")
    print("*************************************************************************")
    print()
    print(" Server's CPU Info:")
    print()
    SOCKET.Send(PROTOCOL_INFO_COMMAND)
    status = SOCKET.Receive()
    if status.startswith('2'):
        lines = SOCKET.ReceiveAllLines()
        for line in lines:
            if not line.strip():
                continue
            print(line.strip())
        print()
    else:
        print(">>> ERROR: Failed to get CPU Info.")
    print()
    print("*************************************************************************")
    input("Press ENTER to go back to Main Menu")


def get_dir_listing():
    clear()
    print("*************************************************************************")
    print("***                         INUYASHA CLIENT                           ***")
    print("*************************************************************************")
    print()
    print("                   ***   List Server Directory   ***")
    print()
    dirChosen = input("> Server directory path [ENTER for home]: > ")
    if not dirChosen.strip():
        dirChosen = "~"
    print()
    clear()
    print("*************************************************************************")
    print("***                         INUYASHA CLIENT                           ***")
    print("*************************************************************************")
    print()
    print("                   ***   List Server Directory   ***")
    print()
    print("Listing of " + dirChosen.strip() + ": ")
    print()
    SOCKET.Send(PROTOCOL_LDIR_COMMAND + " " + dirChosen.strip() + "\n")
    status = SOCKET.Receive()
    if status.startswith('2'):
        lines = SOCKET.ReceiveAllLines()
        for line in lines:
            if not line.strip():
                continue
            print(line.strip())
        print()
    else:
        print(">>> ERROR: Failed to get dir listing!")
    print()
    print("*************************************************************************")
    input("Press ENTER to go back to Main Menu")


def pick_proc_to_kill():
    clear()
    print("*************************************************************************")
    print("***                         INUYASHA CLIENT                           ***")
    print("*************************************************************************")
    print()
    print("         *** Run Some Shellcode that Will Kill A Process ***")
    print()
    input("> Press ENTER when ready to pick a process to kill >")
    clear()
    print("*************************************************************************")
    print("***                         INUYASHA CLIENT                           ***")
    print("*************************************************************************")
    print()
    print(" Here are the processes on the remote machine:")
    print()
    SOCKET.Send(PROTOCOL_LIST_COMMAND)
    status = SOCKET.Receive()
    pid = -1
    if status.startswith('2'):
        lines = SOCKET.ReceiveAllLines()
        for line in lines:
            if not line.strip():
                continue
            print(line.strip())
        print()
        pid = int(input("> PID of the process to kill: > "))   
    else:
        print(">>> ERROR: Failed to get the list of processes.")
        print()
        print("*************************************************************************")
        input("Press ENTER to go back to Main Menu")
        return
    return pid      


def end_session(exit_code):
    global CONNECTED
    SOCKET.Send(PROTOCOL_QUIT_COMMAND)
    _ = SOCKET.Receive()
    CONNECTED = False
    clear()
    print("*************************************************************************")
    print("***                         INUYASHA CLIENT                           ***")
    print("*************************************************************************")
    print()
    print("Thank you for using the Inuyasha Client (of the Onigumo Server)!")
    print()
    print("*************************************************************************")
    exit(exit_code)


def get_exec_return_code(strStatusReply):
    if not len(strStatusReply.strip()) \
        or not strStatusReply.strip().startswith('2'):
        return -32767  # return some nonsense value
    partsOfStatus = list(strStatusReply.strip().split())
    retcode = int((''.join(partsOfStatus[-1]))\
                  .replace(',',''))
    return retcode


def get_exec_return_code_and_errno(strStatusReply):
    if not len(strStatusReply.strip()) \
        or not strStatusReply.strip().startswith('2') \
        and not strStatusReply.strip().startswith('4'):
        return -32767  # return some nonsense value
    partsOfStatus = list(strStatusReply.strip().split())
    retcode = int((''.join(partsOfStatus[-3]))\
                  .replace(',',''))
    errno = int((''.join(partsOfStatus[-1]))\
                .replace('.',''))
    return (retcode, errno)

        
def send_shellcode_and_kill_proc(pid):
    clear()
    print("*************************************************************************")
    print("***                         INUYASHA CLIENT                           ***")
    print("*************************************************************************")
    print()
    print("     ***  Trying to Kill the Remote Process with PID:", pid, "***")
    print()
    if pid == -1:
        end_session(EXIT_FAILURE)
    strCodePath = input("> Path to assembly code to send as " \
                        "shellcode [{}]: > ".format(ASM_CODE_PATH))
    if len(strCodePath.strip()) == 0:
        strCodePath = ASM_CODE_PATH     # use default
    strCodePath = os.path.expanduser(strCodePath.strip())
    if not os.path.exists(strCodePath):
        print("Failed to find any shellcode source at '{}'."\
              .format(strCodePath))
        input(IDS_PRESS_ENTER_TO_CONTINUE)
        end_session(EXIT_FAILURE)
    print("Compiling file '{}'...".format(strCodePath))
    if not AsmCompiler.CompileWitNasm(strCodePath):
        end_session(EXIT_FAILURE)
    strObjectCodePath = strCodePath.replace('.asm', '.o')
    print("Extracting shellcode bytes from", strObjectCodePath, "...")
    shellcode = \
        ObjectCodeParser.ObjectCodeToShellCode(strObjectCodePath)
    if shellcode is None or shellcode[1] <= 0:
        end_session(EXIT_FAILURE)
    print("Sending shellcode to server...")
    shellcode_encoded = BytesToBase64Translator.Translate(shellcode[0])
    if len(shellcode_encoded) == 0:
        print(">>> ERROR: Could not send shellcode!")
        input(IDS_PRESS_ENTER_TO_CONTINUE)
        end_session(EXIT_FAILURE)
        return
    else:
        SOCKET.Send(PROTOCOL_CODE_COMMAND_FORMAT.format(len(shellcode_encoded)))
        status = SOCKET.Receive()
        if not status.startswith('2'):
            print(">>> ERROR: Could not send shellcode!")
        else:
            SOCKET.Send(shellcode_encoded + '\n')
            SOCKET.Send('.\n')
            status = SOCKET.Receive()
            if not status.startswith('2'):
                print(">>> ERROR: Could not send shellcode!")
            else:
                SOCKET.Send(PROTOCOL_EXEC_COMMAND_FORMAT.format(pid))
                status = SOCKET.Receive()
            if not status.startswith('2') and not status.startswith('4'):
                print(">>> ERROR: Could not send shellcode!")
                input(IDS_PRESS_ENTER_TO_CONTINUE)
                end_session(EXIT_FAILURE)
            else:
                nReturnCode = nErrno = 0
                if status.startswith('4'):
                    nReturnCode, nErrno = get_exec_return_code_and_errno(status)
                elif status.startswith('2'): 
                    nReturnCode = get_exec_return_code(status)
                    if nReturnCode == 0:
                        print("The execution of the shellcode succeeded.")
                    else:
                        print(">>> ERROR: Shellcode execution failed.")
                
                if status.startswith('4'):
                    if nReturnCode == 0:
                        print("The execution of the shellcode succeeded.")
                    elif nErrno == 1:
                        print("Access to kill the specified process " \
				"was denied by host machine's OS.")  
                    elif nErrno == 3:
                        print("The process or process group does not exist, " \
                              "or is a zombie..")
                elif not status.startswith('2'):
                    print("Shellcode execution failed for an unknown reason.")
    print()
    print("*************************************************************************")
    input("Press ENTER to go back to Main Menu")
    return
    
    
def main():
    prompt_user_for_server_and_connect()
    global CONNECTED
    global SOCKET
    if CONNECTED == False:
        exit(EXIT_FAILURE)
        
    while(True):
        choice = print_menu()
        print()
        if choice == 1:
            get_cpu_info()
            continue
        
        if choice == 2:
            get_dir_listing()
            continue
        
        if choice == 3:
            pid = pick_proc_to_kill()
            send_shellcode_and_kill_proc(pid)
            if pid == -1:
                end_session(EXIT_FAILURE)
            continue
        
        if choice == 4:
            end_session(EXIT_SUCCESS)
                
    pass


if __name__ == "__main__":
    main()
