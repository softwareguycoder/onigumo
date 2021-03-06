/*
 * clientThreadManager.h
 *
 *  Created on: Mar 22, 2019
 *      Author: bhart
 */

#ifndef __CLIENT_THREAD_MANAGER_H__
#define __CLIENT_THREAD_MANAGER_H__

#include "client_struct.h"

/**
 * @brief Indicates whether a client thread should be terminated.
 */
extern BOOL g_bShouldTerminateClientThread;

/**
 * @brief Returns a value indicating whether more clients are flagged as
 * connected than the maximum number allowed.
 * @returns TRUE if the connected client count exceeds the maximum; FALSE
 * otherwise.
 */
BOOL AreTooManyClientsConnected();

/**
 * @brief Throws away resources (such as threads, sockets, and such) that are
 * associated with the client specified.
 * @param lpSendingClient Address of an instance of CLIENTSTRUCT that refers
 * to the client whose connection needs to be cleaned up.
 * @remarks Kills and destroys the client thread, and closes the socket that
 * leads to the client on the server's end. */
void CleanupClientConnection(LPCLIENTSTRUCT lpSendingClient);

/**
 * @brief Removes all lines of shellcode added to the internal database
 * by the specified client.
 * @param lpSendingClient Address of an instance of CLIENTSTRUCT that refers
 * to the particular client whose shellcode should be removed.
 * @return TRUE if the clear operation was successfull; FALSE otherwise.
 */
BOOL ClearClientShellCodeLines(LPCLIENTSTRUCT lpSendingClient);

/**
 * @brief Ends a chat session for the specified client, upon its request.
 * @returns TRUE if the session was ended successfully; FALSE otherwise.
 * @remarks Responds to the QUIT protocol command from a client.  Acknowledges
 * the action with a 200 Goodbye reply message, and then closes the connection,
 * marks the client entry in the client list as non-connected and then kills
 * the client's comms thread.
 */
BOOL EndClientSession(LPCLIENTSTRUCT lpSendingClient);

/**
 * @brief Determines the count of client entries in the linked list that are
 * flagged as currently connected.
 * @returns Count of connected clients (i.e., bConnected == TRUE).
 */
int GetConnectedClientCount();

/**
 * @brief Extracts the address of a CLIENTSTRUCT instance from the user state
 * bag passed to the client thread.
 * @param pvClientThreadUserState Address of a storage location containing user
 * state data that had been passed to the client thread upon its creation.
 * @returns Pointer to a CLIENTSTRUCT instance containing information on the
 * client who sent the communications, or NULL if it can't be obtained.
 */
LPCLIENTSTRUCT GetSendingClientInfo(void* pvClientThreadUserState);

/**
 * @brief Checks received data for protocol-specific commands and handles them.
 * @param lpSendingClient Address of a CLIENTSTRUCT instance that contains
 * information on the client who sent the command.
 * @param pszBuffer Address of a character array containing the text that was
 * received.
 * @returns TRUE if the text received contained a protocol command and no
 * further processing is needed; FALSE otherwise.
 */
BOOL HandleProtocolCommand(LPCLIENTSTRUCT lpSendingClient,
    const char* pszBuffer);

/**
 * @name IsMultilineResponseTerminator
 * @brief Gets a value indicating whether the specified message is the signal
 * for the termination of a multiline data transmission.
 * @param pszMessage Address of the message to be checked.
 * @return TRUE if the message is the multiline-response terminator; FALSE
 * otherwise.
 */
BOOL IsMultilineResponseTerminator(void* pvUserState /*not used*/,
    const char* pszMessage);

/**
 * @brief Callback that is called for each entry in the client list to kill
 * client threads that are no longer needed for communications.
 * @param pClientStruct Address of an instance of CLIENTSTRUCT containing
 * information about the thread that is being used to communicate with the
 * client at the current element in the list. */
void KillClientThread(void* pClientStruct);

/**
 * @brief Creates and launches a new thread of execution to handle
 * communications with a particular client.
 * @param lpCS Reference to an instance of a CLIENTSTRUCT structure
 * that contains data about the specific client to launch a new thread for.
 * @remarks The HTHREAD thread handle of the new thread is saved in the
 * hClientThread member of the CLIENTSTRUCT instance that is passed to this
 * function.  This funciton kills the whole program if the lpClientData
 * parameter (which is required) is NULL.
 */
void LaunchNewClientThread(LPCLIENTSTRUCT lpCS);

/**
 * @brief Writes the unique client ID to the log file.
 * @param lpCS Client for whom to write the ID.
 */
void LogClientID(LPCLIENTSTRUCT lpCS);

