#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "SocketsSuite.h"

#include "../CUnit/Basic.h"

#include "../../lib/collections/List.h"
#include "../../lib/socket/SocketClient.h"
#include "../../lib/socket/SocketServer.h"
#include "../../lib/oop/Object.h"

struct SocketClient *client1;
struct SocketClient *client2;
struct SocketServer *server;


int SocketsSuite_Build(CU_pSuite suite){
	suite = CU_add_suite("Suite_Sockets", init_SocketsSuite, clean_SocketsSuite);
	if (NULL == suite) {
		CU_cleanup_registry();
		return 0;
	}

	/* add the tests to the suite */
	/* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
	if ((NULL == CU_add_test(suite, "Test of Connect functionality", test_SocketsConnect)) ||
			(NULL == CU_add_test(suite, "Test of Send functionality", test_SocketsSend)) ||
			(NULL == CU_add_test(suite, "Test of Recive functionality", test_SocketsRecive)) ||
			(NULL == CU_add_test(suite, "Test of Select functionality", test_SocketsSelect)))
	{
		CU_cleanup_registry();
		return 0;
	}

	return 1;
}

int init_SocketsSuite(){

	client1 = new(SocketClient, "127.0.0.1", 6541);
	server = new(SocketServer, "127.0.0.1", 6540);

	return 0;
}

void test_SocketsConnect(){

	CU_ASSERT( SocketServerClass.listen(server) == 1 );

	CU_ASSERT( SocketClientClass.connect(client1, "127.0.0.1", 6540) == 1 );

	CU_ASSERT( (client2 = SocketServerClass.accept(server)) != NULL );

}

void test_SocketsSend(){

	CU_ASSERT( SocketClientClass.send2(client1, "Hola Mundo!!!") == 1 );

	CU_ASSERT( SocketClientClass.send2(client2, "Hi!!!!") == 1 );

}

void test_SocketsRecive(){

	{
		char buffer[200];
		CU_ASSERT( SocketClientClass.recive1(client1, buffer, 200) > 0 );
		CU_ASSERT( strcmp("Hi!!!!", buffer) == 0 );
	}

	{
		char buffer[200];
		CU_ASSERT( SocketClientClass.recive1(client2, buffer, 200) > 0 );
		CU_ASSERT( strcmp("Hola Mundo!!!", buffer) == 0 );
	}

}

void test_SocketsSelect(){
	struct List *list = new(List);
	struct SocketClient *client3 = new(SocketClient, "127.0.0.1", 6542);

	ListClass.add(list, client1);
	ListClass.add(list, client2);

	CU_ASSERT( SocketClientClass.send2(client1, "Hola Mundo!!!") == 1 );

	CU_ASSERT( SocketClientClass.send2(client2, "Hola Mundo!!!") == 1 );


	CU_ASSERT( SocketClientClass.connect(client3, "127.0.0.1", 6540) == 1 );

	struct SocketClient * test_onAccept(struct SocketServer *self){
		struct SocketClient *newClient = SocketServerClass.accept(self);
		CU_ASSERT( newClient != NULL );
		return newClient;
	}

	int test_onRecive(struct SocketClient *self){
		char buffer[200];
		CU_ASSERT( SocketClientClass.recive1(self, buffer, 200) > 0 );
		CU_ASSERT( strcmp("Hola Mundo!!!", buffer) == 0 );
		return 1;
	}

	SocketServerClass.select1(server, list, &test_onAccept, &test_onRecive);

	CU_ASSERT( list->size == 3 );

	delete( client3 );
}

int clean_SocketsSuite(){

	delete(client1);
	delete(client2);
	delete(server);

	return 0;
}

