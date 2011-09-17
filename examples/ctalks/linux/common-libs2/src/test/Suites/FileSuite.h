

#ifndef FILE_SUITE_H_
#define FILE_SUITE_H_

	#include "../CUnit/Basic.h"

	int FileSuite_Build(CU_pSuite suite);
	int init_FileSuite();
	void test_FileExist();
	void test_FileIsDirectory();
	void test_FileCreate();
	void test_FileWrite();
	void test_FileRead();
	void test_FileSize();
	void test_FileIterate();
	void test_FileDelete();
	int clean_FileSuite();

#endif /* FILE_SUITE_H_ */
