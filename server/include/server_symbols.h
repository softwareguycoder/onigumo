// server_symbols.h - Unified, single include file for #define'd constants
// that are utilized throughout the server application
//

#ifndef __SERVER_SYMBOLS_H__
#define __SERVER_SYMBOLS_H__

#ifndef ERROR_FAILED_RETRIEVE_SHELLCODE_EXECUTION_STATE
#define ERROR_FAILED_RETRIEVE_SHELLCODE_EXECUTION_STATE \
    "ERROR: Failed to retrieve shellcode-execution user state.\n"
#endif //ERROR_FAILED_RETRIEVE_SHELLCODE_EXECUTION_STATE

#ifndef UNABLE_INSTALL_HANDLER_FOR_SIGNAL
#define UNABLE_INSTALL_HANDLER_FOR_SIGNAL \
  "server: Unable to install handler for signal %d.\n"
#endif //UNABLE_INSTALL_HANDLER_FOR_SIGNAL

#ifndef PS_SHELL_COMMAND
#define PS_SHELL_COMMAND \
  "ps -a"
#endif //PS_SHELL_COMMAND

#ifndef ERROR_CONFIRM_ENCODED_SHELLCODE_BYTES
#define ERROR_CONFIRM_ENCODED_SHELLCODE_BYTES \
  "507 Failed. The CODE command argument is number of Base64-encoded " \
  "shellcode bytes.\n"
#endif //ERROR_CONFIRM_ENCODED_SHELLCODE_BYTES

#ifndef PROTOCOL_INFO_COMMAND
#define PROTOCOL_INFO_COMMAND \
    "INFO\n"
#endif //PROTOCOL_INFO_COMMAND

#ifndef CPUINFO_FILE
#define CPUINFO_FILE "/proc/cpuinfo"
#endif //CPUINFO_FILE

#ifndef PROTOCOL_PURG_COMMAND
#define PROTOCOL_PURG_COMMAND "PURG\n"
#endif //PROTOCOL_PURG_COMMAND

#ifndef OK_PURGD_SUCCESSFULLY
#define OK_PURGD_SUCCESSFULLY \
  "203 OK. All shellcode lines being stored for this client " \
  "have been purged.\n"
#endif //OK_PURGD_SUCCESSFULLY

#ifndef RECD_SHELLCODE_SUCCESSFULLY
#define OK_RECD_SHELLCODE_SUCCESSFULLY \
  "202 OK. Encoded shellcode bytes received successfully.\n"
#endif //RECD_SHELLCODE_SUCCESSFULLY

#ifndef FAILED_ADD_SHELLCODE_BLOCK
#define FAILED_ADD_SHELLCODE_BLOCK \
  "ERROR: Failed to add the block of shellcode to the internal structure.\n"
#endif //FAILED_ADD_SHELLCODE_BLOCK

#ifndef FAILED_ALLOC_ENCODED_SHELLCODE_BYTES
#define FAILED_ALLOC_ENCODED_SHELLCODE_BYTES \
  "ERROR: Failed to allocate memory for encoded shellcode bytes.\n"
#endif //FAILED_ALLOC_ENCODED_SHELLCODE_BYTES

#ifndef FAILED_ALLOC_SHELLCODEINFO
#define FAILED_ALLOC_SHELLCODEINFO \
  "ERROR: Failed to allocate memory for SHELLCODEINFO instance.\n"
#endif //FAILED_ALLOC_SHELLCODEINFO

/**
 * @brief Standardized size for buffers.
 */
#ifndef BUFLEN
#define BUFLEN					1024
#endif //BUFLEN

#ifndef CALLING_CLEANUP_HANDLER
#define CALLING_CLEANUP_HANDLER \
	"\nserver: Calling cleanup handler...\n"
#endif //CALLING_CLEANUP_HANDLER

/**
 * @brief Defines a format string for logging how many bytes were just
 * received from a client.
 */
