APP_TITLE = "inuyasha"
APP_SUBTITLE = "Connect to a remote machine and mess it up"

ASCII_ENCODING = 'ascii'

BUFFER_SIZE = 1024
BYTES_SENT_FORMAT = "C: {} B sent."

CHOICE_GET_CPU_INFO = 1
CHOICE_LIST_DIR = 2
CHOICE_KILL_PROC = 3
CHOICE_EXIT = 4
CLIENT_DATA_FORMAT = "C: {} ({} B sent)"
CODE_DIR = "/home/bhart/code"
CONTINUATION_PROMPT = "> Press any key to continue, or q to quit: > "

ASM_CODE_PATH = "~/code/pidkiller.asm"

DEFAULT_HOSTNAME = "localhost"
DEFAULT_PAGE_LINE_COUNT = 15
DEFAULT_PORT = 9000

EMPTY_BYTES = b''
EMPTY_STRING = ''

ERRNO_REMOTE_PROCESS_ACCESS_DENIED = 1
ERRNO_REMOTE_PROCESS_NOT_EXISTS = 3

ERROR_FAILED_COMPILE_FILE = \
    ">>> ERROR: Compilation of file '{}' failed."
ERROR_FAILED_CONNECT_TO_SERVER = \
    ">>> ERROR: Failed to connect to remote server or server not running."
ERROR_FAILED_CONNECT_TO_SERVER_FORMAT = \
    ">>> ERROR: Failed to connect to server {} on port {}.\n{}"
ERROR_FAILED_CREATE_DIRECTORY_FORMAT = \
    ">>> ERROR: Failed to create the directory '{}'."
ERROR_FAILED_DETERMINE_TEMP_DIR_PATH = \
    ">>> ERROR: Failed to determine temp directory path from assembly file path."
ERROR_FAILED_ESTABLISH_SESSION = \
    ">>> ERROR: Failed to establish new session."
ERROR_FAILED_EXTRACT_SHELLCODE = \
    ">>> ERROR: Unable to extract shellcode bytes from assembly code."
ERROR_FAILED_FIND_FILE_FORMAT = \
    ">>> ERROR: Failed to find the file '{}'."
ERROR_FAILED_FIND_SHELLCODE_SOURCE_FORMAT = \
    ">>> ERROR: Failed to find any shellcode source at '{}'."
ERROR_FAILED_GET_CPU_INFO = \
    ">>> ERROR: Failed to get CPU Info."
ERROR_FAILED_GET_DIR_LISTING_FORMAT = \
    ">>> ERROR: Failed to get a listing of directory '{}'."
ERROR_FAILED_KILL_REMOTE_PROCESS = \
    ">>> ERROR: Failed to kill a remote process."
ERROR_FAILED_RECEIVE_FROM_SERVER_FORMAT = \
    ">>> ERROR:Failed to receive entities from the server.\n{}"
ERROR_FAILED_REMOVE_TEMP_DIR = \
    ">>> ERROR: Failed to remove the temporary directory."
ERROR_FAILED_SEND_MESSAGE_FORMAT = \
    ">>> ERROR: Failed to send message to server.\nMessage: '{}'"
ERROR_FAILED_EXECUTE_SHELLCODE = \
    ">>> ERROR: Shellcode execution failed for an unknown reason."
ERROR_FAILED_SEND_SHELLCODE = \
    ">>> ERROR: Failed to send the shellcode bytes to the server."
ERROR_FAILED_SERIALIZE_SHELLCODE = \
    ">>> ERROR: Failed to serialize shellcode."
ERROR_FAILED_START_USER_SESSION = \
    ">>> ERROR: Failed to start user session!"
ERROR_INVALID_MAIN_MENU_CHOICE = \
    ">>> ERROR: Please enter 1, 2, 3, or 4."
ERROR_MUST_PICK_PID = \
    ">>> ERROR: Please pick the PID of the process to kill."
ERROR_NO_RESPONSE_LINES = \
    ">>> ERROR: No lines of response received from server."
ERROR_OBJECT_CODE_NOT_EXISTS_FORMAT = \
    ">>> ERROR: Object code file '{}' not found."
ERROR_PID_INVALID_VALUE = \
    ">>> ERROR: The PID requested is invalid.  Must be 1 or greater."
ERROR_PORT_INVALID_VALUE = \
    ">>> ERROR: Port number must be in the range [1024-49151] inclusive."
ERROR_PROCESS_INFO_NOT_VALID = \
    ">>> ERROR: ProcessInfo object has not passed the validation criteria."
ERROR_REMOTE_PROCESS_ACCESS_DENIED = \
    ">>> ERROR: Access to kill the remote process denied."
ERROR_REMOTE_PROCESS_NOT_EXISTS = \
    ">>> ERROR: Remote process/process group does not exist, or is a zombie."
EXIT_FAILURE = -1
EXIT_SUCCESS = 0

