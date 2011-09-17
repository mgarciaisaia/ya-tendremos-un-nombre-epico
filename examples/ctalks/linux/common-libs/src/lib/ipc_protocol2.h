/*
 * Copyright (c) 2011, C-Talks and/or its owners. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact C-Talks owners, Matias Dumrauf or Facundo Viale
 * or visit https://sites.google.com/site/utnfrbactalks/ if you need additional
 * information or have any questions.
 */

/*
 *	@FILE: 	ipc_protocol2.h
 *	@AUTOR: Facundo Viale
 *	@VERSION:	1.0	(29/08/2009)
 *					- Full Revision
 */


#ifndef OLD_IPC_PROTOCOL2_H_
#define OLD_IPC_PROTOCOL2_H_

	#define _XOPEN_SOURCE 500

	#include "sockets.h"

	#define IPC_PKG_DEFAULT_DESCID_SIZE 15

	typedef enum {
		IPC_NONE			= 0x0,
		IPC_REQUEST			= 0x21,
		IPC_RESPONSE		= 0x22,
		IPC_OK				= 0x23,
		IPC_FAIL			= 0x24
	}e_ipc_pkg_type;

	typedef enum {
		IPC_PKG_AUTODESTROY_TRUE	= 0x1,
		IPC_PKG_AUTODESTROY_FALSE	= 0x0
	}e_ipc_pkg_autodestroy;

	typedef enum {
		IPC_HANDSHAKE_OK			= 200,
		IPC_HANDSHAKE_UNABLE		= 201
	}e_ipc_handshake_state;


	typedef struct {
		char 		  		 	descId[IPC_PKG_DEFAULT_DESCID_SIZE];
		unsigned char  			payload_desc;
		unsigned int  		  	payload_length;
		void 				 	*payload;
	} __attribute__((__packed__)) t_ipc_pkg;

	t_ipc_pkg 				*ipcpkg2_create1(char descId[], e_ipc_pkg_type type, void* payload, int lenght);
	t_ipc_pkg 				*ipcpkg2_create2(e_ipc_pkg_type type, void* payload, int lenght);
	t_ipc_pkg				*ipcpkg2_createFromBuffer1(char descId[], e_ipc_pkg_type type, t_socket_sbuffer* buffer);
	t_ipc_pkg				*ipcpkg2_createFromBuffer2(e_ipc_pkg_type type, t_socket_sbuffer* buffer);
	t_ipc_pkg				*ipcpkg2_createFromString1(char descId[], e_ipc_pkg_type type, char *str);
	t_ipc_pkg				*ipcpkg2_createFromString2(e_ipc_pkg_type type, char *str);
	t_ipc_pkg				*ipcpkg2_createFromSerializer1(char descId[], e_ipc_pkg_type type, void *data, t_socket_sbuffer *(*serializer)(void*));
	t_ipc_pkg				*ipcpkg2_createFromSerializer2(e_ipc_pkg_type type, void *data, t_socket_sbuffer *(*serializer)(void*));

	int						ipcpkg2_isRequest(t_ipc_pkg *pkg);
	int						ipcpkg2_isResponse(t_ipc_pkg *pkg);
	char					*ipcpkg2_getDescriptorID(t_ipc_pkg *pkg);
	e_ipc_pkg_type	   		ipcpkg2_getPayloadDescriptor1(t_ipc_pkg *pkg);
	int						ipcpkg2_getPayloadLength(t_ipc_pkg *pkg);
	void			 		*ipcpkg2_getPayload(t_ipc_pkg *pkg);
	void 					ipcpkg2_getPayloadInBuffer1(t_ipc_pkg *pkg, t_socket_sbuffer *buffer);
	t_socket_sbuffer 		*ipcpkg2_getPayloadInBuffer2(t_ipc_pkg *pkg);
	char 					*ipcpkg2_getPayloadAsString(t_ipc_pkg *pkg);
	char 					*ipcpkg2_getPayloadAsStringBuffer(t_ipc_pkg *pkg, char buffer[]);
	#define ipcpkg2_getPayloadAs(pkg, type) ((type *)(pkg->payload))

	void					ipcpkg2_destroy(t_ipc_pkg *pkg);

	t_socket_sbuffer		*ipcpkg2_serializer(t_ipc_pkg *pkg);
	t_ipc_pkg				*ipcpkg2_deserializer(t_socket_sbuffer *buffer);

	e_ipc_handshake_state	ipcprotocol2_handshake(t_socket_client *client, int process_id, char *server_ip, int server_port);
	int						ipcprotocol2_accepthandshake1(t_socket_client *client);
	int						ipcprotocol2_accepthandshake2(t_socket_client *client,
														  void *context,
														  e_ipc_handshake_state  (*onAccept)(void *context, int process_id, t_socket_client*));
	t_socket_client			*ipcprotocol2_accepthandshake3(t_socket_server *server);
	t_socket_client			*ipcprotocol2_accepthandshake4(t_socket_server *server,
														   void *context,
														   e_ipc_handshake_state  (*onAccept)(void *context, int process_id, t_socket_client*));
	int						ipcprotocol2_send1(t_socket_client *client, t_ipc_pkg *pkg, e_ipc_pkg_autodestroy destroy);
	int						ipcprotocol2_send2(t_socket_client *client, t_ipc_pkg *pkg);
	t_ipc_pkg 				*ipcprotocol2_syncSend1(t_socket_client *client, t_ipc_pkg *pkg, e_ipc_pkg_autodestroy destroy);
	t_ipc_pkg 				*ipcprotocol2_syncSend2(t_socket_client *client, t_ipc_pkg *pkg);
	int						ipcprotocol2_sendFromReq(t_socket_client *client, t_ipc_pkg *pkg, void* payload, int lenght );
	int						ipcprotocol2_sendWithoutPkg(t_socket_client *client, char descId[], e_ipc_pkg_type type, void* payload, int lenght);
	t_ipc_pkg				*ipcprotocol2_recv(t_socket_client *client);
	int						ipcprotocol2_recvWithClosures(t_socket_client *client, void *context, void (*onRequest)(void *context, t_ipc_pkg*), void (*onResponse)(void *context, t_ipc_pkg*));
	void 					ipcprotocol2_select(t_list* servers,
												t_list* clients,
												int usec_timeout,
												void *context,
												e_ipc_handshake_state  (*onHandshake)(void *context, int process_id, t_socket_client*),
												int  (*onAccept)(void *context, t_socket_client*),
												void (*onRequest)(void *context, t_socket_client*, t_ipc_pkg*),
												void (*onResponse)(void *context, t_socket_client*, t_ipc_pkg*),
												int (*onClose)(void *context, t_socket_client*));

#endif /* OLD_IPC_PROTOCOL2_H_ */
