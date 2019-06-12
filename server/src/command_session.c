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
// DisplayInvokedCommandSessionID function

void DisplayInvokedCommandSessionID(LPCOMMANDSESSION lpCommandSession) {
  if (!IsCommandSessionValid(lpCommandSession)) {
    return;
  }

  if (!IsDiagnosticMode()) {
    return;
  }

  char *pszCommandSessionID =
      UUIDToString(GetCommandSessionID(lpCommandSession));
  if (IsNullOrWhiteSpace(pszCommandSessionID)) {

    FreeBuffer((void**) &pszCommandSessionID);   // just in case

    return;
  }

  fprintf(stdout, BEGAN_COMMAND_INVOCATION_SESSION_FORMAT,
      pszCommandSessionID);

  FreeBuffer((void**) &pszCommandSessionID);
}

///////////////////////////////////////////////////////////////////////////////
// DisplayEndingCommandSessionMessage function

void DisplayEndingCommandSessionMessage(LPCOMMANDSESSION lpCommandSession) {
  if (!IsCommandSessionValid(lpCommandSession)) {
    return;
  }

  if (!IsDiagnosticMode()) {
    return;
  }

  char *pszCommandSessionID =
      UUIDToString(GetCommandSessionID(lpCommandSession));
  if (IsNullOrWhiteSpace(pszCommandSessionID)) {

    FreeBuffer((void**) &pszCommandSessionID);   // just in case

    return;
  }

  fprintf(stdout, ENDING_COMMAND_INVOCATION_SESSION_FORMAT,
      pszCommandSessionID);

  FreeBuffer((void**) &pszCommandSessionID);
}


///////////////////////////////////////////////////////////////////////////////
// DisplayEndedCommandSessionMessage function

void DisplayEndedCommandSessionMessage(LPCOMMANDSESSION lpCommandSession) {
  if (!IsCommandSessionValid(lpCommandSession)) {
    return;
  }

  if (!IsDiagnosticMode()) {
    return;
  }

  char *pszCommandSessionID =
      UUIDToString(GetCommandSessionID(lpCommandSession));
  if (IsNullOrWhiteSpace(pszCommandSessionID)) {

    FreeBuffer((void**) &pszCommandSessionID);   // just in case

    return;
  }

  fprintf(stdout, ENDED_COMMAND_INVOCATION_SESSION_FORMAT,
      pszCommandSessionID);

  FreeBuffer((void**) &pszCommandSessionID);
}

///////////////////////////////////////////////////////////////////////////////
// GenerateNewCommandSessionID function

/**
 * @name GenerateNewCommandSessionID
 * @brief Generates a new UUID value and populates the commandSessionID
 * member of the COMMANDSESSION instance passed with the new value
 * @param lpCommandSession Address of an instance of COMMANDSESSION whose
 * commandSessionID member is to be populated with a newly-generated UUID.
 */
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
// BeginCommandSession function

LPCOMMANDSESSION BeginCommandSession(LPCLIENTSTRUCT lpClient,
    const char* pszCommandString) {
  LPCOMMANDSESSION lpResult = NULL;

  CreateCommandSession(&lpResult, lpClient, pszCommandString);

  return lpResult;
}

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
// EndCommandSession function

void EndCommandSession(LPPCOMMANDSESSION lppCommandSession) {
  // TODO: Add implementation code here
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
// GetCommandSessionMultilineData function

char*** GetCommandSessionMultilineData(LPCOMMANDSESSION lpCommandSession) {
  if (lpCommandSession == NULL) {
    return NULL;
  }

  if (lpCommandSession->ppszMultilineReplyData == NULL
      || GetCommandSessionMultilineDataLineCount(lpCommandSession) <= 0) {
    return NULL;
  }

  return &(lpCommandSession->ppszMultilineReplyData);
}

///////////////////////////////////////////////////////////////////////////////
// GetCommandSessionMultilineDataLineCount function

int GetCommandSessionMultilineDataLineCount(
    LPCOMMANDSESSION lpCommandSession) {
  int nResult = INT_MIN;
  if (lpCommandSession == NULL) {
    return nResult;
  }

  return lpCommandSession->nMultilineDataLineCount;
}

///////////////////////////////////////////////////////////////////////////////
// IsCommandSessionEnded function

BOOL IsCommandSessionEnded(LPCOMMANDSESSION lpCommandSession) {
  if (lpCommandSession == NULL) {
    return FALSE;
  }

  return lpCommandSession->invocationStatus == SESSIONCLOSED;
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

  if (!IsClientStructValid(
      (void*) GetCommandSessionClient(lpCommandSession))) {
    return FALSE; // Must be associated with a client
  }

  if (GetCommandSessionMultilineDataLineCount(lpCommandSession) < 0) {
    return FALSE; // Count must always be zero or greater
  }

  // TODO: Add additional validation here

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// ReleaseCommandSession function

void ReleaseCommandSession(void* pvCommandSession) {
  if (pvCommandSession == NULL) {
    return; // Required parameter
  }

  LPCOMMANDSESSION lpCS = (LPCOMMANDSESSION) pvCommandSession;
  if (lpCS == NULL) {
    return;  // Required parameter
  }

  if (!IsUUIDValid(GetCommandSessionID(lpCS))) {
    return; // Must have valid command session ID at least
  }

  DisplayEndingCommandSessionMessage(lpCS);

  /* Free the contents of the  multiline data, if there */
  FreeStringArray(GetCommandSessionMultilineData(lpCS),
      GetCommandSessionMultilineDataLineCount(lpCS));

  /* set memory to zero to avoid double-free attempts */
  memset(lpCS, 0, 1 * sizeof(COMMANDSESSION));

  /* Release memory occupied by the structure */
  FreeBuffer((void**) &lpCS);

  DisplayEndedCommandSessionMessage(lpCS);
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
// SetCommandSessionInvocationStatus function

INVOCATIONSTATUS SetCommandSessionInvocationStatus(
    LPCOMMANDSESSION lpCommandSession, INVOCATIONSTATUS status) {
  INVOCATIONSTATUS previousStatus = INVOCATION_STATUS_UNKNOWN;

  if (lpCommandSession == NULL) {
    return previousStatus;
  }

  lpCommandSession->invocationStatus = status;

  return previousStatus;
}

///////////////////////////////////////////////////////////////////////////////

