/*
 * SSLServer.h
 *
 *  Created on: 01/05/2010
 *      Author: facundo
 */

#ifndef SSLSERVER_H_
#define SSLSERVER_H_

	#define _XOPEN_SOURCE 500

	#include "../oop/Class.h"
	#include "../oop/Object.h"

	#include "SSLClient.h"


	struct SSLServerClass{
		int (*listen)(struct SSLServer *);
		struct SSLClient * (*accept)(struct SSLServer *);
	};

	struct SSLServer* 	 SSLServer_constructor( struct SSLServer*, va_list *args );
	int			  		 SSLServer_listen(struct SSLServer *);
	struct SSLClient *   SSLServer_accept(struct SSLServer *);
	void*		 		 SSLServer_destroyer( struct SSLServer* );

	static const struct Class _SSLServer = {
		"SSLServer",
		sizeof(struct SSLServer),
		(void*(*)(void*,va_list*))SSLServer_constructor, Class_equals, (void*(*)(void*))SSLServer_destroyer
	};

	static const void * SSLServer = &_SSLServer;

	static const struct SSLServerClass SSLServerClass = {
			SSLServer_listen,
			SSLServer_accept
	};

#endif /* SSLSERVER_H_ */
