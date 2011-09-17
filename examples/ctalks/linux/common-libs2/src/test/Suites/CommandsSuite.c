#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "CommandsSuite.h"

#include "../CUnit/Basic.h"

#include "../../lib/oop/Object.h"
#include "../../lib/Commands.h"
#include "../../lib/Array.h"


int CommandsSuite_Build(CU_pSuite suite){

	/* add a suite to the registry */
	suite = CU_add_suite("Suite_Commands", init_CommandsSuite, clean_CommandsSuite);
	if (NULL == suite) {
		CU_cleanup_registry();
		return 0;
	}

	/* add the tests to the suite */
	/* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
	if ( NULL == CU_add_test(suite, "Test of parser functionality", test_CommandsParser) )
	{
		CU_cleanup_registry();
		return 0;
	}

	return 1;
}

int init_CommandsSuite(){

	return 0;
}

void test_CommandsParser(){
	struct Commands *commands = new( Commands, "GlobalContext", '(', ')', ',', '\0', '\0');

	void add_handler(void *context, struct Array *args){
		CU_ASSERT( strcmp(context, "LocalContext1") == 0 );
		CU_ASSERT( strcmp(ArrayClass.get(args, 0), "AB") == 0 );
		CU_ASSERT( strcmp(ArrayClass.get(args, 1), "CD") == 0 );
	}
	CommandsClass.add2(commands, "add", add_handler, "LocalContext1");

	void del_handler(void *context, struct Array *args){
		CU_ASSERT( strcmp(context, "GlobalContext") == 0 );
		CU_ASSERT( strcmp(ArrayClass.get(args, 0), "1234") == 0 );
	}
	CommandsClass.add2(commands, "del", del_handler, NULL);

	CU_ASSERT( CommandsClass.parser(commands, "add(AB,CD)") == 1 );
	CU_ASSERT( CommandsClass.parser(commands, "del(1234)") == 1 );

	delete( commands );
}

int clean_CommandsSuite(){
	return 0;
}
