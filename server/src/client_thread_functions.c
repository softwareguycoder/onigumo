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
#include "directory_lister.h"
#include "server_functions.h"

#include "shell_code_info.h"

#define RESULTS_BLOCK_SIZE  2

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
      (LPCLIENTSTRUCT) pvSendingClient;
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
// GetCommandIntegerArgument function

long GetCommandIntegerArgument(LPCLIENTSTRUCT lpSendingClient,
    const char *pszBuffer) {
  int nStringCount = 0;
  char **ppszStrings = NULL;

  if (lpSendingClient == NULL) {
    return 0L;
  }

  if (IsNullOrWhiteSpace(pszBuffer)) {
    return 0L;
  }

  char szCommand[MAX_LINE_LENGTH + 1];
  memset(szCommand, 0, MAX_LINE_LENGTH + 1);

  strcpy(szCommand, pszBuffer);

  Split(szCommand, " ", &ppszStrings, &nStringCount);

  if (ppszStrings == NULL || nStringCount <= 1) {
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient, ERROR_FAILED_TO_PARSE_INT);

    FreeStringArray(&ppszStrings, nStringCount);
    return 0L;
  }

  /* We expect ppszStrings[1] to hold the ASCII representation of a
   * positive, 32-bit integer. Convert it to a long, just in case
   * the string is a really big number */
  long lResult = 0L;
  if (0 > StringToLong(ppszStrings[1], &lResult)) {
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient, ERROR_FAILED_TO_PARSE_INT);

    FreeStringArray(&ppszStrings, nStringCount);
    return 0L;
  }

  if (lResult <= 0) {
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient,
        ERROR_QTY_MUST_BE_POS_32BIT_INT);

    FreeStringArray(&ppszStrings, nStringCount);
    return 0L;
  }

  FreeStringArray(&ppszStrings, nStringCount);

  return lResult;
}

///////////////////////////////////////////////////////////////////////////////
// GetCommandStringArgument function

void GetCommandStringArgument(LPCLIENTSTRUCT lpSendingClient,
    const char *pszBuffer, char** ppszOutputBuffer) {
  int nStringCount = 0;
  char **ppszStrings = NULL;

  if (lpSendingClient == NULL) {
    return;
  }

  if (IsNullOrWhiteSpace(pszBuffer)) {
    return;
  }

  if (ppszOutputBuffer == NULL) {
    return;
  }

  char szCommand[MAX_LINE_LENGTH + 1];
  memset(szCommand, 0, MAX_LINE_LENGTH + 1);

  strcpy(szCommand, pszBuffer);

  Split(szCommand, " ", &ppszStrings, &nStringCount);

  if (ppszStrings == NULL || nStringCount <= 1) {
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient, ERROR_FAILED_TO_PARSE_STRING);

    FreeStringArray(&ppszStrings, nStringCount);
    return;
  }

  *ppszStrings = "";  // Set the first element to the empty string

  /* We exepct the 1th through the (N-1)th (where N is the number of
   * elements) elements to, collectively, hold the directory path (
   * delimited by forward slashes of course) */
  int nDirectoryPathLength = 0;
  char* pszOutputBuffer = NULL;

  JoinStrings(ppszStrings, nStringCount, &pszOutputBuffer,
      &nDirectoryPathLength);

  if (IsNullOrWhiteSpace(pszOutputBuffer)) {
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient, ERROR_FAILED_TO_PARSE_STRING);

    FreeStringArray(&ppszStrings, nStringCount);
    return;
  }

  *ppszOutputBuffer = pszOutputBuffer;

  FreeStringArray(&ppszStrings, nStringCount);
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
// VerifyShellcodeBytesReceived function

void VerifyShellcodeBytesReceived(LPCLIENTSTRUCT lpSendingClient,
    long lShellcodeBytes) {
  if (lpSendingClient == NULL) {
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient, ERROR_GENERAL_SERVER_FAILURE);
    return;
  }

  if (lShellcodeBytes <= 0) {
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient, ERROR_GENERAL_SERVER_FAILURE);
    return;
  }

  int nTotalShellCodeBytesReceived = 0;
  nTotalShellCodeBytesReceived =
      SumElementsWhere(g_pShellCodeLines,
          GetCurrentShellCodeInfoBytes,
          &(lpSendingClient->clientID),
          FindShellCodeBlockForClient);

  if (nTotalShellCodeBytesReceived == -1
      || nTotalShellCodeBytesReceived != (int) lShellcodeBytes) {
    // Unknown error during computation of total bytes received.
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient, ERROR_CONFIRM_ENCODED_SHELLCODE_BYTES);
    ClearClientShellCodeLines(lpSendingClient);
    return;
  }

  lpSendingClient->nBytesSent +=
      ReplyToClient(lpSendingClient, OK_RECD_SHELLCODE_SUCCESSFULLY);
}

