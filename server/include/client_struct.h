/*
 * clientStruct.h
 *
 *  Created on: Feb 3, 2019
 *      Author: bhart
 */

#ifndef __CLIENT_STRUCT_H__
#define __CLIENT_STRUCT_H__

#include "stdafx.h"
#include "server_symbols.h"

/**
 * @brief Structure that contains information about connected clients.
 */
typedef struct _tagCLIENTSTRUCT {
	/**
	 * @name clientID
	 * @brief Holds a UUID value that uniquely identifies this client.
	 */
    UUID clientID;

    /**
     * @name szIPAddress
     * @brief Buffer to hold a string that identifies which IP address the
     * client is coming from.
     */
	char szIPAddress[IPADDRLEN];

	/**
	 * @name lpCommandSession
	 * @brief Address of an instance of a COMMANDSESSION struct that is
	 * tracking the progress of a server protocol command issued by this
	 * client.
	 */
	LPCOMMANDSESSION lpCommandSession;

	/**
	 * @name nSocket
	 * @brief Value of the socket file descriptor to use when communicating
	 * with this client.
	 */
	int nSocket;

	/**
	 * @name hClientThread
	 * @brief HTHREAD handle to the thread over which communications with
	 * this client take place.
	 */
	HTHREAD hClientThread; /* handle to the thread this client is chatting on */

	/**
	 * @name nBytesReceived
	 * @brief Set this member to the total number of bytes received from the
	 * client.
	 */
	long long nBytesReceived;

	/**
	 * @name nBytesSent
	 * @brief Set this member to the total number of bytes sent to the client.
	 */
	long long nBytesSent;

	/**
	 * @name bConnected
	 * @brief Flag that indicates whether this client is in the connected
	 * state.
	 */
	BOOL bConnected;
} CLIENTSTRUCT, *LPCLIENTSTRUCT;

/**
 * @brief Creates an instance of a CLIENTSTRUCT structure and fills it with info
 * about the client.
 * @param nClientSocket Client's server endpoint socket file descriptor.
 * @param pszClientIPAddress Client's IP address as a string (i.e., 268.7.34.2)
 * @returns LPCLIENTSTRUCT pointing to the newly-created-and-initialized instance
 * of the client structure.
 * @remarks Supplies a reference to an instance of CLIENTSTRUCT filled with the
 * socket for sending data back to clients in reply to protocol commands or
 * chat messages.
 */
LPCLIENTSTRUCT CreateClientStruct(int nClientSocket,
		const char* pszClientIPAddress);

/**
 * @brief Releases the memory allocated for a client structure pointer back
 * to the system.
 * @param pClientStruct Pointer to a CLIENTSTRUCT instance whose memory is to
 * be freed.
 */
void FreeClient(void* pClientStruct);

/**
 * @name GetCommandSession
 * @brief Gets the address of the COMMANDSESSION instance that this client
 * is tracking for the currently-issued protocol command.  Can be NULL, which
 * means that the server is not curently processing a command sent by the
 * server at this time.
 * @param lpClientStruct Address of a CLIENTSTRUCT instance from whose member
 * to obtain the value sought.
 * @return NULL or the address of the COMMANDSTRUCT instance.
 */
LPCOMMANDSESSION GetCommandSession(LPCLIENTSTRUCT lpClientStruct);

/**
 * @brief Determines whether the client referenced is in the connnected state.
 * @remarks Connected state is defined as (a) being connected over TCP and
 * (b) having successfully issued the HELO protocol command.
 */
BOOL IsClientConnected(void* pvClientStruct);

/**
 * @name IsClientStructValid
 * @brief Validates the CLIENTSTRUCT instance passed to it.
 * @param pvClientStruct Address of an instance of CLIENTSTRUCT whose
 * members' data needs to be validated.
 * @return TRUE if the CLIENTSTRUCT instance referred to by lpClientStruct
 * contains valid information in its members.
 */
BOOL IsClientStructValid(void* pvClientStruct);

/**
 * @name SetCommandSession
 * @brief Sets the address of the COMMANDSESSION instance that is being used
 * by the server to track the progress of its own handling of the protocol
 * command that has just been issued by this client.
 * @param lpClientStruct Address of the CLIENTSTRUCT instance whose member
 * should be set to the value provided.
 * @param lpCommandSession Address of the COMMANDSTRUCT instance to assign
 * to the lpCommandSession member of CLIENTSTRUCT.
 */
void SetCommandSession(LPCLIENTSTRUCT lpClientStruct,
    LPCOMMANDSESSION lpCommandSession);

#endif /* __CLIENT_STRUCT_H__ */
