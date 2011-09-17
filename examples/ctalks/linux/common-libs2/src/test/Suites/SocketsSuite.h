/*
 * SocketsSuite.h
 *
 *  Created on: 24/04/2010
 *      Author: facundo
 */

#ifndef SOCKETSSUITE_H_
#define SOCKETSSUITE_H_

	#include "../CUnit/Basic.h"

	int SocketsSuite_Build(CU_pSuite suite);
	int init_SocketsSuite();
	void test_SocketsConnect();
	void test_SocketsSend();
	void test_SocketsRecive();
	void test_SocketsSelect();
	int clean_SocketsSuite();

#endif /* SOCKETSSUITE_H_ */
