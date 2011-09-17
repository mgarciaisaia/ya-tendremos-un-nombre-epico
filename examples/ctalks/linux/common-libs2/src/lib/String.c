#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "oop/Object.h"

#include "NString.h"
#include "String.h"


struct String*	String_constructor( struct String *self, va_list *args ){
	char *str = va_arg(* args, char*);

	self->length = strlen(str);
	self->text = malloc( self->length + 1 );
	strcpy(self->text, str);

	return self;
}

int	String_startWith( struct String *self, char *prefix ){
	return NStringClass.startWith( self->text, prefix);
}

int	String_asInt( struct String *self ){
	return NStringClass.asInt( self->text );
}

double String_asDouble( struct String *self ){
	return NStringClass.asDouble( self->text );
}

struct Array *  String_split(struct String *self, char delimitor){
	char **narray = NStringClass.split(self->text, delimitor);
	struct Array *array = NULL;
	int maxindex;

	for(maxindex=0; narray[maxindex] != NULL; maxindex++);

	array = new(Array, POINTER_TYPE, &free, maxindex);

	{
		int index;
		for(index=0; index<maxindex; index++){
			ArrayClass.set2(array, index, narray[index]);
		}
	}

	free(narray);

	return array;
}

struct String*	String_trim( struct String *self ){
	return new(String, NStringClass.trim(self->text) );
}

struct String*	String_clone( struct String *self ){
	return new(String, self->text );
}

int	String_equals( struct String *self, struct String *str ){
	return NStringClass.equals(self->text, str->text);
}

void* String_destroyer( struct String *self ){
	free(self->text);
	return self;
}
