

#ifndef LOG_SUITE_H_
#define LOG_SUITE_H_

	#include "../CUnit/Basic.h"

	int LogSuite_Build(CU_pSuite suite);
	int init_LogSuite();
	void test_LogWrite();
	int clean_LogSuite();

#endif /* LOG_SUITE_H_ */
