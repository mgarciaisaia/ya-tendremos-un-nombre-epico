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
 *	@FILE: 	ipc_protocol2.c
 *	@AUTOR: Facundo Viale
 *	@VERSION:	1.0	(29/08/2009)
 *					- Full Revision
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>

#include "sockets.h"
#include "utils.h"

#include "ipc_protocol2.h"


/* -------------------------------------------------------------------------------- */

/*
 * @NAME: ipcpkg2_create1
 * @DESC: Crea un paquete IPC a partir de descId, type, payload y payload lenght
 */
t_ipc_pkg *ipcpkg2_create1(char descId[], e_ipc_pkg_type type, void* payload, int lenght){
	t_ipc_pkg *pkg = malloc( sizeof(t_ipc_pkg) );

	memcpy(pkg->descId, descId, IPC_PKG_DEFAULT_DESCID_SIZE);
	pkg->payload_desc = type;
	pkg->payload_length = lenght;
	pkg->payload = malloc( lenght );
	memcpy(pkg->payload, payload, lenght);

	return pkg;
}

/*
 * @NAME: ipcpkg2_create2
 * @DESC: Crea un paquete IPC a partir de type, payload y payload lenght
 * @NOTE: El descId es generado automaticamente segun el time en milisegundos
 */
t_ipc_pkg *ipcpkg2_create2(e_ipc_pkg_type type, void* payload, int lenght){
	char buffer[IPC_PKG_DEFAULT_DESCID_SIZE];
	struct timeb tmili;

	ftime(&tmili);

	return ipcpkg2_create1(utils_convertIntToStringBuffer(tmili.millitm, buffer), type, payload, lenght);
}

/*
 * @NAME: ipcpkg2_createFromBuffer1
 * @DESC: Crea un paquete IPC a partir de descId, type y un buffer de serializacion (t_socket_sbuffer)
 */
t_ipc_pkg *ipcpkg2_createFromBuffer1(char descId[], e_ipc_pkg_type type, t_socket_sbuffer* buffer){
	return ipcpkg2_create1(descId, type,  buffer->serializated_data,  buffer->size);
}

/*
 * @NAME: ipcpkg2_createFromBuffer2
 * @DESC: Crea un paquete IPC a partir de type y un buffer de serializacion (t_socket_sbuffer)
 * @NOTE: El descId es generado automaticamente segun el time en milisegundos
 */
t_ipc_pkg *ipcpkg2_createFromBuffer2(e_ipc_pkg_type type, t_socket_sbuffer* buffer){
	return ipcpkg2_create2(type,  buffer->serializated_data,  buffer->size);
}

/*
 * @NAME: ipcpkg2_createFromString1
 * @DESC: Crea un paquete IPC a partir de descId, type y un String
 */
t_ipc_pkg *ipcpkg2_createFromString1(char descId[], e_ipc_pkg_type type, char *str){
	return ipcpkg2_create1(descId, type, str, strlen( str ) + 1);
}

/*
 * @NAME: ipcpkg2_createFromString2
 * @DESC: Crea un paquete IPC a partir de type y un String
 * @NOTE: El descId es generado automaticamente segun el time en milisegundos
 */
t_ipc_pkg *ipcpkg2_createFromString2(e_ipc_pkg_type type, char *str){
	return ipcpkg2_create2(type, str, strlen( str ) + 1);
}

/*
 * @NAME: ipcpkg2_createFromSerializer1
 * @DESC: Crea un paquete IPC a partir de descId, type, una estructura y el serializador de la estructura
 */
t_ipc_pkg *ipcpkg2_createFromSerializer1(char descId[], e_ipc_pkg_type type, void *data, t_socket_sbuffer *(*serializer)(void*)){
	t_socket_sbuffer *buffer = serializer(data);
	t_ipc_pkg *pkg = ipcpkg2_createFromBuffer1(descId, type, buffer);
	sockets_sbufferDestroy(buffer);
	return pkg;
}

/*
 * @NAME: ipcpkg2_createFromSerializer2
 * @DESC: Crea un paquete IPC a partir de type, una estructura y el serializador de la estructura
 * @NOTE: El descId es generado automaticamente segun el time en milisegundos
 */
