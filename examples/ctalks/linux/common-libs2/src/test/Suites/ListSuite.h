

#ifndef LIST_SUITE_H_
#define LIST_SUITE_H_

	#include "../CUnit/Basic.h"

	int ListSuite_Build(CU_pSuite suite);
	int init_ListSuite();
	void test_ListStress();
	void test_ListAdd();
	void test_ListGet();
	void test_ListSet();
	void test_ListRemove();
	void test_ListIterator();
	void test_ListFind();
	void test_ListClean();
	int clean_ListSuite();

#endif /* LIST_SUITE_H_ */
