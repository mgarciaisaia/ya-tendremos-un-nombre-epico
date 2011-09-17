#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "LogSuite.h"

#include "../CUnit/Basic.h"

#include "../../lib/oop/Object.h"

#include "../../lib/Log.h"

static struct Log *logfile;

int LogSuite_Build(CU_pSuite suite){

	/* add a suite to the registry */
	suite = CU_add_suite("Suite_Log", init_LogSuite, clean_LogSuite);
	if (NULL == suite) {
		CU_cleanup_registry();
		return 0;
	}

	/* add the tests to the suite */
	/* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
	if( NULL == CU_add_test(suite, "Test of write functionality", test_LogWrite) )
	{
		CU_cleanup_registry();
		return 0;
	}

	return 1;
}

int init_LogSuite(){
	logfile = new(Log, "./logs/test.log", "TestLog", LOG_OUTPUT_FILE);
	return 0;
}

void test_LogWrite(){
	LogClass.info(logfile, "Main", "- %s", "Hola Mundo!");
	CU_ASSERT( 1 );
	LogClass.warning(logfile, "Main", "Hola!!");
	CU_ASSERT( 1 );
	LogClass.error(logfile, "Main", "wtf????");
	CU_ASSERT( 1 );
}

int clean_LogSuite(){
	delete(logfile);
	return 0;
}


