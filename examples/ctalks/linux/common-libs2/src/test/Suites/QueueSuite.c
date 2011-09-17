#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "QueueSuite.h"

#include "../CUnit/Basic.h"

#include "../../lib/collections/Queue.h"
#include "../../lib/oop/Object.h"

static struct Queue *queue;

static char *dic[] = {"A", "B", "C", "D", "E", "F", "G", "H"};

int QueueSuite_Build(CU_pSuite suite){

	suite = CU_add_suite("Suite_Queue", init_QueueSuite, clean_QueueSuite);
	if (NULL == suite) {
		CU_cleanup_registry();
		return 0;
	}

	/* add the tests to the suite */
	/* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
	if ((NULL == CU_add_test(suite, "Test of push functionality", test_QueuePush)) ||
			(NULL == CU_add_test(suite, "Test of pop functionality", test_QueuePop)) ||
			(NULL == CU_add_test(suite, "Test of Iterator functionality", test_QueueIterator)) ||
			(NULL == CU_add_test(suite, "Test of clean functionality", test_QueueClean)))
	{
		CU_cleanup_registry();
		return 0;
	}

	return 1;
}

int init_QueueSuite(){
	queue = new(Queue, NONBLOCKING_QUEUE, POINTER_TYPE, NULL);
	return 0;
}

void test_QueuePush(){

	QueueClass.push(queue, "A");
	QueueClass.push(queue, "B");
	QueueClass.push(queue, "C");
	QueueClass.push(queue, "D");
	QueueClass.push(queue, "E");
	QueueClass.push(queue, "F");
	QueueClass.push(queue, "G");
	QueueClass.push(queue, "H");

	CU_ASSERT( QueueClass.isEmpty(queue) == 0 );
	CU_ASSERT( QueueClass.size(queue) == 8 );
}

void test_QueuePop(){

	CU_ASSERT( strcmp( QueueClass.pop1(queue), "A") == 0 );
	CU_ASSERT( strcmp( QueueClass.pop1(queue), "B") == 0 );
	CU_ASSERT( strcmp( QueueClass.pop1(queue), "C") == 0 );
	CU_ASSERT( strcmp( QueueClass.pop1(queue), "D") == 0 );
	CU_ASSERT( strcmp( QueueClass.pop1(queue), "E") == 0 );
	CU_ASSERT( strcmp( QueueClass.pop1(queue), "F") == 0 );
	CU_ASSERT( strcmp( QueueClass.pop1(queue), "G") == 0 );
	CU_ASSERT( strcmp( QueueClass.pop1(queue), "H") == 0 );

	CU_ASSERT( QueueClass.isEmpty(queue) == 1 );
	CU_ASSERT( QueueClass.size(queue) == 0 );

	CU_ASSERT( QueueClass.pop1(queue) == NULL );

}

void test_QueueIterator(){
	int cont = 0;

	test_QueuePush();

	void inner_print(char* str){ CU_ASSERT( strcmp( dic[cont], str) == 0 ); cont++; }
	QueueClass.iterator(queue, (void*)inner_print);

	CU_ASSERT( cont == 8 );
}

void test_QueueClean(){
	QueueClass.clean(queue);

	CU_ASSERT( QueueClass.size(queue) == 0 );
}

int clean_QueueSuite(){
	delete(queue);
	return 0;
}
