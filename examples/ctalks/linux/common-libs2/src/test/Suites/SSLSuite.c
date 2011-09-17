#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>

#include "SSLSuite.h"

#include "../CUnit/Basic.h"

#include "../../lib/collections/List.h"
#include "../../lib/ssl/SSLClient.h"
#include "../../lib/ssl/SSLServer.h"
#include "../../lib/oop/Object.h"

struct SSLClient *client1;
struct SSLClient *client2;
struct SSLServer *server;


int SSLSuite_Build(CU_pSuite suite){
	suite = CU_add_suite("Suite_SSL", init_SSLSuite, clean_SSLSuite);
	if (NULL == suite) {
		CU_cleanup_registry();
		return 0;
	}

	/* add the tests to the suite */
	/* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
	if ((NULL == CU_add_test(suite, "Test of Connect functionality", test_SSLConnect)) ||
			(NULL == CU_add_test(suite, "Test of Send functionality", test_SSLSend)) ||
			(NULL == CU_add_test(suite, "Test of Recive functionality", test_SSLRecive)))
	{
		CU_cleanup_registry();
		return 0;
	}

	return 1;
}

int init_SSLSuite(){

	SSL_initLibs();

	client1 = new(SSLClient, "127.0.0.1", 6541);
	server = new(SSLServer, "127.0.0.1", 6540, "data/MyCert.pem", "data/MyKey.pem");

	return 0;
}

void test_SSLConnect(){
	pthread_t sthread;

	CU_ASSERT( SSLServerClass.listen(server) == 1 );

	void inner_waitingConnection(struct SSLServer *server){
		CU_ASSERT( (client2 = SSLServerClass.accept(server)) != NULL );
	}

	pthread_create( &sthread, NULL, (void*)inner_waitingConnection, (void*)server);

	CU_ASSERT( SSLClientClass.connect(client1, "127.0.0.1", 6540) == 1 );


}

void test_SSLSend(){

	CU_ASSERT( SSLClientClass.send2(client1, "Hola Mundo!!!") == 1 );

	CU_ASSERT( SSLClientClass.send2(client2, "Hi!!!!") == 1 );

}

void test_SSLRecive(){

	{
		char buffer[200];
		CU_ASSERT( SSLClientClass.recive1(client1, buffer, 200) > 0 );
		CU_ASSERT( strcmp("Hi!!!!", buffer) == 0 );
	}

	{
		char buffer[200];
		CU_ASSERT( SSLClientClass.recive1(client2, buffer, 200) > 0 );
		CU_ASSERT( strcmp("Hola Mundo!!!", buffer) == 0 );
	}

}

int clean_SSLSuite(){

	delete(client1);
	delete(client2);
	delete(server);

	return 0;
}

