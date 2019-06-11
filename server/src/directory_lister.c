// directory_lister.c - Implementations of functions that provide the
// functionality needed to list the contents of directory's on the machine
//

#include "stdafx.h"
#include "server.h"

#include "directory_lister.h"

///////////////////////////////////////////////////////////////////////////////
// ListDirectory function

BOOL ListDirectory(const char* pszDirectoryPath, char*** pppszOutputLines,
    int* pnOutputLines) {
  if (IsNullOrWhiteSpace(pszDirectoryPath)) {
    return FALSE; // Required parameter
  }

  /* Be sure to expand the path name string just like Bash would */
  char szExpandedPathName[MAX_PATH + 1];
  memset(szExpandedPathName, 0, MAX_PATH + 1);
  ShellExpand(pszDirectoryPath, szExpandedPathName, MAX_PATH + 1);

  if (!DirectoryExists(szExpandedPathName)) {
    fprintf(stderr,
        "server: Directory '%s' does not exist, or is not a folder.\n",
        szExpandedPathName);
    return FALSE;
  }

  if (pppszOutputLines == NULL) {
    return FALSE; // Required parameter
  }

  if (pnOutputLines == NULL) {
    return FALSE; // Required parameter
  }

  if (!SetCurrentWorkingDirectory(szExpandedPathName)) {
    return FALSE;
  }

  GetSystemCommandOutput(LINUX_DIRECTORY_LIST_COMMAND,
      pppszOutputLines, pnOutputLines);

  return TRUE;
}
