

#ifndef CONFIGURATION_SUITE_H_
#define CONFIGURATION_SUITE_H_

	#include "../CUnit/Basic.h"

	int ConfigurationSuite_Build(CU_pSuite suite);
	int init_ConfigurationSuite();
	void test_ConfigurationGet();
	int clean_ConfigurationSuite();

#endif /* CONFIGURATION_SUITE_H_ */
