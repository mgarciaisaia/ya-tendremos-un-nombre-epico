/*
 * @FILE: 		main_Test.c
 * @AUTHOR: 	vialef
 * @CREATED_ON: 22/02/2010
 * @VERSION:	
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "CUnit/Basic.h"

#include "Suites/StringSuite.h"

#include "Suites/FileSuite.h"
#include "Suites/LogSuite.h"

#include "Suites/ListSuite.h"
#include "Suites/QueueSuite.h"
#include "Suites/HashMapSuite.h"

#include "Suites/CommandsSuite.h"

#include "Suites/ConfigurationSuite.h"

#include "Suites/SocketsSuite.h"
#include "Suites/SSLSuite.h"

#include "Suites/NNTPClientSuite.h"


int main(void){
	CU_pSuite stringSuite = NULL;
	CU_pSuite fileSuite = NULL;
	CU_pSuite arraySuite = NULL;
	CU_pSuite listSuite = NULL;
	CU_pSuite queueSuite = NULL;
	CU_pSuite hashmapSuite = NULL;
	CU_pSuite configSuite = NULL;
	CU_pSuite commandsSuite = NULL;
	CU_pSuite logSuite = NULL;
	CU_pSuite socketsSuite = NULL;
	CU_pSuite SSLSuite = NULL;
	CU_pSuite nntpSuite = NULL;


	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();


/*	if( ConfigurationSuite_Build( configSuite ) == 0 ) 	return CU_get_error();*/

/*	if( LogSuite_Build( logSuite ) == 0 ) 	return CU_get_error();	*/

/*	if( SocketsSuite_Build( socketsSuite ) == 0 ) 	return CU_get_error();	*/

/*	if( SSLSuite_Build( socketsSuite ) == 0 ) 	return CU_get_error(); */

	if( NNTPClientSuite_Build( commandsSuite ) == 0 ) 	return CU_get_error();

/*	if( CommandsSuite_Build( commandsSuite ) == 0 ) 	return CU_get_error(); */

/*	if( StringSuite_Build( stringSuite ) == 0 ) 	return CU_get_error();
	if( FileSuite_Build( fileSuite ) == 0 ) 	return CU_get_error();

	if( ArraySuite_Build( arraySuite ) == 0 ) 		return CU_get_error();

	if( ListSuite_Build( listSuite ) == 0 ) 		return CU_get_error();
	if( QueueSuite_Build( queueSuite ) == 0 ) 		return CU_get_error();
	if( HashMapSuite_Build( hashmapSuite ) == 0 ) 	return CU_get_error();
*/
	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

	return CU_get_error();
}
