// command_session.c - Implementations of functions that access and manipulate
// COMMANDSESSION struct instances
//

#include "stdafx.h"
#include "server.h"

#include "command_session.h"

///////////////////////////////////////////////////////////////////////////////
// Publicly-exposed functions

///////////////////////////////////////////////////////////////////////////////
// CreateCommandSession function

void CreateCommandSession(LPPCOMMANDSESSION lppCommandSession,
    LPCLIENTSTRUCT lpClient, const char *pszCommandString){
  if (lppCommandSession == NULL) {
    return; // Required parameter
  }

  if (lpClient == NULL || !IsUUIDValid(&(lpClient->clientID))) {
    return; // Required parameter
  }

  if (IsNullOrWhiteSpace(pszCommandString)) {
    return; // Required parameter
  }

  *lppCommandSession = (LPCOMMANDSESSION)malloc(1 * sizeof(COMMANDSESSION));
  if (*lppCommandSession == NULL) {
    ThrowOutOfMemoryException(FAILED_ALLOCATE_COMMAND_SESSION);
  }

  memset(*lppCommandSession, 0, 1 * sizeof(COMMANDSESSION));

  UUID newUUID;
  GenerateNewUUID(&newUUID);

  CopyUUID(&((*lppCommandSession)->commandSessionID), &newUUID);

  strncpy((*lppCommandSession)->szCommand,
      pszCommandString, MAX_LINE_LENGTH);

  (*lppCommandSession)->lpClient = lpClient;
}

///////////////////////////////////////////////////////////////////////////////

