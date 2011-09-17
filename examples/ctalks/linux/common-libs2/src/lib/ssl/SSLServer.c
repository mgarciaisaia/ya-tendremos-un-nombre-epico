#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <assert.h>

#include "../oop/Object.h"

#include "SSLServer.h"

struct SSLServer* SSLServer_constructor( struct SSLServer *self, va_list *args ){
	char *ip = va_arg(* args, char*);
	int port = va_arg(* args, int);

	self->socket = new(SocketServer, ip, port);

	assert( (self->certfile = va_arg(* args, char*)) != NULL );

	assert( (self->keyfile = va_arg(* args, char*)) != NULL );

	assert( (self->context = SSL_initializeContext(SSL_SERVER, AUTHENTICATION_REQUIRE, self->certfile, self->keyfile)) != NULL );

	return self;
}

int SSLServer_listen(struct SSLServer *self){
	return SocketServerClass.listen( self->socket );
}

struct SSLClient *SSLServer_accept(struct SSLServer *self){
	struct SocketClient *newClient = SocketServerClass.accept(self->socket);
	struct SSLClient *newSSLClient = NULL;

	if( newClient == NULL ) return NULL;

	newSSLClient = malloc( sizeof(struct SSLClient) );

	newSSLClient->socket = newClient;

	newSSLClient->context = SSL_CTX_new(  SSLv23_client_method() );

	newSSLClient->connection = SSL_new( self->context );

	SSL_set_fd(newSSLClient->connection, newSSLClient->socket->descriptor);

	if( SSL_accept(newSSLClient->connection) <=0 ){
		SSL_CTX_free( newSSLClient->context );
		delete( newClient );
		free(newSSLClient);
	}

	return newSSLClient;
}

void* SSLServer_destroyer( struct SSLServer *self ){
	SSL_CTX_free( self->context );
	delete( self->socket );
	return self;
}
