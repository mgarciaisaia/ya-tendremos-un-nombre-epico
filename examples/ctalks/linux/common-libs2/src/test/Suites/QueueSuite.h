

#ifndef QUEUE_SUITE_H_
#define QUEUE_SUITE_H_

	#include "../CUnit/Basic.h"

	int QueueSuite_Build(CU_pSuite suite);
	int init_QueueSuite();
	void test_QueuePush();
	void test_QueuePop();
	void test_QueueIterator();
	void test_QueueClean();
	int clean_QueueSuite();

#endif /* QUEUE_SUITE_H_ */
