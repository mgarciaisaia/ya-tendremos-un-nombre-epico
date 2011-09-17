#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "NNTPClientSuite.h"

#include "../CUnit/Basic.h"

#include "../../lib/nntp/NNTPClient.h"
#include "../../lib/nntp/NNTPServer.h"
#include "../../lib/oop/Object.h"

static struct NNTPClient *client;
static struct NNTPClient *server;

int NNTPClientSuite_Build(CU_pSuite suite){
	suite = CU_add_suite("Suite_NNTPClient", init_NNTPClientSuite, clean_NNTPClientSuite);
	if (NULL == suite) {
		CU_cleanup_registry();
		return 0;
	}

	/* add the tests to the suite */
	/* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
	if ( NULL == CU_add_test(suite, "Test ALL functionality", test_NNTPClient ) )
	{
		CU_cleanup_registry();
		return 0;
	}

	return 1;
}

int init_NNTPClientSuite(){

	SSL_initLibs();

	client = new(NNTPClient, "192.168.1.101", 5001);

	server = new(NNTPServer, "192.168.1.101", 5000, "data/MyCert.pem", "data/MyKey.pem");

	char *headHandler(void *context, char *values[]){
		CU_ASSERT( strcmp(context, "1") == 0 );
		return "221\r\nHola!!!!\r\n";
	}
	NNTPServerClass.addCommandHandler(server, NNTP_COMMAND_HEAD, "1", headHandler);


	char *bodyHandler(void *context, char *values[]){
		CU_ASSERT( context == NULL );
		return "222\r\nMundo\r\n";
	}
	NNTPServerClass.addCommandHandler(server, NNTP_COMMAND_BODY, NULL, bodyHandler);



	return 0;
}

void test_NNTPClient(){
	CU_ASSERT( NNTPServerClass.listen(server) == 1 );

	{

		NNTPServerClass.waitForCommand(server);
	}

}

int clean_NNTPClientSuite(){
	delete(client);
	return 0;
}
