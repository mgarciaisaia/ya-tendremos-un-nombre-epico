

#ifndef COMMANDS_SUITE_H_
#define COMMANDS_SUITE_H_

	#include "../CUnit/Basic.h"

	int CommandsSuite_Build(CU_pSuite suite);
	int init_CommandsSuite();
	void test_CommandsParser();
	int clean_CommandsSuite();

#endif /* COMMANDS_SUITE_H_ */
