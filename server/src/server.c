///////////////////////////////////////////////////////////////////////////////
// server.c - TCP chat server in C
// Rudimentary exploit server supporting multiple,
// concurrently-connected clients that can fuck up the machine this server is
// running on.
//
// AUTHOR: Brian Hart
// LAST UPDATE: 27 May 2019
//
// Shout-out to <https://gist.githubusercontent.com/suyash/2488ff6996c98a8ee3a8
// 4fe3198a6f85/raw/ba06e891060b277867a6f9c7c2afa20da431ec91/server.c> and
// <http://www.linuxhowtos.org/C_C++/socket.htm> for 
// inspiration
//

#include "stdafx.h"
#include "server.h"

#include "server_functions.h"

///////////////////////////////////////////////////////////////////////////////
// Main application code

int main(int argc, char *argv[]) {
    if (!InitializeApplication()) {
        CleanupServer(EXIT_FAILURE);
        return EXIT_FAILURE;
    }

    PrintSoftwareTitleAndCopyright();

    // Check the arguments.  If the checks fail, then
    // we should print a message to stderr telling the user what to
    // pass on the command line and then quit
    if (!CheckCommandLineArgs(argc, argv)) {
        fprintf(stderr, USAGE_STRING);
        CleanupServer(EXIT_FAILURE);
        return EXIT_FAILURE;
    }

    int nPort = 0;

    BOOL bDiagnosticMode = FALSE;

    ParseCommandLine(argc, argv, &nPort, &bDiagnosticMode);

    SetDiagnosticMode(bDiagnosticMode);

    SetServerPort(nPort);

    if (IsDiagnosticMode()) {
    	fprintf(stdout, SERVER_DIAGNOSTIC_MODE_ENABLED);
    }

    SetServerSocket(CreateSocket());

    SetUpServerOnPort(nPort);

    CreateMasterAcceptorThread();

    /* Wait until the master acceptor thread terminates.  This thread
     * is in charge of accepting new client connections and then spinning
     * off new threads to handle the new connections. */
    if (INVALID_HANDLE_VALUE != GetMasterThreadHandle()) {
        WaitThread(GetMasterThreadHandle());
    }

    sleep(1);   /* force context switch */

    return EXIT_SUCCESS;
}