t_ipc_pkg *ipcpkg2_createFromSerializer2(e_ipc_pkg_type type, void *data, t_socket_sbuffer *(*serializer)(void*)){
	t_socket_sbuffer *buffer = serializer(data);
	t_ipc_pkg *pkg = ipcpkg2_createFromBuffer2(type, buffer);
	sockets_sbufferDestroy(buffer);
	return pkg;
}

/*
 * @NAME: ipcpkg2_isRequest
 * @DESC: Retorna 1 si el pkg IPC es un request
 */
int ipcpkg2_isRequest(t_ipc_pkg *pkg){
	return pkg->payload_desc == IPC_REQUEST;
}

/*
 * @NAME: ipcpkg2_isResponse
 * @DESC: Retorna 1 si el pkg IPC es un response
 */
int ipcpkg2_isResponse(t_ipc_pkg *pkg){
	return pkg->payload_desc == IPC_RESPONSE;
}

/*
 * @NAME: ipcpkg2_isResponse
 * @DESC: Retorna 1 si el pkg IPC es un response
 */
char *ipcpkg2_getDescriptorID(t_ipc_pkg *pkg){
	return pkg->descId;
}

/*
 * @NAME: ipcpkg2_getPayloadDescriptor
 * @DESC: Retorna el descriptor del payload
 */
e_ipc_pkg_type ipcpkg2_getPayloadDescriptor1(t_ipc_pkg *pkg){
	if( pkg->payload_desc == IPC_REQUEST ) return IPC_REQUEST;
	if( pkg->payload_desc == IPC_RESPONSE ) return IPC_RESPONSE;
	return IPC_NONE;
}

/*
 * @NAME: ipcpkg2_getPayloadDescriptor
 * @DESC: Retorna el descriptor del payload
 */
int ipcpkg2_getPayloadDescriptor2(t_ipc_pkg *pkg){
	return pkg->payload_desc;
}

/*
 * @NAME: ipcpkg2_getPayloadLength
 * @DESC: Retorna la longitud del payload
 */
int	ipcpkg2_getPayloadLength(t_ipc_pkg *pkg){
	return pkg->payload_length;
}

void *ipcpkg2_getPayload(t_ipc_pkg *pkg){
	return pkg->payload;
}

/*
 * @NAME: ipcpkg2_getPayloadInBuffer1
 * @DESC: Retorna el payload en un buffer
 * @NOTE: Se copian los punteros por lo que hay que tener en cuenta como se maneje el free
 */
void ipcpkg2_getPayloadInBuffer1(t_ipc_pkg *pkg, t_socket_sbuffer *buffer){
	buffer->size = pkg->payload_length;
	buffer->serializated_data = pkg->payload;
}

/*
 * @NAME: ipcpkg2_getPayloadInBuffer2
 * @DESC: Retorna el payload en un buffer clonando su contenido en este
 */
t_socket_sbuffer *ipcpkg2_getPayloadInBuffer2(t_ipc_pkg *pkg){
	t_socket_sbuffer *sbuffer = malloc( sizeof(t_socket_sbuffer) );
	sbuffer->size = pkg->payload_length;
	sbuffer->serializated_data = malloc( sizeof(sbuffer->size) );
	memcpy(sbuffer->serializated_data, pkg->payload, pkg->payload_length);
	return sbuffer;
}

/*
 * @NAME: ipcpkg2_getPayloadAsString
 * @DESC: Retorna el Payload como string
 */
char *ipcpkg2_getPayloadAsString(t_ipc_pkg *pkg){
	return pkg->payload;
}

/*
 * @NAME: ipcpkg2_getPayloadAsStringBuffer
 * @DESC: Retorna el Payload en un buffer string
 */
char *ipcpkg2_getPayloadAsStringBuffer(t_ipc_pkg *pkg, char buffer[]){
	memcpy(buffer, pkg->payload, pkg->payload_length);
	return buffer;
}

/*
 * @NAME: ipcpkg2_destroy
 * @DESC: Destruye el pkg IPC incluyendo el payload
 * @NOTE: Cuando el pkg IPC se crea el payload siempre es clonado con respecto al original
 */
void ipcpkg2_destroy(t_ipc_pkg *pkg){
	if( pkg->payload != NULL &&  pkg->payload_length > 0 ) free(pkg->payload);
	free(pkg);
}