#ifndef CLIENT_BYTES_RECD_FORMAT
#define CLIENT_BYTES_RECD_FORMAT \
    "C[%s:%d]: %d B received.\n"
#endif //CLIENT_BYTES_RECD_FORMAT

#ifndef CLIENT_COUNT_ZERO
#define CLIENT_COUNT_ZERO \
    "Client count has dropped to zero.  Waiting for more connections...\n"
#endif //CLIENT_COUNT_ZERO

/**
 * @brief Defines a format string for logging the actual string data received
 * from a client.
 */
#ifndef CLIENT_DATA_FORMAT
#define CLIENT_DATA_FORMAT \
    "C[%s:%d]: %s"
#endif //CLIENT_DATA_FORMAT

/**
 * @brief Client disconnection message.
 */
#ifndef CLIENT_DISCONNECTED
#define CLIENT_DISCONNECTED \
    "C[%s:%d]: <disconnected>\n"
#endif //CLIENT_DISCONNECTED

#define CLIENT_ID_FORMAT        "C[%s:%d] Client ID is '{%s}'.\n"

#ifndef CLIENT_IP_ADDR_UNK
#define CLIENT_IP_ADDR_UNK		"server: Client IP address not known.\n"
#endif //CLIENT_IP_ADDR_UNK

#ifndef CLIENT_SESSION_STATS
#define CLIENT_SESSION_STATS \
	"Client '{%s}': %ld B received, %ld B sent.\n"
#endif //CLIENT_SESSION_STATS

#ifndef CLIENT_THREAD_ENDING
#define CLIENT_THREAD_ENDING \
	"server: Client thread ending.\n"
#endif // CLIENT_THREAD_ENDING

#ifndef CLOSING_SERVER_TCP_ENDPOINT
#define CLOSING_SERVER_TCP_ENDPOINT \
  "server: Closing TCP endpoint...\n"
#endif //CLOSING_SERVER_TCP_ENDPOINT

#ifndef COMMAND_INCORRECT
#define COMMAND_INCORRECT \
  "500 Error.  Command syntax incorrect or server error.\n"
#endif //COMMAND_INCORRECT

/**
 * @brief Copyright message to display on the server's console.
 */
#ifndef COPYRIGHT_MESSAGE
#define COPYRIGHT_MESSAGE	"Copyright (c) 2018-19 by Brian Hart.\n\n"
#endif //COPYRIGHT_MESSAGE

#ifndef DIAGNOSTIC_MODE_PARM_COUNT
#define DIAGNOSTIC_MODE_PARM_COUNT		3
#endif //DIAGNOSTIC_MODE_PARM_COUNT

#ifndef DISCONNECTED_CLIENT_DETECTED
#define DISCONNECTED_CLIENT_DETECTED \
	"server: Disconnected client detected."
#endif //DISCONNECTED_CLIENT_DETECTED

#ifndef ERROR_CANT_ADD_NULL_CLIENT
#define ERROR_CANT_ADD_NULL_CLIENT \
	"ERROR: Can't add a null reference to the list of connected clients.\n"
#endif //ERROR_CANT_ADD_NULL_CLIENT

#ifndef ERROR_CANT_PARSE_DIAGNOSTIC_MODE
#define ERROR_CANT_PARSE_DIAGNOSTIC_MODE \
	"ERROR: No storage specified for diagnostic mode indicator.\n"
#endif //ERROR_CANT_PARSE_DIAGNOSTIC_MODE

/**
 * @brief Message to send to clients indicating that the server application
 * has been forcibly terminated by its console interactive user.
 */
#ifndef ERROR_FORCED_DISCONNECT
#define ERROR_FORCED_DISCONNECT		"503 Server forcibly shut down by its " \
									"operator.\n"
#endif //ERROR_FORCED_DISCONNECT

#ifndef ERROR_GENERAL_SERVER_FAILURE
#define ERROR_GENERAL_SERVER_FAILURE \
  "550 Failed. Unknown server or OS error.\n"
#endif //ERROR_GENERAL_SERVER_FAILURE