///////////////////////////////////////////////////////////////////////////////
// Publicly-exposed functions

///////////////////////////////////////////////////////////////////////////////
// AreTooManyClientsConnected function

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
// ClearClientShellCodeLines function

BOOL ClearClientShellCodeLines(LPCLIENTSTRUCT lpSendingClient) {
  if (lpSendingClient == NULL) {
    return FALSE;
  }

  if (!IsUUIDValid(&(lpSendingClient->clientID))) {
    return FALSE;
  }

  /* cycle through the linked list of shellcode lines, where
   * each entry is tagged with the client ID of the client who sent
   * that shellcode, and clear that client's shellcode lines from the
   * list.
   */

  LockMutex(GetShellCodeListMutex());
  {
    if (g_pShellCodeLines == NULL
        || GetElementCount(g_pShellCodeLines) == 0) {
      UnlockMutex(GetShellCodeListMutex());
      return TRUE; /* TRUE because zero lines of shellcode in the list for
       the current client is what we want. */
    }

    /* Scan the linked list for lines of shell code that are tagged
     * with the unique ID of the current client.  Then, remove all such
     * lines. */

    RemoveElementWhere(&g_pShellCodeLines,
        &(lpSendingClient->clientID), FindShellCodeBlockForClient,
        ReleaseShellCodeBlock);
  }
  UnlockMutex(GetShellCodeListMutex());

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// EndClientSession function

BOOL EndClientSession(LPCLIENTSTRUCT lpSendingClient) {
  if (lpSendingClient == NULL) {
    return FALSE;
  }

  char szReplyBuffer[BUFLEN];
  memset(szReplyBuffer, 0, BUFLEN);

  /* Tell the client who told us they want to quit,
   * "Good bye sucka!" */
  lpSendingClient->nBytesSent += ReplyToClient(lpSendingClient,
  OK_GOODBYE);

  ClearClientShellCodeLines(lpSendingClient);

  CleanupClientConnection(lpSendingClient);

  ReportClientSessionStats(lpSendingClient);

  RemoveClientEntryFromList(lpSendingClient);

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
  if (Equals(pszBuffer, PROTOCOL_HELO_COMMAND)) {
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
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient, ERROR_MUST_SAY_HELLO_FIRST);
    return TRUE; /* command or thing handled by telling client they
     need to say HELO first */
  }

  if (Equals(pszBuffer, MSG_TERMINATOR)) {
    /* Signal for end of multi-line input received. */
    return FALSE;
  }

  /* per protocol, LIST command is client requesting a list of the nicknames
   * of all the chatters who are currently active on the server. */
  if (Equals(pszBuffer, PROTOCOL_LIST_COMMAND)) {
    ProcessListCommand(lpSendingClient);

    return TRUE; /* command successfully handled */
  }

  if (StartsWith(pszBuffer, PROTOCOL_CODE_COMMAND)) {
    ProcessCodeCommand(lpSendingClient, pszBuffer);

    return TRUE; /* command successfully handled */
  }

  if (StartsWith(pszBuffer, PROTOCOL_EXEC_COMMAND)) {
    ProcessExecCommand(lpSendingClient, pszBuffer);

    return TRUE; /* command successfully handled */
  }

  if (StartsWith(pszBuffer, PROTOCOL_LDIR_COMMAND)) {
    ProcessLDirCommand(lpSendingClient, pszBuffer);

    return TRUE; /* command successfully handled */
  }

  if (Equals(pszBuffer, PROTOCOL_PURG_COMMAND)) {
    ProcessPurgCommand(lpSendingClient);

    return TRUE; /* command successfully handled */
  }

  if (Equals(pszBuffer, PROTOCOL_INFO_COMMAND)) {
    ProcessInfoCommand(lpSendingClient);

    return TRUE;
  }

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
  if (lpSendingClient == NULL) {
    return;
  }

  if (lpSendingClient->bConnected == FALSE) {
    return;
  }

  if (IsNullOrWhiteSpace(pszBuffer)) {
    return;
  }

  if (!StartsWith(pszBuffer, PROTOCOL_CODE_COMMAND)) {
    return;
  }

  /* We expect the CODE command to be of the format
   * CODE <integer> where <integer> is a positive 32-bit value.
   */
  long lShellcodeBytes = GetCommandIntegerArgument(lpSendingClient,
      pszBuffer);

  lpSendingClient->nBytesSent +=
      ReplyToClient(lpSendingClient, OK_SEND_SHELLCODE);

  /* Receive multiline data from the client.  Stop receiving when
   * data is a period followed by a LF. */
  ReceiveMultilineData2(
      (void*) lpSendingClient,
      ReceiveFromClient,
      GatherShellCodeLine,
      IsMultilineResponseTerminator);

  VerifyShellcodeBytesReceived(lpSendingClient,
      lShellcodeBytes);
}

