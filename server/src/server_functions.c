// server_functions.c - File to contain the various application-specific
// functions used by the server's main function
//

#include "stdafx.h"
#include "server.h"

#include "client_manager.h"
#include "client_list_manager.h"
#include "mat.h"
#include "mat_functions.h"
#include "server_functions.h"
#include "shell_code_info.h"

HMUTEX g_hShellCodeListMutex = INVALID_HANDLE_VALUE;

BOOL g_bHasServerQuit = FALSE;

///////////////////////////////////////////////////////////////////////////////
// Publicly-available functions

///////////////////////////////////////////////////////////////////////////////
// CheckCommandLineArgs function - Checks the command-line args passed (and the
// count thereof) to ensure that the arguments will be usable

BOOL CheckCommandLineArgs(int argc, char *argv[]) {
  // We expect one non-blank command-line argument, which is the port number
  // that the user wants the server to listen on.  The string should actually
  // be the ASCII representation of a positive integer.

  return argc >= MIN_NUM_ARGS && argv != NULL && !IsNullOrWhiteSpace(argv[1])
      && IsNumeric(argv[1]);
}

///////////////////////////////////////////////////////////////////////////////
// CleanupServer function - Called by routines all over the application to end
// the program gracefully, making sure to release resources and terminate all
// threads in an orderly way

void CleanupServer(int nExitCode) {
  fprintf(stdout, CALLING_CLEANUP_HANDLER);

  if (g_bHasServerQuit) {
    return;
  }

  if (!g_bHasServerQuit) {
    g_bHasServerQuit = TRUE;
  }

  LogInfo(SERVER_SHUTTING_DOWN);

  if (GetLogFileHandle() != stdout) {
    fprintf(stdout, SERVER_SHUTTING_DOWN);
  }

  /* Shut down the thread that waits for new connections. */
  CleanupMasterAcceptorThread();

  FreeAllShellCodeBlocks();

  ForceDisconnectionOfAllClients();

  CloseServerEndpoint();

  FreeSocketMutex();

  DestroyClientListMutex();

  DestroyShellCodeLinesMutex();

  DestroyLoggingMutex();

  DestroyInterlock();

  CloseLogFileHandles();

  exit(nExitCode);	// terminate program
}

///////////////////////////////////////////////////////////////////////////////
// CloseServerEndpoint function

void CloseServerEndpoint() {
  if (IsSocketValid(GetServerSocket())) {
    fprintf(stdout, CLOSING_SERVER_TCP_ENDPOINT);

    CloseSocket(GetServerSocket());

    fprintf(stdout, SERVER_DISCONNECTED);
  }
}

///////////////////////////////////////////////////////////////////////////////
// ConfigureLogFile function - Removes the old log file from disk (if
// applicable) and sets up the file pointers and handles for the new one

void ConfigureLogFile() {
  CreateDirIfNotExists(LOG_FILE_DIR);

  char szExpandedPathName[MAX_PATH + 1];
  ShellExpand(LOG_FILE_PATH, szExpandedPathName, MAX_PATH + 1);

  remove(szExpandedPathName);

  FILE* fpLogFile =
      fopen(szExpandedPathName, LOG_FILE_OPEN_MODE);
  if (fpLogFile == NULL) {
    fprintf(stderr, FAILED_OPEN_LOG_FILE);
    perror("server[ConfigureLogFile]");
    CleanupServer(EXIT_FAILURE);
  }

  SetLogFileHandle(fpLogFile);

  FILE* fpCurrentLogFileHandle =
      GetLogFileHandle();

  SetErrorLogFileHandle(fpCurrentLogFileHandle);

  /*set_log_file(stdout);
   set_error_log_file(stderr);*/
}

///////////////////////////////////////////////////////////////////////////////
// CreateClientListMutex function - Sets up operating system resources for the
// mutex handle which controls threads' access to the list of clients.  Since
// this is only called exactly once during the lifetime of the application, it's
// place is in this file

void CreateClientListMutex() {
  if (INVALID_HANDLE_VALUE != GetClientListMutex()) {
    return;
  }

  SetClientListMutex(CreateMutex());
  if (INVALID_HANDLE_VALUE == GetClientListMutex()) {
    CleanupServer(EXIT_FAILURE);
  }
}

///////////////////////////////////////////////////////////////////////////////
// CreateMasterAcceptorThread function - Creates the master acceptor thread
// (MAT) that accepts incoming client connections and spins off a new thread
// to handle each separate connection