#ifndef ERROR_MUST_SAY_HELLO_FIRST
#define ERROR_MUST_SAY_HELLO_FIRST \
  "504 Failed. Must issue HELO command to begin client session.\n"
#endif //ERROR_MUST_SAY_HELLO_FIRST

/**
 * @brief Protocol response sent when too many clients are already connected.
 * @remarks Error reply to a HELO command from a client when more than the
 * maximum allowed connected clients are already connected.
 */
#ifndef ERROR_MAX_CONNECTIONS_EXCEEDED
#define ERROR_MAX_CONNECTIONS_EXCEEDED \
    "502 The maximum count of connected clients has been exceeded.\n"
#endif //ERROR_MAX_CONNECTIONS_EXCEEDED

/**
 * @brief Error message to displayed when a function receives a NULL pointer
 * for a LPCLIENTSTRUCT parameter.
 */
#ifndef ERROR_NO_SENDING_CLIENT_SPECIFIED
#define ERROR_NO_SENDING_CLIENT_SPECIFIED \
    "Required parameter lpSendingClient not specified.\n"
#endif //ERROR_NO_SENDING_CLIENT_SPECIFIED

#ifndef ERROR_QTY_MUST_BE_POS_32BIT_INT
#define ERROR_QTY_MUST_BE_POS_32BIT_INT \
  "502 Failed.  Integer argument must be a positive, 32-bit quanity.\n"
#endif //ERROR_QTY_MUST_BE_POS_32BIT_INT

#ifndef ERROR_FAILED_TO_PARSE_INT
#define ERROR_FAILED_TO_PARSE_INT \
    "501 Failed.  Cannot parse input as integer.\n"
#endif //ERROR_FAILED_TO_PARSE_INT

#ifndef ERROR_TOO_MANY_CLIENTS
#define ERROR_TOO_MANY_CLIENTS \
    "ERROR: Maximum number of connected clients (%d) exceeded.\n"
#endif //ERROR_TOO_MANY_CLIENTS

#ifndef FAILED_ALLOC_CLIENT_STRUCT
#define FAILED_ALLOC_CLIENT_STRUCT \
    "Failed to allocate memory for client list entry structure.\n"
#endif //FAILED_ALLOC_CLIENT_STRUCT

#ifndef FAILED_ALLOC_SHELLCODE_STORAGE
#define FAILED_ALLOC_SHELLCODE_STORAGE \
    "ERROR: Failed to allocate storage for shellcode.\n"
#endif //FAILED_ALLOC_SHELLCODE_STORAGE

#ifndef FAILED_CREATE_NEW_CLIENT
#define FAILED_CREATE_NEW_CLIENT	"server: Failed to create new client " \
									"list entry.\n"
#endif //FAILED_CREATE_NEW_CLIENT

#ifndef FAILED_GET_CLIENTSTRUCT_FROM_USER_STATE
#define FAILED_GET_CLIENTSTRUCT_FROM_USER_STATE \
    "client thread: Failed to get client information from user state.\n"
#endif //FAILED_GET_CLIENTSTRUCT_FROM_USER_STATE

#ifndef ERROR_CLIENT_ENTRY_COUNT_EXCEEDED
#define ERROR_CLIENT_ENTRY_COUNT_EXCEEDED \
    "ERROR: The maximum number of client list entries has been reached.\n"
#endif //ERROR_CLIENT_ENTRY_COUNT_EXCEEDED

#ifndef FAILED_LAUNCH_CLIENT_THREAD
#define FAILED_LAUNCH_CLIENT_THREAD	"server: Failed to launch client comm " \
									"channel.\n"
#endif //FAILED_LAUNCH_CLIENT_THREAD

#ifndef FAILED_OPEN_LOG_FILE
#define FAILED_OPEN_LOG_FILE \
  "ERROR: Failed to open log file.\n"
#endif //FAILED_OPEN_LOG_FILE

