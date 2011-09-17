/*
 * NNTPClient.h
 *
 *  Created on: 01/05/2010
 *      Author: facundo
 */

#ifndef NNTPSERVER_H_
#define NNTPSERVER_H_

	#define _XOPEN_SOURCE 500

	#include "NNTPProtocol.h"

	struct NNTPServerClass{
		int (*addCommandHandler)(struct NNTPServer *, NNTPCommand command, void * context, char *(*handler)(void *context, char *values[]));
		int (*listen)(struct NNTPServer *);
		int (*waitForCommand)(struct NNTPServer *);
	};

	struct NNTPServer*	NNTPServer_constructor( struct NNTPServer*, va_list *args );
	int 				NNTPServer_addCommandHandler(struct NNTPServer *, NNTPCommand command, void * context, char *(*handler)(void *context, char *values[]));
	int 				NNTPServer_listen(struct NNTPServer *);
	int 				NNTPServer_waitForCommand(struct NNTPServer *);
	void*		 		NNTPServer_destroyer( struct NNTPServer* );

	static const struct Class _NNTPServer = {
		"NNTPServer",
		sizeof(struct NNTPServer),
		(void*(*)(void*,va_list*))NNTPServer_constructor, Class_equals, (void*(*)(void*))NNTPServer_destroyer
	};

	static const void * NNTPServer = &_NNTPServer;

	static const struct NNTPServerClass NNTPServerClass = {
			NNTPServer_addCommandHandler,
			NNTPServer_listen,
			NNTPServer_waitForCommand
	};

#endif /* NNTPSERVER_H_ */
