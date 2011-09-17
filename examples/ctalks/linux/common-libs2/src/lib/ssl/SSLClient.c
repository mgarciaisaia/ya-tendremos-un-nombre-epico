/*
 * SSLClient.h
 *
 *  Created on: 01/05/2010
 *      Author: facundo
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "SSLClient.h"
#include "../socket/SocketClient.h"

int SSLClient_checkCertifications(struct SSLClient *self, char *host);

struct SSLClient* SSLClient_constructor( struct SSLClient *self, va_list *args ){
	char *ip = va_arg(* args, char*);
	int port = va_arg(* args, int);

	self->socket = new(SocketClient, ip, port);

	assert( (self->context = SSL_initializeContext(SSL_CLIENT, AUTHENTICATION_NONE, self->certfile, self->keyfile)) != NULL );

	return self;
}

int	SSLClient_connect(struct SSLClient *self, char *ip, int port){

	if( SocketClientClass.connect(self->socket, ip, port) == 0 ) return 0;

	/* SSL Connection */
	self->connection = SSL_new( SSL_CTX_new(  SSLv23_client_method() ) );
	SSL_set_fd(self->connection, self->socket->descriptor);

    if( SSL_connect(self->connection) <= 0 ){
    	SSL_free(self->connection);
    	return 0;
    }

	return 1;
}

int SSLClient_send1(struct SSLClient *self, char *data, int datalength){
	int ret = SSL_write(self->connection, data, datalength);

	switch( SSL_get_error(self->connection, ret) ){
		case SSL_ERROR_NONE:
			if( datalength != ret)	return 0;
			break;
		default:
			return 0;
	}

	return 1;
}

int	SSLClient_send2(struct SSLClient *self, char *string){
	return SSLClient_send1(self, string, strlen(string) + 1 );
}

int	SSLClient_send3(struct SSLClient *self, void *data, struct SerializedObject *(*serializer)(void *data)){
	struct SerializedObject *stream = serializer(data);

	int ret = SSLClient_send1(self, stream->bytes, stream->size);

	free(stream->bytes);
	free(stream);

	return ret;
}

int	SSLClient_recive1(struct SSLClient *self, char *buffer, int buffersize){
	int ret = SSL_read(self->connection, buffer, buffersize);

	switch( SSL_get_error(self->connection, ret) ){
		case SSL_ERROR_NONE:
			return ret;
		case SSL_ERROR_ZERO_RETURN:
			return -1;
		case SSL_ERROR_SYSCALL:
			return -1;
		default:
			return -1;
	}
	return -1;
}

void *SSLClient_recive2(struct SSLClient *self, void *(*deserializer)(void *buffer, int buffersize)){
	char buffer[DEFAULT_RECIVE_BUFFER_SIZE];
	int rcv_bytes = SocketClient_recive1(self->socket, buffer, DEFAULT_RECIVE_BUFFER_SIZE);

	if( rcv_bytes > 0 ){
		return deserializer(buffer, rcv_bytes);
	}
	return NULL;
}

void* SSLClient_destroyer( struct SSLClient *self ){
	SSL_free( self->connection );
	SSL_CTX_free( self->context );
	delete( self->socket );
	return self;
}

