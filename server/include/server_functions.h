/*
 * server_functions.h
 *
 *  Created on: Apr 11, 2019
 *      Author: bhart
 */

#ifndef __SERVER_FUNCTIONS_H__
#define __SERVER_FUNCTIONS_H__

extern HMUTEX g_hShellCodeListMutex;

BOOL CheckCommandLineArgs(int argc, char *argv[]);

/**
 * @brief Frees resources consumed by the server and exits the application
 * with the specified code.
 * @param nExitCode Exit code to supply to the operating system when this
 * program is terminated.
 */
void CleanupServer(int nExitCode);
void CloseServerEndpoint();
void ConfigureLogFile();
void CreateClientListMutex();
void CreateMasterAcceptorThread();
struct sockaddr_in* CreateSockAddr();
void CreateShellCodeLinesMutex();
void DestroyClientListMutex();
void DestroyShellCodeLinesMutex();
void ForceDisconnectionOfAllClients();
void FreeAllShellCodeBlocks();

/**
 * @brief Gets a HMUTEX value that is an opaque handle to the mutex used
 * for synchronizing access to the linked list of shellcode lines that
 * are currently being processed.
 * @return HMUTEX handle to the shell code list mutex, or INVALID_HANDLE_VALUE
 * if the mutex has not been initialized.
 */
HMUTEX GetShellCodeListMutex();

BOOL InitializeApplication();
void InstallSigintHandler();
void ParseCommandLine(int argc, char *argv[],
    int* pnPort, BOOL* pbDiagnosticMode);
void PrintSoftwareTitleAndCopyright();
void ServerCleanupHandler(int signum);
void SetUpServerOnPort(int nPort);
void TellUserServerIsListening(int nPort);

#endif /* __SERVER_FUNCTIONS_H__ */
