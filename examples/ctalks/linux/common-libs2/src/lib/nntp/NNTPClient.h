/*
 * NNTPClient.h
 *
 *  Created on: 01/05/2010
 *      Author: facundo
 */

#ifndef NNTPCLIENT_H_
#define NNTPCLIENT_H_

	#define _XOPEN_SOURCE 500

	#include "NNTPProtocol.h"

	struct NNTPClientClass{
		int (*connect)(struct NNTPClient *, char *ip, int port);
		char * (*sendCommandArticle)(struct NNTPClient *, char *messageid, int *returnCode);
		char * (*sendCommandHead)(struct NNTPClient *, char *messageid, int *returnCode);
		char * (*sendCommandBody)(struct NNTPClient *, char *messageid, int *returnCode);
		int (*sendCommandStat)(struct NNTPClient *, char *messageid, int *returnCode);
		char ** (*sendCommandListGroup)(struct NNTPClient *, char *group, int *returnCode);
		char ** (*sendCommandListNewsGroup)(struct NNTPClient *, int *returnCode);
		int (*sendCommandQuit)(struct NNTPClient *, int *returnCode);
	};

	struct NNTPClient*	NNTPClient_constructor( struct NNTPClient*, va_list *args );
	int 				NNTPClient_connect(struct NNTPClient *, char *ip, int port);
	char * 				NNTPClient_sendCommandArticle(struct NNTPClient *, char *messageid, int *returnCode);
	char * 				NNTPClient_sendCommandHead(struct NNTPClient *, char *messageid, int *returnCode);
	char * 				NNTPClient_sendCommandBody(struct NNTPClient *, char *messageid, int *returnCode);
	int 				NNTPClient_sendCommandStat(struct NNTPClient *, char *messageid, int *returnCode);
	char ** 			NNTPClient_sendCommandListGroup(struct NNTPClient *, char *group, int *returnCode);
	char ** 			NNTPClient_sendCommandListNewsGroup(struct NNTPClient *, int *returnCode);
	int 				NNTPClient_sendCommandQuit(struct NNTPClient *, int *returnCode);
	void*		 		NNTPClient_destroyer( struct NNTPClient* );

	static const struct Class _NNTPClient = {
		"NNTPClient",
		sizeof(struct NNTPClient),
		(void*(*)(void*,va_list*))NNTPClient_constructor, Class_equals, (void*(*)(void*))NNTPClient_destroyer
	};

	static const void * NNTPClient = &_NNTPClient;

	static const struct NNTPClientClass NNTPClientClass = {
			NNTPClient_connect,
			NNTPClient_sendCommandArticle,
			NNTPClient_sendCommandHead,
			NNTPClient_sendCommandBody,
			NNTPClient_sendCommandStat,
			NNTPClient_sendCommandListGroup,
			NNTPClient_sendCommandListNewsGroup,
			NNTPClient_sendCommandQuit
	};

#endif /* NNTPCLIENT_H_ */
