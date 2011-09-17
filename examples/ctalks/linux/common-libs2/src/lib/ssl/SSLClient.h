/*
 * SSLClient.h
 *
 *  Created on: 01/05/2010
 *      Author: facundo
 */

#ifndef SSLCLIENT_H_
#define SSLCLIENT_H_

	#define _XOPEN_SOURCE 500

	#include "../oop/Class.h"
	#include "../oop/Object.h"

	#include "SSL.h"


	struct SSLClientClass{
		int (*connect)(struct SSLClient *, char *ip, int port);
		int (*send1)(struct SSLClient *, char *data, int datalength);
		int (*send2)(struct SSLClient *, char *string);
		int (*send3)(struct SSLClient *, void *data, struct SerializedObject *(*serializer)(void *data));
		int (*recive1)(struct SSLClient *, char *buffer, int buffersize);
		void *(*recive2)(struct SSLClient *, void *(*deserializer)(void *buffer, int buffersize));
	};

	struct SSLClient* 	 SSLClient_constructor( struct SSLClient*, va_list *args );
	int			  		 SSLClient_connect(struct SSLClient *, char *ip, int port);
	int  				 SSLClient_send1(struct SSLClient *, char *data, int datalength);
	int  				 SSLClient_send2(struct SSLClient *, char *string);
	int 				 SSLClient_send3(struct SSLClient *, void *data, struct SerializedObject *(*serializer)(void *data));
	int  				 SSLClient_recive1(struct SSLClient *, char *buffer, int buffersize);
	void  				*SSLClient_recive2(struct SSLClient *, void *(*deserializer)(void *buffer, int buffersize));
	void*		 		 SSLClient_destroyer( struct SSLClient* );

	static const struct Class _SSLClient = {
		"SSLClient",
		sizeof(struct SSLClient),
		(void*(*)(void*,va_list*))SSLClient_constructor, Class_equals, (void*(*)(void*))SSLClient_destroyer
	};

	static const void * SSLClient = &_SSLClient;

	static const struct SSLClientClass SSLClientClass = {
			SSLClient_connect,
			SSLClient_send1,
			SSLClient_send2,
			SSLClient_send3,
			SSLClient_recive1,
			SSLClient_recive2
	};

#endif /* SSLCLIENT_H_ */
