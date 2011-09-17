#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ConfigurationSuite.h"

#include "../CUnit/Basic.h"

#include "../../lib/oop/Object.h"
#include "../../lib/Configuration.h"

static struct Configuration *configuration;

int ConfigurationSuite_Build(CU_pSuite suite){

	/* add a suite to the registry */
	suite = CU_add_suite("Suite_Configuration", init_ConfigurationSuite, clean_ConfigurationSuite);
	if (NULL == suite) {
		CU_cleanup_registry();
		return 0;
	}

	/* add the tests to the suite */
	/* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
	if ( NULL == CU_add_test(suite, "Test of get functionality", test_ConfigurationGet) )
	{
		CU_cleanup_registry();
		return 0;
	}

	return 1;
}

int init_ConfigurationSuite(){
	configuration = new(Configuration, "./data/test_config.cfg", "=");
	return 0;
}

void test_ConfigurationGet(){

	CU_ASSERT( ConfigurationClass.getInt(configuration, "PORT") == 5000 );
	CU_ASSERT( strcmp(ConfigurationClass.getString(configuration, "TEXT"), "HOLA") == 0 );
	CU_ASSERT( strcmp(ConfigurationClass.getString(configuration, "IP"), "127.0.0.1") == 0 );

}

int clean_ConfigurationSuite(){
	delete( configuration );
	return 0;
}