/* -------------------------------------------------------------------------------- */

/*
 * @NAME: ipcpkg2_serializer
 * @DESC: Transforma un t_ipc_pkg en un buffer de bytes
 */
t_socket_sbuffer	*ipcpkg2_serializer(t_ipc_pkg *pkg){
	t_socket_sbuffer *sbuffer = malloc( sizeof(t_socket_sbuffer) );

	sbuffer->size              = sizeof(t_ipc_pkg) + pkg->payload_length - sizeof(void*);
	sbuffer->serializated_data = malloc(sbuffer->size);
	memcpy(sbuffer->serializated_data, pkg, sizeof(t_ipc_pkg) - sizeof(void*));
	memcpy(sbuffer->serializated_data + sizeof(t_ipc_pkg) - sizeof(void*), pkg->payload, pkg->payload_length);

	return sbuffer;
}

/*
 * @NAME: ipcpkg2_serializer
 * @DESC: Transforma un buffer de bytes en un t_ipc_pkg
 */
t_ipc_pkg			*ipcpkg2_deserializer(t_socket_sbuffer *buffer){
	t_ipc_pkg *pkg = malloc( sizeof(t_ipc_pkg) );

	memcpy(pkg, buffer->serializated_data, sizeof(t_ipc_pkg) - sizeof(void*));
	if( pkg->payload_length > 0 ){
		pkg->payload = malloc(pkg->payload_length);
		memcpy(pkg->payload, buffer->serializated_data + sizeof(t_ipc_pkg) - sizeof(void*), pkg->payload_length);
	}else{
		pkg->payload = NULL;
	}
	return pkg;
}

/* -------------------------------------------------------------------------------- */

/*
 * @NAME: ipcprotocol2_handshake
 * @DESC: Inicia la negociacion para conectarse a un servidor
 */
e_ipc_handshake_state ipcprotocol2_handshake(t_socket_client *client, int process_id, char *server_ip, int server_port){
	e_ipc_handshake_state ret = IPC_HANDSHAKE_OK;

	if( !sockets_connect(client, server_ip, server_port) ) return IPC_HANDSHAKE_UNABLE;

	if( !ipcprotocol2_send1(client, ipcpkg2_create2(process_id, NULL, 0), IPC_PKG_AUTODESTROY_TRUE) ) return IPC_HANDSHAKE_UNABLE;


	{
		t_ipc_pkg *pkg;
		if( (pkg = ipcprotocol2_recv(client)) == NULL ){
			ret = IPC_HANDSHAKE_UNABLE;
		}else{
			if( ipcpkg2_getPayloadLength(pkg) != 0 ){
				ret = IPC_HANDSHAKE_UNABLE;
			} else {
				if( ipcpkg2_getPayloadDescriptor1(pkg) == IPC_FAIL ){
					ret = IPC_HANDSHAKE_UNABLE;
				}
			}
			ipcpkg2_destroy(pkg);
		}
	}

	return ret;
}

/*
 * @NAME: ipcprotocol2_accepthandshake1
 * @DESC: Acepta una conexion IPC realizando la negociacion necesaria
 */
int	ipcprotocol2_accepthandshake1(t_socket_client *client){
	return ipcprotocol2_accepthandshake2(client, NULL, NULL);
}

/*
 * @NAME: ipcprotocol2_accepthandshake2
 * @DESC: Acepta una conexion IPC realizando la negociacion necesaria y permite decidir en base
 * 		  al id recivido en el payloaddesc
 */
int	ipcprotocol2_accepthandshake2(t_socket_client *client, void* context, e_ipc_handshake_state (*onAccept)(void *context, int process_id, t_socket_client*)){
	e_ipc_handshake_state ret = IPC_HANDSHAKE_OK;
	e_ipc_pkg_type type = IPC_OK;
	t_ipc_pkg *pkg;

	if( (pkg = ipcprotocol2_recv(client)) == NULL ) return IPC_HANDSHAKE_UNABLE;


	if( ipcpkg2_getPayloadLength(pkg) != 0 ){
		ret = IPC_HANDSHAKE_UNABLE;
	} else {
		if( onAccept != NULL ){
			ret = onAccept(context, ipcpkg2_getPayloadDescriptor2(pkg) ,client);
		}
	}
	ipcpkg2_destroy(pkg);

	if( ret == IPC_HANDSHAKE_OK ) type = IPC_OK;
	if( ret == IPC_HANDSHAKE_UNABLE ) type = IPC_FAIL;

	if( !ipcprotocol2_send1(client, ipcpkg2_create1(pkg->descId, type, NULL, 0), IPC_PKG_AUTODESTROY_TRUE) ){
		ret = IPC_HANDSHAKE_UNABLE;
	}

	return ret;
}