#ifndef FORCIBLY_CLOSING_ALL_CLIENT_CONNECTIONS
#define FORCIBLY_CLOSING_ALL_CLIENT_CONNECTIONS \
  "server: Forcibly closing all client connections...\n"
#endif //FORCIBLY_CLOSING_ALL_CLIENT_CONNECTIONS

/**
 * @brief Error message to display when we've failed to receive text from the
 * client.
 */
#ifndef FAILED_RECEIVE_TEXT_FROM_CLIENT
#define FAILED_RECEIVE_TEXT_FROM_CLIENT \
    "ERROR: Failed to receive the line of text back from the client.\n"
#endif //FAILED_RECEIVE_TEXT_FROM_CLIENT

/**
 * @brief Error response sent to clients requesting the list of processes
 * running on the host.
 * @remarks If the operation does not work, then it's most likely because the
 * user account under which the onigumo server is executing does not have
 * sufficient permissions to run ps -a.
 */
#ifndef HOST_PROC_LIST_ACCESS_DENIED
#define HOST_PROC_LIST_ACCESS_DENIED \
  "505 Failed. Host operating system denied us access to the list of processes.\n"
#endif //HOST_PROC_LIST_ACCESS_DENIED

#ifndef HOST_OSINFO_ACCESS_DENIED
#define HOST_OSINFO_ACCESS_DENIED \
  "506 Failed. Host operating system denied us access to the CPU info.\n"
#endif //HOST_OSINFO_ACCESS_DENIED

/**
 * @brief Error message that is displayed when a function is given a NULL
 * pointer for its pvData parameter.
 */
#ifndef INVALID_LIST_DATA
#define INVALID_LIST_DATA \
    "ERROR: The pointer for the data to add to the linked list " \
    "is an invalid value.\n"
#endif //INVALID_LIST_DATA

/**
 * @brief Maximum length of a string containing a valid IPv4 IP address.
 */
#ifndef IPADDRLEN
#define IPADDRLEN   				20
#endif //IPADDRLEN

/**
 * @brief Default log file directory.
 */
#ifndef LOG_FILE_DIR
#define LOG_FILE_DIR "~/logs/onigumo"
#endif //LOG_FILE_DIR

/**
 * @brief fopen() mode for opening the log file.
 */
#ifndef LOG_FILE_OPEN_MODE
#define LOG_FILE_OPEN_MODE			"a+"
#endif //LOG_FILE_OPEN_MODE

/**
 * @brief Path to the log file.
 */
#ifndef LOG_FILE_PATH
#define LOG_FILE_PATH				"~/logs/onigumo/server.log"
#endif //LOG_FILE_PATH

#ifndef MAX_ALLOWED_CONNECTIONS
#define MAX_ALLOWED_CONNECTIONS     20
#endif //MAX_ALLOWED_CONNECTIONS

#ifndef MAX_CLIENT_LIST_ENTRIES
#define MAX_CLIENT_LIST_ENTRIES     500
#endif //MAX_CLIENT_LIST_ENTRIES

/**
 * @brief Per protocol, the maximum length a line can be is 255 chars,
 * whether it's a command or a chat message.
 */
#ifndef MAX_LINE_LENGTH
#define MAX_LINE_LENGTH             255
#endif //MAX_LINE_LENGTH

/**
 * @brief Maximum length (in characters) for a nickname.
 */
#ifndef MAX_NICKNAME_LEN
#define MAX_NICKNAME_LEN            15
#endif //MAX_NICKNAME_LEN

#ifndef MIN_NICKNAME_PREFIX_SIZE
#define MIN_NICKNAME_PREFIX_SIZE	4
#endif //MIN_NICKNAME_PREFIX_SIZE

/**
 * @brief The minimum # of cmd line args to pass
 */
#ifndef MIN_NUM_ARGS
#define MIN_NUM_ARGS				2
#endif //MIN_NUM_ARGS

/**
 * @brief A single period on a line terminates a multi-line message.
 */