void CreateMasterAcceptorThread() {

  int nServerSocket = INVALID_SOCKET_VALUE;
  nServerSocket = GetServerSocket();
  SetMasterThreadHandle(CreateThreadEx(MasterAcceptorThread, &nServerSocket));

  if (INVALID_HANDLE_VALUE == GetMasterThreadHandle()) {
    fprintf(stderr, SERVER_FAILED_START_MAT);

    CleanupServer(EXIT_FAILURE);
  }
}

///////////////////////////////////////////////////////////////////////////////
// CreateSockAddr function - Allocates storage for a new insance of sockaddr_in
//

struct sockaddr_in* CreateSockAddr() {
  struct sockaddr_in* pResult = (struct sockaddr_in*) malloc(
      1 * sizeof(struct sockaddr_in));
  if (pResult == NULL) {
    fprintf(stderr, OUT_OF_MEMORY);

    // Failed to allocate memory
    CleanupServer(EXIT_FAILURE);
  }

  // Zero out the memory occupied by the structure
  memset(pResult, 0, sizeof(struct sockaddr_in));

  return pResult;
}

///////////////////////////////////////////////////////////////////////////////
// CreateShellCodeLinesMutex function

void CreateShellCodeLinesMutex() {
  if (INVALID_HANDLE_VALUE != g_hShellCodeListMutex) {
    return;
  }

  g_hShellCodeListMutex = CreateMutex();
}

///////////////////////////////////////////////////////////////////////////////
// DestroyClientListMutex function - Releases the system resources occupied
// by the client list mutex handle.  It's here because this function just
// needs to be called exactly once during the excecution of the server.

void DestroyClientListMutex() {
  if (INVALID_HANDLE_VALUE == GetClientListMutex()) {
    return;
  }

  DestroyMutex(GetClientListMutex());

  SetClientListMutex(INVALID_HANDLE_VALUE);
}

///////////////////////////////////////////////////////////////////////////////
// DestroyShellCodeLinesMutex function

void DestroyShellCodeLinesMutex() {
  if (INVALID_HANDLE_VALUE == g_hShellCodeListMutex) {
    return;
  }

  DestroyMutex(g_hShellCodeListMutex);
  g_hShellCodeListMutex = INVALID_HANDLE_VALUE;
}

///////////////////////////////////////////////////////////////////////////////
// ForceDisconnectionOfAllClients function

void ForceDisconnectionOfAllClients() {
  LogInfo(FORCIBLY_CLOSING_ALL_CLIENT_CONNECTIONS);

  if (GetLogFileHandle() != stdout) {
    fprintf(stdout, FORCIBLY_CLOSING_ALL_CLIENT_CONNECTIONS);
  }

  LockMutex(GetClientListMutex());
  {
    if (GetElementCount(g_pClientList) > 0) {
      DoForEach(g_pClientList, ForceDisconnectionOfClient);
    }

    ClearList(&g_pClientList, FreeClient);
  }
  UnlockMutex(GetClientListMutex());
}

///////////////////////////////////////////////////////////////////////////////
// FreeAllShellCodeBlocks function

void FreeAllShellCodeBlocks() {
  if (INVALID_HANDLE_VALUE == GetShellCodeListMutex()) {
    return;
  }

  LockMutex(GetShellCodeListMutex());
  {
    if (g_pShellCodeLines == NULL
        || GetElementCount(g_pShellCodeLines) == 0) {
      UnlockMutex(GetShellCodeListMutex());
      return;
    }
    ClearList(&g_pShellCodeLines, ReleaseShellCodeBlock);
  }
  UnlockMutex(GetShellCodeListMutex());
}

///////////////////////////////////////////////////////////////////////////////
// GetShellCodeListMutex function

HMUTEX GetShellCodeListMutex() {
  return g_hShellCodeListMutex;
}

///////////////////////////////////////////////////////////////////////////////
// InitializeApplication function - Runs functionality that should be executed
// exactly once during the lifetime of the application, at application startup.

