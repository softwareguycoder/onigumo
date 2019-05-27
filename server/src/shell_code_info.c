// shell_code_info.c - Implementations of functions that manipulate instances
// of SHELLCODEINFO structures
//

#include "stdafx.h"
#include "server.h"

#include "shell_code_info.h"

///////////////////////////////////////////////////////////////////////////////
// Internal-use-only functions

///////////////////////////////////////////////////////////////////////////////
// Publicly-exposed functions

///////////////////////////////////////////////////////////////////////////////
// CreateShellCodeInfo function

void CreateShellCodeInfo(LPPSHELLCODEINFO lppShellCodeInfo,
    UUID* pClientID, int nEncodedShellCodeBytes,
    const char* pszEncodedShellCodeBytes) {
  if (lppShellCodeInfo == NULL) {
    return; // Required parameter
  }

  if (!IsUUIDValid(pClientID)) {
    return; // Required parameter
  }

  if (nEncodedShellCodeBytes <= 0) {
    return; // Count must be a positive number.
  }

  /* We expect the shellcode bytes to arrive as Base64-encoded values.
   * We are trying to make sure we have enough space for the shellcode
   * bytes (with a null-terminator) but we want to strip off any '\n' chars
   * that might be on the end.  First, we check to make sure that we didn't
   * get a blank string. */

  if (IsNullOrWhiteSpace(pszEncodedShellCodeBytes)) {
    return;
  }

  *lppShellCodeInfo = (LPSHELLCODEINFO)
  malloc(1*sizeof(SHELLCODEINFO));
  if (*lppShellCodeInfo == NULL) {
    fprintf(stderr,
        FAILED_ALLOC_SHELLCODEINFO);
    exit(EXIT_FAILURE);
  }
  memset(*lppShellCodeInfo, 0, sizeof(SHELLCODEINFO));

  int nStringLength = strlen(pszEncodedShellCodeBytes);

  fprintf(stdout, "CreateShellCodeInfo: nStringLength = %d\n", nStringLength);

  (*lppShellCodeInfo)->pszEncodedShellCodeBytes =
  (char*)malloc((nEncodedShellCodeBytes + 1)*sizeof(char));
  if ((*lppShellCodeInfo)->pszEncodedShellCodeBytes == NULL) {
    fprintf(stderr,
        FAILED_ALLOC_ENCODED_SHELLCODE_BYTES);
    FreeBuffer((void**)lppShellCodeInfo);
    exit(EXIT_FAILURE);
  }
  memset((*lppShellCodeInfo)->pszEncodedShellCodeBytes, 0,
      nEncodedShellCodeBytes + 1);

  /* Copy just the encoded bytes, not the terminating <LF>, into the
   * data structure's buffer. */
  strncpy((*lppShellCodeInfo)->pszEncodedShellCodeBytes,
      pszEncodedShellCodeBytes, nEncodedShellCodeBytes);

  /* Other structure member initialization here. */
  (*lppShellCodeInfo)->nEncodedShellCodeBytes = nEncodedShellCodeBytes;

  /* Set the clientID structure member to be the same value as the clientID
   * in pClientID.  This is the UUID that tags the client who sent this
   * shellcode, so we can reassemble that client's shellcode blocks later
   * when the client calls the EXEC command. */
  CopyUUID(&((*lppShellCodeInfo)->clientID), pClientID);
}

///////////////////////////////////////////////////////////////////////////////
// DestroyShellCodeInfo function

void DestroyShellCodeInfo(LPSHELLCODEINFO lpShellCodeInfo) {
  if (lpShellCodeInfo == NULL) {
    return; // Required parameter
  }

  /* First, deallocate the buffer of encoded shellcode bytes,
   * and then deallocate the entire structure. */
  FreeBuffer((void**)&(lpShellCodeInfo->pszEncodedShellCodeBytes));

  memset(lpShellCodeInfo, 0, sizeof(SHELLCODEINFO));

  FreeBuffer((void**)&lpShellCodeInfo);
}

///////////////////////////////////////////////////////////////////////////////
// FindShellCodeBlockForClient function

BOOL FindShellCodeBlockForClient(void* pvClientUUID, void* pvShellCodeBlock)
{
  BOOL bResult = FALSE;
  if (pvClientUUID == NULL) {
    return bResult;
  }

  if (pvShellCodeBlock == NULL) {
    return bResult;
  }

  UUID* pClientUUID = (UUID*)pvClientUUID;
  if (!IsUUIDValid(pClientUUID)) {
    return bResult;
  }

  LPSHELLCODEINFO lpShellCodeInfo =
      (LPSHELLCODEINFO)pvShellCodeBlock;
  if (!IsShellCodeInfoValid(lpShellCodeInfo)) {
    return bResult;
  }

  bResult = AreUUIDsEqual(pClientUUID, &(lpShellCodeInfo->clientID));

  return bResult;
}

///////////////////////////////////////////////////////////////////////////////
// IsShellCodeInfoValid function

BOOL IsShellCodeInfoValid(LPSHELLCODEINFO lpShellCodeInfo) {
  if (lpShellCodeInfo == NULL) {
    return FALSE;
  }

  if (!IsUUIDValid(&(lpShellCodeInfo->clientID))) {
    return FALSE;
  }

  if (lpShellCodeInfo->nEncodedShellCodeBytes <= 0) {
    return FALSE;
  }

  if (IsNullOrWhiteSpace(lpShellCodeInfo->pszEncodedShellCodeBytes)) {
    return FALSE;
  }

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// ReleaseShellCodeInfo function

void ReleaseShellCodeBlock(void* pvData) {
  if (pvData == NULL) {
    return;
  }

  LPSHELLCODEINFO lpSCI = (LPSHELLCODEINFO)pvData;
  if (lpSCI == NULL) {
    return;
  }
  DestroyShellCodeInfo(lpSCI);
}

///////////////////////////////////////////////////////////////////////////////
