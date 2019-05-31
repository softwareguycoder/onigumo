ASCII_ENCODING = 'ascii'

BUFFER_SIZE = 1024
BYTES_SENT_FORMAT = "C: {} B sent."

CLIENT_DATA_FORMAT = "C: {} ({} B sent)"

ERROR_FAILED_CONNECT_TO_SERVER_FORMAT = \
    "ERROR: Failed to connect to server {} on port {}.\n{}"
ERROR_FAILED_RECEIVE_FROM_SERVER_FORMAT = \
    "ERROR: Failed to receive data from the server.\n{}"
ERROR_FAILED_SEND_MESSAGE_FORMAT = \
    "ERROR: Failed to send message to server.\nMessage: '{}'"
EXIT_FAILURE = -1
EXIT_SUCCESS = 0

LF = '\n'

MULTILINE_DATA_TERMINATOR = '.'

PROTOCOL_HELO_COMMAND = "HELO\n"
PROTOCOL_LIST_COMMAND = "LIST\n"
PROTOCOL_QUIT_COMMAND = "QUIT\n"

TCP_IP = '127.0.0.1'
TCP_PORT = 9000

SERVER_DATA_FORMAT = \
    "S: {} ({} B received)"