///////////////////////////////////////////////////////////////////////////////
// ProcessExecCommand function

void ProcessExecCommand(LPCLIENTSTRUCT lpSendingClient,
    const char* pszBuffer) {
  if (lpSendingClient == NULL) {
    return;
  }

  if (lpSendingClient->bConnected == FALSE) {
    return;
  }

  if (IsNullOrWhiteSpace(pszBuffer)) {
    return;
  }

  if (!StartsWith(pszBuffer, PROTOCOL_EXEC_COMMAND)) {
    return;
  }

  if (!IsUUIDValid(&(lpSendingClient->clientID))) {
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient, ERROR_GENERAL_SERVER_FAILURE);
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
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient, ERROR_GENERAL_SERVER_FAILURE);
    return;
  }

  if (IsNullOrWhiteSpace(pszEncodedShellCode)) {
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient, ERROR_GENERAL_SERVER_FAILURE);
    return;
  }

  int nDecodedBytes = GetBase64DecodedDataSize(pszEncodedShellCode);
  if (nDecodedBytes <= 0) {
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient, ERROR_GENERAL_SERVER_FAILURE);
    return;
  }

  unsigned char szDecodedBytes[nDecodedBytes];
  memset(szDecodedBytes, 0, nDecodedBytes);

  Base64Decode(pszEncodedShellCode, szDecodedBytes, nDecodedBytes);

  void *pShellCodeBytes = NULL;

  /* Get the argument passed to the EXEC command. This argument
   * will be passed to the shellcode. The GetCommandLineIntegerArgument
   * generically returns a long (since it wants to use as much space in
   * memory to represent a nice range of values) but we will cast its
   * return value to an int since that is what shellcode typically reserves
   * room for in memory. */
  int nShellCodeArgument =
      (int) GetCommandIntegerArgument(lpSendingClient, pszBuffer);

  LPSHELLCODEUSERSTATE lpUserState = NULL;

  HTHREAD hShellCodeThread =
      ExecShellCode2Async(szDecodedBytes, nDecodedBytes, nShellCodeArgument,
          &pShellCodeBytes);
  if (INVALID_HANDLE_VALUE == hShellCodeThread) {
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient, ERROR_GENERAL_SERVER_FAILURE);
    return;
  }

  WaitThreadEx(hShellCodeThread, (void**) &lpUserState);
  if (!IsShellCodeUserStateValid(lpUserState)) {
    fprintf(stderr, ERROR_FAILED_RETRIEVE_SHELLCODE_EXECUTION_STATE);
    exit(EXIT_FAILURE);
  }

  void* pvResult = GetShellCodeUserStateResultPointer(lpUserState);

  SHELLCODERESULTS shellCodeResults = {0};

  DeMarshalBlockFromThread(&shellCodeResults, pvResult,
      sizeof(SHELLCODERESULTS));
  pvResult = NULL;

  /* Normally, it's unnecessary to set a block of memory that is
   * about to be freed to zero.  However, if any of the pointers
   * in the struct refer to memory blocks, those memory blocks are
   * also freed.  To avoid double-calling free on those blocks, assume
   * they've all been freed and wipe the whole block of memory that the
   * SHELLCODEUSERINFO struct instance occupies with zeroes,
   * so that the action of freeing THIS block does not double-call free()
   * on any of the struct's pointer members. */
  memset(lpUserState, 0, sizeof(SHELLCODEUSERSTATE));

  FreeShellCodeUserState(&lpUserState);

  RemoveShellCodeFromMemory(pShellCodeBytes, nDecodedBytes);

  /* Remove all the shellcode blocks for this client from the
   * linked list. This prevents this command from being re-issued
   * successfully without more code being sent. */

  ClearClientShellCodeLines(lpSendingClient);

  FreeBuffer((void**) &pszEncodedShellCode);

  char szReplyBuffer[MAX_LINE_LENGTH + 1];
  memset(szReplyBuffer, 0, MAX_LINE_LENGTH + 1);

  if (shellCodeResults.nSyscallReturnValue != OK) {
    sprintf(szReplyBuffer, ERROR_FORMAT_SHELLCODE_SYSCALL_EXECUTION_FAILED,
        shellCodeResults.nSyscallReturnValue,
          shellCodeResults.nErrnoValue);
  } else {
    sprintf(szReplyBuffer, OK_SHELLCODE_SYSCALL_EXECUTED_FORMAT,
        shellCodeResults.nSyscallReturnValue);
  }

  lpSendingClient->nBytesSent +=
      ReplyToClient(lpSendingClient, szReplyBuffer);
}