#ifndef MSG_TERMINATOR
#define	MSG_TERMINATOR			".\n"
#endif //MSG_TERMINATOR

/**
 * @brief Server's administrative message saying a new chatter joined.
 */
#ifndef NEW_CHATTER_JOINED
#define NEW_CHATTER_JOINED			"!@%s joined the chat room.\n"
#endif //NEW_CHATTER_JOINED

/**
 * @brief Server's administrative message saying a chatter left.
 */
#ifndef NEW_CHATTER_LEFT
#define NEW_CHATTER_LEFT			"!@%s left the chat room.\n"
#endif //NEW_CHATTER_LEFT

/**
 * @brief Message to print or log indicating new client connection detected.
 */
#ifndef NEW_CLIENT_CONN
#define NEW_CLIENT_CONN				"S: <New client connection detected " \
									"from %s.>\n"
#endif //NEW_CLIENT_CONN

/**
 * @brief Response to the HELO command indicating operation succeeded.
 * @remarks The HELO command is issued by clients right after they establish
 * the TCP connection to the server.  This command lets the server know that a
 * new user wants to join the chat room.  Until this command is issued, followed
 * by a proper NICK command, no chat messages will be echoed to that particular
 * client if other users chat.
 */
#ifndef OK_WELCOME
#define OK_WELCOME \
    "200 OK. Welcome to the onigumo server.  Be careful what you wish for!\n"
#endif //OK_WELCOME

/**
 * @brief Response to the QUIT command indicating operation succeeded.
 * @remarks The QUIT command is issued by chat clients who want to tell the
 * server that their user is done chatting.
 */
#ifndef OK_GOODBYE
#define OK_GOODBYE					"206 Goodbye.\n"
#endif //OK_GOODBYE

#ifndef OK_CPU_INFO_FOLLOWS
#define OK_CPU_INFO_FOLLOWS \
  "205 OK. Following lines contain host machine CPU info.  Terminated by .\n"
#endif //OK_CPU_INFO_FOLLOWS

/**
 * @brief Response from the server in the case where a LIST command is issued
 * by the client.
 * @remarks The LIST command is used by clients to ask the server for a list
 * of the nicknames of all currently active chatters.  The list is delivered
 * after this response, one nickname per line, and then a dot on a line by
 * itself follows, indicating the end of the response.
 */
#ifndef OK_PROC_LIST_FOLLOWS
#define OK_PROC_LIST_FOLLOWS \
	"204 OK. List of processes running on this machine follows.\n"
#endif //OK_PROC_LIST_FOLLOWS

#ifndef OK_SEND_SHELLCODE
#define OK_SEND_SHELLCODE \
  "201 OK. Send your shellcode.  Terminate with a '.' on a line by itself.\n"
#endif //OK_SEND_SHELLCODE

#ifndef OUT_OF_MEMORY
#define OUT_OF_MEMORY \
    "server: Insufficient operating system memory.\n"
#endif //OUT_OF_MEMORY

#ifndef PORT_NUMBER_NOT_VALID
#define PORT_NUMBER_NOT_VALID \
        "server: Port number must be in the range 1024-49151 inclusive.\n"
#endif //PORT_NUMBER_NOT_VALID

// Protocol command that gets this client marked as a member of the chat room
#ifndef PROTOCOL_CODE_COMMAND
#define PROTOCOL_CODE_COMMAND "CODE "
#endif //PROTOCOL_CODE_COMMAND

#ifndef PROTOCOL_EXEC_COMMAND
#define PROTOCOL_EXEC_COMMAND "EXEC "
#endif //PROTOCOL_EXEC_COMMAND

// Protocol command that gets this client marked as a member of the chat room
#ifndef PROTOCOL_HELO_COMMAND
#define PROTOCOL_HELO_COMMAND	"HELO\n"
#endif //PROTOCOL_HELO_COMMAND

#ifndef PROTOCOL_LIST_COMMAND
#define PROTOCOL_LIST_COMMAND	"LIST\n"
#endif //PROTOCOL_LIST_COMMAND