/*
 * @NAME: ipcprotocol2_accepthandshake3
 * @DESC: Acepta una conexion IPC realizando la negociacion necesaria
 */
t_socket_client	*ipcprotocol2_accepthandshake3(t_socket_server *server){
	return ipcprotocol2_accepthandshake4(server, NULL, NULL);
}

/*
 * @NAME: ipcprotocol2_accepthandshake4
 * @DESC: Acepta una conexion IPC realizando la negociacion necesaria y permite decidir en base
 * 		  al id recivido en el payloaddesc
 */
t_socket_client	*ipcprotocol2_accepthandshake4(t_socket_server *server, void *context, e_ipc_handshake_state (*onAccept)(void *context, int process_id, t_socket_client*)){
	t_socket_client *newclient = sockets_accept(server);
	if( newclient != NULL ){
		if( ipcprotocol2_accepthandshake2(newclient, context, onAccept) == IPC_HANDSHAKE_UNABLE ) {
			sockets_destroyClient(newclient);
			newclient = NULL;
		}
	}
	return newclient;
}

/*
 * @NAME: ipcprotocol2_send1
 * @DESC: Envia un t_ipc_pkg atravez de una conexion indicando si el pkg es liberado o no automaticamente despues del envio
 */
int	ipcprotocol2_send1(t_socket_client *client, t_ipc_pkg *pkg, e_ipc_pkg_autodestroy destroy){
	t_socket_sbuffer *buffer = ipcpkg2_serializer(pkg);
	int ret = sockets_sendSBuffer(client, buffer);

	sockets_sbufferDestroy(buffer);

	if( destroy == IPC_PKG_AUTODESTROY_TRUE )  ipcpkg2_destroy(pkg);

	return ret;
}

/*
 * @NAME: ipcprotocol2_send2
 * @DESC: Envia un t_ipc_pkg atravez de una conexion
 */
int ipcprotocol2_send2(t_socket_client *client, t_ipc_pkg *pkg){
	return ipcprotocol2_send1(client, pkg, IPC_PKG_AUTODESTROY_FALSE);
}

/*
 * @NAME: ipcprotocol2_syncSend1
 * @DESC: Envia un t_ipc_pkg atravez de una conexion indicando si el pkg es liberado o no automaticamente despues del envio,
 * 		  y se queda esperando a recivir una respuesta retornando el resultado de esta recepcion.
 * @NOTA: Esta funcion es bloqueante
 */
t_ipc_pkg *ipcprotocol2_syncSend1(t_socket_client *client, t_ipc_pkg *pkg, e_ipc_pkg_autodestroy destroy){
	t_socket_sbuffer *buffer = ipcpkg2_serializer(pkg);
	int ret = sockets_sendSBuffer(client, buffer);

	sockets_sbufferDestroy(buffer);

	if( destroy == IPC_PKG_AUTODESTROY_TRUE )  ipcpkg2_destroy(pkg);

	if( ret > 0 ){
		return ipcprotocol2_recv(client);
	}

	return NULL;
}

/*
 * @NAME: ipcprotocol2_syncSend1
 * @DESC: Envia un t_ipc_pkg atravez de una conexion y se queda esperando a recivir una respuesta retornando el resultado de esta recepcion.
 * @NOTA: Esta funcion es bloqueante
 */
t_ipc_pkg *ipcprotocol2_syncSend2(t_socket_client *client, t_ipc_pkg *pkg){
	return ipcprotocol2_syncSend1(client, pkg, IPC_PKG_AUTODESTROY_FALSE);
}


/*
 * @NAME: ipcprotocol2_sendFromReq
 * @DESC: Envia un t_ipc_pkg atravez de una conexion, construyendo este a partir de un t_ipc_pkg request ( es decir usa el descid ),
 * 		  un payload y la longitud de este. SI no es un request no hace nada.
 */
