// command_session.h - Definition of the interface to the new
// COMMANDSESSION structure  This structure helps us to track the
// progress of the invocation of a protocol command from start to finish.
//

#ifndef __COMMAND_SESSION_H__
#define __COMMAND_SESSION_H__

#include "invocation_status.h"
#include "server_symbols.h"

/**
 * @name COMMANDSESSION
 * @brief Structure that tracks data for so-called 'command sessions.'
 * @remarks A command session is an algorithmic construct utilized to capture
 * the essence of the invocation and execution of a server protocol command,
 * from start to finish.  Each command session begins when a command is
 * transmitted from a client, and recognized by the server as a valid protocol
 * command.  The command session is terminated when the server's handling
 * of a protocol command is complete.
 */
typedef struct _tagCOMMANDSESSION {
  /**
   * @name commandSessionID
   * @brief Unique identifer of this COMMANDSESSION instance.
   */
  UUID commandSessionID;

  /**
   * @name lpClient
   * @brief Address of a CLIENTSTRUCT instance.  Identifies which client
   * initiated this command session.
   */
  LPCLIENTSTRUCT lpClient;

  /**
   * @name szCommand
   * @brief Character buffer holding the text of the command as originally
   * transmitted by the client.
   */
  char szCommand[MAX_LINE_LENGTH + 1];

  /**
   * @name szReply
   * @brief Character buffer holding the text of the reply message to be
   * sent by the server.
   */
  char szReply[MAX_LINE_LENGTH + 1];

  /**
   * @name invocationStatus
   * @brief One of the INVOCATIONSTATUS values indicating the status of the
   * prcoessing of this command.
   */
  INVOCATIONSTATUS invocationStatus;

  /**
   * @name  ppszMultilineReplyData
   * @brief Character array pointer holding (if applicable) the lines of
   * text that should be sent to the client as a multiline reply.
   */
  char** ppszMultilineReplyData;

  /**
   * @name nMultilineDataLineCount
   * @brief Count of lines (if applicable) needing to be transmitted as a
   * multiline reply.
   */
  int nMultilineDataLineCount;
} COMMANDSESSION, *LPCOMMANDSESSION, **LPPCOMMANDSESSION;

/**
 * @name BeginCommandSession
 * @brief Begins a new command session and returns an address of an instance
 * of COMMANDSESSION that is initialized for the new session.
 * @param lpClient Address of a CLIENTSTRUCT instance identifying the source
 * of the command transmission,
 * @param pszCommandString Command string originally transmitted by the client.
 * @return Address of an instance of COMMANDSESSION that has been initialized
 * for the new session.
 * @remarks This function is a helpful alias for CreateCommandSession.
 */
LPCOMMANDSESSION BeginCommandSession(LPCLIENTSTRUCT lpClient,
    const char* pszCommandString);

/**
 * @name CreateCommandSession
 * @brief Creates a new instance of COMMANDSESSION, initializes it, and then
 * returns the address of the newly-created and initialized instance.
 * @param lppCommandSession Address of a pointer variable that will receive
 * the address of the newly-created command session struct.
 * @param lpClient Address of a CLIENTSTRUCT instance identifying the source
 * of the command transmission,
 * @param pszCommandString Command string originally transmitted by the client.
 */
void CreateCommandSession(LPPCOMMANDSESSION lppCommandSession,
    LPCLIENTSTRUCT lpClient, const char *pszCommandString);

/**
 * @name EndCommandSession
 * @brief Ends the command session specified.
 * @param lppCommandSession Address of a variable that points to the instance
 * of COMMANDSESSION to be de-allocated.  This pointer's value is set to
 * NULL once the operation completes.
 * @remarks This function is a helpful alias for ReleaseCommandSession.
 */
void EndCommandSession(LPPCOMMANDSESSION lppCommandSession);

/**
 * @name GetCommandSessionClient
 * @brief Gets the address of the CLIENTSTRUCT instance with which this
 * COMMANDSESSION instance is associated.
 * @param lpCommandSession Address of an instance of COMMANDSESSION that refers
 * to its associated client.
 * @return Address of the associated CLIENTSTRUCT, or NULL if not found.
 */
LPCLIENTSTRUCT GetCommandSessionClient(LPCOMMANDSESSION lpCommandSession);

