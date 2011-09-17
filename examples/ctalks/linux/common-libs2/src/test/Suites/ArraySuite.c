#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ArraySuite.h"

#include "../CUnit/Basic.h"

#include "../../lib/oop/Object.h"
#include "../../lib/Array.h"

static struct Array *array = NULL;

int ArraySuite_Build(CU_pSuite suite){

	/* add a suite to the registry */
	suite = CU_add_suite("Suite_Array", init_ArraySuite, clean_ArraySuite);
	if (NULL == suite) {
		CU_cleanup_registry();
		return 0;
	}

	/* add the tests to the suite */
	/* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
	if ((NULL == CU_add_test(suite, "Test of get functionality", test_ArraySetAndGet)) ||
				(NULL == CU_add_test(suite, "Test of find functionality", test_ArrayFind)) ||
				(NULL == CU_add_test(suite, "Test of iterator functionality", test_ArrayIterator)) ||
				(NULL == CU_add_test(suite, "Test of clean functionality", test_ArrayClean)))
	{
		CU_cleanup_registry();
		return 0;
	}

	return 1;
}

int init_ArraySuite(){
	array = new(Array, POINTER_TYPE, NULL, 5);
	return 0;
}

void test_ArraySetAndGet(){

	ArrayClass.set1(array, 0, "A");
	ArrayClass.set1(array, 1, "B");
	ArrayClass.set2(array, 2, "C");
	ArrayClass.set2(array, 3, "D");
	ArrayClass.set2(array, 4, "E");

	CU_ASSERT( strcmp(ArrayClass.get(array, 0), "A") == 0 );
	CU_ASSERT( strcmp(ArrayClass.get(array, 1), "B") == 0 );
	CU_ASSERT( strcmp(ArrayClass.get(array, 2), "C") == 0 );
	CU_ASSERT( strcmp(ArrayClass.get(array, 3), "D") == 0 );
	CU_ASSERT( strcmp(ArrayClass.get(array, 4), "E") == 0 );

}

void test_ArrayFind(){
	int cont=0;
	char *str;

	int inner_TestFind(char *str){
		cont++;
		return strcmp( str, "C") == 0;
	}

	str = (char*)ArrayClass.find(array, (void*)inner_TestFind);

	CU_ASSERT( strcmp( str, "C") == 0 );
	CU_ASSERT( cont == 3 );
}

void test_ArrayIterator(){
	char *vals[] = {"A", "B", "C", "D", "E"};
	int cont=0;

	void inner_TestIterator(char *str){
		CU_ASSERT( strcmp( str, vals[cont]) == 0 );
		cont++;
	}

	ArrayClass.iterator(array, (void*)inner_TestIterator);
}

void test_ArrayClean(){
	int cont;

	ArrayClass.clean(array);

	for(cont=0; cont<5; cont++){
		CU_ASSERT( ArrayClass.get(array, cont) == NULL );
	}

}

int clean_ArraySuite(){
	delete(array);
	return 0;
}
