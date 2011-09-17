/*
 * SSL.h
 *
 *  Created on: 24/04/2010
 *      Author: facundo
 */

#ifndef SSL_H_
#define SSL_H_

	#define _XOPEN_SOURCE 500

	#include <openssl/ssl.h>
	#include <sys/select.h>
	#include <netinet/in.h>

	#include "../oop/Class.h"

	#include "../socket/Socket.h"
	#include "../socket/SocketClient.h"
	#include "../socket/SocketServer.h"
	#include "../socket/SocketAddress.h"

	typedef enum{
		SSL_CLIENT = 1,
		SSL_SERVER = 2
	}SSLContextMode;

	typedef enum{
		AUTHENTICATION_NONE = 1,
		AUTHENTICATION_REQUIRE = 2
	}SSLClientAuthentication;


	struct SSLSocket{
	#define SSLSocket_DEF 				\
		const void * clazz;				\
		SSL_CTX *context;				\
		char *keyfile;					\
		char *certfile;
		SSLSocket_DEF
	};

	struct SSLClient {	extends(SSLSocket)
		struct SocketClient *socket;
		SSL *connection;
	};

	struct SSLServer {	extends(SSLSocket)
		struct SocketServer *socket;
	};

	void SSL_initLibs();

	SSL_CTX *SSL_initializeContext(SSLContextMode mode, SSLClientAuthentication type, char *certfile,  char *keyfile);

#endif /* SSL_H_ */