/**
 * @name GetCommandSessionID
 * @brief Gets the value of the commandSessionID member.
 * @param lpCommandSession Address of an instance of COMMANDSESSION.
 * @return Address of the commandSessionID member of the COMMANDSESSION
 * instance specified by lpCommandSession, or NULL if it cannot be found.
 */
UUID* GetCommandSessionID(LPCOMMANDSESSION lpCommandSession);

/**
 * @name GetCommandSessionMultilineData
 * @brief Gets the address of the string array member, ppszMultilineData,
 * of the COMMANDSESSION instance passed.
 * @param lpCommandSession Address of an instance of COMMANDSESSION from which
 * to retrieve the multiline data.
 * @return Address of the ppszMultilineData member of the COMMANDSESSION
 * instance whose address is provided in the lpCommandSession parameter, or
 * NULL if no data found.
 */
char*** GetCommandSessionMultilineData(LPCOMMANDSESSION lpCommandSession);

/**
 * @name GetCommandSessionMultilineDataLineCount
 * @brief Gets the value of the nMultilineDataLineCount member.
 * @param lpCommandSession
 * @param lpCommandSession Address of an instance of COMMANDSESSION.
 * @return Value of the nMultilineDataLineCount member of the COMMANDSESSION
 * instance specified by lpCommandSession, or INT_MIN if it cannot be found.
 */
int GetCommandSessionMultilineDataLineCount(
    LPCOMMANDSESSION lpCommandSession);

/**
 * @name IsCommandSessionEnded
 * @brief Gets a value indicating whether the current command session has
 * ended.
 * @param lpCommandSession Address of an instance of COMMANDSESSION that has
 * the status value to be checked.
 * @return TRUE if the command session is in the ended state; FALSE otherwise.
 */
BOOL IsCommandSessionEnded(LPCOMMANDSESSION lpCommandSession);

/**
 * @name IsCommandSessionValid
 * @brief Provides logic to tell whether a command session is valid or not.
 * @param lpCommandSession Address of the COMMANDSESSION instance to be
 * checked.
 * @return TRUE if the information contained in the members of the
 * COMMANDSESSION object is valid; FALSE otherwise.
 * @remarks If the lpCommandSession pointer has a NULL value, this function
 * returns FALSE.
 */
BOOL IsCommandSessionValid(LPCOMMANDSESSION lpCommandSession);

/**
 * @name ReleaseCommandSession
 * @brief Frees memory associated with the members of the COMMANDSESSION
 * instance whose address is passed, and then releases the memory occupied
 * by the instance.
 * @param pvCommandSession Address of the COMMANDSESSION object to be
 * released.
 * @remarks Once this function returns, the address provided to this function
 * will point to memory that is no longer usable by the caller.  The function
 * prototype is as such so this function can work as a callback.
 */
void ReleaseCommandSession(void* pvCommandSession);

/**
 * @name SetCommandSessionClient
 * @brief Sets the lpClient member of the COMMANDSESSION struct to the
 * address that lpClient is initialized to.
 * @param lpClient Address of an instance of CLIENTSTRUCT that refers to the
 * client who sent the command.
 */
void SetCommandSessionClient(LPCOMMANDSESSION lpCommandSession,
    LPCLIENTSTRUCT lpClient);

/**
 * @name SetCommandSessionCommand
 * @brief Sets the szCommand member of the COMMANDSESSION struct to the
 * specified value.
 * @param pszCommandString Address of a buffer containing the characters
 * transmitted by a client.  Only up to MAX_LINE_LENGTH characters will be
 * stored in the COMMANDSESSION struct instance referred to by the
 * lpCommandSession parameter.
 */
void SetCommandSessionCommand(LPCOMMANDSESSION lpCommandSession,
    const char* pszCommandString);

/**
 * @name SetCommandSessionInvocationStatus
 * @brief Updates the value of the invocationStatus member of the instance
 * of COMMANDSESSION pointed to by lpCommandSession.
 * @param lpCommandSession Address of an instance of COMMANDSESSION whose
 * invocationStatus member is to be updated.
 * @param status New status value. Must be one of the INVOCATIONSTATUS values.
 * @return The prior value of the invocationStatus member.
 */
INVOCATIONSTATUS SetCommandSessionInvocationStatus(
    LPCOMMANDSESSION lpCommandSession, INVOCATIONSTATUS status);

#endif //__COMMAND_SESSION_H__
