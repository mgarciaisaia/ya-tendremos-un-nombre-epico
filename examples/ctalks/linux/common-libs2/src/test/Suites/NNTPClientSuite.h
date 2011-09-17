/*
 * NNTPClientSuite.h
 *
 *  Created on: 25/05/2010
 *      Author: facundo
 */

#ifndef NNTPCLIENTSUITE_H_
#define NNTPCLIENTSUITE_H_

	#include "../CUnit/Basic.h"

	int NNTPClientSuite_Build(CU_pSuite suite);
	int init_NNTPClientSuite();
	void test_NNTPClient();
	int clean_NNTPClientSuite();

#endif /* NNTPCLIENTSUITE_H_ */