///////////////////////////////////////////////////////////////////////////////
// ProcessHeloCommand function

void ProcessHeloCommand(LPCLIENTSTRUCT lpSendingClient) {
  if (lpSendingClient == NULL) {
    return;
  }

  if (lpSendingClient->bConnected == TRUE) {
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
// ProcessInfoCommand function

void ProcessInfoCommand(LPCLIENTSTRUCT lpSendingClient) {
  if (lpSendingClient == NULL) {
    return;
  }

  if (lpSendingClient->bConnected == FALSE) {
    return;
  }

  int nFileSize = 0;
  int nLineCount = 0;
  char *pszOutputBuffer = NULL;

  ReadAllText(CPUINFO_FILE, &pszOutputBuffer, &nFileSize);

  char** ppszOutputLines = NULL;

  Split(pszOutputBuffer, "\n", &ppszOutputLines, &nLineCount);

  if (ppszOutputLines == NULL || nLineCount <= 0) {
    lpSendingClient->nBytesSent += ReplyToClient(lpSendingClient,
    HOST_OSINFO_ACCESS_DENIED);
    return;
  }

  lpSendingClient->nBytesSent += ReplyToClient(lpSendingClient,
  OK_CPU_INFO_FOLLOWS);

  for (int i = 0; i < nLineCount; i++) {
    if (IsNullOrWhiteSpace(ppszOutputLines[i])) {
      continue; // skip any blank lines
    }

    lpSendingClient->nBytesSent += ReplyToClient(lpSendingClient,
        strcat(ppszOutputLines[i], "\n"));
  }

  SendMultilineDataTerminator(lpSendingClient);

  /* release the memory occupied by the output */
  FreeStringArray(&ppszOutputLines, nLineCount);
  ppszOutputLines = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// ProcessLDirCommand function

void ProcessLDirCommand(LPCLIENTSTRUCT lpSendingClient,
    const char* pszBuffer) {
  if (lpSendingClient == NULL) {
    return;
  }

  if (lpSendingClient->bConnected == FALSE) {
    return;
  }

  if (IsNullOrWhiteSpace(pszBuffer)) {
    return;
  }

  if (!StartsWith(pszBuffer, PROTOCOL_LDIR_COMMAND)) {
    return;
  }

  int nLineCount = 0;
  BOOL bShouldDeallocateDirectoryPathBuffer = FALSE;
  char* pszDirectoryPath = NULL;
  char** ppszOutputLines = NULL;

  char szCommandWithNoArgs[MAX_LINE_LENGTH + 1];
  memset(szCommandWithNoArgs, 0, MAX_LINE_LENGTH + 1);
  sprintf(szCommandWithNoArgs, "%s\n", PROTOCOL_LDIR_COMMAND);

  if (!Equals(pszBuffer, szCommandWithNoArgs)) {
    GetCommandStringArgument(lpSendingClient,
        pszBuffer, &pszDirectoryPath);
    bShouldDeallocateDirectoryPathBuffer = TRUE;
  } else {
    /* If just LDIR<LF> was transmitted by the client,
     * then the home directory of the user that is executing
     * this process should be listed, as the default. */
    GetHomeDirectoryPath(&pszDirectoryPath);
    bShouldDeallocateDirectoryPathBuffer = FALSE;
  }

  char szTrimmedDirectoryName[PATH_MAX + 1];
  memset(szTrimmedDirectoryName, 0, PATH_MAX + 1);
  Trim(szTrimmedDirectoryName, PATH_MAX + 1, pszDirectoryPath);

  ListDirectory(szTrimmedDirectoryName, &ppszOutputLines, &nLineCount);

  if (ppszOutputLines == NULL || nLineCount <= 0) {
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient,
        ERROR_FAILED_TO_PARSE_STRING);
    if (bShouldDeallocateDirectoryPathBuffer)
      FreeBuffer((void**) &pszDirectoryPath);
    if (ppszOutputLines != NULL) {
      FreeStringArray(&ppszOutputLines, nLineCount);
      ppszOutputLines = NULL;
    }
    return;
  }

  lpSendingClient->nBytesSent += ReplyToClient(lpSendingClient,
  OK_DIR_LIST_FOLLOWS);

  SendMultilineData(lpSendingClient, ppszOutputLines, nLineCount);

  /* release the memory occupied by the output */
  if (bShouldDeallocateDirectoryPathBuffer)
    FreeBuffer((void**) &pszDirectoryPath);
  if (ppszOutputLines != NULL) {
    FreeStringArray(&ppszOutputLines, nLineCount);
    ppszOutputLines = NULL;
  }
}

///////////////////////////////////////////////////////////////////////////////
// ProcessListCommand function

void ProcessListCommand(LPCLIENTSTRUCT lpSendingClient) {
  if (lpSendingClient == NULL) {
    return;
  }

  if (lpSendingClient->bConnected == FALSE) {
    return;
  }

  int nLineCount = 0;
  char** ppszOutputLines = NULL;

  GetSystemCommandOutput(PS_SHELL_COMMAND, &ppszOutputLines, &nLineCount);

  if (ppszOutputLines == NULL || nLineCount <= 0) {
    lpSendingClient->nBytesSent += ReplyToClient(lpSendingClient,
    HOST_PROC_LIST_ACCESS_DENIED);
    if (ppszOutputLines != NULL) {
      /* release the memory occupied by the output */
      FreeStringArray(&ppszOutputLines, nLineCount);
      ppszOutputLines = NULL;
    }
    return;
  }

  lpSendingClient->nBytesSent += ReplyToClient(lpSendingClient,
  OK_PROC_LIST_FOLLOWS);

  SendMultilineData(lpSendingClient, ppszOutputLines, nLineCount);

  /* release the memory occupied by the output */
  FreeStringArray(&ppszOutputLines, nLineCount);
  ppszOutputLines = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// ProcessPurgCommand

void ProcessPurgCommand(LPCLIENTSTRUCT lpSendingClient) {
  if (lpSendingClient == NULL) {
    return;
  }

  if (lpSendingClient->bConnected == FALSE) {
    return;
  }

  if (!ClearClientShellCodeLines(lpSendingClient)) {
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient, ERROR_GENERAL_SERVER_FAILURE);
    return;
  }
  lpSendingClient->nBytesSent +=
      ReplyToClient(lpSendingClient, OK_PURGD_SUCCESSFULLY);
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

  FreeBuffer((void**) &pszClientID);
}

void RemoveClientEntryFromList(LPCLIENTSTRUCT lpSendingClient) {
  if (lpSendingClient == NULL) {
    return;
  }

  if (!IsUUIDValid(&(lpSendingClient->clientID))) {
    return;
  }

  LockMutex(GetClientListMutex());
  {
    if (g_pClientList == NULL
        || GetElementCount(g_pClientList) == 0) {
      UnlockMutex(GetClientListMutex());
      return;
    }

    RemoveElementWhere(&g_pClientList,
        &(lpSendingClient->clientID), FindClientByID,
        FreeClient);
  }
  UnlockMutex(GetClientListMutex());
}

void SendMultilineData(LPCLIENTSTRUCT lpSendingClient,
    char** ppszOutputLines, int nLineCount) {
  if (lpSendingClient == NULL) {
    return;
  }

  if (ppszOutputLines == NULL) {
    return;
  }

  if (nLineCount <= 0) {
    return;
  }

  for (int i = 0; i < nLineCount; i++) {
    lpSendingClient->nBytesSent += ReplyToClient(lpSendingClient,
        ppszOutputLines[i]);
  }

  SendMultilineDataTerminator(lpSendingClient);
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
