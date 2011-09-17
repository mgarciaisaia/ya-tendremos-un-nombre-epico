#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "StringSuite.h"

#include "../CUnit/Basic.h"

#include "../../lib/String.h"
#include "../../lib/Array.h"
#include "../../lib/oop/Object.h"

int StringSuite_Build(CU_pSuite suite){
	suite = CU_add_suite("Suite_String", init_StringSuite, clean_StringSuite);
	if (NULL == suite) {
		CU_cleanup_registry();
		return 0;
	}

	/* add the tests to the suite */
	/* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
	if ((NULL == CU_add_test(suite, "Test of clone functionality", test_StringClone)) ||
			(NULL == CU_add_test(suite, "Test of convert to int functionality", test_StringAsInt)) ||
			(NULL == CU_add_test(suite, "Test of convert to double functionality", test_StringAsDouble)) ||
			(NULL == CU_add_test(suite, "Test of equals functionality", test_StringEquals)) ||
			(NULL == CU_add_test(suite, "Test of split functionality", test_StringSplit)) ||
			(NULL == CU_add_test(suite, "Test of trim functionality", test_StringTrim)) ||
			(NULL == CU_add_test(suite, "Test of start with functionality", test_StringStartWith)))
	{
		CU_cleanup_registry();
		return 0;
	}

	return 1;
}

int init_StringSuite(){
	return 0;
}

void test_StringClone(){
	struct String *str1 = new(String, "Hola");
	struct String *str2 = NULL;

	CU_ASSERT( str1 != NULL);

	str2 = StringClass.clone(str1);

	CU_ASSERT( str2 != NULL);

	CU_ASSERT( str1 != str2);

	CU_ASSERT( str1->length == str2->length );

	CU_ASSERT( strcmp(str1->text, str2->text) == 0 );

	CU_ASSERT( str2->text != str1->text);

	delete(str1);
	delete(str2);
}

void test_StringAsInt(){
	struct String *str1 = new(String, "10");
	struct String *str2 = new(String, "10.5");

	CU_ASSERT( StringClass.asInt(str1) == 10 );
	CU_ASSERT( StringClass.asInt(str2) == 10 );

	delete(str1);
	delete(str2);
}

void test_StringAsDouble(){
	struct String *str1 = new(String, "10.5");

	CU_ASSERT( StringClass.asDouble(str1) == 10.5 );

	delete(str1);
}

void test_StringEquals(){
	struct String *str1 = new(String, "Hola");
	struct String *str2 = new(String, "Hola");

	CU_ASSERT( StringClass.equals(str1, str2) == 1 );

	delete(str1);
	delete(str2);
}

void test_StringSplit(){
	struct String *str = new(String, "1--2-3-45");
	struct Array *array = StringClass.split(str, '-');

	CU_ASSERT( ArrayClass.size(array) == 5);

	char *aux;

	aux = ArrayClass.get(array, 0);
	CU_ASSERT( strcmp(aux, "1") == 0 );
	aux = ArrayClass.get(array, 1);
	CU_ASSERT( strcmp(aux, "") == 0 );
	aux = ArrayClass.get(array, 2);
	CU_ASSERT( strcmp(aux, "2") == 0 );
	aux = ArrayClass.get(array, 3);
	CU_ASSERT( strcmp(aux, "3") == 0 );
	aux = ArrayClass.get(array, 4);
	CU_ASSERT( strcmp(aux, "45") == 0 );

	ArrayClass.clean(array);

}

void test_StringTrim(){
	struct String *str1 = new(String, "   Hola   ");
	struct String *str2 = StringClass.trim(str1);

	CU_ASSERT( str2->length == 4 );

	CU_ASSERT( strcmp(str2->text, "Hola") == 0 );

	delete(str1);
	delete(str2);
}

void test_StringStartWith(){
	struct String *str1 = new(String, "Hola, Mundo");

	CU_ASSERT( StringClass.startWith(str1, "Hola") == 1 );
	CU_ASSERT( StringClass.startWith(str1, "Hola,") == 1 );
	CU_ASSERT( StringClass.startWith(str1, "ola") == 0 );

	delete(str1);
}

int clean_StringSuite(){
	return 0;
}
