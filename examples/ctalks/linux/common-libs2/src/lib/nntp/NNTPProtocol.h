/*
 * NNTPProtocol.h
 *
 *  Created on: 01/05/2010
 *      Author: facundo
 */

#ifndef NNTPPROTOCOL_H_
#define NNTPPROTOCOL_H_

	#define _XOPEN_SOURCE 500

	#include "../oop/Class.h"
	#include "../oop/Object.h"
	#include "../ssl/SSLClient.h"
	#include "../ssl/SSLServer.h"
	#include "../collections/HashMap.h"

	#define MAX_MESSAGE_LINE_LENGTH	512


	typedef enum{
    	NNTP_COMMAND_ARTICLE			= 0,
    	NNTP_COMMAND_HEAD 				= 1,
    	NNTP_COMMAND_BODY 				= 2,
    	NNTP_COMMAND_STAT 				= 3,
    	NNTP_COMMAND_LISTGROUP 			= 4,
    	NNTP_COMMAND_LISTNEWSGROUPS  	= 5,
    	NNTP_COMMAND_QUIT 				= 6
	}NNTPCommand;

	static const char *NNTPCommands[] = {"ARTICLE", "HEAD", "BODY", "STAT", "LISTGROUP", "LIST NEWSGROUPS", "QUIT"};

	typedef enum{
		NNTP_REPLY_CLOSING_CONNECTION							= 205,
		NNTP_REPLY_GROUP_SELECTED								= 211,
		NNTP_REPLY_LIST_SELECTED								= 215,
		NNTP_REPLY_ARTICLE_RETRIEVED_HEAD_AND_BODY_FOLLOW		= 220,
		NNTP_REPLY_ARTICLE_RETRIEVED_HEAD_FOLLOWS				= 221,
		NNTP_REPLY_ARTICLE_RETRIEVED_BODY_FOLLOWS				= 222,
		NNTP_REPLY_ARTICLE_RETRIEVED_REQUEST_TEXT_SEPARATELY	= 223,
		NNTP_REPLY_NO_SUCH_NEWSGROUP							= 411,
		NNTP_REPLY_NO_NEWS_GROUP_SELECTED						= 412,
		NNTP_REPLY_NO_SUCH_ARTICLE_FOUND						= 430
	}NNTPReply;

	struct NNTPClient{
		const void * clazz;
		struct SSLClient * connection;
	};

	struct NNTPServer{
		const void * clazz;
		struct SSLServer * server;
		struct SSLClient * connection;
		struct HashMap * handlers;
	};

	struct NNTPMessageReply{
		int code;
		char *information;
	};

	struct NNTPCommandHandler{
		const char *command;
		void * context;
		char *(*handler)(void *context, char **values);
	};

#endif /* NNTPPROTOCOL_H_ */
