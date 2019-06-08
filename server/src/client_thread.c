// client_thread.c - Provides implementation of the thread procedure that
// handles all communications with a specific client
//

#include "stdafx.h"
#include "server.h"

#include "mat.h"
#include "client_manager.h"
#include "client_struct.h"
#include "client_thread.h"
#include "client_thread_functions.h"
#include "client_list_manager.h"

#include "server_functions.h"

///////////////////////////////////////////////////////////////////////////////
// IsLineLengthLimitExceeded function

BOOL IsLineLengthLimitExceeded(LPCLIENTSTRUCT lpSendingClient,
    const char* pszData) {
  if (lpSendingClient == NULL) {
    ThrowNullReferenceException();
  }

  if (IsNullOrWhiteSpace(pszData)) {
    return FALSE;
  }

  /* Per the protocol, lines of input (i.e., commands or data) are
   * restricted to 255 characters in length or less (including the
   * terminating <LF> character but excluding the null-terminator. */
  if (strlen(pszData) >= MAX_LINE_LENGTH + 1) {
    lpSendingClient->nBytesSent +=
        ReplyToClient(lpSendingClient,
            ERROR_FAILED_LINE_LENGTH_LIMIT_EXCEEDED);
    return TRUE;
  }

  return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// ClientThread thread procedure

void *ClientThread(void* pData) {
  SetThreadCancelState(PTHREAD_CANCEL_ENABLE);
  SetThreadCancelType(PTHREAD_CANCEL_DEFERRED);

  /* Be sure to register the termination semaphore so we can be
   * signalled to stop if necessary */
  RegisterEvent(TerminateClientThread);

  /* Valid user state data consisting of a reference to the CLIENTSTRUCT
   * instance giving information for this client must be passed. */
  LPCLIENTSTRUCT lpSendingClient = GetSendingClientInfo(pData);

  lpSendingClient->nBytesReceived =
  ZERO_BYTES_TOTAL_RECEIVED;

  while (1) {
    // Receive all the lines of text that the client wants to send,
    // and put them all into a buffer.
    char* pszData = NULL;

    /* Free the received data so it does not leak memory */
    FreeBuffer((void**) &pszData);

    /* Check whether the client's socket endpoint is valid. */
    if (!IsSocketValid(lpSendingClient->nSocket)) {
      // Nothing to do.
      break;
    }
    int nBytesReceived = 0;

    if ((nBytesReceived = ReceiveFromClient(lpSendingClient, &pszData))
        > 0) {

      if (IsLineLengthLimitExceeded(lpSendingClient, pszData)) {
        continue;
      }

      /* first, check if we have a protocol command.  If so, skip to
       * next loop. We know if this is a protocol command rather than a
       * chat message because the HandleProtocolCommand returns a value
       * of TRUE in this case. */
      if (HandleProtocolCommand(lpSendingClient, pszData))
        continue;

      /* Free the received data so it does not leak memory */
      FreeBuffer((void**) &pszData);

      /* Check if the termination semaphore has been signalled, and
       * stop this loop if so. */
      if (g_bShouldTerminateClientThread) {
        g_bShouldTerminateClientThread = FALSE;
        break;
      }

      /* If the client has closed the connection, bConnected will
       * be FALSE.  This is our signal to stop looking for further input. */
      if (lpSendingClient->bConnected == FALSE
          || !IsSocketValid(lpSendingClient->nSocket)) {

        LogDebug(DISCONNECTED_CLIENT_DETECTED);

        break;
      }
    }
  }

  // reset the termination semaphore
  if (g_bShouldTerminateClientThread) {
    g_bShouldTerminateClientThread = FALSE;
  }

  fprintf(stdout, CLIENT_THREAD_ENDING);

  TellUserServerIsListening(g_nServerPort);

  // done
  return NULL;
}