FILE_MODE_READ = 'r'
FILE_MDOE_WRITE = 'w'

HEXADECIMAL_BASE = 16
HIGHEST_VALID_USER_PORT = 49151

IDM_BACK = "<< Back"
IDM_CONNECT_REMOTE_MACHINE = "Connect to Remote Machine"
IDM_SERVER_LIST_PROCESSES = "List Processes"

IDS_ASSEMBLY_CODE_PATH_PROMPT_FORMAT = \
    "> Path to assembly code to send as shellcode [{}]: > "
IDS_ATTEMPTING_COMPILE_SHELLCODE = \
    "Compiling shellcode source file '{}'..."
IDS_ATTEMPTING_EXTRACT_SHELLCODE_BYTES = \
    "Extracting opcodes from compiled shellcode source file..."
IDS_ATTEMPTING_SERIALIZE_SHELLCODE = \
    "Attempting to serialize shellcode bytes into Base64..."
IDS_COMPILING_FILE_FORMAT = \
    "Compiling file '{}'..."
IDS_FOOTER = \
    "*************************************************************************"
IDS_LIST_SERVER_DIR_HEADER = \
    "                   ***   List Server Directory   ***"
IDS_NOW_CONNECTED_TO_SERVER = \
    "               *** Now connected to the server!!! ***"
IDS_PICK_PROCESS_TO_KILL = "Pick a process to kill"
IDS_PICK_PROCESS_TO_KILL_HEADER = \
    "         *** Run Some Shellcode that Will Kill A Process ***"
IDS_PRESS_ENTER_TO_CONTINUE = \
    "Press ENTER to continue..."
IDS_PRESS_ENTER_TO_RETURN_TO_MAIN_MENU = \
    "Press ENTER to go back to Main Menu"
IDS_PRESS_ENTER_TO_PICK_PROC_TO_KILL = \
    "Press ENTER when ready to pick a process to kill..."
IDS_REMOTE_PROCESS_LIST_HEADER = \
    "                *** Remote Machine Process List ***"
IDS_SERVER_CPU_INFO_HEADER = \
    " Server's CPU Info:"
IDS_SERVER_DIR_LIST_HEADER = \
    "Listing of '{}':"
IDS_SERVER_DIR_PATH_PROMPT = \
    "> Server directory path [~]: > "
IDS_SERVER_OPTIONS = "Server Options"
IDS_SOFTWARE_TITLE = \
    "***                         INUYASHA CLIENT                           ***"
IDS_STAR_ROW = \
    "*************************************************************************"
IDS_THANK_YOU_MESSAGE = \
    "Thank you for using the Inuyasha Client (of the Onigumo Server)!"
IDS_TRYING_TO_KILL_PROC_WITH_PID_HEADER = \
    "     ***  Trying to Kill the Remote Process with PID: {} ***"
IDS_WELCOME = "             ***   Welcome to the Inuyasha Client!   ***"

INVALID_CHOICE_VALUE = 0
INVALID_PID_VALUE = 0
INVALID_PORT_NUMBER = 0

LF = '\n'
LOWEST_VALID_USER_PORT = 1024

MULTILINE_DATA_TERMINATOR = '.'

NASM_SHELL_COMMAND_FORMAT = "nasm -f elf64 -F dwarf -g {}.asm -l {}.lst"

OBJECT_CODE_PATH = '/home/bhart/code/pidkiller.o'
OBJDUMP_SYSTEM_COMMAND = "objdump -d {} -M intel | grep '^ ' | cut -f2"
OK = 0

PROTOCOL_HELO_COMMAND = "HELO\n"
PROTOCOL_CODE_COMMAND_FORMAT = "CODE {}\n"
PROTOCOL_EXEC_COMMAND_FORMAT = "EXEC {}\n"
PROTOCOL_INFO_COMMAND = "INFO\n"
PROTOCOL_LDIR_COMMAND = "LDIR"
PROTOCOL_LIST_COMMAND = "LIST\n"
PROTOCOL_QUIT_COMMAND = "QUIT\n"

TCP_IP = '127.0.0.1'
TCP_PORT = 9000
TEMP_ASM_FILENAME = '~/tmp/asmcode/asmcode.asm'

SERVER_DATA_FORMAT = \
    "S: {} ({} B received)"
SERVER_ADDRESS_PROMPT = \
    "Server address"
SERVER_PORT_PROMPT= \
    "Server port"
SHELLCODE_EXECUTION_SUCCEEDED = \
    ">>> SUCCESS: The execution of the shellcode succeeded!"
SHELLCODE_EXECUTION_RESPONSE_ERROR_STATUS = '4'    
SHELLCODE_EXECUTION_RESPONSE_SUCCESS_STATUS = '2'
SHOULD_PROC_LIST_BE_PAGINATED_PROMPT = \
    "> Paginate the list of processes? (Y/N)[Y]: > "    

SOCKET = None
CONNECTED = False
