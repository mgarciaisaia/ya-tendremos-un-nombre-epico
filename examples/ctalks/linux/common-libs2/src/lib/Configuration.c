
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "collections/HashMap.h"
#include "oop/Object.h"

#include "File.h"
#include "Configuration.h"
#include "NString.h"

struct Configuration* Configuration_constructor( struct Configuration *self, va_list *args ){
	struct File *configfile = NULL;
	char* path = va_arg(* args, char*);

	self->path = malloc( strlen(path) + 1 );
	strcpy(self->path, path);

	self->delimitor = va_arg(* args, char*)[0];

	self->config = new(HashMap, POINTER_TYPE, &free);


	configfile = new(File, self->path);

	FileClass.read(configfile);

	void inner_ConfigurationParser( char *line){
		char **array;

		if( strlen(line) < 3 )	return;

		if( NStringClass.startWith(line, "//") == 1 )	return;

		array = NStringClass.split(line, self->delimitor);
		HashMapClass.put(self->config, array[0], array[1]);
		free(array);
	}

	FileClass.iterateLines(configfile, inner_ConfigurationParser);
	delete(configfile);

	return self;
}

char* Configuration_getString( struct Configuration *self, char *key ){
	return (char*)HashMapClass.get(self->config, key);
}

int Configuration_getInt( struct Configuration *self, char *key ){
	char *data = (char*)HashMapClass.get(self->config, key);

	if( data != NULL) return atoi(data);

	return 0;
}

long Configuration_getLong( struct Configuration *self, char *key ){
	char *data = (char*)HashMapClass.get(self->config, key);

	if( data != NULL) return atol(data);

	return 0;
}

double Configuration_getDouble( struct Configuration* self, char *key ){
	char *data = (char*)HashMapClass.get(self->config, key);

	if( data != NULL) return atof(data);

	return 0;
}

void *Configuration_destroyer( struct Configuration *self){
	free( self->path );
	delete( self->config );
	return self;
}
