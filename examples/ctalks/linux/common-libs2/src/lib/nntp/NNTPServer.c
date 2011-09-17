#include <stdlib.h>
#include <string.h>

#include "../oop/Object.h"
#include "../NString.h"
#include "NNTPServer.h"

struct NNTPServer*	NNTPServer_constructor( struct NNTPServer *self, va_list *args ){
	char *ip = va_arg(* args, char*);
	int port = va_arg(* args, int);
	char *certfile = va_arg(* args, char*);
	char *keyfile = va_arg(* args, char*);

	self->server = new(SSLServer, ip, port, certfile, keyfile);
	self->handlers = new(HashMap,  POINTER_TYPE, free);

	return self;
}

int	NNTPServer_addCommandHandler(struct NNTPServer *self, NNTPCommand command, void * context, char *(*handler)(void *context, char *values[])){
	struct NNTPCommandHandler *cmdhandler = malloc( sizeof(struct NNTPCommandHandler) );

	cmdhandler->command = NNTPCommands[command];
	cmdhandler->context = context;
	cmdhandler->handler = handler;

	HashMapClass.put(self->handlers, NNTPCommands[command], cmdhandler);

	return 1;
}

int NNTPServer_listen(struct NNTPServer *self){
	if( SSLServerClass.listen( self->server ) ){
		self->connection  = SSLServerClass.accept( self->server );
		return self->connection != NULL;
	}
	return 1;
}

int NNTPServer_waitForCommand(struct NNTPServer *self){
	char buffer[MAX_MESSAGE_LINE_LENGTH];

	if( SSLClientClass.recive1(self->connection, buffer, MAX_MESSAGE_LINE_LENGTH) > 0){
		struct NNTPCommandHandler *cmdHandler;
		char cmd[100];
		int cont;

		for( cont=0; cont<MAX_MESSAGE_LINE_LENGTH && buffer[cont] != '\0' && buffer[cont] != ' ' && buffer[cont] != '\t'; cont++){
			cmd[cont] = buffer[cont];
		}
		cmd[cont] = '\0';

		cmdHandler = HashMapClass.get(self->handlers, cmd);

		buffer[ NStringClass.indexOf1(buffer, "\r") ] = '\0';

		if( cmdHandler != NULL ){
			char **args = NStringClass.split(buffer + cont + 1,' ');
			char *reply = cmdHandler->handler(cmdHandler->context, args);
			if( reply != NULL ){
				SSLClientClass.send1(self->connection, reply, strlen(reply) +1 );
				free(reply);
			}
			{
				int index;
				for(index = 0; args[index] != NULL; index++){
					free( args[index] );
				}
				free(args);
			}
		}
	}
	return 0;
}

void* NNTPServer_destroyer( struct NNTPServer *self ){
	delete( self->handlers );
	delete( self->connection );
	return self;
}

