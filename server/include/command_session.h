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
} COMMANDSESSION, *LPCOMMANDESSION, **LPPCOMMANDSESSION;

#endif //__COMMAND_SESSION_H__
