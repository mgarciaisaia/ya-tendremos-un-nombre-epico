#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "FileSuite.h"

#include "../CUnit/Basic.h"

#include "../../lib/oop/Object.h"
#include "../../lib/File.h"

int FileSuite_Build(CU_pSuite suite){

	/* add a suite to the registry */
	suite = CU_add_suite("Suite_File", init_FileSuite, clean_FileSuite);
	if (NULL == suite) {
		CU_cleanup_registry();
		return 0;
	}

	/* add the tests to the suite */
	/* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
	if ((NULL == CU_add_test(suite, "Test of exits functionality", test_FileExist)) ||
				(NULL == CU_add_test(suite, "Test of isDirectory functionality", test_FileIsDirectory)) ||
				(NULL == CU_add_test(suite, "Test of create functionality", test_FileCreate)) ||
				(NULL == CU_add_test(suite, "Test of write functionality", test_FileWrite)) ||
				(NULL == CU_add_test(suite, "Test of read functionality", test_FileRead)) ||
				(NULL == CU_add_test(suite, "Test of size functionality", test_FileSize)) ||
				(NULL == CU_add_test(suite, "Test of iterate functionality", test_FileIterate)) ||
				(NULL == CU_add_test(suite, "Test of delete functionality", test_FileDelete)))
	{
		CU_cleanup_registry();
		return 0;
	}

	return 1;
}

int init_FileSuite(){
	return 0;
}

void test_FileExist(){
	struct File *file = new(File, "./data/test.txt");

	CU_ASSERT( FileClass.exist(file) == 0 );

	delete(file);
}

void test_FileIsDirectory(){

	CU_ASSERT( FileClass.isDirectory( "./data" ) == 1 );
	CU_ASSERT( FileClass.isDirectory( "./data/test.txt" ) == 0 );

}

void test_FileCreate(){
	struct File *file = new(File, "./data/test.txt");

	CU_ASSERT( FileClass.create(file) == 1 );

	CU_ASSERT( FileClass.exist(file) == 1 );

	delete(file);
}

void test_FileWrite(){
	struct File *file = new(File, "./data/test.txt");

	file->text = "Hola, Mundo!\n*El Mundo Saluda*\nComo les va?";

	CU_ASSERT( FileClass.write(file) == 1 );

	file->text = NULL;

	delete(file);
}

void test_FileRead(){
	struct File *file = new(File, "./data/test.txt");

	char *content = FileClass.read(file);

	CU_ASSERT( content != NULL );

	if( content == NULL ){
		delete(file);
		return;
	}

	CU_ASSERT( strcmp(content, "Hola, Mundo!\n*El Mundo Saluda*\nComo les va?") == 0 );

	delete(file);
}

void test_FileSize(){
	struct File *file = new(File, "./data/test.txt");

	CU_ASSERT( FileClass.size(file) == strlen("Hola, Mundo!\n*El Mundo Saluda*\nComo les va?")  );

	delete(file);
}

void test_FileIterate(){
	struct File *file = new(File, "./data/test.txt");
	char *lines[] = { "Hola, Mundo!", "*El Mundo Saluda*", "Como les va?" };
	int lineIndex = 0;

	void inner_lineIt(char *line){
		char *str1 = lines[lineIndex];
		CU_ASSERT( strcmp(str1, line) == 0 );
		lineIndex++;
	}

	FileClass.read(file);
	FileClass.iterateLines(file, inner_lineIt);
	CU_ASSERT( lineIndex == 3 );

	delete(file);
}

void test_FileDelete(){
	struct File *file = new(File, "./data/test.txt");

	CU_ASSERT( FileClass.delete(file) == 1 );

	CU_ASSERT( FileClass.exist(file) == 0 );

	delete(file);
}

int clean_FileSuite(){
	return 0;
}
