

#ifndef HASHMAP_SUITE_H_
#define HASHMAP_SUITE_H_

	#include "../CUnit/Basic.h"

	int HashMapSuite_Build(CU_pSuite suite);
	int init_HashMapSuite();
	void test_HashMapStress();
	void test_HashMapPut();
	void test_HashMapGet();
	void test_HashMapIterator();
	void test_HashMapRemove();
	void test_HashMapClean();
	int clean_HashMapSuite();

#endif /* HASHMAP_SUITE_H_ */
