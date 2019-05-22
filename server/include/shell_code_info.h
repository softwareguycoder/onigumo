// shell_code_info.h - Defines the interface for the SHELLCODEINFO structure
//
// The SHELLCODEINFO structure holds information relevant to the processing
// and/or execution of shellcode.
//

#ifndef __SHELL_CODE_INFO_H__
#define __SHELL_CODE_INFO_H__

typedef struct _tagSHELLCODEINFO {
  int nShellCodeBytes;
  char* pszShellCodeBytes;
} SHELLCODEINFO, *LPSHELLCODEINFO, **LPPSHELLCODEINFO;

/**
 * @brief Allocates storage for, and initializes, a new instance of
 * SHELLCODEINFO in memory.
 * @param lppShellCodeInfo Address of a pointer that will receive the location
 * of the new SHELLCODEINFO instance.
 * @param nShellCodeBytes Number of bytes of shellcode.
 * @param pszShellCodeBytes The actual shellcode bytes themselves.
 * @remarks Constructs a new instances of SHELLCODEINFO and sets its members
 * to the values passed.  Copies the bytes referenced by pszShellCodeBytes
 * into the structure's pszShellCodeBytes member and allocates storage for
 * the same.
 */
void CreateShellCodeInfo(LPPSHELLCODEINFO lppShellCodeInfo,
    int nShellCodeBytes, const char* pszShellCodeBytes);

/**
 * @brief Removes an instance of SHELLCODEINFO, along with the data in its
 * members, from memory.
 * @param lppShellCodeInfo Address of a pointer to an instance of
 * SHELLCODEINFO.
 */
void DestroyShellCodeInfo(LPPSHELLCODEINFO lppShellCodeInfo);

/**
 * @brief Validates the information in the members of the instance of
 * SHELLCODEINFO referred to by this function's argument.
 * @param lpShellCodeInfo Address of an instance of SHELLCODEINFO whose
 * members should have their data validated.
 * @return
 */
BOOL IsShellCodeInfoValid(LPSHELLCODEINFO lpShellCodeInfo);


#endif //__SHELL_CODE_INFO_H__