int	ipcprotocol2_sendFromReq(t_socket_client *client, t_ipc_pkg *pkg, void* payload, int lenght ){
	int ret = 0;
	if( ipcpkg2_isRequest(pkg) ){
		ret = ipcprotocol2_send1(client, ipcpkg2_create1(pkg->descId, IPC_RESPONSE, payload, lenght), IPC_PKG_AUTODESTROY_TRUE);
	}
	return ret;
}

/*
 * @NAME: ipcprotocol2_sendWithoutPkg
 * @DESC: Envia un t_ipc_pkg atravez de una conexion, construyendo este a partir de cada uno de los parametros posibles de un t_ipc_pkg
 */
int	ipcprotocol2_sendWithoutPkg(t_socket_client *client, char descId[], e_ipc_pkg_type type, void* payload, int lenght){
	return ipcprotocol2_send1(client, ipcpkg2_create1(descId, type, payload, lenght), IPC_PKG_AUTODESTROY_TRUE);
}

/*
 * @NAME: ipcprotocol2_recv
 * @DESC: Espera hasta la recepcion de un paquete proveniente de una conexion.
 */
t_ipc_pkg *ipcprotocol2_recv(t_socket_client *client){
	return sockets_recvSerialized(client, (void*)&ipcpkg2_deserializer);
}

/*
 * @NAME: ipcprotocol2_recvWithClosures
 * @DESC: Espera hasta la recepcion de un paquete proveniente de una conexion, dependiendo de si es request o response llama al correspondiente closure.
 * @NOTA: t_ipc_pkg recivido solo dura la ejecucion del closure ya que una vez ejecutado este es liberado.
 */
int ipcprotocol2_recvWithClosures(t_socket_client *client, void *context, void (*onRequest)(void *context, t_ipc_pkg*), void (*onResponse)(void *context, t_ipc_pkg*)){
	t_ipc_pkg *pkg = ipcprotocol2_recv(client);

	if( pkg == NULL ) return 0;

	if( pkg->payload_desc == IPC_REQUEST && &onRequest != NULL ){
		onRequest(context, pkg);
	}else if( pkg->payload_desc == IPC_RESPONSE && &onResponse != NULL ){
		onResponse(context, pkg);
	}
	ipcpkg2_destroy(pkg);

	return 1;
}

/*
 * @NAME: ipcprotocol2_select
 * @DESC: Implementacion de select para IPC.
 * @NOTA: Los t_ipc_pkg recividos en el onRequest y en el onResponse son borrados, tras la llamada.
 */
void ipcprotocol2_select(t_list* servers,
						t_list* clients,
						int usec_timeout, void *context,
						e_ipc_handshake_state (*onHandshake)(void *context, int process_id, t_socket_client*),
						int (*onAccept)(void *context, t_socket_client*),
						void (*onRequest)(void *context, t_socket_client*, t_ipc_pkg*),
						void (*onResponse)(void *context, t_socket_client*, t_ipc_pkg*),
						int (*onClose)(void *context, t_socket_client*)){

	t_socket_client* ipcprotocol2_closureInternalAccept(t_socket_server* server){
		t_socket_client *newcliente = ipcprotocol2_accepthandshake4(server, context, onHandshake);
		if( newcliente != NULL  && onAccept != NULL && onAccept(context, newcliente) == 0 ){
			sockets_destroyClient(newcliente);
		}
		return newcliente;
	}

	int ipcprotocol2_closureInternalRecv(t_socket_client* client){
		t_ipc_pkg *pkg = ipcprotocol2_recv(client);

		if( pkg == NULL && &onClose != NULL ){
			return !onClose(context, client);
		}

		if( pkg->payload_desc == IPC_REQUEST && &onRequest != NULL ){
			onRequest(context, client, pkg);
		}else if( pkg->payload_desc == IPC_RESPONSE && &onResponse != NULL ){
			onResponse(context, client, pkg);
		}

		ipcpkg2_destroy(pkg);
		return 1;
	}

	sockets_select(servers, clients, usec_timeout, &ipcprotocol2_closureInternalAccept, &ipcprotocol2_closureInternalRecv);
	usleep(0);
}