/**
 * @brief Processes the server's behavior upon receiving the CODE command.
 * @param lpSendingClient Pointer to an instance of CLIENSTRUCT that refers
 * to the client who sent the command.
 * @param pszBuffer Address of a string buffer containing the data sent by
 * the client (for parsing).
 */
void ProcessCodeCommand(LPCLIENTSTRUCT lpSendingClient,
    const char* pszBuffer);

/**
 * @name ProcessExecCommand
 * @brief Processes the server's behavior upon receiving the EXEC command.
 * @param lpSendingClient Pointer to an instance of CLIENTSTRUCT that refers
 * to the client who sent the command.
 * @param pszBuffer Address of a string buffer containing the data sent by
 * the client (for parsing).
 * @remarks This function instructs the server to obtain shellcode blocks
 * already previously transmitted with the CODE command, "marry" them
 * together, and then decode and execute the shellcode on the host
 * operating system.  The results of this function, therefore, cannot be
 * guaranteed and may even lead to the crash of the server and/or the host
 * machine (depending on what the shellcode instructs the host machine to
 * do.)
 */
void ProcessExecCommand(LPCLIENTSTRUCT lpSendingClient,
    const char* pszBuffer);

/**
 * @brief Processes the server's behavior upon receiveing the HELO comamnd.
 * @param lpSendingClient Pointer to an instance of CLIENTSTRUCT that refers
 * to the client who sent the command.
 * @remarks This function marks the client as being ready to receive any other
 * commands from the specific client who just said HELO ("Hello!").
 */
void ProcessHeloCommand(LPCLIENTSTRUCT lpSendingClient);

/**
 * @brief Processes the server's behavior when the INFO command is received.
 * @param lpSendingClient Pointer to an instance of CLIENTSTRUCT that refers
 * to the client who sent the command.
 * @remarks Causes the server to send the contents of /proc/cpuinfo to the
 * client.
 */
void ProcessInfoCommand(LPCLIENTSTRUCT lpSendingClient);

void ProcessLDirCommand(LPCLIENTSTRUCT lpSendingClient,
    const char* pszBuffer);

/**
 * @brief Processes the server's behavior upon receiveing the LIST comamnd.
 * @param lpSendingClient Pointer to an instance of CLIENTSTRUCT that refers
 * to the client who sent the command.
 * @remarks This function iterates over the list of clients, sending the
 * list of currently connected clients back to the client who sent the
 * command.
 */
void ProcessListCommand(LPCLIENTSTRUCT lpSendingClient);

void ProcessPurgCommand(LPCLIENTSTRUCT lpSendingClient);

/**
 * @brief Performs a synchronous recieve operation from the client, looking for
 * data until a newline has been receieved.
 * @param pvSendingClient Pointer to a CLIENTSTRUCT instance that refers to the
 * client who is sending the data.  Required.
 * @param ppszReplyBuffer Address of a pointer to be used to refer to memory
 * storage that is allocated character-by-character for the received text.
 * @returns Number of bytes received; negative value if an error occurred.
 */
int ReceiveFromClient(void* pvSendingClient, char** ppszReplyBuffer);

void RemoveClientEntryFromList(LPCLIENTSTRUCT lpSendingClient);

/**
 * @brief Reports statistics to the server log and console for the client
 * whose chat session just ended, like how many total bytes were sent and
 * received, etc.
 * @param lpSendingClient Reference to a CLIENTSTRUCT instance that designates
 * the client whose session just ended.  Required.
 */
void ReportClientSessionStats(LPCLIENTSTRUCT lpSendingClient);

void ReportNumShellcodeBytesReceived(LPCLIENTSTRUCT lpSendingClient,
    int nTotalShellCodeBytesReceived);

void SendMultilineDataTerminator(LPCLIENTSTRUCT lpSendingClient);

/**
 * @brief Sends the data in pszMessage to the client designated.
 * @param lpCurrentClient Pointer to a CLIENTSTRUCT that contains data about
 * the client that the message should be sent to.
 * @param pszMessage Address of the buffer containing the message to be sent.
 * @returns Total number of bytes sent, or -1 if an error occurred.
 */
int SendToClient(LPCLIENTSTRUCT lpCurrentClient, const char* pszMessage);

/**
 * @brief Executes logic from when a newly-connected client makes the count
 * of connected clients exceed the maximum allowed.
 * @param lpSendingClient Reference to a CLIENTSTRUCT instance containing
 * information about the newly-connected client. */
void TellClientTooManyPeopleConnected(LPCLIENTSTRUCT lpSendingClient);

/**
 * @brief Semaphore that gets signaled to indicate that this client thread
 * should terminate in an orderly fashion.
 * @param signum Signal code corresponding to the signal that triggered this
 * semaphore.
 */
void TerminateClientThread(int signum);

#endif /* __CLIENT_THREAD_MANAGER_H__ */
