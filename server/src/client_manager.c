// client_manager.c - Implementation of functionality to drive communication
// of this server with its clients.

#include "stdafx.h"
#include "server.h"

#include "client_manager.h"
#include "client_list_manager.h"
#include "client_thread_functions.h"
#include "server_functions.h"

///////////////////////////////////////////////////////////////////////////////
// ForciblyDisconnectClient function - used when the server console's user
// kills the server, to sever connections with its clients.
//

void ForciblyDisconnectClient(LPCLIENTSTRUCT lpCS) {
  // lpCS is the reference to the structure containing
  // information for the client whose connection you want to sever

  if (lpCS == NULL) {
    // Null value provided for the client structure; nothing to do.
    return;
  }

  /* Check whether there is still a valid socket file descriptor
   * available for the client endpoint... */
  if (!IsSocketValid(lpCS->nSocket)) {
    // Invalid socket file descriptor available for this client; nothing
    // to do.
    return;
  }

  if (lpCS->bConnected == FALSE) {
    // Nothing to do if the client is already marked as
    // not connected
    return;
  }

  fprintf(stdout, SERVER_DATA_FORMAT, ERROR_FORCED_DISCONNECT);

  /* Forcibly close client connections */
  Send(lpCS->nSocket, ERROR_FORCED_DISCONNECT);
  CloseSocket(lpCS->nSocket);

  LogInfo(CLIENT_DISCONNECTED, lpCS->szIPAddress, lpCS->nSocket);

  if (GetLogFileHandle() != stdout) {
    fprintf(stdout, CLIENT_DISCONNECTED, lpCS->szIPAddress, lpCS->nSocket);
  }

  /* set the client socket file descriptor to now have a value of -1,
   * since its socket has been closed and we've said good bye.  This will
   * prevent any other socket functions from working on this now dead socket.
   */
  lpCS->nSocket = INVALID_SOCKET_VALUE;
  lpCS->bConnected = FALSE;
}

void ReplyToClient(LPCLIENTSTRUCT lpCS, const char* pszBuffer) {
  if (!IsClientStructValid(lpCS)) {
    return;
  }

  int nBytesSent = SendToClient(lpCS, pszBuffer);
  if (nBytesSent < 0) {
    perror("server[ReplyToClient]");

    FreeSocketMutex();

    CleanupServer(ERROR);
  }

  // Asume buffer terminates in a newline.  Report what the server
  // is sending to the console and the log file.  Only log the server
  // as successfully having sent a message if and only if a message was
  // actually sent!
  fprintf(stdout, SERVER_DATA_FORMAT, pszBuffer);

  if (GetLogFileHandle() != stdout) {
    LogInfo(SERVER_DATA_FORMAT, pszBuffer);
  }

  /* track the nBytesSent with the corresponding member of LPCLIENTSTRUCT */
  lpCS->nBytesSent += nBytesSent;
}
