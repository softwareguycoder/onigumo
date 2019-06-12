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

  strncpy((*lppCommandSession)->szCommand,
      pszCommandString, MAX_LINE_LENGTH);

  (*lppCommandSession)->lpClient = lpClient;
}

///////////////////////////////////////////////////////////////////////////////
// SetCommandSessionCommand function

void SetCommandSessionCommand(LPCOMMANDSESSION lpCommandSession,
    const char* pszCommandString) {
  if (lpCommandSession == NULL) {
    return; // Required parameter
  }

  if (IsNullOrWhiteSpace(pszCommandString)) {
    return; // Required parameter
  }

  /* clear out any previous value of szCommand */
  memset(lpCommandSession->szCommand, 0, MAX_LINE_LENGTH + 1);

  /* copy the chars from pszCommandString into szCommand, but only
   * do MAX_LINE_LENGTH of them */
  strncpy(lpCommandSession->szCommand,
      pszCommandString, MAX_LINE_LENGTH);
}

///////////////////////////////////////////////////////////////////////////////

