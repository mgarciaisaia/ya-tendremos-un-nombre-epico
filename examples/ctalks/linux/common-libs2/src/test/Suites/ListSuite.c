#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "ListSuite.h"

#include "../CUnit/Basic.h"

#include "../../lib/collections/List.h"
#include "../../lib/oop/Object.h"

static struct List *list;

static char *dic[] = {"A", "B", "C", "D", "E", "F", "G", "H"};

int ListSuite_Build(CU_pSuite suite){

	/* add a suite to the registry */
	suite = CU_add_suite("Suite_List", init_ListSuite, clean_ListSuite);
	if (NULL == suite) {
		CU_cleanup_registry();
		return 0;
	}

	/* add the tests to the suite */
	/* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
	if ((NULL == CU_add_test(suite, "Test of add functionality", test_ListAdd)) ||
				(NULL == CU_add_test(suite, "Test of get functionality", test_ListGet)) ||
				(NULL == CU_add_test(suite, "Test of find functionality", test_ListFind)) ||
				(NULL == CU_add_test(suite, "Test of Iterator functionality", test_ListIterator)) ||
				(NULL == CU_add_test(suite, "Test of set functionality", test_ListSet)) ||
				(NULL == CU_add_test(suite, "Test of remove functionality", test_ListRemove)) ||
				(NULL == CU_add_test(suite, "Test of clean functionality", test_ListClean)) ||
				(NULL == CU_add_test(suite, "Test of Stress functionality", test_ListStress)))
	{
		CU_cleanup_registry();
		return 0;
	}

	return 1;
}

int init_ListSuite(){
	list = new(List, POINTER_TYPE, NULL);
	return 0;
}

void test_ListAdd(){

	ListClass.add(list, "A" );
	ListClass.add(list, "B" );
	ListClass.add(list, "C" );
	ListClass.add(list, "D" );
	ListClass.add(list, "E" );
	ListClass.add(list, "F" );
	ListClass.add(list, "G" );
	ListClass.add(list, "H" );

	CU_ASSERT( ListClass.isEmpty(list) == 0 );
	CU_ASSERT( ListClass.size(list) == 8 );
}

void test_ListStress(){
	int cont = 0, rep;
	time_t init, end;
	double diff;

	init = time(NULL);

	ListClass.clean(list);

	for(rep=0; rep<1000; rep++){
		for(cont=0; cont<6400; cont++){
			ListClass.add(list, "A" );
			CU_ASSERT( strcmp(ListClass.get(list, cont), "A") == 0 );
		}
		ListClass.clean(list);
	}

	end = time(NULL);

	diff = difftime(end, init);
	printf("(Time: %lf) ", diff);

	ListClass.clean(list);

}


void test_ListGet(){

	CU_ASSERT( strcmp( ListClass.get(list, 0), "A") == 0 );
	CU_ASSERT( strcmp( ListClass.get(list, 1), "B") == 0 );
	CU_ASSERT( strcmp( ListClass.get(list, 2), "C") == 0 );
	CU_ASSERT( strcmp( ListClass.get(list, 3), "D") == 0 );
	CU_ASSERT( strcmp( ListClass.get(list, 4), "E") == 0 );
	CU_ASSERT( strcmp( ListClass.get(list, 5), "F") == 0 );
	CU_ASSERT( strcmp( ListClass.get(list, 6), "G") == 0 );
	CU_ASSERT( strcmp( ListClass.get(list, 7), "H") == 0 );

}

void test_ListSet(){
	int index, cont=0;

	void inner_revcmp(char* str){
		CU_ASSERT( strcmp( dic[ListClass.size(list)-cont-1], str) == 0 );
		cont++;
	}

	for(index=0; index<ListClass.size(list); index++){
		ListClass.set1(list, index, dic[ListClass.size(list)-index - 1]);
		CU_ASSERT( strcmp(ListClass.get(list, index), dic[ListClass.size(list)-index-1]) == 0 );
	}

	CU_ASSERT( ListClass.size(list) == 8 );
	ListClass.iterator(list, (void*)inner_revcmp);
	cont = 0;

	for(index=0; index<ListClass.size(list); index++){
		ListClass.set1(list, index, dic[index]);
		CU_ASSERT( strcmp(ListClass.get(list, index), dic[index]) == 0 );
	}

	CU_ASSERT( ListClass.size(list) == 8 );

	for(index=0; index<ListClass.size(list); index++){
		ListClass.set2(list, index, dic[ListClass.size(list)-index - 1]);
		CU_ASSERT( strcmp(ListClass.get(list, index), dic[ListClass.size(list)-index-1]) == 0 );
	}

	CU_ASSERT( ListClass.size(list) == 8 );
	ListClass.iterator(list, (void*)inner_revcmp);
	cont = 0;

	for(index=0; index<ListClass.size(list); index++){
		ListClass.set2(list, index, dic[index]);
		CU_ASSERT( strcmp(ListClass.get(list, index), dic[index]) == 0 );
	}

	CU_ASSERT( ListClass.size(list) == 8 );
}

void test_ListRemove(){
	int index = 0;

	for(index=0; index<100; index++){
		ListClass.remove1(list, 0);
	}

	CU_ASSERT( ListClass.size(list) == 0 );

	test_ListAdd();

	ListClass.remove2(list, 0);
	ListClass.remove2(list, 3);
	ListClass.remove2(list, 5);

	CU_ASSERT( ListClass.size(list) == 5 );

	CU_ASSERT( strcmp( ListClass.get(list, 0), "B") == 0 );
	CU_ASSERT( strcmp( ListClass.get(list, 1), "C") == 0 );
	CU_ASSERT( strcmp( ListClass.get(list, 2), "D") == 0 );
	CU_ASSERT( strcmp( ListClass.get(list, 3), "F") == 0 );
	CU_ASSERT( strcmp( ListClass.get(list, 4), "G") == 0 );

	for(index=0; index<6; index++){
		ListClass.remove2(list, 0);
	}

	CU_ASSERT( ListClass.size(list) == 0 );

}

void test_ListIterator(){
	int cont = 0;
	void inner_print(char* str){ CU_ASSERT( strcmp( dic[cont], str) == 0 ); cont++; }
	ListClass.iterator(list, (void*)inner_print);

	CU_ASSERT( cont == 8 );
}

void test_ListFind(){
	int index;
	for(index=0; index<ListClass.size(list); index++){
		int inner_find(char* str){ return strcmp(str, dic[index]) == 0; }
		char *aux = ListClass.find(list, (void*)inner_find);
		CU_ASSERT( strcmp(aux, dic[index]) == 0 );
	}
}

void test_ListClean(){
	ListClass.clean(list);

	CU_ASSERT( ListClass.size(list) == 0 );
}

int clean_ListSuite(){

	delete(list);

	return 0;
}
