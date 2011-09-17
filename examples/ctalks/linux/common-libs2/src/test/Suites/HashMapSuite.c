#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "HashMapSuite.h"

#include "../CUnit/Basic.h"

#include "../../lib/collections/HashMap.h"
#include "../../lib/oop/Object.h"

static struct HashMap *hashmap;

static char *dic[] = {"AAA", "BBB", "CCC", "DDD", "EEE", "FFF", "GGG", "HHH"};
static char *keys[] = {"A", "B", "C", "D", "E", "F", "G", "H"};


int HashMapSuite_Build(CU_pSuite suite){

	/* add a suite to the registry */
	suite = CU_add_suite("Suite_HashMap", init_HashMapSuite, clean_HashMapSuite);
	if (NULL == suite) {
		CU_cleanup_registry();
		return 0;
	}

	/* add the tests to the suite */
	/* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
	if ((NULL == CU_add_test(suite, "Test of put functionality", test_HashMapPut)) ||
			(NULL == CU_add_test(suite, "Test of get functionality", test_HashMapGet)) ||
			(NULL == CU_add_test(suite, "Test of Iterator functionality", test_HashMapIterator)) ||
			(NULL == CU_add_test(suite, "Test of Remove functionality", test_HashMapRemove)) ||
			(NULL == CU_add_test(suite, "Test of clean functionality", test_HashMapClean)) ||
			(NULL == CU_add_test(suite, "Test of Stress", test_HashMapStress)))
	{
		CU_cleanup_registry();
		return 0;
	}

	return 1;
}

int init_HashMapSuite(){
	srand( time(NULL) );
	hashmap = new(HashMap, POINTER_TYPE, NULL);
	return 0;
}

void test_HashMapStress(){
	char *auxkeys[10000];

	{
		int index;
		for( index=0; index<10000; index++){
			auxkeys[index] = malloc(10);
			auxkeys[index][0] = rand() % 20 + 66;
			auxkeys[index][1] = rand() % 20 + 66;
			auxkeys[index][2] = rand() % 20 + 66;
			auxkeys[index][3] = rand() % 20 + 66;
			auxkeys[index][4] = rand() % 20 + 66;
			auxkeys[index][5] = rand() % 20 + 66;
			auxkeys[index][6] = rand() % 20 + 66;
			auxkeys[index][7] = rand() % 20 + 66;
			auxkeys[index][8] = rand() % 20 + 66;
			auxkeys[index][9] = '\0';
		}
	}


	{
		int index;
		for(index=0; index<10000; index++){
			HashMapClass.put(hashmap, auxkeys[index], auxkeys[index]);
		}
	}

	{
		int index;
		for(index=0; index<10000; index++){
			CU_ASSERT( strcmp(HashMapClass.get(hashmap, auxkeys[index]), auxkeys[index]) == 0 );
		}
	}

	CU_ASSERT( HashMapClass.isEmpty(hashmap) == 0 );
	CU_ASSERT( HashMapClass.size(hashmap) == 10000 );

	HashMapClass.clean(hashmap);

	CU_ASSERT( HashMapClass.isEmpty(hashmap) == 1 );
	CU_ASSERT( HashMapClass.size(hashmap) == 0 );
}

void test_HashMapPut(){
	int index;
	for(index=0; index<8; index++){
		HashMapClass.put(hashmap, keys[index], dic[index]);
	}

	CU_ASSERT( HashMapClass.isEmpty(hashmap) == 0 );
	CU_ASSERT( HashMapClass.size(hashmap) == 8 );
}

void test_HashMapGet(){
	int index;
	for(index=0; index<8; index++){
		CU_ASSERT( strcmp(dic[index], HashMapClass.get(hashmap, keys[index]) ) == 0 );
	}
}

void test_HashMapIterator(){

	void inner_hashmap_it(char* str){ printf("%s,", str); }

	printf("[");
	HashMapClass.iterator(hashmap, (void*)&inner_hashmap_it);
	printf("]");
}

void test_HashMapRemove(){
	HashMapClass.remove1(hashmap, keys[0]);
	HashMapClass.remove2(hashmap, keys[6]);

	CU_ASSERT( HashMapClass.size(hashmap) == 6 );

	CU_ASSERT( HashMapClass.get(hashmap, keys[0]) == NULL );
	CU_ASSERT( HashMapClass.get(hashmap, keys[6]) == NULL );

	HashMapClass.put(hashmap, keys[0], dic[0]);
	HashMapClass.put(hashmap, keys[6], dic[6]);

	CU_ASSERT( HashMapClass.size(hashmap) == 8 );
}

void test_HashMapClean(){

	HashMapClass.clean(hashmap);

	CU_ASSERT( HashMapClass.isEmpty(hashmap) == 1 );
	CU_ASSERT( HashMapClass.size(hashmap) == 0 );
}

int clean_HashMapSuite(){
	delete(hashmap);
	return 0;
}
