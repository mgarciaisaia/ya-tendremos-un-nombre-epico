#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "lib/serializer.h"
#include "lib/utils.h"

typedef struct{
	unsigned char age;
	unsigned int dni;
	char *name;
	char *lastname;
}__attribute__ ((__packed__)) t_person;

t_person *person_create(unsigned char age, unsigned int dni, char *name, char *lastname);
void	  person_destroy(t_person*);

int main(int argc, char **argv) {

	t_serializer *serializer = serializer_create("t_person", SFIELD_TYPE_CHAR /*Age*/,
															SFIELD_TYPE_INT32 /*DNI*/,
															SFIELD_TYPE_CHAR_POINTER /*Name*/,
															SFIELD_TYPE_CHAR_POINTER /*Lastname*/,
															SFIELD_TYPE_END);

	t_person *person1,*person2;
	t_stream *stream;

	person1 = person_create(24, 3000000, "John", "Doe" );

	stream = serializer_build(serializer, person1);

	person2 = serializer_rebuild(serializer, stream);

	assert( person1->age == person2->age);
	assert( person1->dni == person2->dni);
	assert( strcmp(person1->name, person2->name ) == 0 );
	assert( strcmp(person1->lastname, person2->lastname ) == 0 );


	serializer_delete(serializer, &stream);
	serializer_destroy(serializer);

	person_destroy(person1);
	person_destroy(person2);

	puts("Run Success!");

	return EXIT_SUCCESS;
}

t_person *person_create(unsigned char age, unsigned int dni, char *name, char *lastname){
	t_person *person = malloc( sizeof(t_person) );

	person->age = age;
	person->dni = dni;
	person->name = utils_string_clone(name);
	person->lastname = utils_string_clone(lastname);

	return person;
}

void person_destroy(t_person *self){
	free(self->name);
	free(self->lastname);
	free(self);
}
