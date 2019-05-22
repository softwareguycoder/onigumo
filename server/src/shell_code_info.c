// shell_code_info.c - Implementations of functions that manipulate instances
// of SHELLCODEINFO structures
//

#include "shell_code_info.c"

#include "stdafx.h"
#include "server.h"


///////////////////////////////////////////////////////////////////////////////
// Internal-use-only functions

///////////////////////////////////////////////////////////////////////////////
// Publicly-exposed functions

///////////////////////////////////////////////////////////////////////////////
// CreateShellCodeInfo function

void CreateShellCodeInfo(LPPSHELLCODEINFO lppShellCodeInfo,
    int nShellCodeBytes, const char* pszShellCodeBytes) {
  if (lppShellCodeInfo == NULL) {
    return;
  }

  if (nShellCodeBytes <= 0L) {
    return;
  }

  if (pszShellCodeBytes == NULL) {
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
    "Failed to allocate memory for SHELLCODEINFO instance.\n");
    exit(EXIT_FAILURE);
  }

  (*lppShellCodeInfo)->pszShellCodeBytes =
  (char*)malloc(((size_t)nShellCodeBytes)*sizeof(char));
  if ((*lppShellCodeInfo)->pszShellCodeBytes == NULL) {
    fprintf(stderr,
    "Failed to allocate memory for shellcode bytes.\n");
    FreeBuffer((void**)lppShellCodeInfo);
    exit(EXIT_FAILURE);
  }
  memset((*lppShellCodeInfo)->pszShellCodeBytes, 0,
  nShellCodeBytes);

  for(long i = 0;i < nShellCodeBytes;i++) {
    ((*lppShellCodeInfo)->pszShellCodeBytes)[i] =
    pszShellCodeBytes[i];
  }

  (*lppShellCodeInfo)->nShellCodeBytes = nShellCodeBytes;

}

///////////////////////////////////////////////////////////////////////////////
// DestroyShellCodeInfo function

void DestroyShellCodeInfo(LPPSHELLCODEINFO lppShellCodeInfo) {
  if (lppShellCodeInfo == NULL
      || *lppShellCodeInfo == NULL) {
    return; // Required parameter
  }

  FreeBuffer((void**)&((*lppShellCodeInfo)->pszShellCodeBytes));
  FreeBuffer((void**)lppShellCodeInfo);
}

///////////////////////////////////////////////////////////////////////////////
// IsShellCodeInfoValid function

BOOL IsShellCodeInfoValid(LPSHELLCODEINFO lpShellCodeInfo) {
  if (lpShellCodeInfo == NULL) {
    return FALSE;
  }

  if (lpShellCodeInfo->nShellCodeBytes <= 0) {
    return FALSE;
  }

  if (lpShellCodeInfo->pszShellCodeBytes == NULL) {
    return FALSE;
  }

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
