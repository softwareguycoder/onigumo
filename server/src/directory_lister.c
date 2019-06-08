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
    return;
  }

  if (!DirectoryExists(pszDirectoryPath)) {
    ThrowDirectoryNotFoundException(pszDirectoryPath);
  }
}
