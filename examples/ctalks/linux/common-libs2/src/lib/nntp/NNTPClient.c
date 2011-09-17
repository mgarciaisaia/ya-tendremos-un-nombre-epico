#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include "../oop/Object.h"
#include "../ssl/SSLClient.h"
#include "../NString.h"

#include "NNTPClient.h"

static struct NNTPMessageReply* NNTPClient_sendCommand(struct NNTPClient *self, const char *command, int argc, ...);

struct NNTPClient*	NNTPClient_constructor( struct NNTPClient *self, va_list *args ){
	char *ip = va_arg(* args, char*);
	int port = va_arg(* args, int);

	self->connection = new(SSLClient, ip, port);

	return self;
}

int NNTPClient_connect(struct NNTPClient *self, char *ip, int port){
	return SSLClientClass.connect( self->connection, ip, port);
}

static struct NNTPMessageReply* NNTPClient_sendCommand(struct NNTPClient *self, const char *command, int argc, ...){
	char outbuffer[MAX_MESSAGE_LINE_LENGTH] = "";
	char inbuffer[DEFAULT_BUFFER_SIZE];
	struct NNTPMessageReply *reply = NULL;
	int recivebytes = 0;
	va_list argv;

	va_start(argv, argc);

	strcat (outbuffer, command);

	{
		int cont;
		for(cont=0; cont<argc; cont ++){
			strcat (outbuffer, " ");
			strcat (outbuffer, va_arg(argv, char*));
		}
	}

	va_end(argv);

	strcat (outbuffer, "\r\n");

	if( !SSLClientClass.send1(self->connection, outbuffer, strlen(outbuffer)) ) return NULL;

	memset(inbuffer, 0, DEFAULT_BUFFER_SIZE);

	if( (recivebytes = SSLClientClass.recive1(self->connection, inbuffer, DEFAULT_BUFFER_SIZE)) == 0) return NULL;

	reply = malloc( sizeof(struct NNTPMessageReply) );

	{
		char strcode[4];
		strcode[0] = inbuffer[0];
		strcode[1] = inbuffer[1];
		strcode[2] = inbuffer[2];
		strcode[3] = '\0';

		reply->code = NStringClass.asInt(strcode);
	}

	{
		int firstLineEnd = NStringClass.indexOf1(inbuffer, "\r\n");

		if( recivebytes - firstLineEnd + 2 > 0){
			reply->information = malloc( recivebytes - (firstLineEnd + 2 + 2) );
			memcpy(reply->information, inbuffer + firstLineEnd + 2, recivebytes - firstLineEnd + 2);
			reply->information[recivebytes - (firstLineEnd + 2 + 2 + 1)] = '\0';
		} else {
			reply->information = NULL;
		}
	}
	return reply;
}

char* NNTPClient_sendCommandArticle(struct NNTPClient *self, char *messageid, int *returnCode){
	struct NNTPMessageReply *reply = NNTPClient_sendCommand(self, NNTPCommands[NNTP_COMMAND_ARTICLE], 1, messageid);
	char *replayInfo = NULL;

	if( reply != NULL ){
		*returnCode = reply->code;

		if( reply->code == NNTP_REPLY_ARTICLE_RETRIEVED_HEAD_AND_BODY_FOLLOW)
			replayInfo = reply->information;

		free( reply );
	}
	return replayInfo;
}

char* NNTPClient_sendCommandHead(struct NNTPClient *self, char *messageid, int *returnCode){
	struct NNTPMessageReply *reply = NNTPClient_sendCommand(self, NNTPCommands[NNTP_COMMAND_HEAD], 1, messageid);
	char *replayInfo = NULL;

	if( reply != NULL ){
		*returnCode = reply->code;

		if( reply->code == NNTP_REPLY_ARTICLE_RETRIEVED_HEAD_FOLLOWS)
			replayInfo = reply->information;

		free( reply );
	}
	return replayInfo;
}

char* NNTPClient_sendCommandBody(struct NNTPClient *self, char *messageid, int *returnCode){
	struct NNTPMessageReply *reply = NNTPClient_sendCommand(self, NNTPCommands[NNTP_COMMAND_BODY], 1, messageid);
	char *replayInfo = NULL;

	if( reply != NULL ){
		*returnCode = reply->code;

		if( reply->code == NNTP_REPLY_ARTICLE_RETRIEVED_BODY_FOLLOWS)
			replayInfo = reply->information;

		free( reply );
	}
	return replayInfo;
}

int	NNTPClient_sendCommandStat(struct NNTPClient *self, char *messageid, int *returnCode){
	struct NNTPMessageReply *reply = NNTPClient_sendCommand(self, NNTPCommands[NNTP_COMMAND_STAT], 1, messageid);

	if( reply != NULL ){

		*returnCode = reply->code;

		free( reply );

		return *returnCode;
	}
	return 0;
}

char **	NNTPClient_sendCommandListGroup(struct NNTPClient *self, char *group, int *returnCode){
	struct NNTPMessageReply *reply = NNTPClient_sendCommand(self, NNTPCommands[NNTP_COMMAND_LISTNEWSGROUPS], 1, group);
	char **replayInfo = NULL;

	if( reply != NULL ){
		*returnCode = reply->code;

		if( reply->code == NNTP_REPLY_GROUP_SELECTED){
			int pibot=0;
			replayInfo = NStringClass.split( reply->information, '\n');

			while( replayInfo[pibot] != NULL ){
				if( NStringClass.startWith(replayInfo[pibot], ".") ) {
					replayInfo[pibot] = NULL;
					break;
				} else {
					replayInfo[pibot][ strlen( replayInfo[pibot] ) - 1] = '\0';
				}
				pibot++;
			}

			free( reply->information );
		}
		free( reply );
	}
	return replayInfo;
}

char **	NNTPClient_sendCommandListNewsGroup(struct NNTPClient *self, int *returnCode){
	struct NNTPMessageReply *reply = NNTPClient_sendCommand(self, NNTPCommands[NNTP_COMMAND_LISTNEWSGROUPS], 0);
	char **replayInfo = NULL;

	if( reply != NULL ){
		*returnCode = reply->code;

		if( reply->code == NNTP_REPLY_LIST_SELECTED ){
			int pibot=0;
			replayInfo = NStringClass.split( reply->information, '\n');

			while( replayInfo[pibot] != NULL ){
				if( NStringClass.startWith(replayInfo[pibot], ".") ) {
					replayInfo[pibot] = NULL;
					break;
				} else {
					replayInfo[pibot][ strlen( replayInfo[pibot] ) - 1] = '\0';
				}
				pibot++;
			}

			free( reply->information );
		}
		free( reply );
	}
	return replayInfo;
}

int	NNTPClient_sendCommandQuit(struct NNTPClient *self, int *returnCode){
	struct NNTPMessageReply *reply = NNTPClient_sendCommand(self, NNTPCommands[NNTP_COMMAND_QUIT], 0);
	if( reply != NULL ){
		*returnCode = reply->code;
		free( reply );
		return *returnCode;
	}
	return 0;
}

void* NNTPClient_destroyer( struct NNTPClient *self){
	delete( self->connection );
	return self;
}

