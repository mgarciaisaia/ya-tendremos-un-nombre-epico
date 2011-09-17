/*
 * SSLSuite.h
 *
 *  Created on: 24/04/2010
 *      Author: facundo
 */

#ifndef SSLSUITE_H_
#define SSLSUITE_H_

	#include "../CUnit/Basic.h"

	int SSLSuite_Build(CU_pSuite suite);
	int init_SSLSuite();
	void test_SSLConnect();
	void test_SSLSend();
	void test_SSLRecive();
	void test_SSLSelect();
	int clean_SSLSuite();

#endif /* SSLSUITE_H_ */
