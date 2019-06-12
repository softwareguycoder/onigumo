// command_session.c - Implementations of functions that access and manipulate
// COMMANDSESSION struct instances
//

#include "stdafx.h"
#include "server.h"

#include "client_struct.h"
#include "command_session.h"

///////////////////////////////////////////////////////////////////////////////
// Internal-use-only functions

///////////////////////////////////////////////////////////////////////////////
// GenerateNewCommandSessionID function

void GenerateNewCommandSessionID(LPCOMMANDSESSION lpCommandSession) {
  if (lpCommandSession == NULL) {
    return; // Required parameter
  }

  UUID newUUID;
  GenerateNewUUID(&newUUID);

  CopyUUID(&(lpCommandSession->commandSessionID), &newUUID);
}

///////////////////////////////////////////////////////////////////////////////
// Publicly-exposed functions

///////////////////////////////////////////////////////////////////////////////
// CreateCommandSession function

void CreateCommandSession(LPPCOMMANDSESSION lppCommandSession,
    LPCLIENTSTRUCT lpClient, const char *pszCommandString) {
  if (lppCommandSession == NULL) {
    return; // Required parameter
  }

  if (lpClient == NULL || !IsUUIDValid(&(lpClient->clientID))) {
    return; // Required parameter
  }

  if (IsNullOrWhiteSpace(pszCommandString)) {
    return; // Required parameter
  }

  *lppCommandSession = (LPCOMMANDSESSION) malloc(1 * sizeof(COMMANDSESSION));
  if (*lppCommandSession == NULL) {
    ThrowOutOfMemoryException(FAILED_ALLOCATE_COMMAND_SESSION);
  }

  memset(*lppCommandSession, 0, 1 * sizeof(COMMANDSESSION));

  GenerateNewCommandSessionID(*lppCommandSession);

  SetCommandSessionCommand(*lppCommandSession, pszCommandString);

  SetCommandSessionClient(*lppCommandSession, lpClient);
}

///////////////////////////////////////////////////////////////////////////////
// GetCommandSessionClient function

LPCLIENTSTRUCT GetCommandSessionClient(LPCOMMANDSESSION lpCommandSession) {
  if (lpCommandSession == NULL) {
    return NULL;
  }

  return lpCommandSession->lpClient;
}

///////////////////////////////////////////////////////////////////////////////
// GetCommandSessionID function

UUID* GetCommandSessionID(LPCOMMANDSESSION lpCommandSession) {
  if (lpCommandSession == NULL) {
    return NULL;
  }

  if (!IsUUIDValid(&(lpCommandSession->commandSessionID))) {
    return NULL;
  }

  return &(lpCommandSession->commandSessionID);
}

///////////////////////////////////////////////////////////////////////////////
// IsCommandSessionValid function

BOOL IsCommandSessionValid(LPCOMMANDSESSION lpCommandSession) {
  if (lpCommandSession == NULL) {
    return FALSE; // Required parameter; obviously not valid
  }

  if (!IsUUIDValid(GetCommandSessionID(lpCommandSession))) {
    return FALSE; // Must have valid client session ID value
  }

  // TODO: Add additional validation here

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// SetCommandSessionClient function

void SetCommandSessionClient(LPCOMMANDSESSION lpCommandSession,
    LPCLIENTSTRUCT lpClient) {
  if (lpCommandSession == NULL) {
    return; // Required parameter
  }

  lpCommandSession->lpClient = lpClient;
}

///////////////////////////////////////////////////////////////////////////////
// SetCommandSessionCommand function

void SetCommandSessionCommand(LPCOMMANDSESSION lpCommandSession,
    const char* pszCommandString) {
  if (lpCommandSession == NULL) {
    return; // Required parameter
  }

  /* clear out any previous value of szCommand */
  memset(lpCommandSession->szCommand, 0, MAX_LINE_LENGTH + 1);

  if (IsNullOrWhiteSpace(pszCommandString)) {
    return; // Required parameter
  }

  /* copy the chars from pszCommandString into szCommand, but only
   * do MAX_LINE_LENGTH of them */
  strncpy(lpCommandSession->szCommand,
      pszCommandString, MAX_LINE_LENGTH);
}

///////////////////////////////////////////////////////////////////////////////