BOOL InitializeApplication() {
  /* Configure settings for the log file */
  ConfigureLogFile();

  // Since the usual way to exit this program is for the user to
  // press CTRL+C to forcibly terminate it, install a Linux SIGINT
  // handler here so that when the user does this, we may still
  // get a chance to run the proper cleanup code.  Also, install the
  // same function as the handler for the SIGTERM signal (as this
  // allows us to cleanup gracefully in the event that a debugger tries
  // to kill this process).
  InstallSignalHandler(SIGINT, ServerCleanupHandler);

  InstallSignalHandler(SIGTERM, ServerCleanupHandler);

  InitializeInterlock();

  /* Initialize the socket mutex object in the inetsock_core library */
  CreateSocketMutex();

  CreateClientListMutex();

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// InstallSigintHandler function - Registers a function to be called when the
// user presses CTRL+C, in order to perform an orderly shutdown

void InstallSignalHandler(int nSignal,
    LPSIGNALHANDLER lpfnSignalHandler) {
  if (nSignal <= 0) {
    return;
  }

  if (lpfnSignalHandler == NULL) {
    return;
  }

  if (FALSE == RegisterEventEx(nSignal, lpfnSignalHandler)) {
    fprintf(stderr, UNABLE_INSTALL_HANDLER_FOR_SIGNAL, nSignal);

    perror("server[InstallSignalHandler]");

    FreeSocketMutex();

    CleanupServer(EXIT_FAILURE);
  }
}

///////////////////////////////////////////////////////////////////////////////
// ParseCommandLine function

void ParseCommandLine(int argc, char *argv[], int* pnPort,
    BOOL* pbDiagnosticMode) {
  if (argv == NULL) {
    // Blank port number, nothing to do.
    fprintf(stderr, SERVER_NO_PORT_SPECIFIED);

    CleanupServer(EXIT_FAILURE);
  }

  if (pnPort == NULL) {
    // Blank port number, nothing to do.
    fprintf(stderr, SERVER_NO_PORT_SPECIFIED);

    CleanupServer(EXIT_FAILURE);
  }

  if (pbDiagnosticMode == NULL) {
    // Nothing to do.
    fprintf(stderr, ERROR_CANT_PARSE_DIAGNOSTIC_MODE);

    CleanupServer(EXIT_FAILURE);
  }

  if (IsNullOrWhiteSpace(argv[1])) {
    // Blank port number, nothing to do.
    fprintf(stderr, SERVER_NO_PORT_SPECIFIED);

    CleanupServer(EXIT_FAILURE);
  }

  int nResult = StringToLong(argv[1], (long*) pnPort);
  if (nResult != OK && nResult != EXACTLY_CORRECT) {
    fprintf(stderr, SERVER_NO_PORT_SPECIFIED);

    CleanupServer(EXIT_FAILURE);
  }

  if (argc == DIAGNOSTIC_MODE_PARM_COUNT) {
    *pbDiagnosticMode = EqualsNoCase(argv[2], "-v");
  }
}

///////////////////////////////////////////////////////////////////////////////
// PrintSoftwareTitleAndCopyright function - Does exactly what it says on the
// tin.

void PrintSoftwareTitleAndCopyright() {
  ClearScreen();

  printf(SOFTWARE_TITLE);
  printf(COPYRIGHT_MESSAGE);
}

///////////////////////////////////////////////////////////////////////////////
// ServerCleanupHandler function - Called when the user presses CTRL+C.  This
// function initiates an orderly shut down of the server application.

void ServerCleanupHandler(int signum) {
  CleanupServer(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////////////
// SetUpServerOnPort function - Sets up the server to be bound to the specified
// port and starts the server listening on it.

void SetUpServerOnPort(int nPort) {
  if (!IsUserPortNumberValid(nPort)) {
    fprintf(stderr, PORT_NUMBER_NOT_VALID);

    CleanupServer(EXIT_FAILURE);
  }

  struct sockaddr_in* pSockAddr = CreateSockAddr();

  // Intialize the structure with server address and port information
  GetServerAddrInfo(nPort, pSockAddr);

  // Bind the server socket to associate it with this host as a server
  if (BindSocket(GetServerSocket(), pSockAddr) < 0) {
    fprintf(stderr, SERVER_ERROR_FAILED_BIND);

    FreeBuffer((void**) &pSockAddr);

    CleanupServer(EXIT_FAILURE);
  }

  if (ListenSocket(GetServerSocket()) < 0) {
    fprintf(stderr, SERVER_ERROR_FAILED_LISTEN);

    FreeBuffer((void**) &pSockAddr);

    CleanupServer(EXIT_FAILURE);
  }

  TellUserServerIsListening(nPort);

  FreeBuffer((void**) &pSockAddr);
}

///////////////////////////////////////////////////////////////////////////////
// TellUserServerIsListening function

void TellUserServerIsListening(int nPort) {
  if (!IsUserPortNumberValid(nPort)) {
    fprintf(stderr, PORT_NUMBER_NOT_VALID);

    CleanupServer(EXIT_FAILURE);
  }
  fprintf(stdout, SERVER_LISTENING_ON_PORT, nPort);
}
