// directory_lister.h - Interface to the code that lists contents of
// directories on the file system
//

#ifndef __DIRECTORY_LISTER_H__
#define __DIRECTORY_LISTER_H__

BOOL ListDirectory(const char* pszDirectoryPath, char*** pppszOutputLines,
    int* pnOutputLines);

#endif //__DIRECTORY_LISTER_H__
