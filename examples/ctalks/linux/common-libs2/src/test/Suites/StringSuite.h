/*
 * @FILE: 		StringSuite.h
 * @AUTHOR: 	vialef
 * @CREATED_ON: 15/03/2010
 * @VERSION:	
 */

#ifndef STRINGSUITE_H_
#define STRINGSUITE_H_

	#include "../CUnit/Basic.h"

	int StringSuite_Build(CU_pSuite suite);
	int init_StringSuite();
	void test_StringClone();
	void test_StringAsInt();
	void test_StringAsDouble();
	void test_StringEquals();
	void test_StringSplit();
	void test_StringTrim();
	void test_StringStartWith();
	int clean_StringSuite();

#endif /* STRINGSUITE_H_ */