/**
 * @brief Protocol command that 'logs the client off' from the chat server.
 */
#ifndef PROTOCOL_QUIT_COMMAND
#define PROTOCOL_QUIT_COMMAND	"QUIT\n"
#endif //PROTOCOL_QUIT_COMMAND

/**
 * @brief Format string for logging data sent by the server.
 */
#ifndef SERVER_DATA_FORMAT
#define SERVER_DATA_FORMAT      "S: %s"
#endif //SERVER_DATA_FORMAT

#ifndef SERVER_DISCONNECTED
#define SERVER_DISCONNECTED     "S: <disconnected>\n"
#endif //SERVER_DISCONNECTED

/**
 * @brief Error message to be displayed when we can't bind the server's socket.
 */
#ifndef SERVER_ERROR_FAILED_BIND
#define SERVER_ERROR_FAILED_BIND	"server: Could not bind server's TCP" \
									"endpoint.\n"
#endif //SERVER_ERROR_FAILED_BIND

/**
 * @brief Error message to display when the socket handle for a client is
 * invalid.
 */
#ifndef SERVER_CLIENT_SOCKET_INVALID
#define SERVER_CLIENT_SOCKET_INVALID \
	"server: Invalid value for client's TCP endpoint.\n"
#endif //SERVER_CLIENT_SOCKET_INVALID

#ifndef SERVER_DIAGNOSTIC_MODE_ENABLED
#define SERVER_DIAGNOSTIC_MODE_ENABLED \
	"server: Diagnostic mode enabled.\n"
#endif //SERVER_DIAGNOSTIC_MODE_ENABLED

/**
 * @brief Error message to be displayed when we can't set up a listening socket.
 */
#ifndef SERVER_ERROR_FAILED_LISTEN
#define SERVER_ERROR_FAILED_LISTEN	"server: Could not open server's TCP" \
									"endpoint for listening.\n"
#endif //SERVER_ERROR_FAILED_LISTEN

#ifndef SERVER_FAILED_START_MAT
#define SERVER_FAILED_START_MAT		"server: Failed to initialize master " \
									"acceptor thread.\n"
#endif //SERVER_FAILED_START_MAT
/**
 * @brief Message to tell the user the server is now listening on the port.
 */
#ifndef SERVER_LISTENING_ON_PORT
#define SERVER_LISTENING_ON_PORT	"server: Now listening on port %d\n"
#endif //SERVER_LISTENING_ON_PORT

#ifndef SERVER_NO_PORT_SPECIFIED
#define SERVER_NO_PORT_SPECIFIED \
        "server: No port number specified on the command-line.\n"
#endif //SERVER_NO_PORT_SPECIFIED

#ifndef SERVER_SHUTTING_DOWN
#define SERVER_SHUTTING_DOWN        "server: Shutting down...\n"
#endif //SERVER_SHUTTING_DOWN

#ifndef SERVER_SHUTTING_DOWN_MAT
#define SERVER_SHUTTING_DOWN_MAT \
  "server: Shutting down master client connection thread...\n"
#endif //SERVER_SHUTTING_DOWN_MAT
/**
 * @brief Title of this software for displaying on the console.
 */
#ifndef SOFTWARE_TITLE
#define SOFTWARE_TITLE              "onigumo Remote Exploit Server v1.0\n"
#endif //SOFTWARE_TITLE

/**
 * @brief Usage message to be displayed if the user has not specified correct
 * command-line paramters on startup.
 */
#ifndef USAGE_STRING
#define USAGE_STRING				"Usage: server <port_num>\n"
#endif //USAGE_STRING

/**
 * @brief Starting value for the nBytesReceived member of a CLIENTSTRUCT.
 */
#ifndef ZERO_BYTES_TOTAL_RECEIVED
#define ZERO_BYTES_TOTAL_RECEIVED   0L
#endif //ZERO_BYTES_TOTAL_RECEIVED

#endif /* __SERVER_SYMBOLS_H__ */
