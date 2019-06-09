// directory_lister.c - Implementations of functions that provide the
// functionality needed to list the contents of directory's on the machine
//

#include "stdafx.h"
#include "server.h"

#include "directory_lister.h"

///////////////////////////////////////////////////////////////////////////////
// ListDirectory function

void ListDirectory(const char* pszDirectoryPath, char*** pppszOutputLines,
    int* pnOutputLines) {
  if (IsNullOrWhiteSpace(pszDirectoryPath)) {
    return; // Required parameter
  }

  /* Be sure to expand the path name string just like Bash would */
  char szExpandedPathName[MAX_PATH + 1];
  memset(szExpandedPathName, 0, MAX_PATH + 1);
  ShellExpand(pszDirectoryPath, szExpandedPathName, MAX_PATH + 1);

  if (!DirectoryExists(szExpandedPathName)) {
    ThrowDirectoryNotFoundException("ListDirectory",
        szExpandedPathName, NULL);
  }

  if (pppszOutputLines == NULL) {
    return; // Required parameter
  }

  if (pnOutputLines == NULL) {
    return; // Required parameter
  }

  SetCurrentWorkingDirectory(szExpandedPathName);

  GetSystemCommandOutput(LINUX_DIRECTORY_LIST_COMMAND,
      pppszOutputLines, pnOutputLines);
}
