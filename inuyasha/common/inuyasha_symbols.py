APP_TITLE = "inuyasha"
APP_SUBTITLE = "Connect to a remote machine and mess it up"

ASCII_ENCODING = 'ascii'

BUFFER_SIZE = 1024
BYTES_SENT_FORMAT = "C: {} B sent."

CLIENT_DATA_FORMAT = "C: {} ({} B sent)"
CODE_DIR = "/home/bhart/code"
ASM_CODE_PATH = "~/code/pidkiller.asm"

DEFAULT_HOSTNAME = "localhost"
DEFAULT_PORT = 9000

EMPTY_BYTES = b''
EMPTY_STRING = ''

ERROR_FAILED_CONNECT_TO_SERVER = \
    ">>> ERROR:Failed to connect to remote server or server not running."
ERROR_FAILED_CONNECT_TO_SERVER_FORMAT = \
    ">>> ERROR:Failed to connect to server {} on port {}.\n{}"
ERROR_FAILED_CREATE_DIRECTORY_FORMAT = \
    ">>> ERROR:Failed to create the directory '{}'."
ERROR_FAILED_DETERMINE_TEMP_DIR_PATH = \
    ">>> ERROR:Failed to determine temp directory path from assembly file path."
ERROR_FAILED_ESTABLISH_SESSION = \
    ">>> ERROR: Failed to establish new session."
ERROR_FAILED_EXTRACT_SHELLCODE = \
    ">>> ERROR:Unable to extract shellcode bytes from assembly code."
ERROR_FAILED_FIND_FILE_FORMAT = \
    ">>> ERROR:Failed to find the file '{}'."
ERROR_FAILED_RECEIVE_FROM_SERVER_FORMAT = \
    ">>> ERROR:Failed to receive entities from the server.\n{}"
ERROR_FAILED_REMOVE_TEMP_DIR = \
    ">>> ERROR:Failed to remove the temporary directory."
ERROR_FAILED_SEND_MESSAGE_FORMAT = \
    ">>> ERROR:Failed to send message to server.\nMessage: '{}'"
ERROR_NO_RESPONSE_LINES = \
    ">>> ERROR:No lines of response received from server."
ERROR_PROCESS_INFO_NOT_VALID = \
    ">>> ERROR:ProcessInfo object has not passed the validation criteria."
EXIT_FAILURE = -1
EXIT_SUCCESS = 0

FILE_MODE_READ = 'r'
FILE_MDOE_WRITE = 'w'

HEXADECIMAL_BASE = 16

IDM_BACK = "<< Back"
IDM_CONNECT_REMOTE_MACHINE = "Connect to Remote Machine"
IDM_SERVER_LIST_PROCESSES = "List Processes"

IDS_PICK_PROCESS_TO_KILL = "Pick a process to kill"
IDS_PRESS_ENTER_TO_CONTINUE = \
"Press ENTER to continue..."
IDS_SERVER_OPTIONS = "Server Options"

LF = '\n'

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
    
SOCKET = None
CONNECTED = False
