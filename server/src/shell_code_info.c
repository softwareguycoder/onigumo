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
    UUID clientID, int nEncodedShellCodeBytes,
    int nTotalEncodedShellCodeBytes,
    const char* pszEncodedShellCodeBytes) {
  if (lppShellCodeInfo == NULL) {
    return; // Required parameter
  }

  if (nEncodedShellCodeBytes <= 0) {
    return; // Count must be a positive number.
  }

  if (nTotalEncodedShellCodeBytes <= 0) {
    return; // Count must be a positive number.
  }

  if (pszEncodedShellCodeBytes == NULL) {
    return;
  }

  /* We just have to assume pszShellCodeBytes contains
   chars. We cannot validate it with IsNullOrWhiteSpace since
   shellcode can contain '\0' all over the place.  We just
   need to trust that the caller of this function told us
   the correct number of bytes.*/

  *lppShellCodeInfo = (LPSHELLCODEINFO)
  malloc(1*sizeof(SHELLCODEINFO));
  if (*lppShellCodeInfo == NULL) {
    fprintf(stderr,
        FAILED_ALLOC_SHELLCODEINFO);
    exit(EXIT_FAILURE);
  }

  (*lppShellCodeInfo)->pszEncodedShellCodeBytes =
  (char*)malloc(((size_t)nEncodedShellCodeBytes)*sizeof(char));
  if ((*lppShellCodeInfo)->pszEncodedShellCodeBytes == NULL) {
    fprintf(stderr,
        FAILED_ALLOC_ENCODED_SHELLCODE_BYTES);
    FreeBuffer((void**)lppShellCodeInfo);
    exit(EXIT_FAILURE);
  }
  memset((*lppShellCodeInfo)->pszEncodedShellCodeBytes, 0,
      nEncodedShellCodeBytes);

  /* Copy just the encoded bytes, not the terminating <LF>, into the
   * data structure's buffer. */
  strncpy((*lppShellCodeInfo)->pszEncodedShellCodeBytes,
      pszEncodedShellCodeBytes, nEncodedShellCodeBytes);

  /* Other structure member initialization here. */
  (*lppShellCodeInfo)->nEncodedShellCodeBytes = nEncodedShellCodeBytes;
  (*lppShellCodeInfo)->nTotalEncodedShellCodeBytes
      = nTotalEncodedShellCodeBytes;
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
// IsShellCodeInfoValid function

BOOL IsShellCodeInfoValid(LPSHELLCODEINFO lpShellCodeInfo) {
  if (lpShellCodeInfo == NULL) {
    return FALSE;
  }

  if (lpShellCodeInfo->nEncodedShellCodeBytes <= 0) {
    return FALSE;
  }

  if (lpShellCodeInfo->nTotalEncodedShellCodeBytes <= 0) {
    return FALSE;
  }

  if (IsNullOrWhiteSpace(lpShellCodeInfo->pszEncodedShellCodeBytes)) {
    return FALSE;
  }

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// ReleaseShellCodeInfo function

void ReleaseShellCodeInfo(void* pvData) {
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
