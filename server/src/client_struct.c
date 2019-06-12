// client_struct.c - Provides implementations of functions that create and/or
// manipulate a CLIENTSTRUCT instance (CLIENTSTRUCT is a structure that
// provides information about an individual client connection).
//

#include "stdafx.h"
#include "server.h"

#include "client_struct.h"
#include "client_thread_functions.h"
#include "server_functions.h"

///////////////////////////////////////////////////////////////////////////////
// CreateClientStruct - Allocates memory for, and initializes, a new instance
// of a CLIENTSTRUCT structure with the socket handle and IP address provided.
//

LPCLIENTSTRUCT CreateClientStruct(int nClientSocket,
    const char* pszClientIPAddress) {

  if (!IsSocketValid(nClientSocket)) {
    // The client socket handle passed is not valid; nothing to do.
    fprintf(stderr, SERVER_CLIENT_SOCKET_INVALID);

    CleanupServer(ERROR);
  }

  if (IsNullOrWhiteSpace(pszClientIPAddress)) {
    // The client IP address needs to be filled in; nothing to do.
    fprintf(stderr, CLIENT_IP_ADDR_UNK);

    CleanupServer(ERROR);
  }

  // Allocate memory for a new CLIENTSTRUCT instance
  LPCLIENTSTRUCT lpClientStruct =
      (LPCLIENTSTRUCT) malloc(1 * sizeof(CLIENTSTRUCT));
  memset(lpClientStruct, 0, 1 * sizeof(CLIENTSTRUCT));

  if (lpClientStruct == NULL) {
    fprintf(stderr, FAILED_ALLOC_CLIENT_STRUCT);

    CleanupServer(ERROR);
  }

  /* Tag each client with a universally-unique identifier */
  GenerateNewUUID(&(lpClientStruct->clientID));

  // Save the client socket handle into the nSocket field of the structure
  lpClientStruct->nSocket = nClientSocket;

  // Initialize the pszIPAddress string field of the client structure with
  // the IP address passed to us.
  strncpy(lpClientStruct->szIPAddress, pszClientIPAddress,
      MinimumOf(strlen(pszClientIPAddress), IPADDRLEN));

  /* A client isn't 'connected' until the HELO protocol command is issued
   * by the client. This is to allow clients to 'get ready' before they start
   * being sent other chatters' messages. */
  lpClientStruct->bConnected = FALSE;

  /* Write the client ID out to the console and log */
  LogClientID(lpClientStruct);

  return lpClientStruct;
}

///////////////////////////////////////////////////////////////////////////////
// FreeClient function - Releases operating system resources consumed by the
// client information structure.
//

void FreeClient(void* pvClientStruct) {
  if (pvClientStruct == NULL) {
    // Null pointer passed for the thing to be freed; nothing to do.
    return;
  }

  free(pvClientStruct);
}

///////////////////////////////////////////////////////////////////////////////
// GetCommmandSession function

LPCOMMANDSESSION GetCommandSession(LPCLIENTSTRUCT lpClientStruct) {
  // TODO: Add implementation code here
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// IsClientConnected function

BOOL IsClientConnected(void* pvClientStruct) {
  if (pvClientStruct == NULL) {
    return FALSE;
  }

  LPCLIENTSTRUCT lpCS = (LPCLIENTSTRUCT) pvClientStruct;
  return lpCS->bConnected;
}

///////////////////////////////////////////////////////////////////////////////
// IsClientStructValid function

BOOL IsClientStructValid(void* pvClientStruct) {
  if (pvClientStruct == NULL) {
    return FALSE; // Required parameter; obviously not valid.
  }

  LPCLIENTSTRUCT lpCS = (LPCLIENTSTRUCT) pvClientStruct;
  if (lpCS == NULL) {
    return FALSE;
  }

  if (!IsUUIDValid(&(lpCS->clientID))) {
    return FALSE;   // CLIENTSTRUCT must have a valid UUID for clientID
  }

  /* Note we do not check the hClientThread member, since a particular
   * client can have an initialized CLIENTSTRUCT instance, but either its
   * handler thread has not been created yet, or has been cleaned up/
   * terminated. */

  if (lpCS->nBytesReceived < 0) {
    return FALSE; // nBytesReceived is a count; must be a positive int or zero
  }

  if (lpCS->nBytesSent < 0) {
    return FALSE; // nBytesSent is a count; must be a positive int or zero
  }

  /* we do validate on the socket and IP address, since CLIENTSTRUCT
   * instances get created/initialized when the remote client connects
   * over TCP/IP */

  if (!IsSocketValid(lpCS->nSocket)) {
    return FALSE; // Client does not have a valid socket file descriptor
  }

  if (IsNullOrWhiteSpace(lpCS->szIPAddress)) {
    return FALSE; // Client IP address not initialized
  }

  if (!IsHostnameValid(lpCS->szIPAddress)) {
    return FALSE; // szIPAddress member initialized, but not set to a valid
                    // Internet or local network host name or IP address
  }

  return TRUE;  // CLIENTSTRUCT instance passed is valid
}

///////////////////////////////////////////////////////////////////////////////
// SetCommandSession function

void SetCommandSession(LPCLIENTSTRUCT lpClientStruct,
    LPCOMMANDSESSION lpCommandSession) {
  // TODO: Add implementation code here
}
