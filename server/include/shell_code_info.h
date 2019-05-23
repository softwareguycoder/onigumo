// shell_code_info.h - Defines the interface for the SHELLCODEINFO structure
//
// The SHELLCODEINFO structure holds information relevant to the processing
// and/or execution of shellcode.
//

#ifndef __SHELL_CODE_INFO_H__
#define __SHELL_CODE_INFO_H__

/**
 * @name SHELLCODEINFO
 * @brief Defines a packet that contains a chunk of encoded, executable
 * shellcode.
 */
typedef struct _tagSHELLCODEINFO {
  /**
   * @name clientID
   * @brief Universally-unique identifier of the client who owns
   * this block of shellcode.
   */
  UUID clientID;

  /**
   * @name nEncodedShellCodeBytes
   * @brief Count of bytes of Base-64 encoded shellcode in this block.
   */
  int nEncodedShellCodeBytes;

  /**
   * @name nTotalEncodedShellCodeBytes
   * @brief Total bytes of Base-64 encoded shellcode across all blocks.
   */
  int nTotalEncodedShellCodeBytes;

  /**
   * @name pszEncodedShellCodeBytes
   * @brief Address of a character array containing Base64-encoded version
   * of the shellcode block.
   */
  char* pszEncodedShellCodeBytes;
} SHELLCODEINFO, *LPSHELLCODEINFO, **LPPSHELLCODEINFO;

/**
 * @brief Allocates storage for, and initializes, a new instance of
 * SHELLCODEINFO in memory.
 * @param clientID Universally-unique identifier (ID) of the client who
 * owns the shellcode block defined by this structure.
 * @param lppShellCodeInfo Address of a pointer that will receive the location
 * of the new SHELLCODEINFO instance.
 * @param nEncodedShellCodeBytes Count of Base64-encoded bytes of shellcode
 * in this block.
 * @param nTotalEncodedShellCodeBytes Total count of Base64-encoded bytes
 * of shellcode across all blocks.
 * @param pszEncodedShellCodeBytes The Base64-encoded version of the
 * shellcode block.
 * @remarks Constructs a new instance of SHELLCODEINFO and sets its members
 * to the values passed.  Copies the bytes referenced by pszShellCodeBytes
 * into the structure's pszShellCodeBytes member and allocates storage for
 * the same.
 */
void CreateShellCodeInfo(LPPSHELLCODEINFO lppShellCodeInfo,
    UUID clientID, int nEncodedShellCodeBytes,
    int nTotalEncodedShellCodeBytes,
    const char* pszEncodedShellCodeBytes);

/**
 * @brief Removes an instance of SHELLCODEINFO, along with the data in its
 * members, from memory.
 * @param lppShellCodeInfo Address of a pointer to an instance of
 * SHELLCODEINFO.
 */
void DestroyShellCodeInfo(LPSHELLCODEINFO lpShellCodeInfo);

/**
 * @brief Validates the information in the members of the instance of
 * SHELLCODEINFO referred to by this function's argument.
 * @param lpShellCodeInfo Address of an instance of SHELLCODEINFO whose
 * members should have their data validated.
 * @return
 */
BOOL IsShellCodeInfoValid(LPSHELLCODEINFO lpShellCodeInfo);

/**
 * @brief Callback to trigger the logic in DestroyShellCodeInfo.
 * @param pvData Address of the data to be released.
 * @remarks This callback is needed for compatibility with the linked-list
 * implemented in the list_core library.  All this function is, is an alias
 * for DestroyShellCodeInfo.
 */
void ReleaseShellCodeInfo(void* pvData);


#endif //__SHELL_CODE_INFO_H__
