

#ifndef ARRAY_SUITE_H_
#define ARRAY_SUITE_H_

	#include "../CUnit/Basic.h"

	int ArraySuite_Build(CU_pSuite suite);
	int init_ArraySuite();
	void test_ArraySetAndGet();
	void test_ArrayFind();
	void test_ArrayIterator();
	void test_ArrayClean();
	int clean_ArraySuite();

#endif /* ARRAY_SUITE_H_ */
